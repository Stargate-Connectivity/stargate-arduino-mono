/*
    ESP8266GateDevice.cpp - Library to connect ESP8266 module with Stargate server
    Created by Michał Marchel, December 2, 2023.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "GateDevice.h"

GateDevice::GateDevice() : BaseDevice() {
    this->connectionState = 0;
    this->pingInterval = 1000;
    this->pingTimer = 0;
    this->pingInProgress = false;
    this->failedPings = 0;
    this->discoveryKeyword = "GateServer";
    this->discoveryPort = 10001;
    this->networkStopped = true;
    this->usePreviousAddress = false;
    this->lastServerAddress = "";
    this->lastServerPort = -1;
};

void GateDevice::start() {
    if (!this->deviceStarted) {
        this->deviceStarted = true;
        this->onDeviceStart();
        this->connectServer();
        this->networkStopped = false;
    }
};

void GateDevice::stop(bool stopNetwork) {
    if (this->deviceStarted) {
        this->usePreviousAddress = true;
        this->deviceStarted = false;
        this->networkStopped = stopNetwork;
        this->stopSocket();
    }
}

void GateDevice::stop() {
    this->stop(true);
}

void GateDevice::loop() {
    if (this->networkAvailable()) {
        if (this->connectionState == 4) {
            this->loopSocket();
            this->handlePing();
            this->outputBuffer.loop();
        } else {
            this->connectServer();
        }
    } else {
        this->connectionState = 0;
    }
}

void GateDevice::connectServer() {
    switch (this->connectionState) {
        case 0:
        {
            this->stopSocket();
            if (this->usePreviousAddress && this->lastServerAddress.length() > 0 && this->lastServerPort != -1) {
                this->startSocket(lastServerAddress, lastServerPort);
                this->pingTimer = millis() + 5000;
                this->connectionState = 2;
            } else {
                bool success = this->startUdp(this->discoveryPort);
                if (success) {
                    this->connectionState = 1;
                }
            }
            this->usePreviousAddress = false;
            break;
        }
        case 1:
        {
            String discoveryMessage = this->getUdpMessage();
            if (discoveryMessage.length() > this->discoveryKeyword.length()) {
                int separatorIndex = discoveryMessage.indexOf(':');
                if (separatorIndex > -1) {
                    String keyword = discoveryMessage.substring(0, separatorIndex);
                    if (keyword.equals(discoveryKeyword)) {
                        String serverIp = this->getServerIp();
                        this->stopUdp();
                        int connectionPort = discoveryMessage.substring(separatorIndex + 1).toInt();
                        this->lastServerAddress = serverIp;
                        this->lastServerPort = connectionPort;
                        this->startSocket(serverIp, connectionPort);
                        this->pingTimer = millis() + 5000;
                        this->connectionState = 2;
                    }
                }
            }
            break;
        }
        case 2:
        case 3:
        {
            if (this->pingTimer < millis()) {
                this->stopSocket();
                this->connectionState = 0;
            } else {
                this->loopSocket();
            }
            break;
        }
    }
}

void GateDevice::handlePing() {
    unsigned long now = millis();
    if (this->pingTimer < now) {
        if (this->pingInProgress) {
            this->failedPings++;
            if (this->failedPings > 3) {
                this->stopSocket();
                this->connectionState = 0;
            }
        }
        if (this->connectionState == 4) {
            this->pingInProgress = true;
            this->pingTimer = now + this->pingInterval;
            this->send("*?ping||");
        }
    }
}

void GateDevice::socketOpened() {
    this->connectionState = 3;
}

void GateDevice::socketClosed() {
    this->connectionState = 0;
    this->loopSocket();
    if (this->networkStopped) {
        delay(100);
        this->stopNetwork();
    }
}

void GateDevice::onMessage(char* message) {
    if (this->connectionState == 3) {
        bool isAcknowledge = false;
        String remainingMessage = String(message);
        while (remainingMessage.length() > 0) {
            if (remainingMessage.charAt(0) == '*') {
                switch (remainingMessage.charAt(1)) {
                    case '?':
                        if (remainingMessage.charAt(2) == 'm') {
                            String response = "*>manifest|";
                            String manifest = createManifest(this->deviceName, this->groupName, this->factory.getValues());
                            this->outputBuffer.sendFunctionalMessage(response + String(manifest.length()) + "|" + manifest);
                            remainingMessage.remove(0, 12);
                        } else if (remainingMessage.charAt(2) == 't') {
                            String response = "*>type|6|device";
                            this->outputBuffer.sendFunctionalMessage(response);
                            remainingMessage.remove(0, 8);
                        }
                        break;
                    case '!':
                        if (remainingMessage.charAt(2) == 'a') {
                            remainingMessage.remove(0, 13);
                            int separatorIndex = remainingMessage.indexOf('|');
                            int idLength = remainingMessage.substring(0, separatorIndex).toInt();
                            remainingMessage.remove(0, separatorIndex + 1);
                            handleIdAssigned(remainingMessage.substring(0, idLength));
                            remainingMessage.remove(0, idLength);
                        } else if (remainingMessage.charAt(2) == 'r') {
                            this->pingInProgress = false;
                            this->failedPings = 0;
                            this->outputBuffer.reset();
                            this->connectionState = 4;
                            remainingMessage.remove(0, 9);
                        }
                        break;
                    case '+':
                        isAcknowledge = true;
                        remainingMessage.remove(0, 2);
                        break;
                    default:
                        remainingMessage = "";
                }
            } else {
                remainingMessage = "";
            }
        }
        if (isAcknowledge) {
            this->outputBuffer.acknowledgeReceived();
        } else {
            this->outputBuffer.sendAcknowledge();
        }
    } else {
        bool isAcknowledge = false;
        String remainingMessage = String(message);
        while (remainingMessage.length() > 0) {
            if (remainingMessage.charAt(0) == '*') {
                switch (remainingMessage.charAt(1)) {
                    case '>':
                        if (remainingMessage.charAt(2) == 's') {
                            remainingMessage.remove(0, 13);
                            int separatorIndex = remainingMessage.indexOf('|');
                            int responseLength = remainingMessage.substring(0, separatorIndex).toInt();
                            remainingMessage.remove(0, separatorIndex + 1);
                            this->serverStorage.handleGetResponse(remainingMessage.substring(0, responseLength));
                            remainingMessage.remove(0, responseLength);
                        } else if (remainingMessage.charAt(2) == 'p') {
                            if (this->pingInProgress) {
                                if (this->pingInUse && this->pingTimer > this->pingInterval) {
                                    int timePassed = (int) (millis() - (this->pingTimer - this->pingInterval));
                                    this->ping->setValue(timePassed / 2);
                                }
                                this->pingInProgress = false;
                                this->pingTimer = millis() + 3000;
                                this->failedPings = 0;
                            }
                            remainingMessage.remove(0, 10);
                        } else {
                            remainingMessage = "";
                        }
                        break;
                    case '!':
                        if(remainingMessage.charAt(2) == 's') {
                            int messageLength = handleSubscription(true, remainingMessage, this->factory.getValues(), &this->outputBuffer);
                            remainingMessage.remove(0, messageLength);
                        } else if (remainingMessage.charAt(2) == 'u') {
                            int messageLength = handleSubscription(false, remainingMessage, this->factory.getValues(), &this->outputBuffer);
                            remainingMessage.remove(0, messageLength);
                        }
                        break;
                    case '+':
                        isAcknowledge = true;
                        remainingMessage.remove(0, 2);
                        break;
                    default:
                        remainingMessage = "";
                }
            } else {
                handleValueMessage(remainingMessage, this->factory.getValues());
                remainingMessage = "";
            }
        }
        if (isAcknowledge) {
            this->outputBuffer.acknowledgeReceived();
        } else {
            this->outputBuffer.sendAcknowledge();
        }
    }
}

bool GateDevice::isReady() {
    return this->connectionState == 4;
}

void GateDevice::usePing() {
    if (!this->deviceStarted && !this->pingInUse) {
        this->pingInUse = true;
        this->ping = new GateInt(&this->outputBuffer);
        this->ping->id = -1;
        this->ping->direction = GateValue::getDirection("output");
        this->ping->setName("Ping");
        this->factory.addValue(this->ping);
    }
}
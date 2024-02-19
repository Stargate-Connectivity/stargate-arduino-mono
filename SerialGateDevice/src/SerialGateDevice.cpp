/*
    SerialGateDevice.cpp - Library to connect with Stargate server through serial adapter
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

#include "SerialGateDevice.h"

SerialGateDevice::SerialGateDevice() : BaseDevice() {
    this->ready = false;
};

void SerialGateDevice::send(String message) {
    if (Serial) {
        Serial.println(message);
    }
}

void SerialGateDevice::start() {
    if (!this->deviceStarted) {
        this->deviceStarted = true;
        Serial.setTimeout(100);
        Serial.begin(115200);
    }
}

void SerialGateDevice::loop() {
    if (this->ready) {
        this->outputBuffer.loop();
    }
    if (Serial.available() > 0) {
        String message = Serial.readStringUntil('\n');
        this->onMessage(message);
    }
}

bool SerialGateDevice::isReady() {
    return this->ready;
}

void SerialGateDevice::onMessage(String message) {
    if (message.charAt(0) == '*') {
        if (message.charAt(1) == '!') {
            switch (message.charAt(2)) {
                case 's':
                    handleSubscription(true, String(message), this->factory.getValues(), &this->outputBuffer);
                    break;
                case 'u':
                    handleSubscription(false, String(message), this->factory.getValues(), &this->outputBuffer);
                    break;
                case 'a':
                    handleIdAssigned(String(message));
                    break;
                case 'r':
                    this->outputBuffer.clear();
                    this->ready = true;
                    break;
                case 'n':
                    this->ready = false;
                    break;
            }
        } else if (message.charAt(1) == '?') {
            message.setCharAt(1, '>');
            String response = message + "|";
            if (message[2] == 'm') {
                response += createManifest(this->deviceName, this->factory.getValues());
                this->send(response);
            } else if (message[2] == 't') {
                response += "device";
                this->send(response);
            }
        } else if (message.charAt(1) == '>') {
            this->serverStorage.handleGetResponse(message);
        }
    } else {
        handleValueMessage(String(message), this->factory.getValues());
    }
}
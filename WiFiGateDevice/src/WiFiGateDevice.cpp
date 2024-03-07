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

#include "WiFiGateDevice.h"

WiFiGateDevice::WiFiGateDevice(const char* ssid, const char* password) : GateDevice() {
    this->WIFI_SSID = (char*) ssid;
    this->WIFI_PASS = (char*) password;
};

bool WiFiGateDevice::startUdp(int port) {
    return this->UDP.begin(port) == 1;
}

void WiFiGateDevice::stopUdp() {
    this->UDP.stop();
}

String WiFiGateDevice::getUdpMessage() {
    int packetSize = this->UDP.parsePacket();
    if (packetSize > 0) {
        char data[packetSize];
        this->UDP.read(data, packetSize);
        return String(data);
    } else {
        return String("");
    }
}

String WiFiGateDevice::getServerIp() {
    return this->UDP.remoteIP().toString();
}

void WiFiGateDevice::onDeviceStart() {
    if (this->networkStopped) {
        WiFi.begin(this->WIFI_SSID, this->WIFI_PASS);
        while (WiFi.status() != WL_CONNECTED) {
            delay(100);
        }
        this->networkStopped = false;
    }
}

void WiFiGateDevice::stopNetwork() {
    WiFi.disconnect(true);
}

bool WiFiGateDevice::networkAvailable() {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiGateDevice::webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            this->socketClosed();
            break;
        case WStype_CONNECTED:
            this->socketOpened();
            break;
        case WStype_TEXT:
            char * message = (char *) payload;
            this->onMessage(message);
            break;
    }
}

void WiFiGateDevice::startSocket(String serverIp, int port) {
    this->webSocket.begin(serverIp, port, "/");
    this->webSocket.onEvent(std::bind(&WiFiGateDevice::webSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void WiFiGateDevice::stopSocket() {
    this->webSocket.disconnect();
}

void WiFiGateDevice::send(String* message) {
    this->webSocket.sendTXT(*message);
}

void WiFiGateDevice::loopSocket() {
    this->webSocket.loop();
}
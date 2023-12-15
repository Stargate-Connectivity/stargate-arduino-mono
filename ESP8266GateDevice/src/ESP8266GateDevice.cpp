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

#include "ESP8266GateDevice.h"

ESP8266GateDevice::ESP8266GateDevice(String ssid, String password) : GateDevice() {
    this->WIFI_SSID = ssid;
    this->WIFI_PASS = password;
};

bool ESP8266GateDevice::startUdp(int port) {
    return this->UDP.begin(10001) == 1;
}

void ESP8266GateDevice::stopUdp() {
    this->UDP.stop();
}

bool ESP8266GateDevice::wasKeywordReceived(char* keyword) {
    int packetSize = this->UDP.parsePacket();
    if (packetSize > 0) {
        char data[packetSize];
        this->UDP.read(data, packetSize);
        return strcmp(data, keyword);
    } else {
        return false;
    }
}

String ESP8266GateDevice::getServerIp() {
    return this->UDP.remoteIP().toString();
}

void ESP8266GateDevice::onDeviceStart() {
    WiFi.begin(this->WIFI_SSID, this->WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
    }
}

bool ESP8266GateDevice::networkAvailable() {
    return WiFi.status() == WL_CONNECTED;
}

void ESP8266GateDevice::webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
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

void ESP8266GateDevice::startSocket(String serverIp, int port) {
    this->webSocket.begin(serverIp, port, "/");
    this->webSocket.onEvent(std::bind(&ESP8266GateDevice::webSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void ESP8266GateDevice::stopSocket() {
    this->webSocket.disconnect();
}

void ESP8266GateDevice::send(String message) {
    this->webSocket.sendTXT(message);
}

void ESP8266GateDevice::loopSocket() {
    this->webSocket.loop();
}
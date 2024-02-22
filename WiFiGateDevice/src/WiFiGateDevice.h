/*
    WiFiGateDevice.h - Library to connect with Stargate server using WiFi library
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

#ifndef WiFiGateDevice_h
#define WiFiGateDevice_h

#include <Arduino.h>
//#include <WiFi.h>
#include <WiFiUdp.h>
#include <WebSocketsClient.h>
#include "lib/GateDevice/GateDevice.h"

class WiFiGateDevice : public GateDevice
{
    public:
        WiFiGateDevice(String ssid, String password);

    private:
        bool startUdp(int port) override;
        void stopUdp() override;
        String getUdpMessage() override;
        String getServerIp() override;
        void onDeviceStart() override;
        void stopNetwork() override;
        bool networkAvailable() override;
        void startSocket(String serverIp, int port) override;
        void stopSocket() override;
        void loopSocket() override;
        void send(String message) override;

        String WIFI_SSID;
        String WIFI_PASS;
        WiFiUDP UDP;
        WebSocketsClient webSocket;
        void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);
};

#endif
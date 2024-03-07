/*
    SerialGateDevice.h - Library to connect with Stargate server through serial adapter
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

#ifndef SerialGateDevice_h
#define SerialGateDevice_h

#include <Arduino.h>
#include "lib/GateCore/BaseDevice.h"
#include "lib/GateCore/OutputBuffer.h"
#include "lib/GateCore/MessageHandler.h"
#include "lib/GateCore/values/ValueFactory.h"

class SerialGateDevice : public BaseDevice
{
    public:
        SerialGateDevice();
        void start() override;
        void loop() override;
        bool isReady() override;
        void send(String* message) override;

    private:
        void onMessage(String* remainingMessage);
        bool ready;
};

#endif
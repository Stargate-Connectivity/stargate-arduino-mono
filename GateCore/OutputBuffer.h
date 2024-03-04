#ifndef OutputBuffer_h
#define OutputBuffer_h

#include <Arduino.h>
#include "Sender.h"
#include "values/GateValue.h"
#include "utils/GateValuesSet.h"

class OutputBuffer
{
    public:
        OutputBuffer();
        void loop();
        void sendValue(GateValue* value);
        void sendFunctionalMessage(String message);
        void sendAcknowledge();
        void acknowledgeReceived();
        void reset();
        Sender* device;

    private:
        GateValuesSet buffer;
        String functionalBuffer;
        bool lastMessageAcknowledged;
        bool hasContent();
        void flush();
};

#endif
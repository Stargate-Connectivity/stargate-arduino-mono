#ifndef OutputBuffer_h
#define OutputBuffer_h

#include <Arduino.h>
#include "Sender.h"
#include "values/GateValue.h"
#include "utils/GateValuesSet.h"

class OutputBuffer
{
    public:
        void loop();
        void sendValue(GateValue* value);
        void clear();
        Sender* device;

    private:
        GateValuesSet buffer;
};

#endif
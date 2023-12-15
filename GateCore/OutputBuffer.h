#ifndef OutputBuffer_h
#define OutputBuffer_h

#include <Arduino.h>
#include <map>
#include "values/GateValue.h"

class OutputBuffer
{
    public:
        void loop();
        void sendValue(GateValue* value);
        void clear();
        std::function<void(String message)> sendFunction;

    private:
        std::map<int, GateValue*> buffer;
};

#endif
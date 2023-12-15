#ifndef ValueFactory_h
#define ValueFactory_h

#include <Arduino.h>
#include <map>
#include "GateValue.h"
#include "GateBool.h"
#include "GateInt.h"
#include "GateFloat.h"
#include "GateString.h"
#include "../OutputBuffer.h"

class ValueFactory
{
    public:
        GateBool createBool(String direction);
        GateInt createInt(String direction);
        GateFloat createFloat(String direction);
        GateString createString(String direction);
        std::map<int, GateValue*> getValues();
        void addValue(GateValue* gateValue);
        OutputBuffer* outputBuffer;

    private:
        std::map<int, GateValue*> valuesMap;
};

#endif
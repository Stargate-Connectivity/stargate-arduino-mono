#ifndef ValueFactory_h
#define ValueFactory_h

#include <Arduino.h>
#include "GateValue.h"
#include "GateBool.h"
#include "GateInt.h"
#include "GateFloat.h"
#include "GateString.h"
#include "GateSelect.h"
#include "../OutputBuffer.h"
#include "../utils/GateValuesSet.h"

class ValueFactory
{
    public:
        GateBool createBool(String direction);
        GateInt createInt(String direction);
        GateFloat createFloat(String direction);
        GateString createString(String direction);
        GateSelect createSelect(String direction);
        GateValuesSet* getValues();
        void addValue(GateValue* gateValue);
        OutputBuffer* outputBuffer;

    private:
        GateValuesSet valuesSet;
};

#endif
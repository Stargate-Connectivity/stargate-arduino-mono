#ifndef GateSelect_h
#define GateSelect_h

#include <Arduino.h>
#include "GateValue.h"
#include "../OutputBuffer.h"

class GateSelect : public GateValue
{
    public:
        GateSelect(OutputBuffer* outputBuffer);
        String toString() override;
        void fromRemote(String textValue) override;
        String toManifest() override;
        int getValue();
        void setValue(int value);
        void setValue(int value, bool equalityCheck);
        String nothingSelectedLabel;
        String values;

    private:
        OutputBuffer* outputBuffer;
        int value;
};

#endif
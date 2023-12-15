#ifndef GateString_h
#define GateString_h

#include <Arduino.h>
#include "GateValue.h"
#include "../OutputBuffer.h"

class GateString : public GateValue
{
    public:
        GateString(OutputBuffer* outputBuffer);
        String toString() override;
        void fromRemote(String textValue) override;
        String toManifest() override;
        String getValue();
        void setValue(String value);
        void setMinimumLength(int length);

    private:
        int minimumLength;
        bool minLengthSet;
        OutputBuffer* outputBuffer;
        String value;
};

#endif
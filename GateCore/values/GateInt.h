#ifndef GateInt_h
#define GateInt_h

#include <Arduino.h>
#include "GateValue.h"
#include "../OutputBuffer.h"

class GateInt : public GateValue
{
    public:
        GateInt(OutputBuffer* outputBuffer);
        String toString() override;
        void fromRemote(String textValue) override;
        String toManifest() override;
        long getValue();
        void setValue(long value);
        void setMin(long min);
        void setMax(long max);

    private:
        long min;
        long max;
        bool minSet;
        bool maxSet;
        OutputBuffer* outputBuffer;
        long value;
};

#endif
#ifndef GateValuesSet_h
#define GateValuesSet_h

#include <Arduino.h>
#include "../values/GateValue.h"

class GateValuesSet
{
    public:
        GateValuesSet();
        void set(GateValue* value);
        int size();
        GateValue* get(int index);
        int find(int id);
        void clear();

    private:
        int length;
        GateValue** values;
};

#endif
#ifndef GateList_h
#define GateList_h

#include <Arduino.h>

template <typename T>
class GateList
{
    public:
        GateList();
        void push(T value);
        int size();
        T get(int index);

    private:
        int length;
        T* elements;
};

#endif
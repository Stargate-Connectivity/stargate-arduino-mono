#ifndef GateList_h
#define GateList_h

#include <Arduino.h>
#include <vector>

template <typename T>
class GateList
{
    public:
        GateList();
        void push(T value);
        int size();
        T get(int index);
        int stringLength;

    private:
        std::vector<T> elements;
};

#endif
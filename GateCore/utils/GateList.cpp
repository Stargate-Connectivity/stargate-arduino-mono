#include "GateList.h"

template <typename T>
GateList<T>::GateList() {
    this->length = 0;
    this->stringLength = 0;
}

template <typename T>
void GateList<T>::push(T value) {
    if (this->length == 0) {
        this->length = 1;
        this->elements = new T[1];
        this->elements[0] = value;
    } else {
        T* newElements = new T[this->length + 1];
        for (int i = 0; i < this->length; i++) {
            newElements[i] = this->elements[i];
        }
        newElements[this->length] = value;
        delete[] this->elements;
        this->elements = newElements;
        this->length++;
    }
}

template <typename T>
int GateList<T>::size() {
    return this->length;
}

template <typename T>
T GateList<T>::get(int index) {
    return this->elements[index];
}

template class GateList<int>;
template class GateList<String>;
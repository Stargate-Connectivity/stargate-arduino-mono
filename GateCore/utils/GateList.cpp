#include "GateList.h"

template <typename T>
GateList<T>::GateList() {
    this->stringLength = 0;
}

template <typename T>
void GateList<T>::push(T value) {
    this->elements.resize(this->elements.size() + 1);
    this->elements.push_back(value);
}

template <typename T>
int GateList<T>::size() {
    return this->elements.size();
}

template <typename T>
T GateList<T>::get(int index) {
    return this->elements.at(index);
}

template class GateList<int>;
template class GateList<String>;
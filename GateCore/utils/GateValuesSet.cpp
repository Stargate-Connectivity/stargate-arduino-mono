#include "GateValuesSet.h"

GateValuesSet::GateValuesSet() {
    this->length = 0;
}

void GateValuesSet::set(GateValue* value) {
    if (this->length == 0) {
        this->length = 1;
        this->values = new GateValue*[1];
        this->values[0] = value;
    } else {
        bool isNew = true;
        for (int i = 0; i < this->length; i++) {
            if (this->values[i]->id == value->id) {
                isNew = false;
                break;
            }
        }
        if (isNew) {
            GateValue** newElements = new GateValue*[this->length + 1];
            for (int i = 0; i < this->length; i++) {
                newElements[i] = this->values[i];
            }
            newElements[this->length] = value;
            delete[] this->values;
            this->values = newElements;
            this->length++;
        }
    }
}

int GateValuesSet::find(int id) {
    for (int i = 0; i < this->length; i++) {
        if (this->values[i]->id == id) {
            return i;
        }
    }
    return -1;
}

int GateValuesSet::size() {
    return this->length;
}

GateValue* GateValuesSet::get(int index) {
    return this->values[index];
}

void GateValuesSet::clear() {
    if (this->length > 0) {
        delete[] this->values;
        this->length = 0;
    }
}
#include "ValueFactory.h"

GateBool ValueFactory::createBool(String direction) {
    GateBool value(this->outputBuffer);
    value.direction = GateValue::getDirection(direction);
    this->valuesSet.set(&value);
    return value;
}

GateInt ValueFactory::createInt(String direction) {
    GateInt value(this->outputBuffer);
    value.direction = GateValue::getDirection(direction);
    this->valuesSet.set(&value);
    return value;
}

GateFloat ValueFactory::createFloat(String direction) {
    GateFloat value(this->outputBuffer);
    value.direction = GateValue::getDirection(direction);
    this->valuesSet.set(&value);
    return value;
}

GateString ValueFactory::createString(String direction) {
    GateString value(this->outputBuffer);
    value.direction = GateValue::getDirection(direction);
    this->valuesSet.set(&value);
    return value;
}

GateValuesSet* ValueFactory::getValues() {
    return &this->valuesSet;
}

void ValueFactory::addValue(GateValue* value) {
    this->valuesSet.set(value);
}
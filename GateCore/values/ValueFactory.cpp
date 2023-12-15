#include "ValueFactory.h"

GateBool ValueFactory::createBool(String direction) {
    GateBool value(this->outputBuffer);
    value.direction = GateValue::getDirection(direction);
    this->valuesMap[value.id] = &value;
    return value;
}

GateInt ValueFactory::createInt(String direction) {
    GateInt value(this->outputBuffer);
    value.direction = GateValue::getDirection(direction);
    this->valuesMap[value.id] = &value;
    return value;
}

GateFloat ValueFactory::createFloat(String direction) {
    GateFloat value(this->outputBuffer);
    value.direction = GateValue::getDirection(direction);
    this->valuesMap[value.id] = &value;
    return value;
}

GateString ValueFactory::createString(String direction) {
    GateString value(this->outputBuffer);
    value.direction = GateValue::getDirection(direction);
    this->valuesMap[value.id] = &value;
    return value;
}

std::map<int, GateValue*> ValueFactory::getValues() {
    return this->valuesMap;
}

void ValueFactory::addValue(GateValue* value) {
    this->valuesMap[value->id] = value;
}
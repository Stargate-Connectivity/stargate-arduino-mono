#include "GateValue.h"

int GateValue::nextId = 1;
String GateValue::getDirectionString(int direction) {
    switch (direction) {
        case 0:
            return "output";
        case 1:
            return "input";
        default:
            return "output";
    }
}

int GateValue::getDirection(String directionString) {
    switch (directionString.charAt(0)) {
        case 'o':
            return 0;
        case 'i':
            return 1;
        default:
            return 0;
    }
}

GateValue::GateValue() {
    this->onRemoteChange = []() {};
    this->id = GateValue::nextId;
    GateValue::nextId++;
}

void GateValue::setName(String valueName) {
    this->valueName = valueName;
}

String GateValue::toPartialManifest() {
    String manifest = "{\"id\":\"" + String(this->id)
            + "\",\"type\":\"" + this->type
            + "\",\"valueName\":\"" + this->valueName
            + "\",\"direction\":\"" + GateValue::getDirectionString(this->direction)
            + "\"";
    if (this->info.length() > 0) {
        manifest += ",\"info\":\"" + this->info + "\"";
    }
    return manifest;
}


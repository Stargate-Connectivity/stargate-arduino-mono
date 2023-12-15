#include "GateString.h"

GateString::GateString(OutputBuffer* outputBuffer) : GateValue() {
    this->type = "string";
    this->outputBuffer = outputBuffer;
}

String GateString::toString() {
    return this->value;
}

void GateString::fromRemote(String textValue) {
    this->setValue(textValue);
    this->onRemoteChange();
}

String GateString::toManifest() {
    String manifest = this->toPartialManifest();
    if (this->minLengthSet) {
        manifest += ",\"options\":{\"minimumLength\":" + String(this->minimumLength) + "}}";
    } else {
        manifest += "}";
    }
    return manifest;
}

String GateString::getValue() {
    return this->value;
}

void GateString::setValue(String value) {
    this->value = value;
    this->outputBuffer->sendValue(this);
}

void GateString::setMinimumLength(int length) {
    this->minLengthSet = true;
    this->minimumLength = length;
}


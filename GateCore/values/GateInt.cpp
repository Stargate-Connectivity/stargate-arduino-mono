#include "GateInt.h"

GateInt::GateInt(OutputBuffer* outputBuffer) : GateValue() {
    this->type = "integer";
    this->outputBuffer = outputBuffer;
    this->minSet = false;
    this->maxSet = false;
}

String GateInt::toString() {
    return String(this->value);
}

void GateInt::fromRemote(String textValue) {
    this->setValue(textValue.toInt());
    this->onRemoteChange();
}

String GateInt::toManifest() {
    String manifest = this->toPartialManifest();
    if (this->minSet || this->maxSet) {
        String rangeMin = this->minSet ? String(this->min) : "null";
        String rangeMax = this->maxSet ? String(this->max) : "null";
        manifest += ",\"options\":{\"range\":[" + rangeMin + "," + rangeMax + "]}}";
    } else {
        manifest += "}";
    }
    return manifest;
}

long GateInt::getValue() {
    return this->value;
}

void GateInt::setValue(long value) {
    if(this->value != value) {
        if (this->minSet && value < this->min) {
            value = this->min;
        } else if (this->maxSet && value > this->max) {
            value = this->max;
        }
        this->value = value;
        this->outputBuffer->sendValue(this);
    }
}

void GateInt::setMin(long min) {
    this->minSet = true;
    this->min = min;
    if (this->value < min) {
        this->value = min;
    }
}

void GateInt::setMax(long max) {
    this->maxSet = true;
    this->max = max;
    if (this->value > max) {
        this->value = max;
    }
}

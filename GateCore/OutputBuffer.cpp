#include "OutputBuffer.h"

void OutputBuffer::loop() {
    if (this->buffer.size() > 0) {
        String ids = "";
        String lengths = "";
        String values = "";
        for (int i = 0; i < this->buffer.size(); i++) {
            ids += (this->buffer.get(i)->id == -1 ? "@ping" : String(this->buffer.get(i)->id)) + ",";
            String textValue = this->buffer.get(i)->toString();
            lengths += String(textValue.length()) + ",";
            values += textValue;
        }
        ids.remove(ids.length() - 1);
        lengths.remove(lengths.length() - 1);
        this->device->send(ids + "|" + lengths + "|" + values);
        this->buffer.clear();
    }
}

void OutputBuffer::sendValue(GateValue* value) {
    if (value->subscribed) {
        this->buffer.set(value);
    }
}

void OutputBuffer::clear() {
    this->buffer.clear();
}
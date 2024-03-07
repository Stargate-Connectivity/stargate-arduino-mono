#include "OutputBuffer.h"

OutputBuffer::OutputBuffer() {
    this->lastMessageAcknowledged = true;
    this->functionalBuffer = new String();
}

void OutputBuffer::loop() {
    if (this->lastMessageAcknowledged && this->hasContent()) {
        this->flush();
        this->lastMessageAcknowledged = false;
    }
}

void OutputBuffer::sendValue(GateValue* value) {
    if (value->subscribed) {
        this->buffer.set(value);
    }
}

void OutputBuffer::sendFunctionalMessage(String* message) {
    this->functionalBuffer->concat(*message);
}

void OutputBuffer::sendAcknowledge() {
    this->functionalBuffer->concat("*+");
    this->flush();
}

void OutputBuffer::acknowledgeReceived() {
    this->lastMessageAcknowledged = true;
}

void OutputBuffer::reset() {
    this->buffer.clear();
    this->functionalBuffer->remove(0);
    this->lastMessageAcknowledged = true;
}

bool OutputBuffer::hasContent() {
    return (this->buffer.size() > 0) || (this->functionalBuffer->length() > 0);
}

void OutputBuffer::flush() {
    if (this->buffer.size() > 0) {
        String ids = "";
        String lengths = "";
        String values = "";
        for (int i = 0; i < this->buffer.size(); i++) {
            ids += String(this->buffer.get(i)->id) + ",";
            String textValue = this->buffer.get(i)->toString();
            lengths += String(textValue.length()) + ",";
            values += textValue;
        }
        ids.remove(ids.length() - 1);
        lengths.remove(lengths.length() - 1);
        this->functionalBuffer->concat(ids + "|" + lengths + "|" + values);
        this->buffer.clear();
    }
    this->device->send(this->functionalBuffer);
    this->functionalBuffer->remove(0);
}
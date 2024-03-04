#include "OutputBuffer.h"

OutputBuffer::OutputBuffer() {
    this->lastMessageAcknowledged = true;
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

void OutputBuffer::sendFunctionalMessage(String message) {
    this->functionalBuffer += message;
}

void OutputBuffer::sendAcknowledge() {
    this->functionalBuffer += "*+";
    this->flush();
}

void OutputBuffer::acknowledgeReceived() {
    this->lastMessageAcknowledged = true;
}

void OutputBuffer::reset() {
    this->buffer.clear();
    this->functionalBuffer = "";
    this->lastMessageAcknowledged = true;
}

bool OutputBuffer::hasContent() {
    return (this->buffer.size() > 0) || (this->functionalBuffer.length() > 0);
}

void OutputBuffer::flush() {
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
        this->functionalBuffer += ids + "|" + lengths + "|" + values;
        this->buffer.clear();
    }
    this->device->send(this->functionalBuffer);
    this->functionalBuffer = "";
}
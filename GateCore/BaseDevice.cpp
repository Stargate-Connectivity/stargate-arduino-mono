#include "BaseDevice.h"

BaseDevice::BaseDevice() {
    this->deviceStarted = false;
    this->outputBuffer.device = this;
    this->serverStorage.device = this;
    this->factory.outputBuffer = &this->outputBuffer;
};

void BaseDevice::setName(String name) {
    if (!this->deviceStarted) {
        this->deviceName = name;
    }
}

void BaseDevice::setGroup(String name) {
    if (!this->deviceStarted) {
        this->groupName = name;
    }
}

void BaseDevice::sendFunctionalMessage(String* message) {
    this->outputBuffer.sendFunctionalMessage(message);
}
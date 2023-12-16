#include "BaseDevice.h"

BaseDevice::BaseDevice() {
    this->deviceStarted = false;
    this->deviceName = "";
    this->outputBuffer.device = this;
    this->factory.outputBuffer = &this->outputBuffer;
};

void BaseDevice::setName(String name) {
    if (!this->deviceStarted) {
        this->deviceName = name;
    }
}

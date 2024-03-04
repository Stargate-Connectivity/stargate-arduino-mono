#ifndef BaseDevice_h
#define BaseDevice_h

#include <Arduino.h>
#include "Sender.h"
#include "OutputBuffer.h"
#include "values/ValueFactory.h"
#include "ServerStorage.h"

class BaseDevice : public Sender
{
    public:
        BaseDevice();
        void setName(String name);
        void setGroup(String name);
        virtual void start() = 0;
        virtual void loop() = 0;
        virtual bool isReady() = 0;
        void sendFunctionalMessage(String message) override;
        ValueFactory factory;
        ServerStorage serverStorage;

    protected:
        OutputBuffer outputBuffer;
        bool deviceStarted;
        String deviceName;
        String groupName;
};

#endif
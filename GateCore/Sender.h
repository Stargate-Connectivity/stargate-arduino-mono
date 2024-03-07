#ifndef Sender_h
#define Sender_h

#include <Arduino.h>

class Sender
{
    public:
        virtual void send(String* message) = 0;
        virtual void sendFunctionalMessage(String* message) = 0;
};

#endif
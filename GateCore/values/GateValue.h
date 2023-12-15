#ifndef GateValue_h
#define GateValue_h

#include <Arduino.h>

class GateValue
{
    public:
        GateValue();
        int id;
        String valueName;
        int direction;
        bool subscribed;
        virtual String toString() = 0;
        virtual void fromRemote(String textValue) = 0;
        virtual String toManifest() = 0;
        static int getDirection(String directionString);
        std::function<void()> onRemoteChange;
        void setName(String valueName);

    protected:
        String type;
        String toPartialManifest();
        void send();

    private:
        static int nextId;
        static String getDirectionString(int direction);
};

#endif
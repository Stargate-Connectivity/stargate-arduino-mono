#ifndef ServerStorage_h
#define ServerStorage_h

#include <Arduino.h>
#include "Sender.h"

class ServerStorage
{
    public:
        #ifdef __AVR__
            typedef void (*StorageGetCallback)(String data);
        #else
            typedef std::function<void(String data)> StorageGetCallback;
        #endif
        ServerStorage();
        Sender* device;
        void get(String key, StorageGetCallback onResponse);
        void get(String key, String directory, StorageGetCallback onResponse);
        void set(String key, String value);
        void set(String key, String value, String directory);
        void append(String key, String value);
        void append(String key, String value, String directory);
        void remove();
        void remove(String key);
        void remove(String key, String directory);
        void handleGetResponse(String message);

    private:
        StorageGetCallback onGetResponse;
        String oneParam(String p1);
        String twoParams(String p1, String p2);
        String threeParams(String p1, String p2, String p3);
};

#endif
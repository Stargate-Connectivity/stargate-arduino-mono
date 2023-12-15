#include "MessageHandler.h"

std::list<String> parseArray(String array) {
    std::list<String> params;
    int separatorIndex = array.indexOf('|');
    String lengths = array.substring(0, separatorIndex);
    String values = array.substring(separatorIndex + 1);
    int nextValueIndex;
    int valueLength;
    while(true) {
        nextValueIndex = lengths.indexOf(',');
        if (nextValueIndex == -1) {
            params.push_back(values);
            break;
        } else {
            valueLength = lengths.substring(0, nextValueIndex).toInt();
            params.push_back(values.substring(0, valueLength));
            lengths = lengths.substring(nextValueIndex + 1);
            values = values.substring(valueLength);
        }
    }
    return params;
}

String createManifest(String deviceName, std::map<int, GateValue*> valuesMap) {
    String manifest("{");
    EEPROM.begin(40);
    delay(10);
    if (EEPROM.read(0) == 0xFF) {
        String id("");
        byte currentByte;
        int currentIndex = 1;
        while(true) {
            currentByte = EEPROM.read(currentIndex);
            if (currentByte == 0) {
                break;
            } else {
                currentIndex++;
                id += (char) currentByte;
            }
        }
        manifest += "\"id\":\"" + id + "\", ";
    }
    EEPROM.end();
    manifest += "\"deviceName\":\"" + deviceName + "\", \"values\":[";
    for (auto valueEntry : valuesMap) {
        manifest += valueEntry.second->toManifest() + ",";
    }
    manifest.remove(manifest.length() - 1);
    manifest += "]}";
    return manifest;
}

void handleIdAssigned(String idAssignedMessage) {
    String idValue = idAssignedMessage.substring(13);
    int separatorIndex = idValue.indexOf('|');
    String id = idValue.substring(separatorIndex + 1);
    int bufferSize = id.length() + 1;
    byte *buffer = new byte[bufferSize];
    id.getBytes(buffer, bufferSize);
    EEPROM.begin(40);
    delay(10);
    EEPROM.write(0, 0xFF);
    for(int i = 0; i < bufferSize; i++) {
        EEPROM.write(i + 1, buffer[i]);
    }
    EEPROM.end();
}

void handleValueMessage(String message, std::map<int, GateValue*> valuesMap) {
    std::map<int, String> messageMap;
    int separatorIndex = message.indexOf('|');
    if (separatorIndex != -1) {
        String ids = message.substring(0, separatorIndex);
        String values = message.substring(separatorIndex + 1);
        std::list<int> idsList;
        int nextIdIndex = 0;
        while(true) {
            nextIdIndex = ids.indexOf(',');
            if (nextIdIndex == -1) {
                idsList.push_back(ids.toInt());
                break;
            } else {
                idsList.push_back(ids.substring(0, nextIdIndex).toInt());
                ids = ids.substring(nextIdIndex + 1);
            }
        }
        std::list<String> valuesList = parseArray(values);
        if (idsList.size() == valuesList.size()) {
            auto idsIterator = idsList.begin();
            auto valuesIterator = valuesList.begin();
            for (int i = 0; i < idsList.size(); i++) {
                auto entry = valuesMap.find(*idsIterator);
                if (entry == valuesMap.end()) {
                    continue;
                } else if (entry->second->direction == 1) {
                    entry->second->fromRemote(*valuesIterator);
                }
                std::advance(idsIterator, 1);
                std::advance(valuesIterator, 1);
            }
        }
    }
}

void handleSubscription(bool subscribed, String message, std::map<int, GateValue*> valuesMap, OutputBuffer* outputBuffer) {
    int separatorIndex = message.indexOf('|');
    if (separatorIndex != -1) {
        String idsString = message.substring(separatorIndex + 1);
        auto ids = parseArray(idsString);
        for (auto id : ids) {
            auto entry = valuesMap.find(id.toInt());
            if (entry == valuesMap.end()) {
                continue;
            } else {
                entry->second->subscribed = subscribed;
                if (subscribed) {
                    outputBuffer->sendValue(entry->second);
                }
            }
        }
    }
}
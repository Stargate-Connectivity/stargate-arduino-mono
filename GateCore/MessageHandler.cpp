#include "MessageHandler.h"

GateList<String> parseArray(String array) {
    GateList<String> params;
    int separatorIndex = array.indexOf('|');
    if (separatorIndex != -1) {
        String lengths = array.substring(0, separatorIndex);
        String values = array.substring(separatorIndex + 1);
        params.stringLength = lengths.length() + values.length() + 1;
        int nextValueIndex;
        int valueLength;
        while(true) {
            nextValueIndex = lengths.indexOf(',');
            if (nextValueIndex == -1) {
                params.push(values);
                break;
            } else {
                valueLength = lengths.substring(0, nextValueIndex).toInt();
                if (valueLength > values.length()) {
                    return GateList<String>();
                }
                params.push(values.substring(0, valueLength));
                lengths = lengths.substring(nextValueIndex + 1);
                values = values.substring(valueLength);
            }
        }
    }
    return params;
}

String createManifest(String deviceName, String deviceGroup, GateValuesSet* valuesSet) {
    String manifest("{");
    #ifdef __AVR__
        EEPROM.begin();
    #else
        EEPROM.begin(40);
        delay(10);
    #endif
    if (EEPROM.read(0) == 0xFF) {
        String id("");
        byte currentByte;
        int currentIndex = 1;
        bool success = true;
        while(true) {
            currentByte = EEPROM.read(currentIndex);
            if (currentByte == 0) {
                break;
            } else {
                currentIndex++;
                if (isAscii((char) currentByte)) {
                    id += (char) currentByte;
                } else {
                    success = false;
                    break;
                }
            }
        }
        if (success) {
            manifest += "\"id\":\"" + id + "\", ";
        } else {
            EEPROM.write(0, 0);
        }
    }
    EEPROM.end();
    manifest += "\"deviceName\":\"" + deviceName + "\"";
    if (deviceGroup.length() > 0) {
        manifest += ", \"group\":\"" + deviceGroup + "\"";
    }
    manifest += ", \"values\":[";
    if (valuesSet->size() > 0) {
        for (int i = 0; i < valuesSet->size(); i++) {
            manifest += valuesSet->get(i)->toManifest() + ",";
        }
        manifest.remove(manifest.length() - 1);
    }
    manifest += "]}";
    return manifest;
}

void handleIdAssigned(String id) {
    int bufferSize = id.length() + 1;
    byte *buffer = new byte[bufferSize];
    id.getBytes(buffer, bufferSize);
    #ifdef __AVR__
        EEPROM.begin();
    #else
        EEPROM.begin(40);
        delay(10);
    #endif
    EEPROM.write(0, 0xFF);
    for(int i = 0; i < bufferSize; i++) {
        EEPROM.write(i + 1, buffer[i]);
    }
    EEPROM.end();
}

void handleValueMessage(String message, GateValuesSet* valuesSet) {
    message.trim();
    if (message.length() > 0) {
        int separatorIndex = message.indexOf('|');
        if (separatorIndex != -1) {
            String ids = message.substring(0, separatorIndex);
            String values = message.substring(separatorIndex + 1);
            GateList<int> idsList;
            int nextIdIndex = 0;
            while(true) {
                nextIdIndex = ids.indexOf(',');
                if (nextIdIndex == -1) {
                    idsList.push(ids.toInt());
                    break;
                } else {
                    idsList.push(ids.substring(0, nextIdIndex).toInt());
                    ids = ids.substring(nextIdIndex + 1);
                }
            }
            GateList<String> valuesList = parseArray(values);
            if ((idsList.size() > 0) && (idsList.size() == valuesList.size())) {
                for (int i = 0; i < idsList.size(); i++) {
                    int valueIndex = valuesSet->find(idsList.get(i));
                    if (valueIndex > -1) {
                        if (valuesSet->get(valueIndex)->direction == 1) {
                            valuesSet->get(valueIndex)->fromRemote(valuesList.get(i));
                        }
                    }
                }
            }
        }
    }
}

int handleSubscription(bool subscribed, String message, GateValuesSet* valuesSet, OutputBuffer* outputBuffer) {
    int separatorIndex = message.indexOf('|');
    int messageLength = separatorIndex + 1;
    if (separatorIndex != -1) {
        String idsString = message.substring(separatorIndex + 1);
        GateList<String> ids = parseArray(idsString);
        messageLength += ids.stringLength;
        for (int i = 0; i < ids.size(); i++) {
            int valueIndex = valuesSet->find(ids.get(i).toInt());
            if (valueIndex > -1) {
                valuesSet->get(valueIndex)->subscribed = subscribed;
                if (subscribed) {
                    outputBuffer->sendValue(valuesSet->get(valueIndex));
                }
            }
        }
    }
    return messageLength;
}
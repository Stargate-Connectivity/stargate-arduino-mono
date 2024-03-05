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

void handleManifestRequest(String* message, BaseDevice* device) {
    String response = "*>manifest|";
    String manifest = createManifest(device->deviceName, device->groupName, device->factory.getValues());
    device->outputBuffer.sendFunctionalMessage(response + String(manifest.length()) + "|" + manifest);
    message->remove(0, 12);
}

void handleTypeRequest(String* message, BaseDevice* device) {
    String response = "*>type|6|device";
    device->outputBuffer.sendFunctionalMessage(response);
    message->remove(0, 8);
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
            manifest += "\"id\":\"" + id + "\",";
        } else {
            EEPROM.write(0, 0);
        }
    }
    EEPROM.end();
    manifest += "\"deviceName\":\"" + deviceName + "\"";
    if (deviceGroup.length() > 0) {
        manifest += ",\"group\":\"" + deviceGroup + "\"";
    }
    manifest += ",\"values\":[";
    if (valuesSet->size() > 0) {
        for (int i = 0; i < valuesSet->size(); i++) {
            manifest += valuesSet->get(i)->toManifest() + ",";
        }
        manifest.remove(manifest.length() - 1);
    }
    manifest += "]}";
    return manifest;
}

void handleIdAssigned(String* message) {
    message->remove(0, 13);
    int separatorIndex = message->indexOf('|');
    int idLength = message->substring(0, separatorIndex).toInt();
    message->remove(0, separatorIndex + 1);
    String id = message->substring(0, idLength);
    message->remove(0, idLength);

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

void handleServerStorageGetResponse(String* message, BaseDevice* device) {
    message->remove(0, 13);
    int separatorIndex = message->indexOf('|');
    int responseLength = message->substring(0, separatorIndex).toInt();
    message->remove(0, separatorIndex + 1);
    device->serverStorage.handleGetResponse(message->substring(0, responseLength));
    message->remove(0, responseLength);
}

void handleSubscription(bool subscribed, String* message, BaseDevice* device) {
    int separatorIndex = message->indexOf('|');
    int messageLength = separatorIndex + 1;
    if (separatorIndex != -1) {
        String idsString = message->substring(separatorIndex + 1);
        GateList<String> ids = parseArray(idsString);
        messageLength += ids.stringLength;
        GateValuesSet* valuesSet = device->factory.getValues();
        for (int i = 0; i < ids.size(); i++) {
            int valueIndex = valuesSet->find(ids.get(i).toInt());
            if (valueIndex > -1) {
                valuesSet->get(valueIndex)->subscribed = subscribed;
                if (subscribed) {
                    device->outputBuffer.sendValue(valuesSet->get(valueIndex));
                }
            }
        }
    }
    message->remove(0, messageLength);
}

void handleValueMessage(String* message, GateValuesSet* valuesSet) {
    message->trim();
    if (message->length() > 0) {
        int separatorIndex = message->indexOf('|');
        if (separatorIndex != -1) {
            String ids = message->substring(0, separatorIndex);
            String values = message->substring(separatorIndex + 1);
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

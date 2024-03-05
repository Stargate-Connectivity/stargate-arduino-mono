#ifndef MessageHandler_h
#define MessageHandler_h

#include <Arduino.h>
#include <EEPROM.h>
#include "BaseDevice.h"
#include "OutputBuffer.h"
#include "values/GateValue.h"
#include "utils/GateList.h"
#include "utils/GateValuesSet.h"

GateList<String> parseArray(String array);
void handleManifestRequest(String* message, BaseDevice* device);
void handleTypeRequest(String* message, BaseDevice* device);
void handleIdAssigned(String* message);
void handleServerStorageGetResponse(String* message, BaseDevice* device);
void handleValueMessage(String* message, GateValuesSet* valuesSet);
String createManifest(String deviceName, String deviceGroup, GateValuesSet* valuesSet);
void handleSubscription(bool subscribed, String* message, BaseDevice* device);

#endif
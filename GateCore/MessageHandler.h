#ifndef MessageHandler_h
#define MessageHandler_h

#include <Arduino.h>
#include <EEPROM.h>
#include "OutputBuffer.h"
#include "values/GateValue.h"
#include "utils/GateList.h"
#include "utils/GateValuesSet.h"

GateList<String> parseArray(String array);
void handleValueMessage(String message, GateValuesSet* valuesSet);
String createManifest(String deviceName, String deviceGroup, GateValuesSet* valuesSet);
void handleIdAssigned(String idAssignedMessage);
void handleSubscription(bool subscribed, String message, GateValuesSet* valuesSet, OutputBuffer* outputBuffer);

#endif
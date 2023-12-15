#ifndef MessageHandler_h
#define MessageHandler_h

#include <list>
#include <Arduino.h>
#include <EEPROM.h>
#include <map>
#include "values/GateValue.h"
#include "OutputBuffer.h"

std::list<String> parseArray(String array);
void handleValueMessage(String message, std::map<int, GateValue*> valuesMap);
String createManifest(String deviceName, std::map<int, GateValue*> valuesMap);
void handleIdAssigned(String idAssignedMessage);
void handleSubscription(bool subscribed, String message, std::map<int, GateValue*> valuesMap, OutputBuffer* outputBuffer);

#endif
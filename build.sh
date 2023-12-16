#!/bin/bash

rm -rf ./ESP8266GateDevice/src/lib
mkdir ./ESP8266GateDevice/src/lib
cp -r ./GateCore ./ESP8266GateDevice/src/lib
cp -r ./GateDevice ./ESP8266GateDevice/src/lib

rm -rf ./SerialGateDevice/src/lib
mkdir ./SerialGateDevice/src/lib
cp -r ./GateCore ./SerialGateDevice/src/lib
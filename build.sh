#!/bin/bash

rm -rf ./ESP8266GateDevice/src/lib
mkdir ./ESP8266GateDevice/src/lib
cp -r ./GateCore ./ESP8266GateDevice/src/lib
cp -r ./GateDevice ./ESP8266GateDevice/src/lib
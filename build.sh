#!/bin/bash

rm -rf ./WiFiGateDevice/src/lib
mkdir ./WiFiGateDevice/src/lib
cp -r ./GateCore ./WiFiGateDevice/src/lib
cp -r ./GateDevice ./WiFiGateDevice/src/lib

rm -rf ./SerialGateDevice/src/lib
mkdir ./SerialGateDevice/src/lib
cp -r ./GateCore ./SerialGateDevice/src/lib
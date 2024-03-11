# Monorepo for Arduino libraries

## Local setup
1. Clone
2. Run build.sh script (will copy dependencies to proper locations)
3. Compress WiFiGateDevice and SerialGateDevice into .zip files
4. From Arduino IDE, select \
Sketch -> Include Library -> Add .ZIP Library
5. Add WiFiGateDevice.zip and SerialGateDevice.zip

## Repo structure
**GateCore**: reusable code \
**GateDevice**: base class for network-enabled boards (abstract) \
**SerialGateDevice**: library for boards without network interface \
**WiFiGateDevice**: library for boards with WiFi communication

## Dependencies
WiFiGateDevice requires WebSockets library. In case library would not be installed automatically,
need to add it manually from Arduino IDE:
1. Sketch -> Include Library -> Manage Libraries...
2. Type "websockets" in search field
3. Click "INSTALL" on "WebSockets by Markus Sattler" library

## Usage with various boards
Libraries follows same semantics, so differences in usage with various boards lies within including proper header files and creating device object of proper class.
Each library has examples demonstrating proper usage in conjunction with specific board family.

Examples available from Arduino IDE under: \
File -> Examples (section "Examples from Custom Libraries" on the bottom of the list)

- **All boards without network interface (or if not willing to use embedded network interface):** \
Include SerialGateDevice.h, connect board to USB port of machine running GateHub (app from stargate-js-mono repository)
- **ESP-32 or equivalent:** \
Include WiFi.h, WiFiGateDevice.h
- **ESP-8266 or equivalent:** \
Include ESP8266WiFi.h, WiFiGateDevice.h
- **Raspberry Pico W** \
Include WiFiGateDevice.h
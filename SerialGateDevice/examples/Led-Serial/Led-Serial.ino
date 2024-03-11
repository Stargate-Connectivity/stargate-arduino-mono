/*
  Example for all boards

  Allows remote control of Led Diode connected to pin 3.

  To access device:
  1. Download LocalServer and GateHub from Stargate page
  2. Run LocalServer on machine within the same network
  3. Upload program to your board
  4. Run GateHub on machine connected with your board by USB cable
    (if you need to upload your program again, stop GateHub first)

  Before uploading, make sure ledPin on your board is an analog pin
  and is not RX/TX pin (library is using RX/TX to communicate)
*/

#include <SerialGateDevice.h>

int ledPin = 3;

SerialGateDevice device;
GateInt led = device.factory.createInt("input");

void setup() {
  pinMode(ledPin, OUTPUT);
  device.setName("Led - Serial");
  led.setName("Brightness");
  led.setMin(0);
  led.setMax(255);
  led.onRemoteChange = onLedChange;
  device.start();
}

void loop() {
  device.loop();
}

void onLedChange() {
  analogWrite(ledPin, led.getValue());
}
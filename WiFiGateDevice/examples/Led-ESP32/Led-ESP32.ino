/*
  Example for ESP32 family boards

  Allows remote control of Led Diode connected to D1 pin.
  To access device, download LocalServer from Stargate page
  and run it on machine within the same network.

  Before uploading, replace <YOUR WIFI NAME> and <YOUR WIFI PASSWORD>
  with your WiFi credentials
*/

#include <WiFi.h>
#include <WiFiGateDevice.h>

int ledPin = D1;

WiFiGateDevice device("<YOUR WIFI NAME>", "<YOUR WIFI PASSWORD>");
GateInt led = device.factory.createInt("input");

void setup() {
  pinMode(ledPin, OUTPUT);
  device.setName("Led - ESP32");
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

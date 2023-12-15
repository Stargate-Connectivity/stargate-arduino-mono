#include <ESP8266GateDevice.h>
#include "max6675.h"
#define CS 15
#define CLK 14
#define MISO 12

MAX6675 thermocouple(CLK, CS, MISO);

ESP8266GateDevice device("your-wifi-ssid", "your-wifi-password");

GateFloat temperature = device.factory.createFloat("output");

void setup() {
  device.setName("Thermocouple");
  temperature.setName("°C");
  temperature.setMin(0);
  temperature.setMax(800);
  device.start();
}

void loop() {
  device.loop();
  if (device.isReady()) {
    temperature.setValue(thermocouple.readCelsius());
    delay(1000);
  }
}

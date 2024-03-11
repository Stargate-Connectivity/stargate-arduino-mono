/*
  Example of observing device state and value subscription state

  Led Diode connected to GP1 pin behaves as follows:
    - no connection with LocalServer - led off;
    - device connected, observedValue not subscribed - blinks
    - observedValue subscribed - led on

  Usage:
    1. Upload sketch to your board - led should be off
    2. Start LocalServer - led should blik indicating successful connection
      (some boards needs restart after uploading program)
    3. Open LocalServer in browser - led should be on
      (page subscribes all visible values)
    4. Close browser - led should blink
      (values are unsubscribed on closing page)
    5. Stop LocalServer - led should be off indicating connection lost

  Example written for Raspberry Pico W.
  To use with other boards, include relevant WiFi library (as in Led-xxx example)

  Before uploading, replace <YOUR WIFI NAME> and <YOUR WIFI PASSWORD>
  with your WiFi credentials
*/

#include <WiFiGateDevice.h>

int ledPin = 1;

WiFiGateDevice device("<YOUR WIFI NAME>", "<YOUR WIFI PASSWORD>");
GateBool observedValue = device.factory.createBool("output");

bool ledState = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  device.setName("State Observer");
  device.start();
}

void loop() {
  device.loop();
  if (device.isReady()) {
    if (observedValue.subscribed) {
      digitalWrite(ledPin, HIGH);
    } else {
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
      delay(200);
    }
  } else {
    digitalWrite(ledPin, LOW);
  }
}

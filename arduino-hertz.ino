/**
   Mock Mode
   =========
   If enabled, will mock a sine wave signal instead of reading physical sensor.
*/
#define MOCK_MODE false

#if MOCK_MODE

#include "mock.c"

#else

#define tab "\t"

#include <Arduino.h>
#include<hardwareSerial.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include "lib.c"
#include "config.h"

#if defined(ARDUINO_ARCH_SAMD)

#define Serial SerialUSB

#endif

Adafruit_LIS3DH sensor = Adafruit_LIS3DH();

void setup() {
  // Temporarily pause serial shields
#ifndef ESP8266
  while (!Serial);
#endif
  Serial.begin(BAUD_RATE);
  Serial.println("Connecting to sensor...");
  if (! sensor.begin(0x18)) {
    while (1);
  }
  Serial.println("Connected.");
  sensor.setRange(SENSOR_RANGE);
}

void loop() {
  sensor.read();
  Serial.print(sensor.x);
  Serial.print(tab);
  Serial.print(sensor.y);
  Serial.print(tab);
  Serial.println(sensor.z);
}

#endif

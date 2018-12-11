#include <Wire.h>
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
    Serial.println(sensor.x, sensor.y, sensor.z);
}
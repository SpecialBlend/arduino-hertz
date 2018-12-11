#include <Arduino.h>
#include<hardwareSerial.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include "lib.c"
#include "config.c"
#include "axis.c"
#include "calibration.c"

#if defined(ARDUINO_ARCH_SAMD)

#define Serial SerialUSB

#endif

Adafruit_LIS3DH sensor = Adafruit_LIS3DH();
Axis_s x, y, z;

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
  x = Axis();
  y = Axis();
  z = Axis();
  if (!get_axis_calibration_state(&x)) {
    //    calibrate();
  }
}

void loop() {
  unsigned long time = millis() / 1000;
  unsigned long offset = (time % 2);
  sensor.read();
  set_amplitude(&z, sensor.z);
  calibrate_axis(&z, CALIBRATION_ZERO_SENSITIVITY, CALIBRATION_FILTER_SENSITIVITY);
  float raw = z.amplitude_s.raw;
  float zero = z.calibration_s.zero;
  float normalized = get_amplitude_normalized(&z);
  Serial.print(raw);
  Serial.print("\t");
  Serial.print(zero);
  Serial.print("\t");
  Serial.print(normalized);
  Serial.print("\r\n");
}

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

void plotAxis(struct Axis_s *axis_s, unsigned int is_last) {
  const float raw = axis_s->amplitude_s.raw;
  const float zero = axis_s->calibration_s.zero;
  const float low = axis_s->calibration_s.low;
  const float high = axis_s->calibration_s.high;
  const float zeroed = get_amplitude_zeroed(axis_s);
  const float normalized = get_amplitude_normalized(axis_s, CALIBRATION_FILTER_SENSITIVITY);
  Serial.print(zeroed);
  Serial.print("\t");
  Serial.print(normalized);
  Serial.print("\t");
  Serial.print(raw);
  Serial.print("\t");
  Serial.print(low);
  Serial.print("\t");
  Serial.print(high);
  Serial.print("\t");
  if (is_last) {
    Serial.println(zero);
    return;
  }
  Serial.print(zero);
  Serial.print("\t");
}

void loop() {
  unsigned long time = millis() / 1000;
  unsigned long offset = (time % 2);
  sensor.read();
  set_amplitude(&x, sensor.x);
  set_amplitude(&y, sensor.y);
  set_amplitude(&z, sensor.z);
  calibrate_axis(&x, CALIBRATION_ZERO_SENSITIVITY);
  calibrate_axis(&y, CALIBRATION_ZERO_SENSITIVITY);
  calibrate_axis(&z, CALIBRATION_ZERO_SENSITIVITY);
  //  plotAxis(&x, 0);
  //  plotAxis(&y, 0);
  plotAxis(&z, 1);
}

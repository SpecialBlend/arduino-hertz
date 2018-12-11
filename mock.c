#ifndef INCLUDE_MOCK_C
#define INCLUDE_MOCK_C

//#include<hardwareSerial.h>
//#include "lib.c"
//#include "config.h"
//#include "axis.c"
//#include "frequency.c"
//
//#define MOCK_FREQUENCY_LOW 20
//#define MOCK_FREQUENCY_HIGH 60
//#define MOCK_BURST_LENGTH 2000000
//#define PI 22 / 7
//
///**
//   Lowest Target Frequency
//   ===========
//   The lowest frequency needed to measure. This is used to set the sample size.
//*/
//#define LOWEST_TARGET_FREQUENCY MOCK_FREQUENCY_LOW
//
///**
//   Simulate a sine wave tick
//*/
//float sine(float frequency, float u_time) {
//  return sin((u_time / 1000000) * frequency * 2 * PI);
//}
//
//float MOCK_FREQUENCY_CURRENT;
//
//struct Axis_s x;
//
//void setup() {
//  MOCK_FREQUENCY_CURRENT = MOCK_FREQUENCY_LOW;
//  x = Axis();
//  Serial.begin(BAUD_RATE);
//}
//
//void loop() {
//  const unsigned long u_time = micros();
//  set_amplitude(&x, sine(MOCK_FREQUENCY_CURRENT, u_time));
//  Serial.println(get_amplitude_raw(&x));
//}

#endif

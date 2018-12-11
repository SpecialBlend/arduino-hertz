#ifndef INCLUDE_AXIS_C
#define INCLUDE_AXIS_C

#include "axis.h"

struct Axis_s Axis() {
  struct AxisCalibration_s calibration_s = {0, 0, 0, 0, 0};
  struct AxisFrequency_s frequency_s = {0, 0, 0, 0};
  struct AxisAmplitude_s amplitude_s = {0, 0};
  struct Axis_s axis_s = {amplitude_s, frequency_s, calibration_s};
  return axis_s;
};

void set_amplitude(struct Axis_s *axis_s, float raw) {
  axis_s->amplitude_s.raw = raw;
};

float get_amplitude_raw(struct Axis_s *axis_s) {
  return axis_s->amplitude_s.raw;
};

float get_frequency(struct Axis_s *axis_s) {
  return axis_s->frequency_s.frequency;
}

#endif

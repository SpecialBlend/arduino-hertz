#ifndef INCLUDE_CALIBRATION_C
#define INCLUDE_CALIBRATION_C

#include "axis.h"

struct Axis_s *zero_axis(struct Axis_s *axis_s) {
  axis_s->amplitude_s.normalized = axis_s->amplitude_s.raw - axis_s->calibration_s.zero;
  return axis_s;
}

struct Axis_s *filter_axis(struct Axis_s *axis_s, float sensitivity) {
  float ceiling = sensitivity / axis_s->calibration_s.filter;
  if (axis_s->amplitude_s.normalized < ceiling) {
    axis_s->amplitude_s.normalized = 0;
  }
  return axis_s;
}

struct Axis_s *normalize_axis(struct Axis_s *axis_s, float sensitivity) {
  return filter_axis(zero_axis(axis_s), sensitivity);
}

void calibrate_zero(struct Axis_s *axis_s, float sensitivity) {
  float normalized_amplitude = normalize_axis(axis_s, sensitivity)->amplitude_s.normalized;
  axis_s->calibration_s.zero = avg(axis_s->calibration_s.zero, normalized_amplitude);
}

void calibrate_filter(struct Axis_s *axis_s, float sensitivity) {
  float normalized_amplitude = normalize_axis(axis_s, sensitivity)->amplitude_s.normalized;
  axis_s->calibration_s.filter = avg(axis_s->calibration_s.filter, max(axis_s->calibration_s.filter, abs(normalized_amplitude)));
}

void calibrate_axis(struct Axis_s *axis_s, float sensitivity) {
  calibrateZero(axis_s, sensitivity);
  calibrateFilter(axis_s, sensitivity);
}

float get_amplitude_normalized(struct Axis_s *axis_s, float sensitivity) {
  return normalize_axis(axis_s, sensitivity)->amplitude_s.normalized;
}

void set_axis_calibration_state(struct Axis_s *axis_s, unsigned int state) {
  axis_s->calibration_s.state = state;
}

unsigned int get_axis_calibration_state(struct Axis_s *axis_s) {
  return axis_s->calibration_s.state;
}

#endif

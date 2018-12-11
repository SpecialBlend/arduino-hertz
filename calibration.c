#ifndef INCLUDE_CALIBRATION_C
#define INCLUDE_CALIBRATION_C

#include "axis.h"

struct Axis_s *zero_axis(struct Axis_s *axis_s) {
  axis_s->amplitude_s.normalized = axis_s->amplitude_s.raw - axis_s->calibration_s.zero;
  return axis_s;
}

struct Axis_s *filter_axis(struct Axis_s *axis_s, float sensitivity) {
  if (axis_s->calibration_s.filter) {
    float ceiling = sensitivity / axis_s->calibration_s.filter;
    if (axis_s->amplitude_s.normalized < ceiling) {
      axis_s->amplitude_s.normalized = 0;
    }

  }
  return axis_s;
}

struct Axis_s *normalize_axis(struct Axis_s *axis_s, float sensitivity) {
  return zero_axis(axis_s);
}

void calibrate_zero(struct Axis_s *axis_s, float sensitivity) {
  float raw = axis_s->amplitude_s.raw;
  float zero = axis_s->calibration_s.zero;
  float low = axis_s->calibration_s.low;
  float high = axis_s->calibration_s.high;
  float state = axis_s->calibration_s.state;
  if (state == 0) {
    // First calibration loop
    axis_s->calibration_s.low = raw;
    axis_s->calibration_s.high = raw;
    axis_s->calibration_s.state = state + 1;
    return;
  }
  float new_low = ease(min(raw, low), raw, sensitivity);
  float new_high = ease(max(raw, high), raw, sensitivity);
  if (raw < 0) {
    const float temp = new_low;
    new_low = new_high;
    new_high = temp;
  }
  axis_s->calibration_s.low = ease(axis_s->calibration_s.low, new_low, sensitivity);
  axis_s->calibration_s.high = ease(axis_s->calibration_s.high, new_high, sensitivity);
  axis_s->calibration_s.zero = ease(axis_s->calibration_s.zero, avg(low, high), sensitivity);
}

void calibrate_filter(struct Axis_s *axis_s, float sensitivity) {
  //  float normalized_amplitude = normalize_axis(axis_s, sensitivity)->amplitude_s.normalized;
  //  axis_s->calibration_s.filter = avg(axis_s->calibration_s.filter, max(axis_s->calibration_s.filter, abs(normalized_amplitude)));
}

void calibrate_axis(struct Axis_s *axis_s, float sensitivity_zero, float sensitivity_filter) {
  calibrate_zero(axis_s, sensitivity_zero);
  calibrate_filter(axis_s, sensitivity_filter);
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

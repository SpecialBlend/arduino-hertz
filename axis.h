#ifndef INCLUDE_AXIS_H
#define INCLUDE_AXIS_H

struct AxisAmplitude_s {
  float raw, normalized;
};

struct AxisCalibration_s {
  float zero, filter, state;
};

struct AxisFrequency_s {
  unsigned long clock, cycles;
  unsigned int signal;
  float frequency;
};

struct Axis_s {
  struct AxisAmplitude_s amplitude_s;
  struct AxisFrequency_s frequency_s;
  struct AxisCalibration_s calibration_s;
};

struct Axis_s Axis();

void set_amplitude(struct Axis_s *axis_s, float raw);

float get_amplitude_raw(struct Axis_s *axis_s);

float get_frequency(struct Axis_s *axis_s);

#endif

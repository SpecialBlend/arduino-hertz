#ifndef INCLUDE_CALIBRATION_H
#define INCLUDE_CALIBRATION_H

struct Axis_s *zero_axis(struct Axis_s *axis_s);

struct Axis_s *filter_axis(struct Axis_s *axis_s, float sensitivity);

struct Axis_s *normalize_axis(struct Axis_s *axis_s, float sensitivity);

void calibrate_zero(struct Axis_s *axis_s);

void calibrate_filter(struct Axis_s *axis);

void calibrate_axis(struct Axis_s *axis_s, AxisCalibration_s *calibration_s);

float get_amplitude_normalized(struct Axis_s *axis_s);

void set_axis_calibration_state(struct Axis_s *axis_s, bool state);

unsigned int get_axis_calibration_state(struct Axis_s *axis_s);

#endif

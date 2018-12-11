#pragma once

#include "lib.c"
#include "axis.h"

Axis_s *zero_axis(Axis_s *axis_s) {
    axis_s->amplitude_s.normalized = axis_s->amplitude_s.raw - axis_s->calibration_s.zero;
    return axis_s;
}

Axis_s *filter_axis(Axis_s *axis_s, float sensitivity) {
    float ceiling = sensitivity / axis_s->calibration_s.filter;
    if (axis_s->amplitude_s.normalized < ceiling) {
        axis_s->amplitude_s.normalized = 0;
    }
    return axis_s;
}

Axis_s *normalize_axis(Axis_s *axis_s, float sensitivity) {
    return filter_axis(zero_axis(axis_s), sensitivity);
}

void calibrate_zero(Axis_s *axis_s) {
    float normalized_amplitude = normalize_axis(axis_s).amplitude_s.normalized;
    axis_s->calibration_s.zero = avg(axis_s->calibration_s.zero, normalized_amplitude);
}

void calibrate_filter(Axis_s *axis) {
    calibration_s->filter = avg(calibration_s->filter, max(calibration_s->filter, abs(axis->amplitude_s.zeroed)));
}

void calibrate_axis(Axis_s *axis_s, AxisCalibration_s *calibration_s) {
    calibrateZero(axis_s, calibration_s);
    calibrateFilter(axis_s, calibration_s);
}

float get_amplitude_normalized(Axis_s *axis_s) {
    return normalize_axis(axis_s).amplitude_s.normalized;
}

void set_axis_calibration_state(Axis_s *axis_s, bool state) {
    axis_s->calibration_s.state = state;
}

unsigned int get_axis_calibration_state(Axis_s *axis_s) {
    return axis_s->calibration_s.state;
}
#include "axis.h"

Axis_s Axis() {
    AxisCalibration_s calibration_s = {0, 0, 0};
    AxisFrequency_s frequency_s = {0, 0, 0, 0, 0};
    AxisAmplitude_s amplitude_s = {0, 0, 0, 0};
    Axis_s axis_s = {amplitude_s, frequency_s, calibration_s};
    return axis_s;
};

void set_amplitude(Axis_s *axis_s, float raw) {
    axis_s->amplitude_s.raw = raw;
};

float get_amplitude_raw(Axis_s *axis_s) {
    return axis_s->amplitude_s.raw;
};

float get_frequency(Axis_s *axis_s) {
    return axis_s->frequency_s.frequency;
}
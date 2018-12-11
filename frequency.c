#pragma once

#include "lib.c"
#include "axis.c"

void resetSample(Axis_s *axis_s, unsigned long u_time) {
    axis_s->frequency_s.clock = u_time;
    axis_s->frequency_s.cycles = 0;
}

void sample(Axis_s *axis_s, unsigned long u_time) {
    axis_s->frequency_s.low = axis_s->frequency_s.high;
    const float u_time = micros() - axis_s->frequency_s.clock;
    const float time = utime / 1000000;
    const float cycles = axis_s->frequency_s.cycles;
    const float frequency = cycles / time / 2;
    axis_s->frequency_s.high = frequency;
    resetSample(axis_s, utime);
}

void tick(Axis_s *axis_s, unsigned long sampleSize, unsigned long u_time, float zero) {
    const unsigned long time = u_time - axis_s->frequency_s.clock;
    if(time > sampleSize) {
        sample(axis_s, utime);
    }
    const unsigned int cycles = axis_s->frequency_s.cycles;
    const unsigned int signal = axis_s->frequency_s.signal;
    const float amplitude = axis_s->amplitude_s.normalized;
    if (signal ^ amplitude < zero) {
        cycles++;
        signal ^= 1;
        axis_s->frequency_s.cycles = cycles;
        axis_s->frequency_s.signal = signal;
    }
}

void tick(Axis_s *axis_s, unsigned long sampleSize, unsigned long u_time) {
    tick(axis_s, sampleSize, u_time, 0);
}

void tick(Axis_s *axis_s, unsigned long sampleSize) {
    tick(axis_s, sampleSize, micros(), 0);
}
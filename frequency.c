#ifndef INCLUDE_FREQUENCY_C
#define INCLUDE_FREQUENCY_C

#include "axis.h"

void resetSample(struct Axis_s *axis_s, unsigned long u_time) {
  axis_s->frequency_s.clock = u_time;
  axis_s->frequency_s.cycles = 0;
}

void sample(struct Axis_s *axis_s, float u_time) {
  const float time = u_time / 1000000;
  const float cycles = axis_s->frequency_s.cycles;
  const float frequency = cycles / time / 2;
  axis_s->frequency_s.frequency = frequency;
  resetSample(axis_s, u_time);
}

void tick(struct Axis_s *axis_s, unsigned long sampleSize, unsigned long u_time, float zero) {
  const unsigned long time = u_time - axis_s->frequency_s.clock;
  if (time > sampleSize) {
    sample(axis_s, u_time);
  }
  unsigned int cycles = axis_s->frequency_s.cycles;
  unsigned int signal = axis_s->frequency_s.signal;
  const float amplitude = axis_s->amplitude_s.normalized;
  if (signal ^ amplitude < zero) {
    cycles++;
    signal ^= 1;
    axis_s->frequency_s.cycles = cycles;
    axis_s->frequency_s.signal = signal;
  }
}

#endif

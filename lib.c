#ifndef INCLUDE_LIB_C
#define INCLUDE_LIB_C

float avg(float high, float low) {
  return (high + low) / 2;
}

float vector(float x, float y, float z) {
  return sqrt(x * x + y * y + z * z);
}

#endif

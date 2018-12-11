#ifndef INCLUDE_LIB_C
#define INCLUDE_LIB_C

float avg(float high, float low) {
  return (high + low) / 2;
}

float vector(float x, float y, float z) {
  return sqrt(x * x + y * y + z * z);
}

float sq_negative(float base) {
  if (base < 0 ) {
    return -sq(base);
  }
  return sq(base);
}

float ease(float old_value, float new_value, float velocity) {
  const float delta = (new_value - old_value) * velocity;
  return old_value + delta;
}

void swap(float *a, float *b) {
  const float temp = *b;
  *a = *b;
  *b = temp;
}

#endif

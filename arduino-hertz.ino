#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

/**
   Mock Mode
   =========
   If enabled, will mock a sine wave signal instead of reading physical sensor.
*/
#define MOCK_MODE false

#if MOCK_MODE

#define MOCK_FREQUENCY_LOW 20
#define MOCK_FREQUENCY_HIGH 60
#define MOCK_BURST_LENGTH 2000000

float MOCK_FREQUENCY_CURRENT = 0;
unsigned long MOCK_BURST_CLOCK = 0;
unsigned int MOCK_BURST_STATE = 1;

/**
   Simulate a sine wave tick
*/
float sine(float frequency) {
  const float utime = micros();
  const float time = utime / 1000000;
  return sin(time * frequency * 2 * M_PI);
}

#endif

/**
   ================
   Plotting Options
   ================
*/

#define AMPLITUDE_MULTIPLIER 1
#define FREQUENCY_MULTIPLIER 1

/**
   =====================
   Configuration Options
   =====================
*/

/**
   ZERO
   ====
   Threshold for Zero Crossing Detection algorithm
*/
#define ZERO 0.0

/**
   Baud Rate
   =========
   The max baud rate for the LIS3DH sensor is 230400, which gives a max sample rate of ~5.3 KHz.
*/
#define BAUD_RATE 230400

/**
   Lowest Target Frequency
   ===========
   The lowest frequency needed to measure. This is used to set the sample size.
*/
#if MOCK_MODE

#define LOWEST_TARGET_FREQUENCY MOCK_FREQUENCY_LOW

#else

#define LOWEST_TARGET_FREQUENCY 1

#endif

/**
   Set sample size according to Lowest Target Frequency.
   Anything lower will produce inaccurate results.
*/
#define SAMPLE_SIZE 1000000 / LOWEST_TARGET_FREQUENCY

/**
   ===== !! Do not edit anything below this line !! =====
*/

float vector(float x, float y, float z) {
  return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

float avg(float high, float low) {
  return (high + low) / 2;
}

unsigned long clock_X = 0;
unsigned int cycles_X = 0;
unsigned int signal_X = 0;
float frequency_X = 0;

unsigned long clock_Y = 0;
unsigned int cycles_Y = 0;
unsigned int signal_Y = 0;
float frequency_Y = 0;

unsigned long clock_Z = 0;
unsigned int cycles_Z = 0;
unsigned int signal_Z = 0;
float frequency_Z = 0;

unsigned long clock_V = 0;
unsigned int cycles_V = 0;
unsigned int signal_V = 0;
float frequency_V = 0;

/**
   Reset sample counters
*/
void resetSample(unsigned int *cycles, float *frequency) {
  *cycles = 0;
}

/**
   Analyze sample and calculate Current Frequency.
*/
void sample(unsigned long *clock, unsigned int *cycles, float *frequency) {
  const float utime = micros() - *clock;
  const float time = utime / 1000000;
  const float c = *cycles;
  const float f = c / time / 2;
  *frequency = f;
  resetSample(cycles, frequency);
}

/**
   Tick tock, app clock.
*/
void tick(float amplitude, unsigned long *clock, unsigned int *cycles, unsigned int *signal, float *frequency) {
  const float time = micros() - *clock;
  if (time > SAMPLE_SIZE) {
    sample(clock, cycles, frequency);
    *clock = micros();
  }
  const unsigned int c = *cycles;
  const unsigned int s = *signal;
  if (s ^ amplitude < ZERO) {
    *cycles = c + 1;
    *signal ^= 1;
  }
}

#if MOCK_MODE

void setup() {
  Serial.begin(BAUD_RATE);
}

void loop() {
  const unsigned long time = micros() - MOCK_BURST_CLOCK;
  if (time > MOCK_BURST_LENGTH) {
    MOCK_FREQUENCY_CURRENT = MOCK_BURST_STATE ? random(MOCK_FREQUENCY_LOW, MOCK_FREQUENCY_HIGH) : 0;
    MOCK_BURST_CLOCK = micros();
    MOCK_BURST_STATE ^= 1;
  }
  const float MOCK_FREQUENCY_CURRENT_X = MOCK_FREQUENCY_CURRENT;
  const float MOCK_FREQUENCY_CURRENT_Y = MOCK_FREQUENCY_CURRENT / 2;
  const float MOCK_FREQUENCY_CURRENT_Z = MOCK_FREQUENCY_CURRENT / 4;
  const float MOCK_FREQUENCY_CURRENT_V = vector(MOCK_FREQUENCY_CURRENT_X, MOCK_FREQUENCY_CURRENT_Y, MOCK_FREQUENCY_CURRENT_Z);
  const float amplitude_X = sine(MOCK_FREQUENCY_CURRENT_X);
  const float amplitude_Y = sine(MOCK_FREQUENCY_CURRENT_Y);
  const float amplitude_Z = sine(MOCK_FREQUENCY_CURRENT_Z);
  tick(amplitude_X, &clock_X, &cycles_X, &signal_X, &frequency_X);
  tick(amplitude_Y, &clock_Y, &cycles_Y, &signal_Y, &frequency_Y);
  tick(amplitude_Z, &clock_Z, &cycles_Z, &signal_Z, &frequency_Z);
  frequency_V = vector(frequency_X, frequency_Y, frequency_Z);
  Serial.print(MOCK_FREQUENCY_HIGH);
  Serial.print("\t");
  Serial.print(MOCK_FREQUENCY_CURRENT_V);
  Serial.print("\t");
  Serial.print(frequency_V);
  Serial.print("\t");
  Serial.print(amplitude_X * MOCK_FREQUENCY_HIGH / 2);
  Serial.print("\t");
  Serial.print(amplitude_Y * MOCK_FREQUENCY_HIGH / 2);
  Serial.print("\t");
  Serial.print(amplitude_Z * MOCK_FREQUENCY_HIGH / 2);
  Serial.print("\t");
  Serial.println(-MOCK_FREQUENCY_HIGH);
}

#else

#define CALIBRATION_TIME 3000 // 3 seconds
#define SENSITIVTY 0.7
#define SENSOR_RANGE LIS3DH_RANGE_2_G // LIS3DH_RANGE_2_G, LIS3DH_RANGE_4_G, LIS3DH_RANGE_8_G, LIS3DH_RANGE_16_G

#if defined(ARDUINO_ARCH_SAMD)

#define Serial SerialUSB

#endif

Adafruit_LIS3DH sensor = Adafruit_LIS3DH();

void calibrateZero(float *zero, float raw) {
  *zero = avg(*zero, raw);
}

void calibrateFilter(float *filter, float zeroed) {
  *filter = max(abs(*filter), zeroed);
}

float zeroAxis(float zero, float value) {
  return value - zero;
}

float filterAxis(float filter, float value) {
  if (abs(value) > (filter / SENSITIVTY)) {
    return value;
  }
  return 0;
}

float normalizeAxis(float zero, float filter, float raw) {
  return filterAxis(filter, zeroAxis(zero, raw));
}

float zero_x = 0;
float zero_y = 0;
float zero_z = 0;
float filter_x = 0;
float filter_y = 0;
float filter_z = 0;

void setup() {

  // pause serial shields until serial console opens
#ifndef ESP8266

  while (!Serial);

#endif

  Serial.begin(BAUD_RATE);
  Serial.println("Connecting to sensor...");
  if (! sensor.begin(0x18)) {
    while (1);
  }
  Serial.println("Connected.");
  sensor.setRange(SENSOR_RANGE);
}

/**
   If true, will print frequency.
   Otherwise, print amplitude.
   Can't mix and match due to amplitude having
   a much higher scale on the plotter.
*/
#define PRINT_FREQUENCY false

void loop() {

  sensor.read();

  const float time = millis();
  const float raw_x = sensor.x;
  const float raw_y = sensor.y;
  const float raw_z = sensor.z;
  const float zeroed_x = zeroAxis(zero_x, raw_x);
  const float zeroed_y = zeroAxis(zero_y, raw_y);
  const float zeroed_z = zeroAxis(zero_z, raw_z);
  const float filtered_x = filterAxis(filter_x, zeroed_x);
  const float filtered_y = filterAxis(filter_y, zeroed_y);
  const float filtered_z = filterAxis(filter_z, zeroed_z);

  if (time < CALIBRATION_TIME) {
    if (time < (CALIBRATION_TIME / 2)) {
      calibrateZero(&zero_x, raw_x);
      calibrateZero(&zero_y, raw_y);
      calibrateZero(&zero_z, raw_z);
    } else {
      calibrateFilter(&filter_x, zeroed_x);
      calibrateFilter(&filter_y, zeroed_y);
      calibrateFilter(&filter_z, zeroed_z);
    }
  }
  tick(filtered_x, &clock_X, &cycles_X, &signal_X, &frequency_X);
  tick(filtered_y, &clock_Y, &cycles_Y, &signal_Y, &frequency_Y);
  tick(filtered_z, &clock_Z, &cycles_Z, &signal_Z, &frequency_Z);

#if PRINT_FREQUENCY

  Serial.print(frequency_X);
  Serial.print("\t");
  Serial.print(frequency_Y);
  Serial.print("\t");
  Serial.println(frequency_Z);

#else

  Serial.print(filtered_x);
  Serial.print("\t");
  Serial.print(filtered_y);
  Serial.print("\t");
  Serial.println(filtered_z);

#endif

}

#endif

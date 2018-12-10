
/**
   Mock Mode
   =========
   If enabled, will mock a sine wave signal instead of reading physical sensor.
*/
#define MOCK_MODE true

#if MOCK_MODE
#define MOCK_FREQUENCY_LOW 50
#define MOCK_FREQUENCY_HIGH 120

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
#define LOWEST_TARGET_FREQUENCY 1

/**
   Set sample size as twice the Lowest Target Frequency.
   Anything lower will produce inaccurate results.
*/
#define SAMPLE_SIZE LOWEST_TARGET_FREQUENCY * 2000000

/**
   ===== !! Do not edit anything below this line !! =====
*/

unsigned long clock = 0;
unsigned int cycles = 0;
unsigned int signal = 0;
float frequency = 0;

#if MOCK_MODE
// Current mock frequency to simulate (if MOCK_MODE is enabled)
float currentMockFrequency = MOCK_FREQUENCY_LOW;
#endif

/**
   Reset sample counters
*/
void resetSample() {
  cycles = 0;
#if MOCK_MODE
  currentMockFrequency = random(MOCK_FREQUENCY_LOW, MOCK_FREQUENCY_HIGH);
#endif
}

/**
   Read and normalize physical sensor
*/
float readSensor() {

}

/**
   Read measurement
*/

#if MOCK_MODE

float readAmplitude() {
  return sine(currentMockFrequency);
}

#else
float readAmplitude() {
  return readSensor();
}
#endif

/**
   Analyze sample and calculate Current Frequency.
*/
float sample() {
  const float utime = micros() - clock;
  const float time = utime / 1000000;
  const float f = cycles / time / 2;
  resetSample();
  return f;
}

/**
   Tick tock, app clock.
*/
float tick() {
  float amplitude = readAmplitude();
  if (signal ^ amplitude < ZERO) {
    cycles++;
    signal ^= 1;
  }
  return amplitude;
}

/**
   App loop
*/
void loop() {
  const float time = micros() - clock;
  if (time > SAMPLE_SIZE) {
    frequency = sample();
    clock = micros();
  }
  const float amplitude = tick();
  Serial.print(amplitude);
  Serial.print("\t");
  Serial.println(frequency);
}

/**
   Setup app
*/
void setup() {
  resetSample();
  Serial.begin(BAUD_RATE);
}


/**
   Mock Mode
   =========
   If enabled, will mock a sine wave signal instead of reading physical sensor.
*/
#define MOCK_MODE true

#if MOCK_MODE
#define MOCK_FREQUENCY_LOW 20
#define MOCK_FREQUENCY_HIGH 240
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

unsigned long clock_X = 0;
unsigned int cycles_X = 0;
unsigned int signal_X = 0;
float amplitude_X = 0;
float frequency_X = 0;

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
  *frequency = (f + *frequency) /2;
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
void loop() {
  const unsigned long time = micros() - MOCK_BURST_CLOCK;
  if (time > MOCK_BURST_LENGTH) {
    MOCK_FREQUENCY_CURRENT = MOCK_BURST_STATE ? random(MOCK_FREQUENCY_LOW, MOCK_FREQUENCY_HIGH) : 0;
    MOCK_BURST_CLOCK = micros();
    MOCK_BURST_STATE ^= 1;
  }
  const float amplitude_X = sine(MOCK_FREQUENCY_CURRENT);
  tick(amplitude_X, &clock_X, &cycles_X, &signal_X, &frequency_X);
  Serial.print(amplitude_X * MOCK_FREQUENCY_HIGH / 2);
  Serial.print("\t");
  Serial.print(MOCK_FREQUENCY_CURRENT);
  Serial.print("\t");
  Serial.print(frequency_X);
  Serial.print("\t");
  Serial.print(MOCK_FREQUENCY_HIGH);
  Serial.print("\t");
  Serial.println(-MOCK_FREQUENCY_HIGH);
}
#endif

/**
   Setup app
*/
void setup() {
  Serial.begin(BAUD_RATE);
}

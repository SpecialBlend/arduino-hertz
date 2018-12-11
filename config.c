#ifndef INCLUDE_CONFIG_H
#define INCLUDE_CONFIG_H

/**
   =============
   Configuration
   =============
*/

/**
   The max baud rate for the LIS3DH sensor is 230400, which gives a max sample rate of ~5.3 KHz.
*/
#define BAUD_RATE 230400

/**
   Sensor range: LIS3DH_RANGE_2_G, LIS3DH_RANGE_4_G, LIS3DH_RANGE_8_G, LIS3DH_RANGE_16_G
*/
#define SENSOR_RANGE LIS3DH_RANGE_2_G

/**
   Threshold for Zero Crossing Detection
*/
#define ZERO 0.0

/**
   The lowest frequency needed to measure in Hz. This is used to set the sample size.
*/
#define LOWEST_TARGET_FREQUENCY 1

/**
   Set sample size according to Lowest Target Frequency.
   Anything lower will produce inaccurate results.
*/
#define SAMPLE_SIZE 1000000 / LOWEST_TARGET_FREQUENCY

/**
   Filter sensitivity threshold
   Higher is more sensitive.
*/
#define CALIBRATION_FILTER_SENSITIVITY .9

/**
   Zero sensitivity threshold
   Higher is more sensitive.
*/
#define CALIBRATION_ZERO_SENSITIVITY 0.05

/**
   Minimum calibration time in milliseconds
*/
#define CALIBRATION_TIME_MIN 1000

/**
   Maximum calibration time in milliseconds
*/

#define CALIBRATION_TIME_MAX 5000

#endif

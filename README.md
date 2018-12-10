# arduino-hertz
Measure amplitude and frequency of vibrations using an Arduino Uno with an LIS3DH accelerometer/sensor

- Uses a simple and fast zero crossing detection algorithm to estimate frequency.
- Outputs amplitude and frequency for X, Y, Z axis respectively.
- Auto-calibrates against gravity/tilt zero and natural resonance in the first few seconds of program start.

![](demo.gif)

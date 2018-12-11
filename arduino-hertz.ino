/**
   Mock Mode
   =========
   If enabled, will mock a sine wave signal instead of reading physical sensor.
*/
#define MOCK_MODE true

#if MOCK_MODE

#include "mock.c"

#else

#include "app.c"

#endif
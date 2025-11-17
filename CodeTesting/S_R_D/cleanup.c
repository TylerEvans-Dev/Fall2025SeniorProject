#if defined(__APPLE__)
#include "../GPIO/wiringOP/wiringPi/wiringPi.h"
#include "../GPIO/wiringOP/wiringPi/wiringPiI2C.h"
#else
#include <wiringPi.h>
#include <wiringPiI2C.h>
#endif

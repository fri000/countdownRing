#ifndef utils_h_header
#define utils_h_header

#include <stdint.h>

long map(long x, long in_min, long in_max, long out_min, long out_max);
int analogRead(uint8_t pin);
void delayMicroseconds(unsigned int us);


#endif //utils_h_header
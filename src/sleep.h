#ifndef SLEEP_H
#define SLEEP_H

#include <stdint.h>
#include <pico/time.h>

void sleep_ns(uint32_t ns);

#if 0
void sleep_us(uint32_t us);
#endif

#endif /* SLEEP_H */

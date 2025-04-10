#include "pico/stdlib.h"
#include "sleep.h"

void sleep_ns(uint32_t ns) {
#if 1
        // __asm volatile ("nop\n"); := 1 cycle
        // pico w = 125MHz / sec
        // 1 cycle = 8ns
	const uint32_t cycles = (ns * 125) / 1000;
	volatile uint32_t i = 0;
	while(i < cycles) {
		__asm volatile ("nop");
		i++;
	}
#if 0
        uint32_t rem = (ns / 8); // round up to nearest?
        while(rem > 0) {
                __asm volatile ("nop\n");
                rem--;
        }
#endif

#else
	sleep_ms(ns % 1000);
#endif
}

#if 0
void sleep_us(uint32_t us) {
        // __asm volatile ("nop\n"); := 1 cycle
        // pico w = 125MHz / sec
        // 1 cycle = 8ns

        uint32_t rem = (us % 8) % 1000; // do bitwise?
        while(rem) {
                __asm volatile ("nop\n");
                rem--;
        }
}
#endif

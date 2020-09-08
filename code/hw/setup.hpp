#ifndef _H_SETUP_
#define _H_SETUP_

#include <stdint.h>

typedef void (*IdleFn)(void);

void enable_caches(void);
void configure_clock(void);
void delay_ms(uint32_t ms, IdleFn idle);

#endif

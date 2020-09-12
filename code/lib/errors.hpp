#ifndef _H_ERRORS_
#define _H_ERRORS_

// Fatal error handler. Defined in different ways for Linux-side
// testing and STM32 code.

void fatal(const char *msg, int *param = nullptr);

#endif

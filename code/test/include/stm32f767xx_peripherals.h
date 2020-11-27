#include <stdint.h>

extern uint8_t *AHB1PERIPH_BASE;
extern uint8_t *AHB2PERIPH_BASE;
extern uint8_t *APB1PERIPH_BASE;
extern uint8_t *APB2PERIPH_BASE;

void init_peripheral_clocks(void);
void init_mock_gpios(void);
void init_mock_timers(void);
void init_mock_mcu(void);

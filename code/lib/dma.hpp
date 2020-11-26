#ifndef _H_DMA_
#define _H_DMA_

#include <stdint.h>
#include "bsp-generic.h"


// Simple wrapper around information defining an STM32 DMA channel.
struct DMAChannel {
  const uint8_t dma;
  const uint8_t stream;
  const uint8_t channel;
};

// Determine IRQ number for DMA events.
IRQn_Type dma_irqn(const DMAChannel &dma);

#endif

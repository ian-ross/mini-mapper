#ifndef _H_DMA_
#define _H_DMA_

#include <stdint.h>

struct DMAChannel {
  const uint8_t dma;
  const uint8_t stream;
  const uint8_t channel;
};

#endif

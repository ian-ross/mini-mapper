#include "dma.hpp"


IRQn_Type dma_irqn(const DMAChannel &dma) {
  switch (dma.dma * 10 + dma.stream) {
  case 10: return DMA1_Stream0_IRQn;
  case 11: return DMA1_Stream1_IRQn;
  case 12: return DMA1_Stream2_IRQn;
  case 13: return DMA1_Stream3_IRQn;
  case 14: return DMA1_Stream4_IRQn;
  case 15: return DMA1_Stream5_IRQn;
  case 16: return DMA1_Stream6_IRQn;
  case 17: return DMA1_Stream7_IRQn;
  case 20: return DMA2_Stream0_IRQn;
  case 21: return DMA2_Stream1_IRQn;
  case 22: return DMA2_Stream2_IRQn;
  case 23: return DMA2_Stream3_IRQn;
  case 24: return DMA2_Stream4_IRQn;
  case 25: return DMA2_Stream5_IRQn;
  case 26: return DMA2_Stream6_IRQn;
  case 27: return DMA2_Stream7_IRQn;
  default: return NonMaskableInt_IRQn;
  }
}

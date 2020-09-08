#include "stm32f767xx.h"

static TIM_TypeDef tim2;
TIM_TypeDef *TIM2 = &tim2;

static TIM_TypeDef tim3;
TIM_TypeDef *TIM3 = &tim3;

static TIM_TypeDef tim4;
TIM_TypeDef *TIM4 = &tim4;

static TIM_TypeDef tim5;
TIM_TypeDef *TIM5 = &tim5;

static TIM_TypeDef tim6;
TIM_TypeDef *TIM6 = &tim6;

static TIM_TypeDef tim7;
TIM_TypeDef *TIM7 = &tim7;

static TIM_TypeDef tim12;
TIM_TypeDef *TIM12 = &tim12;

static TIM_TypeDef tim13;
TIM_TypeDef *TIM13 = &tim13;

static TIM_TypeDef tim14;
TIM_TypeDef *TIM14 = &tim14;

static LPTIM_TypeDef lptim1;
LPTIM_TypeDef *LPTIM1 = &lptim1;

static RTC_TypeDef rtc;
RTC_TypeDef *RTC = &rtc;

static WWDG_TypeDef wwdg;
WWDG_TypeDef *WWDG = &wwdg;

static IWDG_TypeDef iwdg;
IWDG_TypeDef *IWDG = &iwdg;

static SPI_TypeDef spi2;
SPI_TypeDef *SPI2 = &spi2;

static SPI_TypeDef spi3;
SPI_TypeDef *SPI3 = &spi3;

static SPDIFRX_TypeDef spdifrx;
SPDIFRX_TypeDef *SPDIFRX = &spdifrx;

static USART_TypeDef usart2;
USART_TypeDef *USART2 = &usart2;

static USART_TypeDef usart3;
USART_TypeDef *USART3 = &usart3;

static USART_TypeDef uart4;
USART_TypeDef *UART4 = &uart4;

static USART_TypeDef uart5;
USART_TypeDef *UART5 = &uart5;

static I2C_TypeDef i2c1;
I2C_TypeDef *I2C1 = &i2c1;

static I2C_TypeDef i2c2;
I2C_TypeDef *I2C2 = &i2c2;

static I2C_TypeDef i2c3;
I2C_TypeDef *I2C3 = &i2c3;

static I2C_TypeDef i2c4;
I2C_TypeDef *I2C4 = &i2c4;

static CAN_TypeDef can1;
CAN_TypeDef *CAN1 = &can1;

static CAN_TypeDef can2;
CAN_TypeDef *CAN2 = &can2;

static CEC_TypeDef cec;
CEC_TypeDef *CEC = &cec;

static PWR_TypeDef pwr;
PWR_TypeDef *PWR = &pwr;

static DAC_TypeDef dac1;
DAC_TypeDef *DAC1 = &dac1;

static DAC_TypeDef dac;
DAC_TypeDef *DAC = &dac;

static USART_TypeDef uart7;
USART_TypeDef *UART7 = &uart7;

static USART_TypeDef uart8;
USART_TypeDef *UART8 = &uart8;

static TIM_TypeDef tim1;
TIM_TypeDef *TIM1 = &tim1;

static TIM_TypeDef tim8;
TIM_TypeDef *TIM8 = &tim8;

static USART_TypeDef usart1;
USART_TypeDef *USART1 = &usart1;

static USART_TypeDef usart6;
USART_TypeDef *USART6 = &usart6;

static ADC_Common_TypeDef adc;
ADC_Common_TypeDef *ADC = &adc;

static ADC_TypeDef adc1;
ADC_TypeDef *ADC1 = &adc1;

static ADC_TypeDef adc2;
ADC_TypeDef *ADC2 = &adc2;

static ADC_TypeDef adc3;
ADC_TypeDef *ADC3 = &adc3;

static ADC_Common_TypeDef adc123_common;
ADC_Common_TypeDef *ADC123_COMMON = &adc123_common;

static SDMMC_TypeDef sdmmc1;
SDMMC_TypeDef *SDMMC1 = &sdmmc1;

static SPI_TypeDef spi1;
SPI_TypeDef *SPI1 = &spi1;

static SPI_TypeDef spi4;
SPI_TypeDef *SPI4 = &spi4;

static SYSCFG_TypeDef syscfg;
SYSCFG_TypeDef *SYSCFG = &syscfg;

static EXTI_TypeDef exti;
EXTI_TypeDef *EXTI = &exti;

static TIM_TypeDef tim9;
TIM_TypeDef *TIM9 = &tim9;

static TIM_TypeDef tim10;
TIM_TypeDef *TIM10 = &tim10;

static TIM_TypeDef tim11;
TIM_TypeDef *TIM11 = &tim11;

static SPI_TypeDef spi5;
SPI_TypeDef *SPI5 = &spi5;

static SPI_TypeDef spi6;
SPI_TypeDef *SPI6 = &spi6;

static SAI_TypeDef sai1;
SAI_TypeDef *SAI1 = &sai1;

static SAI_TypeDef sai2;
SAI_TypeDef *SAI2 = &sai2;

static SAI_Block_TypeDef sai1_block_a;
SAI_Block_TypeDef *SAI1_Block_A = &sai1_block_a;

static SAI_Block_TypeDef sai1_block_b;
SAI_Block_TypeDef *SAI1_Block_B = &sai1_block_b;

static SAI_Block_TypeDef sai2_block_a;
SAI_Block_TypeDef *SAI2_Block_A = &sai2_block_a;

static SAI_Block_TypeDef sai2_block_b;
SAI_Block_TypeDef *SAI2_Block_B = &sai2_block_b;

static LTDC_TypeDef ltdc;
LTDC_TypeDef *LTDC = &ltdc;

static LTDC_Layer_TypeDef ltdc_layer1;
LTDC_Layer_TypeDef *LTDC_Layer1 = &ltdc_layer1;

static LTDC_Layer_TypeDef ltdc_layer2;
LTDC_Layer_TypeDef *LTDC_Layer2 = &ltdc_layer2;

uint8_t ahb1periph_base[0x10000];
uint8_t *AHB1PERIPH_BASE = ahb1periph_base;

static DMA_TypeDef dma1;
DMA_TypeDef *DMA1 = &dma1;

static DMA_Stream_TypeDef dma1_stream0;
DMA_Stream_TypeDef *DMA1_Stream0 = &dma1_stream0;

static DMA_Stream_TypeDef dma1_stream1;
DMA_Stream_TypeDef *DMA1_Stream1 = &dma1_stream1;

static DMA_Stream_TypeDef dma1_stream2;
DMA_Stream_TypeDef *DMA1_Stream2 = &dma1_stream2;

static DMA_Stream_TypeDef dma1_stream3;
DMA_Stream_TypeDef *DMA1_Stream3 = &dma1_stream3;

static DMA_Stream_TypeDef dma1_stream4;
DMA_Stream_TypeDef *DMA1_Stream4 = &dma1_stream4;

static DMA_Stream_TypeDef dma1_stream5;
DMA_Stream_TypeDef *DMA1_Stream5 = &dma1_stream5;

static DMA_Stream_TypeDef dma1_stream6;
DMA_Stream_TypeDef *DMA1_Stream6 = &dma1_stream6;

static DMA_Stream_TypeDef dma1_stream7;
DMA_Stream_TypeDef *DMA1_Stream7 = &dma1_stream7;

static DMA_TypeDef dma2;
DMA_TypeDef *DMA2 = &dma2;

static DMA_Stream_TypeDef dma2_stream0;
DMA_Stream_TypeDef *DMA2_Stream0 = &dma2_stream0;

static DMA_Stream_TypeDef dma2_stream1;
DMA_Stream_TypeDef *DMA2_Stream1 = &dma2_stream1;

static DMA_Stream_TypeDef dma2_stream2;
DMA_Stream_TypeDef *DMA2_Stream2 = &dma2_stream2;

static DMA_Stream_TypeDef dma2_stream3;
DMA_Stream_TypeDef *DMA2_Stream3 = &dma2_stream3;

static DMA_Stream_TypeDef dma2_stream4;
DMA_Stream_TypeDef *DMA2_Stream4 = &dma2_stream4;

static DMA_Stream_TypeDef dma2_stream5;
DMA_Stream_TypeDef *DMA2_Stream5 = &dma2_stream5;

static DMA_Stream_TypeDef dma2_stream6;
DMA_Stream_TypeDef *DMA2_Stream6 = &dma2_stream6;

static DMA_Stream_TypeDef dma2_stream7;
DMA_Stream_TypeDef *DMA2_Stream7 = &dma2_stream7;

static ETH_TypeDef eth;
ETH_TypeDef *ETH = &eth;

static DMA2D_TypeDef dma2d;
DMA2D_TypeDef *DMA2D = &dma2d;

static DCMI_TypeDef dcmi;
DCMI_TypeDef *DCMI = &dcmi;

static RNG_TypeDef rng;
RNG_TypeDef *RNG = &rng;

static FMC_Bank1_TypeDef fmc_bank1;
FMC_Bank1_TypeDef *FMC_Bank1 = &fmc_bank1;

static FMC_Bank1E_TypeDef fmc_bank1e;
FMC_Bank1E_TypeDef *FMC_Bank1E = &fmc_bank1e;

static FMC_Bank3_TypeDef fmc_bank3;
FMC_Bank3_TypeDef *FMC_Bank3 = &fmc_bank3;

static FMC_Bank5_6_TypeDef fmc_bank5_6;
FMC_Bank5_6_TypeDef *FMC_Bank5_6 = &fmc_bank5_6;

static QUADSPI_TypeDef quadspi;
QUADSPI_TypeDef *QUADSPI = &quadspi;

static DBGMCU_TypeDef dbgmcu;
DBGMCU_TypeDef *DBGMCU = &dbgmcu;

static USB_OTG_GlobalTypeDef usb_otg_fs;
USB_OTG_GlobalTypeDef *USB_OTG_FS = &usb_otg_fs;

static USB_OTG_GlobalTypeDef usb_otg_hs;
USB_OTG_GlobalTypeDef *USB_OTG_HS = &usb_otg_hs;

static CAN_TypeDef can3;
CAN_TypeDef *CAN3 = &can3;

static SDMMC_TypeDef sdmmc2;
SDMMC_TypeDef *SDMMC2 = &sdmmc2;

static MDIOS_TypeDef mdios;
MDIOS_TypeDef *MDIOS = &mdios;

static DFSDM_Channel_TypeDef dfsdm1_channel0;
DFSDM_Channel_TypeDef *DFSDM1_Channel0 = &dfsdm1_channel0;

static DFSDM_Channel_TypeDef dfsdm1_channel1;
DFSDM_Channel_TypeDef *DFSDM1_Channel1 = &dfsdm1_channel1;

static DFSDM_Channel_TypeDef dfsdm1_channel2;
DFSDM_Channel_TypeDef *DFSDM1_Channel2 = &dfsdm1_channel2;

static DFSDM_Channel_TypeDef dfsdm1_channel3;
DFSDM_Channel_TypeDef *DFSDM1_Channel3 = &dfsdm1_channel3;

static DFSDM_Channel_TypeDef dfsdm1_channel4;
DFSDM_Channel_TypeDef *DFSDM1_Channel4 = &dfsdm1_channel4;

static DFSDM_Channel_TypeDef dfsdm1_channel5;
DFSDM_Channel_TypeDef *DFSDM1_Channel5 = &dfsdm1_channel5;

static DFSDM_Channel_TypeDef dfsdm1_channel6;
DFSDM_Channel_TypeDef *DFSDM1_Channel6 = &dfsdm1_channel6;

static DFSDM_Channel_TypeDef dfsdm1_channel7;
DFSDM_Channel_TypeDef *DFSDM1_Channel7 = &dfsdm1_channel7;

static DFSDM_Filter_TypeDef dfsdm1_filter0;
DFSDM_Filter_TypeDef *DFSDM1_Filter0 = &dfsdm1_filter0;

static DFSDM_Filter_TypeDef dfsdm1_filter1;
DFSDM_Filter_TypeDef *DFSDM1_Filter1 = &dfsdm1_filter1;

static DFSDM_Filter_TypeDef dfsdm1_filter2;
DFSDM_Filter_TypeDef *DFSDM1_Filter2 = &dfsdm1_filter2;

static DFSDM_Filter_TypeDef dfsdm1_filter3;
DFSDM_Filter_TypeDef *DFSDM1_Filter3 = &dfsdm1_filter3;

static JPEG_TypeDef jpeg;
JPEG_TypeDef *JPEG = &jpeg;

void init_peripheral_clocks(void)
{
  RCC->AHB1ENR = 0x00100000;
  RCC->AHB2ENR = 0x00000000;
  RCC->AHB3ENR = 0x00000000;
  RCC->APB1ENR = 0x00000400;
  RCC->APB2ENR = 0x00000000;
}

static void init_mock_gpio(GPIO_TypeDef *gpio)
{
  uint32_t moder = 0x00;
  uint32_t speedr = 0x00;
  uint32_t pupdr = 0x00;
  if (gpio == GPIOA) {
    moder = 0xA8000000;
    speedr = 0x0C000000;
    pupdr = 0x64000000;
  } else if (gpio == GPIOB) {
    moder = 0x00000280;
    speedr = 0x000000C0;
    pupdr = 0x00000100;
  }
  gpio->MODER = moder;
  gpio->OTYPER = 0x00;
  gpio->OSPEEDR = speedr;
  gpio->PUPDR = pupdr;
  gpio->IDR = 0x00;
  gpio->ODR = 0x00;
  gpio->BSRR = 0x00;
  gpio->LCKR = 0x00;
  gpio->AFR[0] = 0x00;
  gpio->AFR[1] = 0x00;
}

void init_mock_gpios(void)
{
  init_mock_gpio(GPIOA);
  init_mock_gpio(GPIOB);
  init_mock_gpio(GPIOC);
  init_mock_gpio(GPIOD);
  init_mock_gpio(GPIOE);
  init_mock_gpio(GPIOF);
  init_mock_gpio(GPIOG);
  init_mock_gpio(GPIOH);
  init_mock_gpio(GPIOI);
  init_mock_gpio(GPIOJ);
  init_mock_gpio(GPIOK);
}

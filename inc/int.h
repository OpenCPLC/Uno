#ifndef INT_H_
#define INT_H_

#include <stdint.h>
#include "stm32g0xx.h"
#include "startup.h"

//-------------------------------------------------------------------------------------------------

typedef enum {
  INT_NonMaskableInt = -14,
  INT_HardFault = -13,
  INT_SVCall = -5,
  INT_PendSV = -2,
  INT_SysTick = -1,
  INT_Watchdog = 0,
  INT_PVD_VDDIO2 = 1,
  INT_RTC_TAMP = 2,
  INT_FLASH = 3,
  INT_RCC_CRS = 4,
  INT_EXTI0_1 = 5,
  INT_EXTI2_3 = 6,
  INT_EXTI4_15 = 7,
  INT_USB_UCPD1_2 = 8,
  INT_DMA1_CH1 = 9,
  INT_DMA1_CH2_3 = 10,
  INT_DMA1_CH4_7_DMA2_CH1_5_DMAMUX1_OVR = 11, 
  INT_ADC1_COMP = 12,
  INT_TIM1_BRK_UP_TRG_COM = 13, 
  INT_TIM1_CC = 14,
  INT_TIM2 = 15,
  INT_TIM3_TIM4 = 16,  
  INT_TIM6_DAC_LPTIM1 = 17,  
  INT_TIM7_LPTIM2 = 18,
  INT_TIM14 = 19,   
  INT_TIM15 = 20,   
  INT_TIM16_FDCAN_IT0 = 21, 
  INT_TIM17_FDCAN_IT1 = 22,
  INT_I2C1 = 23,   
  INT_I2C2_3 = 24,
  INT_SPI1 = 25,
  INT_SPI2_3 = 26,
  INT_USART1 = 27,  
  INT_USART2_LPUART2 = 28,
  INT_USART3_4_5_6_LPUART1 = 29,
  INT_CEC = 30,
  INT_AES_RNG = 31
} INT_e;

void INT_EnableTIM(TIM_TypeDef *tim_typedef, uint8_t priority, void (*handler)(void *), void *object);
void INT_EnableUART(USART_TypeDef *uart_typedef, uint8_t priority, void (*handler)(void *), void *object);
void INT_EnableI2C(I2C_TypeDef *i2c_typedef, uint8_t priority, void (*handler)(void *), void *object);
void INT_EnableADC(uint8_t priority, void (*handler)(void *), void *object);
void INT_EnableDMA(uint8_t dma_nbr, uint8_t priority, void (*handler)(void *), void *object);
void INT_EnableEXTI(uint8_t exti_nbr, uint8_t priority, void (*handler)(void *), void *object);

//-------------------------------------------------------------------------------------------------

#endif

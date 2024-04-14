#include "int.h"

//-------------------------------------------------------------------------------------------------

void INT_EnableTIM(TIM_TypeDef *tim_typedef, uint8_t priority, void (*handler)(void *), void *object)
{
  switch((uint32_t)tim_typedef) {
    case (uint32_t)TIM1:
      TIM1_IRQFnc = handler;
      TIM1_IRQSrc = object;
      NVIC_SetPriority(INT_TIM1_BRK_UP_TRG_COM, priority);
      NVIC_EnableIRQ(INT_TIM1_BRK_UP_TRG_COM);
      break;
    case (uint32_t)TIM2:
      TIM2_IRQFnc = handler;
      TIM2_IRQSrc = object;
      NVIC_SetPriority(INT_TIM2, priority);
      NVIC_EnableIRQ(INT_TIM2);
      break;
    case (uint32_t)TIM3:
      TIM3_IRQFnc = handler;
      TIM3_IRQSrc = object;
      NVIC_SetPriority(INT_TIM3_TIM4, priority);
      NVIC_EnableIRQ(INT_TIM3_TIM4);
      break;
    case (uint32_t)TIM6:
      TIM6_IRQFnc = handler;
      TIM6_IRQSrc = object;
      NVIC_SetPriority(INT_TIM6_DAC_LPTIM1, priority);
      NVIC_EnableIRQ(INT_TIM6_DAC_LPTIM1);
      break;
    case (uint32_t)TIM7:
      TIM7_IRQFnc = handler;
      TIM7_IRQSrc = object;
      NVIC_SetPriority(INT_TIM7_LPTIM2, priority);
      NVIC_EnableIRQ(INT_TIM7_LPTIM2);
      break;
    case (uint32_t)TIM14:
      TIM14_IRQFnc = handler;
      TIM14_IRQSrc = object;
      NVIC_SetPriority(INT_TIM3_TIM4, priority);
      NVIC_EnableIRQ(INT_TIM3_TIM4);
      break;
    case (uint32_t)TIM15:
      TIM15_IRQFnc = handler;
      TIM15_IRQSrc = object;
      NVIC_SetPriority(INT_TIM15, priority);
      NVIC_EnableIRQ(INT_TIM15);
      break;
    case (uint32_t)TIM16:
      TIM16_IRQFnc = handler;
      TIM16_IRQSrc = object;
      NVIC_SetPriority(INT_TIM16_FDCAN_IT0, priority);
      NVIC_EnableIRQ(INT_TIM16_FDCAN_IT0);
      break;
    case (uint32_t)TIM17:
      TIM17_IRQFnc = handler;
      TIM17_IRQSrc = object;
      NVIC_SetPriority(INT_TIM17_FDCAN_IT1, priority);
      NVIC_EnableIRQ(INT_TIM17_FDCAN_IT1);
      break;
  }
}

//-------------------------------------------------------------------------------------------------

void INT_EnableUART(USART_TypeDef *uart_typedef, uint8_t priority, void (*handler)(void *), void *object)
{
  switch((uint32_t)uart_typedef)
  {
    case (uint32_t)USART1:
      USART1_IRQFnc = handler;
      USART1_IRQSrc = object;
      NVIC_SetPriority(INT_USART1, priority);
      NVIC_EnableIRQ(INT_USART1);
      break;
    case (uint32_t)USART2:
      USART2_IRQFnc = handler;
      USART2_IRQSrc = object;
      NVIC_SetPriority(INT_USART2_LPUART2, priority);
      NVIC_EnableIRQ(INT_USART2_LPUART2);
      break;
    case (uint32_t)USART3:
      USART3_IRQFnc = handler;
      USART3_IRQSrc = object;
      NVIC_SetPriority(INT_USART3_4_5_6_LPUART1, priority);
      NVIC_EnableIRQ(INT_USART3_4_5_6_LPUART1);
      break;
    case (uint32_t)USART4:
      USART4_IRQFnc = handler;
      USART4_IRQSrc = object;
      NVIC_SetPriority(INT_USART3_4_5_6_LPUART1, priority);
      NVIC_EnableIRQ(INT_USART3_4_5_6_LPUART1);
      break;
    case (uint32_t)LPUART1:
      LPUART1_IRQFnc = handler;
      LPUART1_IRQSrc = object;
      NVIC_SetPriority(INT_USART3_4_5_6_LPUART1, priority);
      NVIC_EnableIRQ(INT_USART3_4_5_6_LPUART1);
      break;
  }
}

void INT_EnableI2C(I2C_TypeDef *i2c_typedef, uint8_t priority, void (*handler)(void *), void *object)
{
  switch((uint32_t)i2c_typedef)
  {
    case (uint32_t)I2C1:
      I2C1_IRQFnc = handler;
      I2C1_IRQSrc = object;
      NVIC_SetPriority(INT_I2C1, priority);
      NVIC_EnableIRQ(INT_I2C1);
      break;
    case (uint32_t)I2C2:
      I2C2_IRQFnc = handler;
      I2C2_IRQSrc = object;
      NVIC_SetPriority(INT_I2C2_3, priority);
      NVIC_EnableIRQ(INT_I2C2_3);
      break;
  }
}

void INT_EnableADC(uint8_t priority, void (*handler)(void *), void *object)
{
  ADC_IRQFnc = handler;
  ADC_IRQSrc = object;
  NVIC_SetPriority(INT_ADC1_COMP, priority);
  NVIC_EnableIRQ(INT_ADC1_COMP);
}

//-------------------------------------------------------------------------------------------------

void INT_EnableDMA(uint8_t dma_nbr, uint8_t priority, void (*handler)(void *), void *object)
{
  switch(dma_nbr) {
    case 1:
      DMA1_IRQFnc = handler;
      DMA1_IRQSrc = object;
      NVIC_SetPriority(INT_DMA1_CH1, priority);
      NVIC_EnableIRQ(INT_DMA1_CH1);
      break;
    case 2:
      DMA2_IRQFnc = handler;
      DMA2_IRQSrc = object;
      NVIC_SetPriority(INT_DMA1_CH2_3, priority);
      NVIC_EnableIRQ(INT_DMA1_CH2_3);
      break;
    case 3:
      DMA3_IRQFnc = handler;
      DMA3_IRQSrc = object;
      NVIC_SetPriority(INT_DMA1_CH2_3, priority);
      NVIC_EnableIRQ(INT_DMA1_CH2_3);
      break;
case 4:
      DMA4_IRQFnc = handler;
      DMA4_IRQSrc = object;
      NVIC_SetPriority(INT_DMA1_CH4_7_DMA2_CH1_5_DMAMUX1_OVR, priority);
      NVIC_EnableIRQ(INT_DMA1_CH4_7_DMA2_CH1_5_DMAMUX1_OVR);
      break;
    case 5:
      DMA5_IRQFnc = handler;
      DMA5_IRQSrc = object;
      NVIC_SetPriority(INT_DMA1_CH4_7_DMA2_CH1_5_DMAMUX1_OVR, priority);
      NVIC_EnableIRQ(INT_DMA1_CH4_7_DMA2_CH1_5_DMAMUX1_OVR);
      break;
    case 6:
      DMA6_IRQFnc = handler;
      DMA6_IRQSrc = object;
      NVIC_SetPriority(INT_DMA1_CH4_7_DMA2_CH1_5_DMAMUX1_OVR, priority);
      NVIC_EnableIRQ(INT_DMA1_CH4_7_DMA2_CH1_5_DMAMUX1_OVR);
      break;
    case 7:
      DMA7_IRQFnc = handler;
      DMA7_IRQSrc = object;
      NVIC_SetPriority(INT_DMA1_CH4_7_DMA2_CH1_5_DMAMUX1_OVR, priority);
      NVIC_EnableIRQ(INT_DMA1_CH4_7_DMA2_CH1_5_DMAMUX1_OVR);
      break;
  }
}

//-------------------------------------------------------------------------------------------------

void INT_EnableEXTI(uint8_t exti_nbr, uint8_t priority, void (*handler)(void *), void *object)
{
  switch(exti_nbr)
  {
    case 0:
      EXTI0_IRQFnc = handler;
      EXTI0_IRQSrc = object;
      NVIC_SetPriority(INT_EXTI0_1, priority);
      NVIC_EnableIRQ(INT_EXTI0_1);
      break;
    case 1:
      EXTI1_IRQFnc = handler;
      EXTI1_IRQSrc = object;
      NVIC_SetPriority(INT_EXTI0_1, priority);
      NVIC_EnableIRQ(INT_EXTI0_1);
      break;
    case 2:
      EXTI2_IRQFnc = handler;
      EXTI2_IRQSrc = object;
      NVIC_SetPriority(INT_EXTI2_3, priority);
      NVIC_EnableIRQ(INT_EXTI2_3);
      break;
    case 3:
      EXTI3_IRQFnc = handler;
      EXTI3_IRQSrc = object;
      NVIC_SetPriority(INT_EXTI2_3, priority);
      NVIC_EnableIRQ(INT_EXTI2_3);
      break;
    case 4:
      EXTI4_IRQFnc = handler;
      EXTI4_IRQSrc = object;
      NVIC_SetPriority(INT_EXTI4_15, priority);
      NVIC_EnableIRQ(INT_EXTI4_15);
      break;
    case 5:
      EXTI5_IRQFnc = handler;
      EXTI5_IRQSrc = object;
      NVIC_SetPriority(INT_EXTI4_15, priority);
      NVIC_EnableIRQ(INT_EXTI4_15);
      break;
    case 6:
      EXTI6_IRQFnc = handler;
      EXTI6_IRQSrc = object;
      NVIC_SetPriority(INT_EXTI4_15, priority);
      NVIC_EnableIRQ(INT_EXTI4_15);
      break;
    case 7:
      EXTI7_IRQFnc = handler;
      EXTI7_IRQSrc = object;
      NVIC_SetPriority(INT_EXTI4_15, priority);
      NVIC_EnableIRQ(INT_EXTI4_15);
      break;
    case 8:
      EXTI8_IRQFnc = handler;
      EXTI8_IRQSrc = object;
      NVIC_SetPriority(INT_EXTI4_15, priority);
      NVIC_EnableIRQ(INT_EXTI4_15);
      break;
    case 9:
      EXTI9_IRQFnc = handler;
      EXTI9_IRQSrc = object;
      NVIC_SetPriority(INT_EXTI4_15, priority);
      NVIC_EnableIRQ(INT_EXTI4_15);
      break;
    case 10:
      EXTI10_IRQFnc = handler;
      EXTI10_IRQSrc = object;
      NVIC_SetPriority(INT_EXTI4_15, priority);
      NVIC_EnableIRQ(INT_EXTI4_15);
      break;
    case 11:
      EXTI11_IRQFnc = handler;
      EXTI11_IRQSrc = object;
      NVIC_SetPriority(INT_EXTI4_15, priority);
      NVIC_EnableIRQ(INT_EXTI4_15);
      break;
    case 12:
      EXTI12_IRQFnc = handler;
      EXTI12_IRQSrc = object;
      NVIC_SetPriority(INT_EXTI4_15, priority);
      NVIC_EnableIRQ(INT_EXTI4_15);
      break;
    case 13:
      EXTI13_IRQFnc = handler;
      EXTI13_IRQSrc = object;
      NVIC_SetPriority(INT_EXTI4_15, priority);
      NVIC_EnableIRQ(INT_EXTI4_15);
      break;
    case 14:
      EXTI14_IRQFnc = handler;
      EXTI14_IRQSrc = object;
      NVIC_SetPriority(INT_EXTI4_15, priority);
      NVIC_EnableIRQ(INT_EXTI4_15);
      break;
    case 15:
      EXTI15_IRQFnc = handler;
      EXTI15_IRQSrc = object;
      NVIC_SetPriority(INT_EXTI4_15, priority);
      NVIC_EnableIRQ(INT_EXTI4_15);
      break;
  }
}

//-------------------------------------------------------------------------------------------------

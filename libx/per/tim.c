#include "tim.h"

//------------------------------------------------------------------------------------------------- Channel-Map

const GPIO_Map_t tim_channel_map[] = {
  [TIM1_CH1_PA8] = { .port = GPIOA, .pin = 8, .alternate = 2 },
  [TIM1_CH1_PC8] = { .port = GPIOC, .pin = 8, .alternate = 2 },
  [TIM1_CH1N_PA7] = { .port = GPIOA, .pin = 7, .alternate = 2 },
  [TIM1_CH1N_PB13] = { .port = GPIOB, .pin = 13, .alternate = 2 },
  [TIM1_CH1N_PD2] = { .port = GPIOD, .pin = 2, .alternate = 2 },
  [TIM1_CH2_PA9] = { .port = GPIOA, .pin = 9, .alternate = 2 },
  [TIM1_CH2_PB3] = { .port = GPIOB, .pin = 3, .alternate = 1 },
  [TIM1_CH2_PC9] = { .port = GPIOC, .pin = 9, .alternate = 2 },
  [TIM1_CH2N_PB0] = { .port = GPIOB, .pin = 0, .alternate = 2 },
  [TIM1_CH2N_PB14] = { .port = GPIOB, .pin = 14, .alternate = 2 },
  [TIM1_CH2N_PD3] = { .port = GPIOD, .pin = 3, .alternate = 2 },
  [TIM1_CH3_PA10] = { .port = GPIOA, .pin = 10, .alternate = 2 },
  [TIM1_CH3_PB6] = { .port = GPIOB, .pin = 6, .alternate = 1 },
  [TIM1_CH3_PC10] = { .port = GPIOC, .pin = 10, .alternate = 2 },
  [TIM1_CH3N_PB1] = { .port = GPIOB, .pin = 1, .alternate = 2 },
  [TIM1_CH3N_PB15] = { .port = GPIOB, .pin = 15, .alternate = 2 },
  [TIM1_CH3N_PD4] = { .port = GPIOD, .pin = 4, .alternate = 2 },
  [TIM1_CH4_PA11] = { .port = GPIOA, .pin = 11, .alternate = 2 },
  [TIM1_CH4_PC11] = { .port = GPIOC, .pin = 11, .alternate = 2 },
  [TIM2_CH1_PA0] = { .port = GPIOA, .pin = 0, .alternate = 2 },
  [TIM2_CH2_PA1] = { .port = GPIOA, .pin = 1, .alternate = 2 },
  [TIM2_CH3_PA2] = { .port = GPIOA, .pin = 2, .alternate = 2 },
  [TIM2_CH4_PA3] = { .port = GPIOA, .pin = 3, .alternate = 2 },
  [TIM2_CH1_PA5] = { .port = GPIOA, .pin = 5, .alternate = 2 },
  [TIM2_CH1_PA15] = { .port = GPIOA, .pin = 15, .alternate = 2 },
  [TIM2_CH2_PB3] = { .port = GPIOB, .pin = 3, .alternate = 2 },
  [TIM2_CH3_PB10] = { .port = GPIOB, .pin = 10, .alternate = 2 },
  [TIM2_CH4_PB11] = { .port = GPIOB, .pin = 11, .alternate = 2 },
  [TIM2_CH1_PC4] = { .port = GPIOC, .pin = 4, .alternate = 2 },
  [TIM2_CH2_PC5] = { .port = GPIOC, .pin = 5, .alternate = 2 },
  [TIM2_CH3_PC6] = { .port = GPIOC, .pin = 6, .alternate = 2 },
  [TIM2_CH4_PC7] = { .port = GPIOC, .pin = 7, .alternate = 2 },
  [TIM3_CH1_PA6] = { .port = GPIOA, .pin = 6, .alternate = 1 },
  [TIM3_CH2_PA7] = { .port = GPIOA, .pin = 7, .alternate = 1 },
  [TIM3_CH3_PB0] = { .port = GPIOB, .pin = 0, .alternate = 1 },
  [TIM3_CH4_PB1] = { .port = GPIOB, .pin = 1, .alternate = 1 },
  [TIM3_CH1_PB4] = { .port = GPIOB, .pin = 4, .alternate = 1 },
  [TIM3_CH2_PB5] = { .port = GPIOB, .pin = 5, .alternate = 1 },
  [TIM3_CH1_PC6] = { .port = GPIOC, .pin = 6, .alternate = 1 },
  [TIM3_CH2_PC7] = { .port = GPIOC, .pin = 7, .alternate = 1 },
  [TIM3_CH3_PC8] = { .port = GPIOC, .pin = 8, .alternate = 1 },
  [TIM3_CH4_PC9] = { .port = GPIOC, .pin = 9, .alternate = 1 },
  [TIM15_CH1N_PA1] = { .port = GPIOA, .pin = 1, .alternate = 5 },
  [TIM15_CH1_PA2] = { .port = GPIOA, .pin = 2, .alternate = 5 },
  [TIM15_CH2_PA3] = { .port = GPIOA, .pin = 3, .alternate = 5 },
  [TIM15_CH1N_PB13] = { .port = GPIOB, .pin = 13, .alternate = 5 },
  [TIM15_CH1_PB14] = { .port = GPIOB, .pin = 14, .alternate = 5 },
  [TIM15_CH1N_PB15] = { .port = GPIOB, .pin = 15, .alternate = 5 },
  [TIM15_CH2_PB15] = { .port = GPIOB, .pin = 15, .alternate = 4 },
  [TIM15_CH1_PC1] = { .port = GPIOC, .pin = 1, .alternate = 2 },
  [TIM15_CH2_PC2] = { .port = GPIOC, .pin = 2, .alternate = 2 },
  [TIM16_CH1_PA6] = { .port = GPIOA, .pin = 6, .alternate = 5 },
  [TIM16_CH1N_PB6] = { .port = GPIOB, .pin = 6, .alternate = 2 },
  [TIM16_CH1_PB8] = { .port = GPIOB, .pin = 8, .alternate = 2 },
  [TIM16_CH1_PD0] = { .port = GPIOD, .pin = 0, .alternate = 2 },
  [TIM17_CH1_PA7] = { .port = GPIOA, .pin = 7, .alternate = 5 },
  [TIM17_CH1N_PB7] = { .port = GPIOB, .pin = 7, .alternate = 2 },
  [TIM17_CH1_PB9] = { .port = GPIOB, .pin = 9, .alternate = 2 },
  [TIM17_CH1_PD1] = { .port = GPIOD, .pin = 1, .alternate = 2 }
};

//------------------------------------------------------------------------------------------------- TIM-Interface

inline void TIM_Enable(TIM_t *tim)
{
  tim->reg->CR1 |= TIM_CR1_CEN;
  tim->reg->SR &= ~TIM_SR_UIF;
  tim->enable = true;
}

inline void TIM_Disable(TIM_t *tim)
{
  tim->reg->CR1 &= ~TIM_CR1_CEN;
  tim->_inc = 0;
  tim->enable = false;
}

inline void TIM_InterruptEnable(TIM_t *tim)
{
  tim->reg->DIER |= TIM_DIER_UIE;
  tim->enable_interrupt = true;
}

inline void TIM_InterruptDisable(TIM_t *tim)
{
  tim->reg->DIER &= ~TIM_DIER_UIE;
  tim->_inc = 0;
  tim->enable_interrupt = false;
}

inline bool TIM_IsDisable(TIM_t *tim)
{
  return !(tim->reg->CR1 & TIM_CR1_CEN_Msk);
}

inline bool TIM_IsEnable(TIM_t *tim)
{
  return tim->reg->CR1 & TIM_CR1_CEN_Msk;
}

inline void TIM_ResetValue(TIM_t *tim)
{
  if(tim->reg == TIM2) tim->reg->CNT = 0;
  else tim->reg->CNT &= 0xFFFF0000;
  tim->_inc = 0;
}

inline uint32_t TIM_GetValue(TIM_t *tim)
{
  if(tim->reg == TIM2) return tim->reg->CNT & 0xFFFFFFFF;
  else return tim->reg->CNT & 0x0000FFFF;
}

void TIM_SetPrescaler(TIM_t *tim, uint32_t prescaler)
{
  tim->prescaler = prescaler;
  tim->reg->PSC = tim->prescaler;
}

void TIM_SetAutoreload(TIM_t *tim, uint32_t auto_reload)
{
  tim->auto_reload = auto_reload;
  tim->reg->ARR = tim->auto_reload;
}

void TIM_MaxAutoreload(TIM_t *tim)
{
  if((uint32_t)tim->reg == (uint32_t)TIM2) tim->auto_reload = 0xFFFFFFFF;
  else tim->auto_reload = 0xFFFF;
  tim->reg->ARR = tim->auto_reload;
}

uint16_t TIM_Event(TIM_t *tim)
{
  if(tim->_inc) {
    uint16_t response = tim->_inc;
    tim->_inc = 0;
    return response;
  }
  return 0;
}

//------------------------------------------------------------------------------------------------- INIT

static void TIM_Interrupt(TIM_t *tim)
{
  if(tim->reg->SR & TIM_SR_UIF)
  {
    tim->reg->SR &= ~TIM_SR_UIF;
    if(tim->function) tim->function(tim->function_struct);
    tim->_inc++;
  }
}

void TIM_Init(TIM_t *tim)
{
  RCC_EnableTIM(tim->reg);
	tim->reg->PSC = tim->prescaler;
	if(!tim->auto_reload) TIM_MaxAutoreload(tim);
	else tim->reg->ARR = tim->auto_reload;
	tim->reg->CR1 |= (!(tim->one_pulse_mode) << TIM_CR1_ARPE_Pos) | (tim->one_pulse_mode << TIM_CR1_OPM_Pos);
	if(tim->enable) TIM_Enable(tim);
	if(tim->enable_interrupt) {
	  INT_EnableTIM(tim->reg, tim->interrupt_level, (void (*)(void *))&TIM_Interrupt, tim);
	  TIM_InterruptEnable(tim);
	}
}

/**
 * @brief This function sets timer in master mode so it outputs TRGO signal
 * @param tim - pointer to timer which should be set as master
 * @param mode - type of master mode
 * @retval void
 */
void TIM_MasterMode(TIM_t *tim, TIM_MasterMode_e mode)
{
  tim->reg->CR1 &= ~TIM_CR1_UDIS;
  tim->reg->CR2 &=  ~TIM_CR2_MMS_Msk;
  tim->reg->CR2 |= (mode << TIM_CR2_MMS_Pos);
}

//-------------------------------------------------------------------------------------------------

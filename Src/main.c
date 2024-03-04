#include "stm32g0xx.h"

#define delay_ms(ms) for(int _i = 3200 * ms; _i; _i--);

// Programuję wielowątkowo. Czy jak w jakieś funkcji tworzę tablicę buffer[100],
// to czy stack przekazany wątkowi musi przewidaywać miejce na buffer

// static uint32_t stack_1[128];
// static uint32_t stack_2[128];
// static uint32_t stack_3[128];

// int main(void)
// {
//   RCC->IOPENR |= RCC_IOPSMENR_GPIOASMEN;
//   GPIOA->MODER = (GPIOA->MODER & ~(0x03 << (2 * 5))) | (0x01 << (2 * 5));
//   // set PA5 as output (LED)
//   SYSTICK_Init(10); // basetime 10ms
//   thread(&Thread_1, stack_1, sizeof(stack_1));
//   thread(&Thread_2, stack_2, sizeof(stack_2));
//   thread(&Thread_3, stack_3, sizeof(stack_3));

// Przemyśleć bibliotekę mem - żeby dobrze działała wielowątkowo
// umieścić ją w folderze sys
// jeżeli wielowątkowość to użyj vrts_state.thread_nbr do indeksowania !!!
// aloc i dloc nie potrzebne

#include "uno.h"
#include "cron.h"

// TIM_t timer = ""
// TIM7
// TIM14
// #define TIM15
// #define TIM16
// #define TIM17

void WednesdayNight(void)
{

}

void CheckInput(DIN_t *din)
{

}

void FirstDayMonth(void)
{
  
}

#include "uno.h"

int main(void)
{
  CRON_Task(&WednesdayNight, NULL, CRON_NULL, RTC_WEDNESDAY, 23, 00); // Wednesday 23:00
  CRON_Task(&CheckInput,     &DI1, CRON_NULL, CRON_NULL,     06, 30); // Everyday  06:30
  CRON_Task(&FirstDayMonth,  NULL, 1,         CRON_NULL,     17, 45); // Everyday  17:45
  PLC_Init(); 
  while(1) {
    PLC_Loop();
  }
}


// int main2(void)
// {
//   RCC->IOPENR |= RCC_IOPSMENR_GPIOASMEN;
//   GPIOA->MODER = (GPIOA->MODER & ~(0x03 << (2 * 5))) | (0x01 << (2 * 5));
//   while(1) {
//     GPIOA->BSRR |= (1 << 5); // PA5 LED turn on
//     delay_ms(100);
//     GPIOA->BRR |= (1 << 5); // PA5 LED turn off
//     delay_ms(100);
//   }
// }


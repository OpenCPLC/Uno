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

// uint8_t

int main(void)
{
  PLC_Init();
  while(1) {
    RELAY_Set(&RO1); // DOUT_Set(&TO1);
    delay(1000);
    RELAY_Rst(&RO1); // DOUT_Rst(&TO1);
    delay(1000);
    PLC_Loop();
  }
}


#include "uno.h"

static uint32_t stack_plc[64];
static uint32_t stack_pwm[64];

int main(void)
{
  PLC_Init();
  thread(&PLC_Loop, stack_plc, sizeof(stack_plc));
  thread(&pwm_control, stack_pwm, sizeof(stack_pwm));
  VRTS_Init();
  while(1);
}

void pwm_control(void)
{
  TO_Frequency(200); // 200Hz
  XO_Frequency(1);   // 1Hz
  float value = 0;
  while(1) {
    DOUT_Duty(&TO1, value);
    DOUT_Duty(&XO1, value);
    delay(10);
    value += 0.1;
    if(value > 100) value = 0;
  }
}



#include "uno.h"

// uint8_t

int main(void)
{

  DI1.gpif.ton_ms = 100;
  DI1.gpif.toff_ms = 500;
  PLC_Init();
  while(1) {
  }
}

#include "uno.h"

int main2(void)
{
  PLC_Init();
  while(1) {
    RELAY_Set(&RO1);
    delay(1000);
    RELAY_Rst(&RO1);
    delay(1000);
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


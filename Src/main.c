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
#include "modbus-master.h"

int loop(void);

static uint32_t stack_plc[64];
static uint32_t stack_loop[512];

UART_t *rs485 = &RS1;

#define ADDR 0x02

int main(void)
{
  PLC_Init();
  MODBUS_PresetRegister(rs485, ADDR, 0x10, 1152, sec(1));
  thread(&PLC_Loop, stack_plc, sizeof(stack_plc));
  thread(&loop, stack_plc, sizeof(stack_plc));
}

struct {
  uint16_t uint16;
  uint32_t uint32;
} regmap;

int loop(void)
{
  MODBUS_ReadHoldingRegisters(rs485, ADDR, 0x14, 2, &regmap, sec(1));
  DBG_String("uint16 = ");
  DBG_uDec(regmap.uint16);
  // regmap.uint32 = (regmap.uint32 >> 16) | (regmap.uint32 << 16); // swap
  DBG_String(", uint32 = ");
  DBG_uDec(regmap.uint32);
  DBG_Enter();
  delay(sec(0.5));
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


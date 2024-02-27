#include "stm32g0xx.h"

#define delay_ms(ms) for(int _i = 3200 * ms; _i; _i--);

int main(void)
{
  RCC->IOPENR |= RCC_IOPSMENR_GPIOASMEN;
  GPIOA->MODER = (GPIOA->MODER & ~(0x03 << (2 * 5))) | (0x01 << (2 * 5));
  while(1) {
    GPIOA->BSRR |= (1 << 5); // PA5 LED turn on
    delay_ms(100);
    GPIOA->BRR |= (1 << 5); // PA5 LED turn off
    delay_ms(100);
  }
}









// #include "uno.h"
// #include "eeprom.h"

// /**
//  * @file main.c
//  * @brief Template OpenCPLC - System start-stop
//  */

// DIN_t *start_button = &DI1;
// DIN_t *stop_button = &DI2;
// RELAY_t *motor_running = &RO1;

// uint32_t x;

// EEPROM_t myeeprom = { .page_a = 62, .page_a = 63 };

// int main(void)
// {
//   EEPROM_Read(&myeeprom, &x);

//   PLC_Init();

//   EEPROM_Load(&x);


//   while(1) {
//     if(DIN_Rais(stop_button)) {
//       RELAY_Rst(motor_running);
//     }
//     else if(DIN_Rais(start_button)) {
//       x = 12;
//       EEPROM_Save(&x);
//       RELAY_Set(motor_running);
//     }
//     PLC_Loop();
//   }
// }
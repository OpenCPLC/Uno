// #include "vrts.h"

// #define delay_ms(ms) for(int _i = 3200 * ms; _i; _i--);

// int main(void)
// {
//   SYSTICK_Init(10);
//   RCC->IOPENR |= RCC_IOPSMENR_GPIOASMEN;
//   GPIOA->MODER = (GPIOA->MODER & ~(0x03 << (2 * 5))) | (0x01 << (2 * 5));
//   while(1) {
//     GPIOA->BSRR |= (1 << 5); // PA5 LED turn on
//     delay_ms(100);
//     GPIOA->BRR |= (1 << 5); // PA5 LED turn off
//     delay_ms(100);
//   }
// }





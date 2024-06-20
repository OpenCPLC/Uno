#include "gpio.h"
#include "extime.h"

uint8_t data = 12;

GPIO_t pin_scl = { .port = GPIOA, .pin = 9, .mode = GPIO_Mode_Output, .set = true };
GPIO_t pin_sda = { .port = GPIOA, .pin = 10, .mode = GPIO_Mode_Output, .out_type = GPIO_OutType_OpenDrain, .speed = GPIO_Speed_VeryHigh };

TIM_t time_delay = { .reg = TIM15 };

int main(void)
{
  GPIO_Init(&pin_sda);
  sleep_us_init(&time_delay);
  // W to miejsce program nigdy nie powinien dojść
  while(1) {
    GPIO_Set(&pin_sda);
    // sleep_core_us(500);
    sleep_us(480);
    GPIO_Rst(&pin_sda);

    sleep_us(480);
    // sleep_core_us(500);
  }
}
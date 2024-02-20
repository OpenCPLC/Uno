#include "uno.h"

/**
 * @file main.c
 * @brief Template OpenCPLC - System start-stop
 */

DIN_t *start_button = &DI1;
DIN_t *stop_button = &DI2;
RELAY_t *motor_running = &RO1;

int main(void)
{
  PLC_Init();
  while(1) {
    if(DIN_Rais(stop_button)) {
      RELAY_Rst(motor_running);
    }
    else if(DIN_Rais(start_button)) {
      RELAY_Set(motor_running);
    }
    PLC_Loop();
  }
}
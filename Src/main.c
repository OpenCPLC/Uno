#include "uno.h"

bool safety_signal;
bool start_button;
bool motor_running;

void main_classic(void)
{
  PLC_Init();
  while(1) {
    safety_signal = DIN_State(&DI1);
    start_button = DIN_State(&DI2);
    if(safety_signal && start_button) motor_running = true;
    else if(!safety_signal) {
      motor_running = false;
    }
    if(motor_running) RELAY_Preset(&RO1, motor_running);
    PLC_Loop();
  }
}

DIN_t *safety_sig = &DI1;
DIN_t *start_btn = &DI2;
RELAY_t *motor_run = &RO1;

void main_modern(void)
{
  PLC_Init();
  while(1) {
    if(DIN_State(safety_sig) && DIN_Rais(start_btn)) {
      RELAY_Set(motor_run);
    }
    else if(!DIN_State(safety_sig)) {
      RELAY_Rst(motor_run);
    }
    PLC_Loop();
  }
}

int main(void)
{
  // main_classic();
  main_modern();
}

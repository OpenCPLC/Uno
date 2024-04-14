#include "opencplc-uno.h"

static uint32_t stack_plc[256];
static uint32_t stack_loop[256];

void loop(void)
{
  RGB_e color = RGB_Off;
  while(1) {
    if(DIN_Fall(&BTN)) {
      color++;
      if(color > RGB_END_COLOR) color = RGB_Off;
      LED_Set(color);
    }
    let();
  }
}

int main(void)
{
  PLC_Init();
  thread(&PLC_Thread, stack_plc, sizeof(stack_plc));
  thread(&loop, stack_loop, sizeof(stack_loop));
  VRTS_Init();
  while(1);
}
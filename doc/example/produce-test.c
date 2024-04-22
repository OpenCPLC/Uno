#include "opencplc-uno.h"

static uint32_t stack_plc[256];
static uint32_t stack_loop[256];

void loop(void)
{
  uint8_t state = 0;
  while(1) {
    if(DIN_Fall(&BTN)) {
      state++;
      if(state > 3) state = 0;
    }
    switch(state) {
      case 0:
        DOUT_Set(&TO1);
        DOUT_Rst(&TO2);
        DOUT_Rst(&TO3);
        DOUT_Rst(&TO4);
        LED_Rst();
        break;
      case 1:
        DOUT_Rst(&TO1);
        DOUT_Set(&TO2);
        DOUT_Rst(&TO3);
        DOUT_Rst(&TO4);
        LED_Set(RGB_Red);
        break;
      case 2:
        DOUT_Rst(&TO1);
        DOUT_Rst(&TO2);
        DOUT_Set(&TO3);
        DOUT_Rst(&TO4);
        LED_Set(RGB_Green);
        break;
      case 3:
        DOUT_Rst(&TO1);
        DOUT_Rst(&TO2);
        DOUT_Rst(&TO3);
        DOUT_Set(&TO4);
        LED_Set(RGB_Blue);
        break;
    }
    if(DIN_State(&DI1)) DOUT_Set(&RO1);
    else DOUT_Rst(&RO1);
    if(DIN_State(&DI2)) DOUT_Set(&RO2);
    else DOUT_Rst(&RO2);
    if(DIN_State(&DI3)) DOUT_Set(&RO3);
    else DOUT_Rst(&RO3);
    if(DIN_State(&DI4)) DOUT_Set(&RO4);
    else DOUT_Rst(&RO4);
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

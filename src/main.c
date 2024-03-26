#include "opencplc-uno.h"

static uint32_t stack_plc[64];
static uint32_t stack_loop[64];

int main(void)
{
  PLC_Init();
  thread(&PLC_Loop, stack_plc, sizeof(stack_plc));
  thread(&loop, stack_loop, sizeof(stack_loop));
  VRTS_Init();
  while(1);
}

void loop(void)
{
  while(1) {
    DOUT_Set(&RO1);
    delay(1000);
    DOUT_Rst(&RO1);
    delay(1000);
    //
  }
}
#include "opencplc-uno.h"

static uint32_t stack_plc[256];
static uint32_t stack_loop[256];

#define ADDR 0x18 // LIS3DH device address

void loop(void)
{
  uint8_t buffer[6];
  buffer[0] = 0x77;
  I2C_Write(ADDR, 0x20, buffer, 1);
  while(1) {
    if(I2C_Read(ADDR, 0x28 | 0x80, buffer, 6)) {
      int16_t x = ((int16_t)buffer[1] << 8) | buffer[0];
      int16_t y = ((int16_t)buffer[3] << 8) | buffer[2];
      int16_t z = ((int16_t)buffer[5] << 8) | buffer[4];
      DBG_String("LIS3DH x:"); DBG_Dec(x);
      DBG_String(" y:"); DBG_Dec(y);
      DBG_String(" z:"); DBG_Dec(z);
      DBG_Enter();
      LED_Set(RGB_Green);
    }
    else {
      LED_Set(RGB_Red);
      delay(100);
    }
    delay(100);
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

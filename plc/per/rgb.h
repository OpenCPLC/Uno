#ifndef RGB_H_
#define RGB_H_

#include "gpio.h"

//-------------------------------------------------------------------------------------------------

typedef enum {
  RGB_Off = 0,
  RGB_Red = 1,
  RGB_Green = 2,
  RGB_Blue = 3,
  RGB_Yellow = 4,
  RGB_Cyan = 5,
  RGB_Magenta = 6,
  RGB_White = 7
} RGB_e;

#define RGB_END_COLOR RGB_White

typedef struct {
  GPIO_t *red;
  GPIO_t *green;
  GPIO_t *blue;
  RGB_e state;
  uint16_t blink_ms;
  uint64_t tick;
  bool blink_on;
} RGB_t;

void RGB_Init(RGB_t *rgb);
void RGB_Set(RGB_t *rgb, RGB_e color);
void RGB_Rst(RGB_t *rgb);
void RGB_Tgl(RGB_t *rgb);
void RGB_Loop(RGB_t *rgb);

//-------------------------------------------------------------------------------------------------

#endif
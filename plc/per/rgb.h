#ifndef RGB_H_
#define RGB_H_

#include "gpio.h"

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

typedef struct {
  GPIO_t *reg;
  GPIO_t *green;
  GPIO_t *blue;
  RGB_e color;
  bool isset;
} RGB_t;

void RGB_Init(RGB_t *rgb);
void RGB_Set(RGB_t *rgb, RGB_e color);
void RGB_Rst(RGB_t *rgb);
void RGB_Tgl(RGB_t *rgb);


#endif
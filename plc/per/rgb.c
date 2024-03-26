#include "rgb.h"

//---------------------------------------------------------------------------------------------------------------------

const char *rgb_color_name[] = { "off\0", "red\0", "green\0", "blue\0", "yellow\0", "cyan\0", "magenta\0", "white\0" };

//---------------------------------------------------------------------------------------------------------------------

static void RGB_SetRed(RGB_t *rgb) { if(rgb->red.gpio_typedef) GPIO_Set(&rgb->red); }
static void RGB_RstRed(RGB_t *rgb) { if(rgb->red.gpio_typedef) GPIO_Rst(&rgb->red); }
static void RGB_SetGreen(RGB_t *rgb) { if(rgb->green.gpio_typedef) GPIO_Set(&rgb->green); }
static void RGB_RstGreen(RGB_t *rgb) { if(rgb->green.gpio_typedef) GPIO_Rst(&rgb->green); }
static void RGB_SetBlue(RGB_t *rgb) { if(rgb->blue.gpio_typedef) GPIO_Set(&rgb->blue); }
static void RGB_RstBlue(RGB_t *rgb) { if(rgb->blue.gpio_typedef) GPIO_Rst(&rgb->blue); }

//---------------------------------------------------------------------------------------------------------------------

void RGB_Init(RGB_t *rgb)
{
  rgb->red.mode = GPIO_M
  rgb->green.mode = GPIO_Output;
  rgb->blue.mode = GPIO_Output;

  if(rgb->red.gpio_typedef) {
    rgb->red->mode = GPIO_Mod
    GPIO_Init(&rgb->red);
  }
  if(rgb->green.gpio_typedef) GPIO_Init(&rgb->green);
  if(rgb->blue.gpio_typedef) GPIO_Init(&rgb->blue);

  if(rgb->color) RGB_Set(rgb, rgb->color);
}

//---------------------------------------------------------------------------------------------------------------------

void RGB_Set(RGB_t *rgb, RGB_Color_e color)
{
  rgb->color = color;

  switch(rgb->color) {
    case RGB_Off:     RGB_RstRed(rgb); RGB_RstGreen(rgb); RGB_RstBlue(rgb); rgb->_set = false; return;
    case RGB_Red:     RGB_SetRed(rgb); RGB_RstGreen(rgb); RGB_RstBlue(rgb); break;
    case RGB_Green:   RGB_RstRed(rgb); RGB_SetGreen(rgb); RGB_RstBlue(rgb); break;
    case RGB_Blue:    RGB_RstRed(rgb); RGB_RstGreen(rgb); RGB_SetBlue(rgb); break;
    case RGB_Yellow:  RGB_SetRed(rgb); RGB_SetGreen(rgb); RGB_RstBlue(rgb); break;
    case RGB_Cyan:    RGB_RstRed(rgb); RGB_SetGreen(rgb); RGB_SetBlue(rgb); break;
    case RGB_Magenta: RGB_SetRed(rgb); RGB_RstGreen(rgb); RGB_SetBlue(rgb); break;
    case RGB_White:   RGB_SetRed(rgb); RGB_SetGreen(rgb); RGB_SetBlue(rgb); break;
  }
  rgb->_set = true;
}

void RGB_Rst(RGB_t *rgb)
{
  RGB_RstRed(rgb); RGB_RstGreen(rgb); RGB_RstBlue(rgb);
  rgb->_set = false;
}

void RGB_Tgl(RGB_t *rgb)
{
  if(rgb->_set) RGB_Rst(rgb);
  else RGB_Set(rgb, rgb->color);
}

//--------------------------------------------------------------------------------------------------------------------------------


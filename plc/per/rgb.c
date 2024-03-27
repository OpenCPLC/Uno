#include "rgb.h"

//-------------------------------------------------------------------------------------------------

const char *rgb_color_name[] = { "off", "red", "green", "blue", "yellow", "cyan", "magenta", "white" };

//-------------------------------------------------------------------------------------------------

static void RGB_SetRed(RGB_t *rgb) { if(rgb->red) GPIO_Set(rgb->red); }
static void RGB_RstRed(RGB_t *rgb) { if(rgb->red) GPIO_Rst(rgb->red); }
static void RGB_SetGreen(RGB_t *rgb) { if(rgb->green) GPIO_Set(rgb->green); }
static void RGB_RstGreen(RGB_t *rgb) { if(rgb->green) GPIO_Rst(rgb->green); }
static void RGB_SetBlue(RGB_t *rgb) { if(rgb->blue) GPIO_Set(rgb->blue); }
static void RGB_RstBlue(RGB_t *rgb) { if(rgb->blue) GPIO_Rst(rgb->blue); }

//-------------------------------------------------------------------------------------------------

void RGB_Init(RGB_t *rgb)
{
  if(rgb->red) {
    rgb->red->mode = GPIO_Mode_Output;
    GPIO_Init(rgb->red);
  }
  if(rgb->green) {
    rgb->green->mode = GPIO_Mode_Output;
    GPIO_Init(rgb->green);
  }
  if(rgb->blue) {
    rgb->blue->mode = GPIO_Mode_Output;
    GPIO_Init(rgb->blue);
  }
  RGB_Set(rgb, rgb->state);
}

//-------------------------------------------------------------------------------------------------

static void RGB_Preset(RGB_t *rgb, RGB_e color)
{
  switch(color) {
    case RGB_Off: RGB_RstRed(rgb); RGB_RstGreen(rgb); RGB_RstBlue(rgb); break;
    case RGB_Red: RGB_SetRed(rgb); RGB_RstGreen(rgb); RGB_RstBlue(rgb); break;
    case RGB_Green: RGB_RstRed(rgb); RGB_SetGreen(rgb); RGB_RstBlue(rgb); break;
    case RGB_Blue: RGB_RstRed(rgb); RGB_RstGreen(rgb); RGB_SetBlue(rgb); break;
    case RGB_Yellow: RGB_SetRed(rgb); RGB_SetGreen(rgb); RGB_RstBlue(rgb); break;
    case RGB_Cyan: RGB_RstRed(rgb); RGB_SetGreen(rgb); RGB_SetBlue(rgb); break;
    case RGB_Magenta: RGB_SetRed(rgb); RGB_RstGreen(rgb); RGB_SetBlue(rgb); break;
    case RGB_White: RGB_SetRed(rgb); RGB_SetGreen(rgb); RGB_SetBlue(rgb); break;
  }
}

void RGB_Set(RGB_t *rgb, RGB_e color)
{
  RGB_Preset(rgb, color);
  rgb->state = color;
}

void RGB_Rst(RGB_t *rgb)
{
  RGB_Set(rgb, RGB_Off);
}

void RGB_Tgl(RGB_t *rgb)
{
  if(rgb->state) RGB_Rst(rgb);
  else RGB_Set(rgb, rgb->state);
}

void RGB_Loop(RGB_t *rgb)
{
  if(rgb->blink_ms && rgb->state) {
    if(waitfor(&rgb->tick)) {
      rgb->blink_on = !rgb->blink_on;
      if(rgb->blink_on) RGB_Preset(rgb, rgb->state);
      else RGB_Preset(rgb, RGB_Off);
    };
    if(!rgb->tick) {
      rgb->tick = gettick(rgb->blink_ms);
    }
  }
}

//-------------------------------------------------------------------------------------------------

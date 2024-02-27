#ifndef DOUT_H_
#define DOUT_H_

/**
 * @file dout.h
 * @brief Obsługa wyjść cyfrowych tranzystorowych i triakowych OpenCPLC
 */

#include <stdint.h>
#include <stdbool.h>
#include "eeprom.h"
#include "pwm.h"
#include "vrts.h"

//-------------------------------------------------------------------------------------------------

typedef struct {
  PWM_t *pwm;            // Wskaźnik na kontroler PWM_t.
  TIM_Channel_e channel; // Kanał kontrolera PWM_t sterujący 
  EEPROM_t *eeprom;      // Wskaźnik na pamięć EEPROM_t do przechowywania wartości nieulotnych.
  uint32_t save;         // Określa, czy zachować stan wyjścia po resecie.
  uint32_t value;        // Wypełnienie [%] sygnału na wyjściu. Zaleca się ustawić funkcją `DOUT_Set`.
} DOUT_t;

float PWM_Frequency(PWM_t *pwm, float frequency);
float DOUT_Fill(DOUT_t *dout, float fill);
void DOUT_Set(DOUT_t *dout);
void DOUT_Rst(DOUT_t *dout);
void DOUT_Tgl(DOUT_t *dout);
void DOUT_Preset(DOUT_t *dout, bool value);
float DOUT_GetFill(DOUT_t *dout);
void DOUT_Init(DOUT_t *dout);
void DOUT_Config(DOUT_t *dout, bool save);

//-------------------------------------------------------------------------------------------------
#endif
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

#define DOUT_RELAY_DELAY 200

typedef struct {
  bool relay;            // Czy wyjście jest przekaźnikowe RO
  GPIO_t gpio;           // Wskaźnik na wyjście GPIO_t. Należy skonfigurować pola `port` i `pin`.
  PWM_t *pwm;            // Wskaźnik na kontroler PWM_t.
  TIM_Channel_e channel; // Kanał kontrolera PWM_t sterujący 
  EEPROM_t *eeprom;      // Wskaźnik na pamięć EEPROM_t do przechowywania wartości nieulotnych.
  uint32_t save;         // Określa, czy zachować stan wyjścia po resecie.
  uint32_t value;        // Wypełnienie [%] sygnału na wyjściu. Zaleca się ustawić funkcją `DOUT_Set`.
  uint32_t cycles;       // Całkowita liczba przełączeń przekażnika.
  bool pulse;            // Flaga informująca, czy przekażnik wykonuje impuls
  uint64_t _stun;
  uint16_t _pulse;
} DOUT_t;

float PWM_Frequency(PWM_t *pwm, float frequency);
float DOUT_Duty(DOUT_t *dout, float duty);
void DOUT_Set(DOUT_t *dout);
void DOUT_Rst(DOUT_t *dout);
void DOUT_Tgl(DOUT_t *dout);
void DOUT_Preset(DOUT_t *dout, bool value);
bool DOUT_Pulse(DOUT_t *dout, uint16_t time_ms);
uint32_t DOUT_State(DOUT_t *dout);
bool DOUT_IsPulse(DOUT_t *relay);
float DOUT_GetDuty(DOUT_t *dout);
void DOUT_Init(DOUT_t *dout);
void DOUT_Loop(DOUT_t *dout);
void DOUT_Settings(DOUT_t *dout, bool save);

//-------------------------------------------------------------------------------------------------
#endif
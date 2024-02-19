#ifndef RELAY_H_
#define RELAY_H_

/**
 * @file relay.h
 * @brief Obsługa wyjść cyfrowych przekaźnikowych OpenCPLC
 */

#include <stdint.h>
#include <stdbool.h>
#include "eeprom.h"
#include "gpio.h"
#include "vrts.h"

//-------------------------------------------------------------------------------------------------

#define RELAY_SWITCH_DELAY 200

typedef struct {
  GPIO_t gpio;      // Wskaźnik na wyjście GPIO_t. Należy skonfigurować pola `port` i `pin`.
  EEPROM_t *eeprom; // Wskaźnik na pamięć EEPROM_t do przechowywania wartości nieulotnych.
  uint32_t save;    // Określa, czy zachować stan wyjścia po resecie.
  uint32_t value;   // Aktualny stan przekażnika.
  uint32_t cycles;  // Całkowita liczba przełączeń przekażnika.
  bool pulse;       // Flaga informująca, czy przekażnik wykonuje impuls
  uint64_t _stun;
  uint16_t _pulse;
} RELAY_t;

void RELAY_Config(RELAY_t *relay, bool save);
void RELAY_Set(RELAY_t *relay);
void RELAY_Rst(RELAY_t *relay);
void RELAY_Tgl(RELAY_t *relay);
void RELAY_Preset(RELAY_t *relay, bool value);
void RELAY_Pulse(RELAY_t *relay, uint16_t time_ms);
bool RELAY_State(RELAY_t *relay);
bool RELAY_IsPulse(RELAY_t *relay);
void RELAY_Init(RELAY_t *relay);
bool RELAY_Loop(RELAY_t *relay);

//-------------------------------------------------------------------------------------------------
#endif
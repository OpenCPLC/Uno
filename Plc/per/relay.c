/**
 * @file relay.c
 * @brief Obsługa wyjść cyfrowych przekaźnikowych OpenCPLC
 */

#include "relay.h"

//-------------------------------------------------------------------------------------------------

/**
 * @brief Wykonuje zwarcie na wyjściu przekaźnikowym (RO)
 * Jeśli opcja `save` jest włączona, aktualny stan jest zapisywany do EEPROM.
 * @param relay Wskaźnik do struktury reprezentującej wyjście przekaźnikowe (RO).
 */
void RELAY_Set(RELAY_t *relay)
{
  if(relay->value) return;
  relay->value = true;
  if(relay->save) EEPROM_Write(&relay->eeprom, &relay->value);
}

/**
 * @brief Wykonuje rozwarcie na wyjściu przekaźnikowym (RO)
 * Jeśli opcja `save` jest włączona, aktualny stan jest zapisywany do EEPROM.
 * @param relay Wskaźnik do struktury reprezentującej wyjście przekaźnikowe (RO).
 */
void RELAY_Rst(RELAY_t *relay)
{
  if(!relay->value) return;
  relay->value = false;
  if(relay->save) EEPROM_Write(&relay->eeprom, &relay->value);
}

/**
 * @brief Przełącza wyjście przekaźnikowego (zmienia stan na przeciwny).
 * Jeśli opcja `save` jest włączona, aktualny stan jest zapisywany do EEPROM.
 * @param relay Wskaźnik do struktury reprezentującej wyjście przekaźnikowe (RO).
 */
void RELAY_Tgl(RELAY_t *relay)
{
  relay->value = !relay->value;
  if(relay->save) EEPROM_Write(&relay->eeprom, &relay->value);
}

/**
 * @brief Ustawia wyjście przekaźnikowe na określony stan `true`/`false`.
 * Jeśli wartość to `true`, funkcja jest równoważna `RELAY_Set`, w przeciwnym razie `RELAY_Rst`.
 * @param relay Wskaźnik do struktury reprezentującej wyjście przekaźnikowe (RO).
 * @param value Wartość `bool`, na którą ma zostać ustawione wyjście przekaźnikowe.
 */
void RELAY_Preset(RELAY_t *relay, bool value)
{
  if(value) RELAY_Set(relay);
  else RELAY_Rst(relay); 
}

/**
 * @brief Aktywuje impuls - zmienia stan przekaźnika na określony czas `time_ms`.
 * @param relay Wskaźnik do struktury reprezentującej wyjście przekaźnikowe (RO).
 * @param time_ms Czas trwania impulsu w milisekundach.
 */
void RELAY_Pulse(RELAY_t *relay, uint16_t time_ms)
{
  if(!relay->_pulse) relay->_pulse = time_ms;
}

/**
 * @brief Pobiera aktualny stan wyjścia przekaźnikowego.
 * Funkcja zwraca wartość logiczną, czy wyjście przekaźnikowe jest aktywne.
 * @param relay Wskaźnik do struktury reprezentującej wyjście przekaźnikowe (RO).
 * @return Wartość `true` dla aktywnego stanu, `false` dla nieaktywnego.
 */
bool RELAY_State(RELAY_t *relay)
{
  return GPIO_In(&relay->gpio);
}

/**
 * @brief Sprawdza, czy wyjście przekaźnikowe jest w trakcie impulsu.
 * Funkcja zwraca wartość logiczną, czy wyjście przekaźnikowe jest w trakcie impulsu.
 * @param relay Wskaźnik do struktury reprezentującej wyjście przekaźnikowe (RO).
 * @return Wartość `true` dla trwającego impulsu, `false` w przeciwnym razie.
 */
bool RELAY_IsPulse(RELAY_t *relay)
{
  return relay->pulse;
}

//-------------------------------------------------------------------------------------------------

/**
 * @brief Inicjalizuje określone wyjście cyfrowe przekaźnikowe (RO).
 * @param relay Wskaźnik do struktury reprezentującej wejście przekaźnikowe (RO).
 */
void RELAY_Init(RELAY_t *relay)
{
  EEPROM_Init(&relay->eeprom);
  EEPROM_Read(&relay->eeprom, &relay->save);
  if(relay->save) EEPROM_Read(&relay->eeprom, &relay->value);
  EEPROM_Read(&relay->eeprom, &relay->cycles);
  relay->gpio.mode = GPIO_Mode_Output;
  GPIO_Init(&relay->gpio);
}

static inline void _RELAY_CyclesInc(RELAY_t *relay)
{
  relay->cycles++;
  EEPROM_Write(&relay->eeprom, &relay->cycles);
}

/**
 * @brief Pętla obsługująca wyjście cyfrowe przekaźnikowe (RO).
 * Funkcję należy wywoływać w każdej iteracji pętli głównej lub dowolnego wątku.
 * Zalecane jest, aby była uruchamiana co najmniej raz na 100 ms.
 * @param relay Wskaźnik do struktury reprezentującej wyjście przekaźnikowe (RO).
 * @return Wartość `true` dla zwartego styku przekaźnika, `false` dla rozwartego.
 */
bool RELAY_Loop(RELAY_t *relay)
{
  waitfor(&relay->_stun);
  if(relay->_stun) return relay->gpio.set;
  if(relay->_pulse) {
    if(!relay->gpio.set) _RELAY_CyclesInc(relay);
    GPIO_Tgl(&relay->gpio);
    relay->_stun = gettick(relay->_pulse);
    relay->_pulse = 0;
    relay->pulse = true;
    return relay->gpio.set;
  }
  relay->pulse = false;
  if(relay->gpio.set != relay->value) {
    if(relay->value) {
      GPIO_Set(&relay->gpio);
      relay->value = true;
      _RELAY_CyclesInc(relay);
    }
    else {
      GPIO_Rst(&relay->gpio);
      relay->value = false;
    }
    relay->_stun = gettick(RELAY_SWITCH_DELAY);
  }
  return relay->gpio.set;
}

/**
 * @brief Ustawia parametr `save` dla wyjścia cyfrowego przekaźnikowego (RO).
 * @param relay Wskaźnik do struktury reprezentującej wyjście przekaźnikowe (RO).
 * @param save Określa, czy zachować stan wyjścia przekaźnikowego po resecie.
 */
void RELAY_Config(RELAY_t *relay, bool save)
{
  if(save != relay->save) {
    EEPROM_Write(&relay->eeprom, &relay->save);
  }
}

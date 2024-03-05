/**
 * @file dout.c
 * @brief Obsługa wyjść cyfrowych tranzystorowych i triakowych OpenCPLC
 */

#include "dout.h"

//-------------------------------------------------------------------------------------------------

/**
 * @brief Ustawia częstotliwość dla sygnału PWM.
 * Zeruje wartości wypełnienia `duty` dla wszystkich kanałów PWM.
 * Kontroler PWM_t jest powiązany z kilkoma wyjściami cyfrowymi.
 * Zatem częstotliwość zostanie zmieniona na wszystkich powiązanych wyjściach cyfrowych.
 * @param pwm Wskaźnik do struktury reprezentującej sygnał PWM.
 * @param frequency Docelowa częstotliwość sygnału PWM.
 * @return Rzeczywista ustawiona częstotliwość sygnału PWM.
 */
float PWM_Frequency(PWM_t *pwm, float frequency)
{
  uint32_t auto_reload = (float)SystemCoreClock / (pwm->prescaler + 1) / frequency / (pwm->center_aligned + 1);
  PWM_SetAutoreload(pwm, auto_reload);
  if(pwm->channel[TIM_CH1]) PWM_SetValue(pwm, TIM_CH1, 0);
  if(pwm->channel[TIM_CH2]) PWM_SetValue(pwm, TIM_CH2, 0);
  if(pwm->channel[TIM_CH3]) PWM_SetValue(pwm, TIM_CH3, 0);
  if(pwm->channel[TIM_CH4]) PWM_SetValue(pwm, TIM_CH4, 0);
  float frequency = (float)SystemCoreClock / (pwm->prescaler + 1) / pwm->auto_reload / (pwm->center_aligned + 1);
  return frequency;
}

/**
 * @brief Ustawia wartość wypełnienia `duty` sygnału PWM na wyjściu cyfrowym.
 * Jeśli opcja `save` jest włączona, aktualna wartość jest zapisywana do EEPROM.
 * @param dout Wskaźnik do struktury reprezentującej wyjście tranzystorowe (TO) lub triakowe (XO).
 * @param duty Wartość wypełnienia [%] sygnału PWM (0% - 100%).
 * @return Rzeczywista ustawiona wartość wypełnienia `duty` na wyjściu cyfrowym.
 */
float DOUT_Duty(DOUT_t *dout, float duty)
{
  if(!dout->pwm) return 0;
  uint32_t old_value = dout->pwm->value[dout->channel];
  PWM_SetValue(dout->pwm, dout->channel, duty * dout->pwm->auto_reload / 100);
  dout->value = dout->pwm->value[dout->channel];
  float duty = (float)dout->value * 100 / dout->pwm->auto_reload;
  if(dout->eeprom && (old_value != dout->value) && dout->save) EEPROM_Write(&dout->eeprom, &dout->value);
  return duty;
}

//-------------------------------------------------------------------------------------------------

/**
 * @brief Ustawia stan wysoki na wyjściu cyfrowym.
 * Wyjściu przekaźnikowym (RO): wykonuje zwarcie na styku przekaźnika.
 * Wyjście tranzystorowe (TO): wystawia potencjał 24V.
 * Wyjście triakowe (XO): przenosi napięcie przemienne z XCOM.
 * Jeśli opcja `save` jest włączona, aktualny stan jest zapisywany do EEPROM.
 * @param dout Wskaźnik do struktury reprezentującej wyjście cyfrowe.
 */
void DOUT_Set(DOUT_t *dout)
{
  if(dout->pwm) {
    if(dout->value == dout->pwm->auto_reload) return;
    dout->value = dout->pwm->auto_reload;
    PWM_SetValue(dout->pwm, dout->channel, dout->value);
    if(dout->eeprom && dout->save) EEPROM_Write(&dout->eeprom, &dout->value);
  }
  else {
    if(dout->value) return;
    dout->value = true;
    if(dout->eeprom && dout->save) EEPROM_Write(&dout->eeprom, &dout->value);
  }
}

/**
 * @brief Ustawia stan niski na wyjściu cyfrowym.
 * Wyjściu przekaźnikowym (RO): pozostawia rozwarty styk przekaźnika.
 * Wyjście tranzystorowe (TO): pozostawia brak potencjału (floating).
 * Wyjście triakowe (XO): pozostawia brak potencjału (floating).
 * Jeśli opcja `save` jest włączona, aktualny stan jest zapisywany do EEPROM.
 * @param dout Wskaźnik do struktury reprezentującej wyjście cyfrowe.
 */
void DOUT_Rst(DOUT_t *dout)
{
  if(dout->pwm) {
    if(dout->value == 0) return;
    dout->value = 0;
    PWM_SetValue(dout->pwm, dout->channel, dout->value);
    if(dout->eeprom && dout->save) EEPROM_Write(&dout->eeprom, &dout->value);
  }
  else {
    if(!dout->value) return;
    dout->value = false;
    if(dout->eeprom && dout->save) EEPROM_Write(&dout->eeprom, &dout->value);
  }
}

/**
 * @brief Zmienia stan na wyjściu cyfrowym.
 * Jeśli opcja `save` jest włączona, aktualny stan jest zapisywany do EEPROM.
 * @param dout Wskaźnik do struktury reprezentującej wyjście cyfrowe.
 */
void DOUT_Tgl(DOUT_t *dout)
{
  if(dout->pwm) {
    if(dout->value) DOUT_Rst(dout);
    else DOUT_Set(dout);
  }
  else {
    dout->value = !dout->value;
    if(dout->eeprom && dout->save) EEPROM_Write(&dout->eeprom, &dout->value);
  }
}

/**
 * @brief Ustawia wyjście cyfrowe na określony stan wysoki `true`, lub niski `false`.
 * @param dout Wskaźnik do struktury reprezentującej wyjście cyfrowe.
 * @param value Jeśli `true`, funkcja jest równoważna `DOUT_Set`, w przeciwnym razie `DOUT_Rst`.
 */
void DOUT_Preset(DOUT_t *dout, bool value)
{
  if(value) DOUT_Set(dout);
  else DOUT_Rst(dout); 
}

/**
 * @brief Aktywuje impuls, czyli zmienia stan wyjścia cyfrowego na czas `time_ms`.
 * W przypadku aktywnego trybu PWM, na czas `time_ms` wyjście zostanie wyłączone.
 * @param dout Wskaźnik do struktury reprezentującej wyjście cyfrowe.
 * @param time_ms Czas trwania impulsu w milisekundach.
 */
void RELAY_Pulse(DOUT_t *dout, uint16_t time_ms)
{
  dout->_pulse = time_ms;
}

//-------------------------------------------------------------------------------------------------

/**
 * @brief Pobiera aktualny stan wyjścia.
 * @param dout Wskaźnik do struktury reprezentującej wyjście cyfrowe.
 * @return Wartość `true` dla stanu wysokiego, `false` stanu dla niskiego.
 * W przypadku kanału PWM, wartoś cyfrową wypełnienia sygnału.
 */
uint32_t DOUT_State(DOUT_t *dout)
{
  if(dout->pwm) return dout->pwm->value[dout->channel];
  else dout->gpio.set;
}

/**
 * @brief Sprawdza, czy wyjście cyfrowe jest w trakcie wykonywania impulsu.
 * @param dout Wskaźnik do struktury reprezentującej wyjście cyfrowe.
 * @return Wartość `true` dla trwającego impulsu, `false` w przeciwnym razie.
 */
bool DOUT_IsPulse(DOUT_t *relay)
{
  return relay->pulse;
}

/**
 * @brief Pobiera aktualną wartość wypełnienia sygnału [%] na wyjściu cyfrowym.
 * Funkcja tylko dla wyjść z kanałami PWM.
 * @param dout Wskaźnik do struktury reprezentującej wyjście cyfrowe.
 * @return Wartość `true` dla aktywnego stanu, `false` dla nieaktywnego.
 */
float DOUT_GetDuty(DOUT_t *dout)
{
  if(!dout->pwm) return 0;
  return (float)dout->value * 100 / dout->pwm->auto_reload;
}

//-------------------------------------------------------------------------------------------------


/**
 * @brief Inicjalizuje określone wyjście cyfrowe przekaźnikowe (RO).
 * @param relay Wskaźnik do struktury reprezentującej wejście przekaźnikowe (RO).
 */

/**
 * @brief Inicjalizuje określone wyjście cyfrowe.
 * W przypadku wyjść z kanałami PWM konieczna jest jego inicjalizacja z funkcji wywołującej.
 * @param dout Wskaźnik do struktury reprezentującej wyjście cyfrowe.
 */
void DOUT_Init(DOUT_t *dout)
{
  if(dout->eeprom)
  {
    EEPROM_Init(&dout->eeprom);
    if(!dout->save) EEPROM_Read(&dout->eeprom, &dout->save);
    if(dout->save) EEPROM_Read(&dout->eeprom, &dout->value);
    if(dout->relay) EEPROM_Read(&dout->eeprom, &dout->cycles);
  }
  if(!dout->pwm) {
    dout->gpio.mode = GPIO_Mode_Output;
    GPIO_Init(&dout->gpio);
  }
}

static inline void DOUT_RelayCyclesInc(DOUT_t *dout)
{
  dout->cycles++;
  if(dout->eeprom) EEPROM_Write(&dout->eeprom, &dout->cycles);
}

/**
 * @brief Pętla obsługująca wyjście cyfrowe.
 * Funkcję należy wywoływać w każdej iteracji pętli głównej lub dowolnego wątku.
 * Zalecane jest, aby była uruchamiana nie rzadziej niż 100 ms.
 * @param dout Wskaźnik do struktury reprezentującej wyjście cyfrowe.
 */
void DOUT_Loop(DOUT_t *dout)
{
  waitfor(&dout->_stun);
  if(dout->_stun) return;
  if(dout->_pulse) { // Gdy zostanie ustawiony tryb pulse
    if(dout->relay && !RELAY_State(dout)) DOUT_RelayCyclesInc(dout);
    if(dout->pwm) {
      uint32_t value = dout->value ? 0 : dout->pwm->auto_reload;
      PWM_SetValue(dout->pwm, dout->channel, value);
    }
    else GPIO_Tgl(&dout->gpio);
    dout->_stun = gettick(dout->_pulse);
    dout->_pulse = 0;
    dout->pulse = true;
    return;
  }
  dout->pulse = false;
  if(RELAY_State(dout) != dout->value) {
    if(dout->pwm) PWM_SetValue(dout->pwm, dout->channel, dout->value);
    else {
      if(dout->value) {
        GPIO_Set(&dout->gpio);
        dout->value = true;
        if(dout->relay) DOUT_RelayCyclesInc(dout);
      }
      else {
        GPIO_Rst(&dout->gpio);
        dout->value = false;
      }
    }
    if(dout->relay) dout->_stun = gettick(DOUT_RELAY_DELAY);
  }
  return dout->gpio.set;
}

/**
 * @brief Określa, czy zachować wartość wyjścia cyfrowego po resecie.
 * @param dout Wskaźnik do struktury reprezentującej wyjście cyfrowe.
 * @param save Wartość `true` zapisywanie stanu wyjścia, natomiast `false` brak zapisu.
 */
void DOUT_Settings(DOUT_t *dout, bool save)
{
  if(dout->eeprom && save != dout->save) {
    EEPROM_Write(&dout->eeprom, &dout->save);
  }
}

//-------------------------------------------------------------------------------------------------

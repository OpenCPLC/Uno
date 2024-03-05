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

//-------------------------------------------------------------------------------------------------

/**
 * @brief Ustawia wartość wypełnienia `duty` sygnału PWM na wyjściu cyfrowym.
 * Jeśli opcja `save` jest włączona, aktualna wartość jest zapisywana do EEPROM.
 * @param dout Wskaźnik do struktury reprezentującej wyjście tranzystorowe (TO) lub triakowe (XO).
 * @param duty Wartość wypełnienia [%] sygnału PWM (0% - 100%).
 * @return Rzeczywista ustawiona wartość wypełnienia `duty` na wyjściu cyfrowym.
 */
float DOUT_Duty(DOUT_t *dout, float duty)
{
  uint32_t old_value = dout->pwm->value[dout->channel];
  PWM_SetValue(dout->pwm, dout->channel, duty * dout->pwm->auto_reload / 100);
  dout->value = dout->pwm->value[dout->channel];
  float duty = (float)dout->value * 100 / dout->pwm->auto_reload;
  if(dout->eeprom && (old_value != dout->value) && dout->save) EEPROM_Write(&dout->eeprom, &dout->value);
  return duty;
}

/**
 * @brief Ustawia stan wysoki na wyjściu cyfrowym.
 * Wystawia potencjał 24V na wyjściu tranzystorowym (TO) lub
 * Przenosi podłączone napięcie przemienne XCOM na wyjście triakowe (XO).
 * Jeśli opcja `save` jest włączona, aktualny stan jest zapisywany do EEPROM.
 * @param dout Wskaźnik do struktury reprezentującej wyjście tranzystorowe (TO) lub triakowe (XO).
 */
void DOUT_Set(DOUT_t *dout)
{
  if(dout->value == dout->pwm->auto_reload) return;
  dout->value = dout->pwm->auto_reload;
  PWM_SetValue(dout->pwm, dout->channel, dout->value);
  if(dout->eeprom && dout->save) EEPROM_Write(&dout->eeprom, &dout->value);
}

/**
 * @brief Ustawia stan niski na wyjściu cyfrowym.
 * Pozostawia brak potencjału (floating) na wyjściu tranzystorowym (TO) lub triakowym (XO).
 * Jeśli opcja `save` jest włączona, aktualny stan jest zapisywany do EEPROM.
 * @param dout Wskaźnik do struktury reprezentującej wyjście tranzystorowe (TO) lub triakowe (XO).
 */
void DOUT_Rst(DOUT_t *dout)
{
  if(dout->value == 0) return;
  dout->value = 0;
  PWM_SetValue(dout->pwm, dout->channel, dout->value);
  if(dout->eeprom && dout->save) EEPROM_Write(&dout->eeprom, &dout->value);
}

/**
 * @brief Zmienia stan na wyjściu cyfrowym tranzystorowym (TO) lub triakowym (XO).
 * Jeśli opcja `save` jest włączona, aktualny stan jest zapisywany do EEPROM.
 * @param dout Wskaźnik do struktury reprezentującej wyjście tranzystorowe (TO) lub triakowe (XO).
 */
void DOUT_Tgl(DOUT_t *dout)
{
  dout->value = !dout->value;
  PWM_SetValue(dout->pwm, dout->channel, dout->value);
  if(dout->eeprom && dout->save) EEPROM_Write(&dout->eeprom, &dout->value);
}

/**
 * @brief Ustawia wyjście cyfrowe na określony stan `true`/`false`.
 * Jeśli wartość to `true`, funkcja jest równoważna `DOUT_Set`, w przeciwnym razie `DOUT_Rst`.
 * @param dout Wskaźnik do struktury reprezentującej wyjście tranzystorowe (TO) lub triakowe (XO).
 * @param value Wartość `bool`, na którą ma zostać ustawione wyjście cyfrowe.
 */
void DOUT_Preset(DOUT_t *dout, bool value)
{
  if(value) DOUT_Set(dout);
  else DOUT_Rst(dout); 
}

/**
 * @brief Pobiera aktualną wartość wypełnienia sygnału [%] na wyjściu cyfrowym.
 * @param dout Wskaźnik do struktury reprezentującej wyjście tranzystorowe (TO) lub triakowe (XO).
 * @return Wartość `true` dla aktywnego stanu, `false` dla nieaktywnego.
 */
float DOUT_GetDuty(DOUT_t *dout)
{
  return (float)dout->value * 100 / dout->pwm->auto_reload;
}

//-------------------------------------------------------------------------------------------------

/**
 * @brief Inicjalizuje określone wyjście cyfrowe tranzystorowe (TO) lub triakowe (XO).
 * Konieczna jest inicjalizacja licznika w trybie PWM z funkcji wywołującej.
 * @param dout Wskaźnik do struktury reprezentującej wyjście tranzystorowe (TO) lub triakowe (XO).
 */
void DOUT_Init(DOUT_t *dout)
{
  if(dout->eeprom)
  {
    EEPROM_Init(&dout->eeprom);
    if(!dout->save) EEPROM_Read(&dout->eeprom, &dout->save);
    if(dout->save) EEPROM_Read(&dout->eeprom, &dout->value);
  }
}

/**
 * @brief Ustawia parametr `save` dla wyjścia cyfrowego tranzystorowego (TO) lub triakowego (XO).
 * @param dout Wskaźnik do struktury reprezentującej wyjście tranzystorowe (TO) lub triakowe (XO).
 * @param save Określa, czy zachować wartość wyjścia cyfrowego po resecie.
 */
void DOUT_Settings(DOUT_t *dout, bool save)
{
  if(dout->eeprom && save != dout->save) {
    EEPROM_Write(&dout->eeprom, &dout->save);
  }
}

//---------------------------------------------------------------------------------------------------------------------

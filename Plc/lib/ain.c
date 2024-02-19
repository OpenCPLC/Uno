/**
 * @file ain.c
 * @brief Obsługa wejść analogowych OpenCPLC
 */

#include "ain.h"

/**
 * @brief Zwraca wartość pomiaru ADC bez konwersji.
 * @param ain Wskaźnik na strukturę AIN_t reprezentującą dane wejście analogowe (AI).
 * @return Wskazanie przetwornika ADC bez konwersji.
 */
uint16_t AIN_Raw(AIN_t *ain)
{
  return ain->adc->measurements.output[ain->channel];
}

/**
 * @brief Zwraca wartość pomiaru ADC po przeliczeniu na prąd lub napięcie.
 * @param ain Wskaźnik na strukturę AIN_t reprezentującą dane wejście analogowe (AI).
 * @return Wartość prądu [mA] lub napięcia [V] na wejściu analogowym.
 */
float AIN_Value(AIN_t *ain)
{
  uint16_t raw = ain->adc->measurements.output[ain->channel];
  float value = resistor_divider_factor(3.3, 160, 330, 16) * raw;
  if(ain->type = AIN_Type_mAmps) value *= 2;
  return value;
}

/**
 * @file din.c
 * @brief Obsługa wejść cyfrowych OpenCPLC
 */

#include "din.h"

//-------------------------------------------------------------------------------------------------

/**
 * @brief Zwraca stan wyjścia na wejścia cyfrowego (DI).
 * @return Wartość `true` dla stanu wysokiego, `false` dla niskiego.
 */
bool DIN_State(DIN_t *din)
{
  return GPIF_Input(&din->gpif);
}

/**
 * @brief Wykrywa stale zmieniającej się wartości sygnału na wejściu cyfrowym (DI).
 * @return Wartość `true`, gdy sygnał zmienia swój stan w czasie
 * nie przekraczającym `din->gpif.filter_ms`; w przeciwnym razie, `false`.
 */
bool DIN_Toggling(DIN_t *din)
{
  return GPIF_Toggling(&din->gpif);
}

/**
 * @brief Wykrywa zbocze narastającego na wejściu cyfrowym (DI).
 * Wywołanie tej funkcji skutkuje wyzerowaniem flagi zbocza narastającego.
 * Zmiana sygnału krótsza niż din->gpif.edge_ms nie będzie traktowana jako nowe zbocze.
 * @param din Wskaźnik do struktury reprezentującej wejście cyfrowe (DI).
 * @return Wartość `true`, gdy wystąpiło zbocze narastającego; w przeciwnym razie, `false`.
 */
bool DIN_Rais(DIN_t *din)
{
  return GPIF_Rais(&din->gpif);
}

/**
 * @brief Wykrywa zbocze opadającego na wejściu cyfrowym (DI).
 * Wywołanie tej funkcji skutkuje wyzerowaniem flagi zbocza opadającego.
 * Zmiana sygnału krótsza niż din->gpif.edge_ms nie będzie traktowana jako nowe zbocze.
 * @param din Wskaźnik do struktury reprezentującej wejście cyfrowe (DI).
 * @return Wartość `true`, gdy wystąpiło zbocze opadające; w przeciwnym razie, `false`.
 */
bool DIN_Fall(DIN_t *din)
{
  return GPIF_Fall(&din->gpif);
}

/**
 * @brief Wykrywa zbocze na wejściu cyfrowym (DI).
 * Wywołanie tej funkcji skutkuje wyzerowaniem flagi zbocza narastającego i opadającego.
 * Zmiana sygnału krótsza niż din->gpif.edge_ms nie będzie traktowana jako nowe zbocze.
 * @param din Wskaźnik do struktury reprezentującej wejście cyfrowe (DI).
 * @return Wartość `true`, gdy wystąpiło zbocze narastające lub opadające; w przeciwnym razie, `false`.
 */
bool DIN_Edge(DIN_t *din)
{
  return GPIF_Edge(&din->gpif);
}

//-------------------------------------------------------------------------------------------------

/**
 * @brief Zwraca wartość częstotliwości sygnału z określonego wejścia cyfrowego (DI).
 * Wejście cyfrowe musi pracować w trybie szybkiego licznika.
 * Tryb szybkiego licznika aktywuje się, ustawiając `fast_counter` na `true`.
 * @param din Wskaźnik do struktury reprezentującej wejście cyfrowe (DI).
 * @return Częstotliwość sygnału na określonym wejściu cyfrowym (DI).
 */
float DIN_Freq(DIN_t *din)
{
  if(din->frequency) return *din->frequency;
  return 0;
}

/**
 * @brief Zwraca wartość wypełnienia sygnału z określonego wejścia cyfrowego (DI).
 * Wejście cyfrowe musi pracować w trybie szybkiego licznika.
 * Tryb szybkiego licznika aktywuje się, ustawiając `fast_counter` na `true`.
 * @param din Wskaźnik do struktury reprezentującej wejście cyfrowe (DI).
 * @return Wypełnienie sygnału na określonym wejściu cyfrowym (DI).
 */
float DIN_Fill(DIN_t *din)
{
  if(din->fill) return *din->fill;
  return 0;
}

//-------------------------------------------------------------------------------------------------

/**
 * @brief Inicjalizuje określone wejście cyfrowe (DI).
 * @param din Wskaźnik do struktury reprezentującej wejście cyfrowe (DI).
 * @return Wartość `true` dla konfiguracji trybu szybkiego licznika.
 * Wówczas konieczna jest inicjalizacja szybkiego licznika w funkcji wywołującej.
 */
bool DIN_Init(DIN_t *din)
{
  if(din->eeprom) {
    EEPROM_Read(din->eeprom, &din->gpif.filter_ms);
    EEPROM_Read(din->eeprom, &din->gpif.toggle_ms);
    EEPROM_Read(din->eeprom, &din->gpif.edge_ms);
  }
  if(din->fast_counter) return true;
  GPIF_Init(&din->gpif);
  return false;
}

/**
 * @brief Pętla obsługująca wejście cyfrowe (DI).
 * Funkcję należy wywoływać w każdej iteracji pętli głównej lub dowolnego wątku.
 * Zalecane jest, aby była uruchamiana co najmniej raz na 100 ms.
 * @return Wartość `true` dla stanu wysokiego, `false` dla niskiego.
 */
bool DIN_Loop(DIN_t *din)
{
  if(din->fast_counter) return false;
  return GPIF_Loop(&din->gpif);
}

/**
 * @brief Ustawienia konfiguracyjne dla wejścia cyfrowego (DI).
 * Ustawia parametry `filter_ms`, `toggle_ms` oraz `edge_ms`.
 * Ustawienie wartości na `0xFFFF` przywróci domyślną wartość.
 * @param din Wskaźnik do struktury reprezentującej wejście cyfrowe (DI).
 * @param filter_ms Czas trwania filtra sygnału w milisekundach.
 * @param toggle_ms Maksymalny czas, gdzie zmiany sygnału traktowane są jako sygnał przemienny (Toggling).
 * @param edge_ms Czas po którym może nastąpić kolejne zbocze narastające lub opadające.
 */
void DIN_Settings(DIN_t *din, uint16_t filter_ms, uint16_t toggle_ms, uint16_t edge_ms)
{
  if(filter_ms && filter_ms != din->gpif.filter_ms) {
    if(filter_ms == 0xFFFF) din->gpif.filter_ms = GPIF_FILTER_DELAULT;
    else din->gpif.filter_ms = filter_ms;
    EEPROM_Write(din->eeprom, &din->gpif.filter_ms);
  }
  if(toggle_ms && toggle_ms != din->gpif.toggle_ms) {
    if(toggle_ms == 0xFFFF) din->gpif.toggle_ms = GPIF_FILTER_DELAULT;
    else din->gpif.toggle_ms = toggle_ms;
    EEPROM_Write(din->eeprom, &din->gpif.toggle_ms);
  }
    if(edge_ms && edge_ms != din->gpif.edge_ms) {
    if(edge_ms == 0xFFFF) din->gpif.edge_ms = GPIF_FILTER_DELAULT;
    else din->gpif.edge_ms = edge_ms;
    EEPROM_Write(din->eeprom, &din->gpif.edge_ms);
  }
}

//-------------------------------------------------------------------------------------------------

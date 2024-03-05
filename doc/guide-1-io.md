# 🕹️ General IO [➥](../readme.md)

### Wyjścia cyfrowe **`RO`**, **`TO`** i **`XO`**

Włączanie

```c
#include "uno.h"

static uint32_t stack_plc[64];
static uint32_t stack_blink[64];

int main(void)
{
  PLC_Init();
  thread(&PLC_Loop, stack_plc, sizeof(stack_plc));
  thread(&blink, stack_blink, sizeof(stack_blink));
  VRTS_Init();
  while(1);
}

void blink(void)
{
  while(1) {
    RELAY_Set(&RO1); // DOUT_Set(&TO1);
    delay(1000);
    RELAY_Rst(&RO1); // DOUT_Rst(&TO1);
    delay(1000);
  }
}
```


Płynna regulacja mocą

```c
#include "uno.h"

static uint32_t stack_plc[64];
static uint32_t stack_pwm[64];

int main(void)
{
  PLC_Init();
  thread(&PLC_Loop, stack_plc, sizeof(stack_plc));
  thread(&pwm_control, stack_pwm, sizeof(stack_pwm));
  VRTS_Init();
  while(1);
}

void pwm_control(void)
{
  TO_Frequency(200); // 200Hz
  XO_Frequency(1);   // 1Hz
  float value = 0;
  while(1) {
    DOUT_Duty(&TO1, value);
    DOUT_Duty(&XO1, value);
    delay(10);
    value += 0.1;
    if(value > 100) value = 0;
  }
}
```


### Wejścia cyfrowe **`DI`**





### Wejścia analogowe **`AI`**

W sterowniku **Uno** mamy do dyspozycji 2 wejścia analogowe `AI1` i `AI2`. Wejście analogowe pozwala na pomiar wartości napięcia w zakresie **0-10V**, gdy pole type jest ustawione na `AIN_Type_Volts` _(domyślnie)_, lub prądu w zakresie **0-20mA**, gdy pole type jest ustawione na AIN_Type_mAmps. Funkcją, która zwraca nam zmierzoną wartość, jest `AIN_Value`.  W przykładzie pobierana jest wartość prądu, sprawdzane jest, czy nie jest ona mniejsza niż **2mA**, co wskazywałoby na brak podpiętego czujnika, a następnie prąd jest przeliczany na temperaturę.

```c
#include "uno.h"

AIN_t *analog_input = &AI1;

#define TEMPERATURE_MIN_4mA  -40.0 // [°C]
#define TEMPERATURE_MAX_20mA 125.0 // [°C]

int main(void)
{
  analog_input->type = AIN_Type_mAmps;
  PLC_Init();
  float a = (TEMPERATURE_MIN_4mA - TEMPERATURE_MAX_20mA) / (20 - 4);
  float b = TEMPERATURE_MIN_4mA - (a * 4);
  while(1) {
    float_t current_mA = AIN_Value(analog_input);
    float_t temperature = -273;
    if(current_mA < 2) {
      // ERROR
    }
    else {
      temperature = (a * current_mA) + b;
      // TODO: use temperature
    }
    PLC_Loop();
  }
}
```
# 🕹️ General IO [➥](../readme.md)

### Wyjścia cyfrowe

Ustawianie stanu diody informacyjnej `INFO`

```c
#include "opencplc-uno.h"

static uint32_t stack_plc[64];
static uint32_t stack_loop[64];

void loop(void)
{
  while(1) {
    INFO_Set(RGB_Red);
    delay(1000);
    INFO_Set(RGB_Green);
    delay(1000);
    INFO_Set(RGB_Blue);
    delay(1000);
  }
}

int main(void)
{
  PLC_Init();
  thread(&PLC_Loop, stack_plc, sizeof(stack_plc));
  thread(&loop, stack_loop, sizeof(stack_loop));
  VRTS_Init();
  while(1);
}
```

Miganie diodą informacyjną `INFO`

```c
#include "opencplc-uno.h"

static uint32_t stack_plc[64];
static uint32_t stack_loop[64];

void loop(void)
{
  bool blink = true;
  while(1) {
    if(blink) INFO_Blink_ON(200);
    else INFO_Blink_OFF();
    INFO_Set(RGB_Red);
    delay(3000);
    INFO_Set(RGB_Green);
    delay(3000);
    INFO_Set(RGB_Blue);
    delay(3000);
    blink = !blink;
  }
}

int main(void)
{
  PLC_Init();
  
  thread(&PLC_Loop, stack_plc, sizeof(stack_plc));
  thread(&loop, stack_loop, sizeof(stack_loop));
  VRTS_Init();
  while(1);
}
```

Włączanie / wyłącznie wyjścia

```c
#include "opencplc-uno.h"

static uint32_t stack_plc[64];
static uint32_t stack_loop[64];

int main(void)
{
  PLC_Init();
  thread(&PLC_Loop, stack_plc, sizeof(stack_plc));
  thread(&loop, stack_loop, sizeof(stack_loop));
  VRTS_Init();
  while(1);
}

void loop(void)
{
  while(1) {
    DOUT_Set(&RO1);
    delay(1000);
    DOUT_Rst(&RO1);
    delay(1000);
    //
  }
}
```

Ten sam efekt możemy uzyskać zmieniając stan wyjścia 

```c
#include "opencplc-uno.h"

static uint32_t stack_plc[64];
static uint32_t stack_loop[64];

int main(void)
{
  PLC_Init();
  thread(&PLC_Loop, stack_plc, sizeof(stack_plc));
  thread(&loop, stack_loop, sizeof(stack_loop));
  VRTS_Init();
  while(1);
}

void loop(void)
{
  while(1) {
    DOUT_Tgl(&RO1);
    delay(1000);
  }
}
```

Jak również stosując zmienną pomocniczą

```c
#include "opencplc-uno.h"

static uint32_t stack_plc[64];
static uint32_t stack_loop[64];

int main(void)
{
  PLC_Init();
  thread(&PLC_Loop, stack_plc, sizeof(stack_plc));
  thread(&loop, stack_loop, sizeof(stack_loop));
  VRTS_Init();
  while(1);
}

void loop(void)
{
  bool state = false;
  while(1) {
    DOUT_Preset(&RO1, state);
    delay(1000);
    state = !state;
  }
}
```

Pulse

```c
#include "opencplc-uno.h"

static uint32_t stack_plc[64];
static uint32_t stack_loop[64];

int main(void)
{
  PLC_Init();
  thread(&PLC_Loop, stack_plc, sizeof(stack_plc));
  thread(&loop, stack_loop, sizeof(stack_loop));
  VRTS_Init();
  while(1);
}

void loop(void)
{
  uint8_t x = 100;
  while(1) {
    if(x >= 100) {
      DOUT_Pulse(&RO1, 1000);
    }
    delay(5)
  }
}
```

Płynna regulacja mocą

```c
#include "opencplc-uno.h"

static uint32_t stack_plc[64];
static uint32_t stack_loop[64];

int main(void)
{
  PLC_Init();
  thread(&PLC_Loop, stack_plc, sizeof(stack_plc));
  thread(&loop, stack_loop, sizeof(stack_loop));
  VRTS_Init();
  while(1);
}

void loop(void)
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

Detekcja stanu wyjścia oraz czasy tin tout

```c
#include "opencplc-uno.h"

static uint32_t stack_plc[64];
static uint32_t stack_loop[64];

int main(void)
{
  DI1.gpif.ton_ms = 100;
  DI1.gpif.toff_ms = 500;
  PLC_Init();
  thread(&PLC_Loop, stack_plc, sizeof(stack_plc));
  thread(&loop, stack_loop, sizeof(stack_loop));
  VRTS_Init();
  while(1);
}

void loop(void)
{
  while(1) {
    if(DIN_Rais(&DI1)) {
      DOUT_Set(&RO1);
    }
    if(DIN_Fall(&DI2)) {
      DOUT_Rst(&RO1);
    }
    if(DIN_Edge(&DI3)) {
      // DOUT_Tgl(&RO1);
    }
  }
}
```

Detekcja zbocza

```c
#include "opencplc-uno.h"

static uint32_t stack_plc[64];
static uint32_t stack_loop[64];

int main(void)
{
  DI1.gpif.ton_ms = 100;
  DI1.gpif.toff_ms = 500;
  PLC_Init();
  thread(&PLC_Loop, stack_plc, sizeof(stack_plc));
  thread(&loop, stack_loop, sizeof(stack_loop));
  VRTS_Init();
  while(1);
}

void loop(void)
{
  DI1.gpif.toggle_ms = 2000;
  PLC_Init();
  while(1) {
    if(DIN_Toggling(&DI1)) {
      DOUT_Set(&RO1);
    }
    else {
      DOUT_Rst(&RO1);
    }
  }
}
```

Detekcja zmiany stanu

```c
#include "opencplc-uno.h"

static uint32_t stack_plc[64];
static uint32_t stack_loop[64];

int main(void)
{
  DI1.gpif.toggle_ms = 2000;
  PLC_Init();
  thread(&PLC_Loop, stack_plc, sizeof(stack_plc));
  thread(&loop, stack_loop, sizeof(stack_loop));
  VRTS_Init();
  while(1);
}

void loop(void)
{
  while(1) {
    if(DIN_Toggling(&DI1)) {
      DOUT_Set(&RO1);
    }
    else {
      DOUT_Rst(&RO1);
    }
  }
}
```

Obsługa przycisku

```c
#include "opencplc-uno.h"

static uint32_t stack_plc[64];
static uint32_t stack_loop[64];

void loop(void)
{
  RGB_e color = RGB_Off;
  while(1) {
    if(DIN_Fall(&BTN)) {
      color++;
      if(color > RGB_END_COLOR) color = RGB_Off;
      INFO_Set(color);
    }
  }
}

int main(void)
{
  PLC_Init();
  thread(&PLC_Loop, stack_plc, sizeof(stack_plc));
  thread(&loop, stack_loop, sizeof(stack_loop));
  VRTS_Init();
  while(1);
}
```

### Wejścia analogowe **`AI`**

W sterowniku **Uno** mamy do dyspozycji 2 wejścia analogowe `AI1` i `AI2`. Wejście analogowe pozwala na pomiar wartości napięcia w zakresie **0-10V**, gdy pole type jest ustawione na `AIN_Type_Volts` _(domyślnie)_, lub prądu w zakresie **0-20mA**, gdy pole type jest ustawione na AIN_Type_mAmps. Funkcją, która zwraca nam zmierzoną wartość, jest `AIN_Value`.  W przykładzie pobierana jest wartość prądu, sprawdzane jest, czy nie jest ona mniejsza niż **2mA**, co wskazywałoby na brak podpiętego czujnika, a następnie prąd jest przeliczany na temperaturę.

Wersja 1. Podejście `Linear function`

```c
#include "opencplc-uno.h"

AIN_t *analog_input = &AI1;

#define TEMPERATURE_MIN_4mA  -40.0 // [°C]
#define TEMPERATURE_MAX_20mA 125.0 // [°C]

int main(void)
{
  analog_input->type = AIN_Type_mAmps;
  PLC_Init();
  float a = (TEMPERATURE_MAX_20mA - TEMPERATURE_MIN_4mA) / (20 - 4);
  float b = TEMPERATURE_MIN_4mA - (a * 4);
  while(1) {
    float current_mA = AIN_Value(analog_input);
    float temperature = -273;
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

Wersja 2. Podejście `PLC style`

```c
#include "opencplc-uno.h"

// Define the analog input pointer
AIN_t *analog_input = &AI1;

// Define temperature range constants
#define TEMPERATURE_MIN  -60.0 // [°C]
#define TEMPERATURE_MAX 100.0 // [°C]

// Define the scaling parameters
#define INPUT_MIN 4.0 // Minimum sensor input in mA
#define INPUT_MAX 20.0 // Maximum sensor input in mA
#define ERROR_THRESHOLD 2.0 // Error threshold in mA
#define ERROR_VAL -273.0 // [°C]

int main(void)
{
  // Set the analog input type
  analog_input->type = AIN_Type_mAmps;

  // Initialize the PLC
  PLC_Init();

  while(1) {
    float current_mA = AIN_Value(analog_input);
    float normalized_current = (current_mA - INPUT_MIN) / (INPUT_MAX - INPUT_MIN);
    float temperature;
    if (current_mA < ERROR_THRESHOLD) {
      temperature = ERROR_VAL;
      // Some other error handling
    } else {
      // Otherwise, calculate temperature based on the current value
      temperature = TEMPERATURE_MIN + normalized_current * (TEMPERATURE_MAX - TEMPERATURE_MIN);
      // TODO: use temperature
    }
    // Run the PLC loop
    PLC_Loop();
  }
}
```
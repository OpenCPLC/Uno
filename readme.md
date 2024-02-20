## Essential tools

Do pracy z projektem **OpenCPLC** wymagany jest minimalny zestaw narzędzi, identyczny z tym używanym do pracy z mikrokontrolerami STM32. W skład tego zestawu wchodzą:

- Pakiet narzędzi [**GNU Arm Embedded Toolchain**](https://developer.arm.com/downloads/-/gnu-rm): Obejmuje on między innymi kompilator.
- On-Chip Debugger: [**OpenOCD** ](https://gnutoolchains.com/arm-eabi/openocd/) Umożliwia komunikację z mikrokontrolerem za pomocą programatora ST-Link.
- Narzędzia do zarządzania procesem kompilacji programów, jakim jest [**Make**](https://www.gnu.org/software/make/)

Aby zainstalować narzędzia, można skorzystać z menedżera pakietów [**Chocolatey**](https://chocolatey.org/), który umożliwia prostą instalację wymaganych komponentów.

```
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
choco install make
```

Instalacja **Make** automatycznie utworzy zmienną systemową, jednak w przypadku pozostałych programów konieczne będzie ręczne jej utworzenie.

🪟 Run » `sysdm.cpl` » Advanced » **Environment Variables**

- ARMGCC → `C:\Embedded\ArmGCC\bin`
- Path » `%ARMGCC%` and `C:\Embedded\OpenOCD\bin`

Na zakończenie należy otworzyć konsolę i zweryfikować, czy wszystkie pakiety zostały zainstalowane poprawnie. Można to zrobić przy użyciu komendy `--version`.

```bash
make --version
openocd --version
arm-none-eabi-gcc --version
```

## Examples

### System `start`-`stop`

Classic Style

```c
bool start_button;
bool stop_button;
bool motor_running;

int main(void)
{
  PLC_Init();
  while(1) {
    start_button = DIN_State(&DI1);
    stop_button = DIN_State(&DI2);
    motor_running = RELAY_State(&RO1);
    if(stop_button) {
      motor_running = false;
    }
    else if(start_button || motor_running) {
      motor_running = true;
    }
    RELAY_Preset(&RO1, motor_running);
    PLC_Loop();
  }
}
```

Modern Style

```c
DIN_t *start_button = &DI1;
DIN_t *stop_button = &DI2;
RELAY_t *motor_running = &RO1;

int main(void)
{
  PLC_Init();
  while(1) {
    if(DIN_Rais(stop_button)) {
      RELAY_Rst(motor_running);
    }
    else if(DIN_Rais(start_button)) {
      RELAY_Set(motor_running);
    }
    PLC_Loop();
  }
}
```

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

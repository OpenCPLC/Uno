## OpenCPLC

Projekt zapewnia warstwę pośrednią pomiędzy Twoją aplikacją, a peryferiami mikrokontrolera. Trochę podobnie jak w **Arduino**, jednak bardziej w kierunku automatyki. Bez włsnego IDE oraz angażowania C++.

W świecie technologii, programowanie staje się coraz bardziej złożone _(często na życzenie samych programistów)_. Niekiedy poziom skomplikowania aplikacji jest nieproporcjonalny do problemu, który rozwiązuje lub wartości, jaką dostarcza. Chcemy, aby nasza biblioteka była możliwie prosta, interfejs intuicyjny, a nakład technologiczny minimalny. Wykorzystujemy dobrze znane narzędzia, takie jak [**Visual Studio Code**](https://code.visualstudio.com/), system kontroli wersji [**Git**](https://git-scm.com/) oraz język [**C**](https://www.learn-c.org/pl/), który pomimo swojego wieku nadal jest numerem jeden wśród programistów Embedded. Nic nie stoi więc na przeszkodzie, aby jego pojawiło się go trochę więcej w automatyce, co pozwoli iść branży z duchem IT!

Zapotrzebowanie na automatyków było, jest i będzie bardzo duże. W przeszłości, kiedy programistów było niewiele, a za automatykę brali się elektrycy, zaprojektowanie języka, jakim jest ladder logic było strzałem w dziesiątkę, bo wykorzystywało logikę znaną z elektryki. Obecnie sytuacja jest odwrotna, a kod w języku C często jest bardziej czytelny dla absolwentów kierunków technicznych niż drzewo logiczne złożone z styków i cewek.

Porównajmy implementacje systemu **start-stop** w języku **SCL**, **ladder** oraz **ANSI C** z wykorzystaniem bibliotek OpenCPLC, biorąc pod uwagę zastosowanie dwóch różnych stylów mapowania zmiennych. Jeśli kod w języku **C** wydaje Ci się bardziej zrozumiały to prawdopodobnie ta droga jest dla Ciebie:

- System start-stop w **SCL**

```scl
PROGRAM main

VAR
  start_button: BOOL := FALSE;
  stop_button: BOOL := FALSE;
  motor_running: BOOL := FALSE;
END_VAR

start_button := I0.1
stop_button := I0.2
motor_running := Q0.0

IF safety_signal AND start_button THEN
  motor_running := TRUE;
ELSIF NOT safety_signal THEN
  motor_running := FALSE;
END_IF

Q0.0 := motor_running

END_PROGRAM
```

- System start-stop w **lader logic**

![Lader](/Image/lader.png)

- System start-stop w ANSI C _(mapowanie z użyciem zmiennych)_

```c
#import "uno"

bool start_button = false;
bool stop_button = false;
bool motor_running = false;

int main(void)
{
  PLC_Init();
  while(1) {
    start_button = DIN_State(&DI1);
    stop_button = DIN_State(&DI2);
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

- System start-stop w ANSI C _(mapowanie z użyciem wskaźników)_

```c
#import "uno"

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

Nie zapominajmy, że język [C](https://pl.wikipedia.org/wiki/C_(j%C4%99zyk_programowania)) powstał jako język ogólnego przeznaczenia, zatem charakteryzuje się dużą uniwersalnością, szczególnie względem sandbox'ów dostarczanych przez producentów sterowników PLC.

## Essential tools

Progamowanie sterownika **Uno** oraz całej linii **OpenCPLC** należy rozpoczą od sklonowania repozytorium, co jest rownoważne z skopiowaniem wszystkich plików projektowych. Potrzeby jest do tego [klient GIT](https://git-scm.com/download/win). Po jego instalacji wystarczy włączyć konsolę systemową i wpisać komendę:

```bash
git clone https://github.com/OpenCPLC/Uno
```

Zalecamy pracować z [Visual Studio Code](https://code.visualstudio.com/), gdyż jest to bardzo popularne IDE i właśnie dla niego zapewniamy wsparcie. Te narzędzia są dość uniwersalne i duża szansa, że już jest znasz i wykorzystujesz, jeśli nie to napewno znajdziesz do nich wiele zastosować.

Do pracy ze sterownikami OpenCPLC wymagany jest również zestaw bardziej specjalistycznych narzędzi, identyczny z tym używanym do pracy z mikrokontrolerami **STM32**. W skład tego zestawu wchodzą:

- Pakiet narzędzi [**GNU Arm Embedded Toolchain**](https://developer.arm.com/downloads/-/gnu-rm): Obejmuje on między innymi kompilator.
- On-Chip Debugger: [**OpenOCD** ](https://gnutoolchains.com/arm-eabi/openocd/) Umożliwia komunikację z mikrokontrolerem za pomocą programatora ST-Link.
- Narzędzia do zarządzania procesem kompilacji programów, jakim jest [**Make**](https://www.gnu.org/software/make/)

Aby zainstalować **Make**, można skorzystać z menedżera pakietów [**Chocolatey**](https://chocolatey.org/), który umożliwia prostą instalację wymaganych komponentów. Wystarczy otworzyć **PowerShell** jako 🛡️administrator i wywołać komendy:

```
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
choco install make
```

Instalacja **Make** automatycznie utworzy zmienną systemową, jednak w przypadku pozostałych programów konieczne będzie ręczne ich utworzenie.

🪟 Run » `sysdm.cpl` » Advanced » **Environment Variables**

- ARMGCC → `C:\Embedded\ArmGCC\bin`
- Path » `%ARMGCC%` oraz `C:\Embedded\OpenOCD\bin`

![Env](/Image/env.png)

Na zakończenie należy otworzyć konsolę i zweryfikować, czy wszystkie pakiety zostały zainstalowane poprawnie. Można to zrobić przy użyciu komendy `--version`.

```bash
arm-none-eabi-gcc --version
openocd --version
make --version
```

Gdy zmienne systemowe to dla nas czarna magia to możemy zdać się na skrypt 🔮`wizard.exe`🪄. Pozwoli on zainstalować GNU Arm Embedded Toolchain, OpenOCD oraz Make, jeżeli tego nie zrobiliśmy ręcznie. Ustawi odpowiednio zmienne systemowe oraz stworzy pliki konfiguracyjne dla projektu. Trzeba tylko wywołać skrypt z konsoli jako 🛡️administrator podając nazwę projektu `-n`.

```bash
./wizard.exe -n [naza-projektu]
```

Wizard umożliwia także wykorzystanie wersji sterownika z mniejszą ilością pamięci `-m`, wymuszenie innego poziomu optymalizacji `-o` oraz ponowne wygenerowanie plików konfiguracyjnych `-r`.

```bash
./wizard.exe -n [naza-projektu] -m 128kB -o 0g -r
```

## Examples

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

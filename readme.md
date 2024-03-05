## ⚓ Content

- 👋 [OpenCPLC](#-opencplc-) - Wstęp
- ⚙️ [Essential-tools](#%EF%B8%8F-essential-tools-) - Konfiguracja środowiska
- 🐞 [Programing-debugging](#-programing-debugging-) - Programowanie i debugowanie
- 🧵 [Multi-thread](#-multi-thread-) - Programowanie wielowątkowe
- 🧩 Examples - Przykłady
    - 1\. [General IO](./doc/guide-1-io.md) - 🕹️ Wyjścia i wejścia
    - 2\. [Communication](./doc/guide-2-com.md) - 🔗 Komunikacja RS485 i I2C
    - 3\. [Time](./doc/guide-3-time.md) - ⌚ Kontrola i zarządzanie czasem
    - 4\. [Bash](./doc/guide-4-bash.md) - ⌨️ Sterowanie komendami powłoki

# 👋 OpenCPLC [➥](#-content)

Projekt zapewnia warstwę pośrednią pomiędzy Twoją aplikacją, a peryferiami mikrokontrolera. Trochę podobnie jak w **Arduino**, jednak bardziej w kierunku automatyki. Bez włsnego IDE oraz angażowania C++.

W świecie technologii, programowanie staje się coraz bardziej złożone _(często na życzenie samych programistów)_. Niekiedy poziom skomplikowania aplikacji jest nieproporcjonalny do problemu, który rozwiązuje lub wartości, jaką dostarcza. Chcemy, aby nasza biblioteka była możliwie prosta, interfejs intuicyjny, a nakład technologiczny minimalny. Wykorzystujemy dobrze znane narzędzia, takie jak [**Visual Studio Code**](https://code.visualstudio.com/), system kontroli wersji [**Git**](https://git-scm.com/) oraz język [**C**](https://www.learn-c.org/pl/), który pomimo swojego wieku nadal jest numerem jeden wśród programistów Embedded. Nic nie stoi więc na przeszkodzie, aby jego pojawiło się go trochę więcej w automatyce, co pozwoli iść branży z duchem IT!

Zapotrzebowanie na automatyków było, jest i będzie bardzo duże. W przeszłości, kiedy programistów było niewiele, a za automatykę brali się elektrycy, zaprojektowanie języka, jakim jest ladder logic było strzałem w dziesiątkę, bo wykorzystywało logikę znaną z elektryki. Obecnie sytuacja jest odwrotna, a kod w języku C często jest bardziej czytelny dla absolwentów kierunków technicznych niż drzewo logiczne złożone z styków i cewek.

Porównajmy implementacje systemu **start-stop** w języku **SCL**, **ladder** oraz **ANSI C** z wykorzystaniem bibliotek OpenCPLC, biorąc pod uwagę zastosowanie dwóch różnych stylów mapowania zmiennych. Jeśli kod w języku **C** wydaje Ci się bardziej zrozumiały to prawdopodobnie ta droga jest dla Ciebie:

- System start-stop w SCL

```scl
PROGRAM main

VAR
  start_button: BOOL := FALSE;
  stop_button: BOOL := FALSE;
  motor_running: BOOL := FALSE;
END_VAR

start_button := I0.1
stop_button := I0.2
motor_running := Q0.1

IF safety_signal AND start_button THEN
  motor_running := TRUE;
ELSIF NOT safety_signal THEN
  motor_running := FALSE;
END_IF

Q0.1 := motor_running

END_PROGRAM
```

- System start-stop w lader logic

![Lader](/img/lader.png)

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

## ⚙️ Essential-tools [➥](#-content)

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

- ARMGCC → `C:\OpenCPLC\ArmGCC\bin`
- Path » `%ARMGCC%` oraz `C:\OpenCPLC\OpenOCD\bin`

![Env](/img/env.png)

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

## 🐞 Programing-debugging [➥](#-content)

Poruszyć temat magistrali SWD.
Złącza do programowania IDC.
Programatora.

### Strumień danych wyjściowych `DBG`

W procesie tworzenia i testowania oprogramowania kluczową rolę odgrywa etap debugowania, który polega na identyfikowaniu, lokalizowaniu i eliminowaniu błędów w kodzie źródłowym. W tym celu przygotowano zestaw funkcji `DBG`, które wykorzystują interfejs UART do wypisywania zmiennych różnych typów.  To rozwiązanie jest zdecydowanie bardziej efektywne od korzystania z implementacji funkcji `sprintf`.

```c
#include "uno.h"

int main2(void)
{
  PLC_Init();
  while(1) {
    char *text = "text";
    DBG_String("DBG string "); DBG_String(text); DBG_Enter();
    DBG_String("DBG char "); DBG_Char('$'); DBG_Enter();
    uint8_t array[] = { 49, 50, 51 };
    DBG_String("DBG array "); DBG_Array(array, sizeof(array)); DBG_Enter();
    DBG_String("DBG udec "); DBG_Dec(-69); DBG_Enter();
    DBG_String("DBG dec "); DBG_uDec(46); DBG_Enter();
    DBG_String("DBG float "); DBG_Float(21.37, 2); DBG_Enter();
    DBG_String("DBG hex "); DBG_Hex8(0xF1); DBG_Enter();
    DBG_String("DBG bin "); DBG_Bin8(0b11001010); DBG_Enter();
    DBG_String("DBG now "); DBG_Now();
    PLC_Loop();
    delay(seconds(2));
  }
}
```

## 🧵 Multi-thread [➥](#-content)

Podczas implementacji operacji/funkcji blokujących w projekcie, czyli tych, gdzie rozpoczynamy pewne zadanie i oczekujemy na jego zakończenie, korzystanie z programowania wielowątkowego jest dobrym praktyką. W projekcie został zaimplementowany system zwalnia wątków [**VRTS**](https://github.com/Xaeian/VRTS). Pozwala to na tworzenie czytelnego kodu, gdzie w każdym wątku możemy obsłużyć różne funkcjonalności.  Taką funkcjonalnością może być obsługa komunikacji **RS485**, gdzie jako **master** wysyłamy ramkę nadawczą, oczekujemy na odpowiedź urządzenia **slave**, a następnie analizujemy ją. Warto, aby w trakcie oczekiwania procesor zajmował się innymi zadaniami. Z poziomu aplikacji w funkcji głównej `main` przekazujemy funkcję wątków wraz z pamięcią podręczną `stack` _(za pomocą funkcji `thread`)_. Konieczne jest dość dokładne oszacowanie, ile pamięci będzie potrzebował dany wątek. Następnie wystarczy uruchomić system przełączania wątków `VRTS_Init`.


# 🔗 Communication [➥](../readme.md)

## Komunikacja `RS485`

W sterowniku **Uno** dostępne są dwa interfejsy **RS485**: `RS1` oraz `RS2`. Wsparcie obejmuje protokoły **Modbus RTU** oraz **BACnet** w trybach master i slave.

### Modbus Master 

W przykładzie nawiązujemy komunikację z urządzeniem o adresie `0x02` za pomocą protokołu **Modbus RTU**. W konfiguracji rejestr `0x10` jest ustawiany na wartość `1152`. Proces konfiguracji jest powtarzany, dopóki urządzenie nie udzieli odpowiedzi. W głównej pętli loop dokonuje się odczytu trzech rejestrów. Wartość `uint16` jest odczytywana z rejestru `0x14`, natomiast wartości `uint32` z rejestru `0x15` i `0x16`. Warto zauważyć, że protokół Modbus nie narzuca konkretnej kolejności bajtów dla zmiennych 32-bitowych, co może wymagać odwrócenia kolejności słów 16-bitowych, aby uzyskać prawidłową wartość. W trakcie komunikacji, `timeout` jest ustawiany na `1000`ms, a przerwa między odpowiedzią a kolejnym zapytaniem wynosi `500`ms.

```c
#include "opencplc-uno.h"
#include "modbus-master.h"

int loop(void);

static uint32_t stack_plc[256];
static uint32_t stack_app[512];

UART_t *rs485 = &RS1;

#define ADDR 0x02

int main(void)
{
  PLC_Init();
  thread(&PLC_Loop, stack_plc, sizeof(stack_plc));
  thread(&app, stack_app, sizeof(stack_app));
  VRTS_Init();
  while(1);
}

struct {
  uint16_t uint16;
  uint32_t uint32;
} regmap;

int app(void)
{
  while(MODBUS_PresetRegister(rs485, ADDR, 0x10, 1152, 1000)) { // config
    DBG_String("MODBUS no-respond");
    delay(1000);
  }
  while(1) {
    if(MODBUS_ReadHoldingRegisters(rs485, ADDR, 0x14, 3, (uint16_t *)&regmap, 1000)) {
      DBG_String("MODBUS uint16:NULL uint32:NULL");
      delay(1000);
    }
    else {
      DBG_String("MODBUS uint16:");
      DBG_uDec(regmap.uint16);
      // regmap.uint32 = (regmap.uint32 >> 16) | (regmap.uint32 << 16); // swap if necessary
      DBG_String(" uint32:");
      DBG_uDec(regmap.uint32);
      DBG_Enter();
      delay(500);
    }
  }
}
```

### Modbus Slave

W konfiguracji **Modbus** w trybie **Slave** należy stworzyć strukturę `MODBUS_Slave_t` oraz ustawić w niej:

- `uart` - wskaźnik na wykorzystywany interfejs **UART**,
- `address` - adres urządzenia Modbus,
- `regmap` - tablicę stanowiącą pamięć Modbus z domyślnymi/początkowymi wartościami,
- `regmap_size` - wielkość tablicy/pamięci `regmap`,
- `write_mask` - tablicę z wartościami `true` dla rejestrów, które mogą zostać nadpisane _(`RW`)_,
- `update_flag` - deklaracja pustej tablicy o długości `regmap_size`, w której będą automatycznie wskazywane rejestry, które zostały zaktualizowane/nadpisane.

Tablice regmap, write_mask i update_flag muszą mieć taką samą długość wynoszącą regmap_size i są powiązane ze sobą indeksem tablicy. Zatem o tym, czy wartość `regmap[index]` będzie można nadpisać, decyduje maska `write_mask[index]`. Gdy wartość zostanie nadpisana, wartość `update_flag[index]` zostanie ustawiona na true. Dodatkowo warto stworzyć sobie zmienną wyliczeniową `enum` z nazwami rejestrów powiązanymi z ich numerami.

[W przykładzie](./example/rs485-modbus-slave.c) urządzeniu slave został nadany adres `0x12`. Urządzenie udostępnia `3` rejestry: `DigitalInputs`, `HexConfig` i `DecConfig`. Rejestry `HexConfig` oraz `DecConfig` mogą zostać nadpisane.

```c
#define MODBUS_ADDR 0x12 // Adres urządzenia Modbus slave
#define MODBUS_REG_COUNT 3 // Ilość rejestrów Modbus

// Stworzenie mapy rejestrów Modbusa
typedef enum {
  MODBUS_Reg_DigitalInputs,
  MODBUS_Reg_HexConfig,
  MODBUS_Reg_DecConfig,
} MODBUS_Reg_e;

// Deklaracja pamięci dla rejestrów Modbusa Slave
uint16_t modbus_memory[MODBUS_REG_COUNT];
// Deklaracja tablic pomocniczych dla Modbusa Slave
bool modbus_write[MODBUS_REG_COUNT];
bool modbus_update[MODBUS_REG_COUNT];

void init(void)
{
  // Ustawienie wartości początkowej rejestru `DigitalInputs`
  modbus_memory[MODBUS_Reg_DigitalInputs] = 0b1100101011110000;
  // Ustawienie wartości początkowej rejestru `HexConfig`
  modbus_memory[MODBUS_Reg_HexConfig] = 0x69CF;
  // Umożliwienie nadpisania/aktualizacji wartości rejestru `HexConfig`
  modbus_write[MODBUS_Reg_HexConfig] = true;
  // Ustawienie wartości początkowej rejestru `DecConfig`
  modbus_memory[MODBUS_Reg_DecConfig] = 2137;
  // Umożliwienie nadpisania/aktualizacji wartości rejestru `DecConfig`
  modbus_write[MODBUS_Reg_DecConfig] = true;
}

// Deklaracja struktury Modbusa Slave
MODBUS_Slave_t modbus_slave = {
  .uart = &RS2, // Wybór interfejsu `RS2`
  .address = MODBUS_ADDR,
  .regmap = modbus_memory,
  .regmap_size = MODBUS_REG_COUNT,
  .write_mask = modbus_write,
  .update_flag = modbus_update
};
```

Dla komunikacji, która oczekuje na przychodzące ramki, jaką jest **Modbus Slave**, należy przeznaczyć osobny wątek. W tym wątku należy umieścić funkcję `MODBUS_Loop`, która obsługuje całość komunikacji na przekazanej strukturze `MODBUS_Slave_t`.

Jeśli w wyniku komunikacji jakakolwiek wartość została nadpisana, funkcja `MODBUS_IsUpdate` zwróci `true`. Wówczas należy przejrzeć tablicę `update_flag`. Jeśli natrafimy na komórkę o wartości true, oznacza to, że rejestr odpowiadający indeksowi z tablicy `update_flag` został nadpisany i możemy obsłużyć taką operację. W przypadku wartości konfiguracyjnej może być konieczna ponowna inicjalizacji lub zmiany stanu powiązanych peryferiów, a czasem będziemy chcieli tę wartość zapisać w pamięci nieulotnej. Po obsłużeniu aktualizacji zminnej `update_flag` należy ponownie ustawić ją na `false`.

```c
void loop(void)
{
  init(); 
  while(1) {
    MODBUS_Loop(&modbus_slave); // Modbus Slave Engine
    // Sprawdzanie, czy jakikolwiek rejestr został nadpisany/zaktualizowany
    if(MODBUS_IsUpdate(&modbus_slave)) {
      // Poszukiwania nadpisanych/zaktualizowanych rejestrów
      for(MODBUS_Reg_e reg = 0; reg < MODBUS_REG_COUNT; reg++) {
        if(modbus_slave.update_flag[reg]) {
          switch(reg) {
            case MODBUS_Reg_HexConfig: // aktualizacja `HexConfig`
              // TODO: HexConfig Job
              DBG_String("UPDATE HexConfig:");
              DBG_Hex16(modbus_memory[reg]);
              DBG_Enter();
              break;
            case MODBUS_Reg_DecConfig:  // aktualizacja `DecConfig`
              // TODO: DecConfig Job
              DBG_String("UPDATE HexConfig:");
              DBG_Dec(modbus_memory[reg]);
              DBG_Enter();
              break;
            default:
              break;
          }
          // Reset flagi aktualizacji `update_flag`
          modbus_slave.update_flag[reg] = false;
        } 
      }
    }
    let();
  }
}
```

## Komunikacja `I2C`

```c
#include "opencplc-uno.h"

static uint32_t stack_plc[256];make

static uint32_t stack_loop[256];

#define ADDR 0x18 // LIS3DH device address

void loop(void)
{
  uint8_t buffer[6];
  buffer[0] = 0x77;
  I2C_Write(ADDR, 0x20, buffer, 1);
  while(1) {
    if(I2C_Read(ADDR, 0x28 | 0x80, buffer, 6)) {
      int16_t x = ((int16_t)buffer[1] << 8) | buffer[0];
      int16_t y = ((int16_t)buffer[3] << 8) | buffer[2];
      int16_t z = ((int16_t)buffer[5] << 8) | buffer[4];
      DBG_String("LIS3DH x:"); DBG_Dec(x);
      DBG_String(" y:"); DBG_Dec(y);
      DBG_String(" z:"); DBG_Dec(z);
      DBG_Enter();
      LED_Set(RGB_Green);
    }
    else {
      LED_Set(RGB_Red);
      delay(100);
    }
    delay(100);
  }
}

int main(void)
{
  PLC_Init();
  thread(&PLC_Thread, stack_plc, sizeof(stack_plc));
  thread(&loop, stack_loop, sizeof(stack_loop));
  VRTS_Init();
  while(1);
}
```

### Pomiar temperatury `RTD`

W przykładzie przeprowadzany jest pomiar temperatury przy użyciu **4**-przewodowego czujnika rezystancyjnego **PT1000**. W funkcji `RTD_Loop` pomiar jest wykonywany `10` razy, a następnie uzyskane wartości są uśredniane i przeliczane na temperaturę. Odczyt temperatury odbywa się z poziomu głównej pętli `loop`.

```c
#include "opencplc-uno.h"

int loop(void);

static uint32_t stack_plc[64];
static uint32_t stack_rtd[64];
static uint32_t stack_loop[512];

int main(void)
{
  PLC_Init();
  RTD.type = MAX31865_Type_PT1000;
  RTD.wire = MAX31865_Wire_4;
  RTD.oversampling = 10;
  thread(&PLC_Loop, stack_plc, sizeof(stack_plc));
  thread(&RTD_Main, stack_rtd, sizeof(stack_rtd));
  thread(&loop, stack_loop, sizeof(stack_loop));
  VRTS_Init();
  while(1);
}

int loop(void)
{
  while(1) {
    float temperature = RTD_Temperature();
    DBG_String("TEMP ");
    DBG_Float(temperature, 3);
    DBG_Char("C");
    DBG_Enter();
    delay(1000);
  }
}
```
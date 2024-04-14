# 🔗 Communication [➥](../readme.md)

### Komunikacja `RS485`

W sterowniku **Uno** dostępne są dwa interfejsy **RS485**: `RS1` oraz `RS2`. Wsparcie obejmuje protokoły **Modbus RTU** oraz **BACnet** w trybach master i slave.

W przykładzie nawiązujemy komunikację z urządzeniem o adresie `0x02` za pomocą protokołu **Modbus RTU**. W konfiguracji rejestr `0x10` jest ustawiany na wartość `1152`. Proces konfiguracji jest powtarzany, dopóki urządzenie nie udzieli odpowiedzi. W głównej pętli loop dokonuje się odczytu trzech rejestrów. Wartość `uint16` jest odczytywana z rejestru `0x14`, natomiast wartości `uint32` z rejestru `0x15` i `0x16`. Warto zauważyć, że protokół Modbus nie narzuca konkretnej kolejności bajtów dla zmiennych 32-bitowych, co może wymagać odwrócenia kolejności słów 16-bitowych, aby uzyskać prawidłową wartość. W trakcie komunikacji, `timeout` jest ustawiany na `1000`ms, a przerwa między odpowiedzią a kolejnym zapytaniem wynosi `500`ms.

```c
#include "opencplc-uno.h"
#include "modbus-master.h"

int loop(void);

static uint32_t stack_plc[64];
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

### Komunikacja `I2C`

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
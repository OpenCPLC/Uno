#ifndef OPENCPLC_UNO_H_
#define OPENCPLC_UNO_H_

#include "dout.h"
#include "din.h"
#include "ain.h"
#include "max31865.h"
#include "dbg.h"
#include "bash.h"
#include "rgb.h"
#include "one-wire.h"
#include "i2c-master.h"
#include "vrts.h"
#include "main.h"

#ifndef PLC_BOOTLOADER
  #define PLC_BOOTLOADER 0
#endif

// Wyjścia cyfrowe przekaźnikowse (RO)
extern DOUT_t RO1;
extern DOUT_t RO2;
extern DOUT_t RO3;
extern DOUT_t RO4;

//  Wyjścia cyfrowe tranzystorowe (TO)
extern DOUT_t TO1;
extern DOUT_t TO2;
extern DOUT_t TO3;
extern DOUT_t TO4;
void TO_Frequency(float frequency);

// Wyjścia cyfrowe triakowe (XO)
extern DOUT_t XO1;
extern DOUT_t XO2;
void XO_Frequency(float frequency);

// Wejścia cyfrowe (DI)
extern DIN_t DI1;
extern DIN_t DI2;
extern DIN_t DI3;
extern DIN_t DI4;

// Wejścia analogowe (AI)
extern AIN_t AI1;
extern AIN_t AI2;
float VCC_Value(void);

// RS485
#ifndef RS_BUFFER_SIZE
  #define RS_BUFFER_SIZE 1000
#endif
extern UART_t RS1;
extern UART_t RS2;

// I2C
bool I2C_JustRead(uint8_t addr, uint8_t *ary, uint16_t n);
bool I2C_JustWrite(uint8_t addr, uint8_t *ary, uint16_t n);
bool I2C_Read(uint8_t addr, uint8_t reg, uint8_t *ary, uint16_t n);
bool I2C_Write(uint8_t addr, uint8_t reg, uint8_t *ary, uint16_t n);

// 1WIRE
bool ONE_WIRE_Init(void);
bool ONE_WIRE_Reset(void);
void ONE_WIRE_Write(uint8_t value);
void ONE_WIRE_WriteParasitePower(uint8_t value);
uint8_t ONE_WIRE_Read(void);
void ONE_WIRE_Select(uint8_t *addr);
void ONE_WIRE_Skip(void);
bool ONE_WIRE_Search(uint8_t *addr);

// Dioda RGB i przycisk BTN
extern RGB_t RGB;
extern DIN_t BTN;

// Functions
void PLC_Init(void);
void PLC_Loop(void);
void PLC_Thread(void);

// PT100/PT1000
extern MAX31865_t RTD;
void RTD_Main(void);
float RTD_Temperature(void);

//-------------------------------------------------------------------------------------------------
#endif

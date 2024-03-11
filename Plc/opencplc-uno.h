#ifndef OPENCPLC_UNO_H_
#define OPENCPLC_UNO_H_

#include "dout.h"
#include "din.h"
#include "ain.h"
#include "max31865.h"
#include "dbg.h"
#include "bash.h"

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
extern DIN_t DI3;

// Wejścia analogowe (AI)
extern AIN_t AI1;
extern AIN_t AI2;

// Functions
void PLC_Init(void);
void PLC_Loop(void);

#ifndef PLC_RS485_BUFFER_SIZE
  #define PLC_RS485_BUFFER_SIZE 1000
#endif

extern UART_t RS1;
extern UART_t RS2;

extern MAX31865_t RTD;
void RTD_Main(void);
float RTD_Temperature(void);

//---------------------------------------------------------------------------------------------------------------------
#endif

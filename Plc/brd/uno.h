#ifndef PLC_UNO_H_
#define PLC_UNO_H_

#include "relay.h"
#include "din.h"
#include "dout.h"
#include "ain.h"
#include "max31865.h"
#include "dbg.h"
#include "bash.h"

// Wyjścia cyfrowe przekaźnikowe (RO)
extern RELAY_t RO1;
extern RELAY_t RO2;
extern RELAY_t RO3;
extern RELAY_t RO4;

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
void RTD_Init(void);
void RTD_Loop(void);
float RTD_Temperature(void);

#ifndef PLC_RS485_BUFFER_SIZE
  #define PLC_RS485_BUFFER_SIZE 1000
#endif

extern UART_t RS1;
extern UART_t RS2;

//---------------------------------------------------------------------------------------------------------------------
#endif

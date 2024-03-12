#ifndef MODBUS_SLAVE_H_
#define MODBUS_SLAVE_H_

#include "uart.h"
#include "modbus.h"

#ifndef MODBUS_SLAVE_DBG
  #define MODBUS_SLAVE_DBG 0
#endif

typedef struct {
  UART_t *uart;
  uint8_t address;
  uint16_t *memory;
  uint16_t limit;
  FILE_t *file_tx; // TODO: use 'new'
  void (*Read)(MODBUS_Fnc_e);
  void (*Write)(MODBUS_Fnc_e, uint16_t, uint16_t);
} MODBUS_Slave_t;

//-------------------------------------------------------------------------------------------------
#endif
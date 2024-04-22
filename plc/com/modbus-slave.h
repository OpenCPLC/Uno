#ifndef MODBUS_SLAVE_H_
#define MODBUS_SLAVE_H_

#include "uart.h"
#include "modbus.h"
#include "crc.h"

//-------------------------------------------------------------------------------------------------

#ifndef MODBUS_SLAVE_DBG
  #define MODBUS_SLAVE_DBG 0
#endif

typedef struct {
  UART_t *uart;
  uint8_t address;
  uint16_t *regmap;
  bool *write_mask; // Ustaw 1 jeżeli pozwala na wpisywanie do danego rejestru
  bool update_global_flag;
  bool *update_flag; // Ustawia 1, gdy wartość została odświerzona
  uint16_t regmap_size;
} MODBUS_Slave_t;

MODBUS_Error_e MODBUS_Loop(MODBUS_Slave_t *modbus);
bool MODBUS_IsUpdate(MODBUS_Slave_t *modbus);

//-------------------------------------------------------------------------------------------------
#endif
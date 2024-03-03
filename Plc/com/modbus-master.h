#ifndef MODBUS_MASTER_H_
#define MODBUS_MASTER_H_

#include "uart.h"
#include "modbus.h"

//-------------------------------------------------------------------------------------------------

MODBUS_Error_e MODBUS_ReadBits(UART_t *uart, uint8_t addr, uint16_t start, uint16_t count, bool *memory, uint32_t timeout_ms);
MODBUS_Error_e MODBUS_ReadOuts(UART_t *uart, uint8_t addr, uint16_t start, uint16_t count, bool *memory, uint32_t timeout_ms);
MODBUS_Error_e MODBUS_PresetBit(UART_t *uart, uint8_t addr, uint16_t index, bool value, uint32_t timeout_ms);
MODBUS_Error_e MODBUS_WriteBits(UART_t *uart, uint8_t addr, uint16_t count, uint16_t start, bool *memory, uint32_t timeout_ms);
MODBUS_Error_e MODBUS_ReadInputRegisters(UART_t *uart, uint8_t addr, uint16_t start, uint16_t count, uint16_t *memory, uint32_t timeout_ms);
MODBUS_Error_e MODBUS_ReadHoldingRegisters(UART_t *uart, uint8_t addr, uint16_t start, uint16_t count, uint16_t *memory, uint32_t timeout_ms);
MODBUS_Error_e MODBUS_PresetRegister(UART_t *uart, uint8_t addr, uint16_t index, uint16_t value, uint32_t timeout_ms);
MODBUS_Error_e MODBUS_WriteRegisters(UART_t *uart, uint8_t addr, uint16_t start, uint16_t count, uint16_t *memory, uint32_t timeout_ms);

//-------------------------------------------------------------------------------------------------
#endif

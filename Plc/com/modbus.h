#ifndef MODBUS_H_
#define MODBUS_H_

typedef enum {
  MODBUS_Ok = 0,
  MODBUS_Error_Uart,
  MODBUS_Error_Sending,
  MODBUS_Error_Timeout,
  MODBUS_Error_MinLength,
  MODBUS_Error_Length,
  MODBUS_Error_Adrress,
  MODBUS_Error_Crc,
  MODBUS_Error_Function,
  MODBUS_Error_Start,
  MODBUS_Error_Index,
  MODBUS_Error_Count,
  MODBUS_Error_Value,
  MODBUS_Error_BufferSize
} MODBUS_Error_e;

typedef enum {
  MODBUS_Fnc_Unknown = 0x00,
  MODBUS_Fnc_ReadBits = 0x01,
  MODBUS_Fnc_ReadOuts = 0x02,
  MODBUS_Fnc_ReadHoldingRegisters = 0x03,
  MODBUS_Fnc_ReadInputRegisters = 0x04,
  MODBUS_Fnc_PresetBit = 0x05,
  MODBUS_Fnc_PresetRegister = 0x06,
  MODBUS_Fnc_WriteBits = 0x0F,
  MODBUS_Fnc_WriteRegisters = 0x10
} MODBUS_Fnc_e;

#endif
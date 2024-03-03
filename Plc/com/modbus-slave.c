#include "modbus-slave.h"

MODBUS_Error_e MODBUS_Loop(MODBUS_Slave_t *modbus)
{
  uint16_t length;
  UART_Loop(modbus->uart);
  length = UART_ReadSize(modbus->uart);
  if(!length) return MODBUS_Ok;
  uint8_t *buffer_rx = (uint8_t *)new(length);
  length = UART_ReadArray(modbus->uart, buffer_rx);
  if(length <= 5) return MODBUS_Error_MinLength;
  if(buffer_rx[0] != modbus->address) return MODBUS_Error_Adrress;
  if(CRC_Error(&crc16_modbus, buffer_rx, length)) return MODBUS_Error_Crc;
  if(UART_IsBusy(modbus->uart)) return MODBUS_Error_Uart;
  uint16_t reg, start, count;
  uint8_t bit;
  FILE_Clear(modbus->file_tx);
  MODBUS_Fnc_e function_code = (MODBUS_Fnc_e)buffer_rx[1];
  switch(function_code) {
    case MODBUS_Fnc_ReadBits:
    case MODBUS_Fnc_ReadOuts:
      // TODO: określ długość odpowiedzi
      if((length < 8) || (length % 4)) return MODBUS_Error_Length;
      FILE_Char(modbus->file_tx, buffer_rx[0]);
      FILE_Char(modbus->file_tx, buffer_rx[1]);
      FILE_Char(modbus->file_tx, 0);
      for(uint16_t j = 0; j < ((length - 4) / 4); j++) {
        start = (buffer_rx[(j * 4) + 2] << 8) | (buffer_rx[(j * 4) + 3]);
        count = (buffer_rx[(j * 4) + 4] << 8) | (buffer_rx[(j * 4) + 5]);
        reg = start / 16;
        bit = start % 16;
        uint16_t icount = count / 8;
        if(count % 8) icount++;
        uint16_t high, low;
        for(uint16_t i = 0; i < icount; i++) {
          if(reg < modbus->limit) {
            high = modbus->memory[reg] >> bit;
            if(reg + 1 < modbus->limit) low = modbus->memory[reg + 1] << (16 - bit);
            else low = 0;
          }
          else {
            high = 0;
            low = 0;
          }
          if(FILE_Char(modbus->file_tx, high | low)) return MODBUS_Error_BufferSize;
          bit += 8;
          if(bit > 15) {
            bit -= 16;
            reg++;
          }
        }
      }
      modbus->file_tx->buffer[2] = modbus->file_tx->size - 3;
      if(FILE_Crc(modbus->file_tx, &crc16_modbus)) return MODBUS_Error_BufferSize;
      if(modbus->Read) modbus->Read(function_code);
      break;
    case MODBUS_Fnc_ReadHoldingRegisters:
    case MODBUS_Fnc_ReadInputRegisters:
      if((length < 8) || (length % 4)) return MODBUS_Error_Length;
      FILE_Char(modbus->file_tx, buffer_rx[0]);
      FILE_Char(modbus->file_tx, buffer_rx[1]);
      FILE_Char(modbus->file_tx, 0);
      for(uint16_t j = 0; j < ((length - 4) / 4); j++) {
        start = (buffer_rx[(j * 4) + 2] << 8) | (buffer_rx[(j * 4) + 3]);
        count = (buffer_rx[(j * 4) + 4] << 8) | (buffer_rx[(j * 4) + 5]);
        uint16_t value;
        for(uint16_t i = 0; i < count; i++) {
          if(start + i < modbus->limit) value = modbus->memory[start + i];
          else value = 0;
          FILE_Char16(modbus->file_tx, value);
        }
      }
      modbus->file_tx->buffer[2] = modbus->file_tx->size - 3;
      if(FILE_Crc(modbus->file_tx, &crc16_modbus)) return MODBUS_Error_BufferSize;
      if(modbus->Read) modbus->Read(function_code);
      break;
    case MODBUS_Fnc_PresetBit:
      if(length != 8) return MODBUS_Error_Length;
      for(uint16_t i = 0; i < 6; i++) FILE_Char(modbus->file_tx, buffer_rx[i]);
      start = (buffer_rx[2] << 8) | (buffer_rx[3]);
      reg = start / 16;
      bit = start % 16;
      if(reg < modbus->limit) {
        if(buffer_rx[4]) modbus->memory[reg] |= (1 << bit);
        else modbus->memory[reg] &= ~(1 << bit);
      }
      if(FILE_Crc(modbus->file_tx, &crc16_modbus)) return MODBUS_Error_BufferSize;
      if(modbus->Write) modbus->Write(function_code, reg, 1);
      break;
    case MODBUS_Fnc_PresetRegister:
      if(length != 8) return MODBUS_Error_Length;
      for(uint16_t i = 0; i < 6; i++) FILE_Char(modbus->file_tx, buffer_rx[i]);
      reg = (buffer_rx[2] << 8) | (buffer_rx[3]);
      if(reg < modbus->limit) modbus->memory[reg] = (buffer_rx[4] << 8) | (buffer_rx[5]);
      if(FILE_Crc(modbus->file_tx, &crc16_modbus)) return MODBUS_Error_BufferSize;
      if(modbus->Write) modbus->Write(function_code, reg, 1);
      break;
    case MODBUS_Fnc_WriteBits:
      if(length < 10 || (length != buffer_rx[6] + 9)) return MODBUS_Error_Length;
      for(uint16_t i = 0; i < 6; i++) FILE_Char(modbus->file_tx, buffer_rx[i]);
      start = (buffer_rx[2] << 8) | (buffer_rx[3]);
      count = (buffer_rx[4] << 8) | (buffer_rx[5]);
      reg = start / 16;
      bit = start % 16;
      uint16_t icount = (count + bit) / 16;
      uint16_t ibit = (count + bit) % 16;
      if(ibit) icount++;
      buffer_rx[7 + buffer_rx[6]] = 0;
      buffer_rx[6] = (uint8_t)(modbus->memory[reg] >> 8);
      uint16_t tmp;
      for(uint16_t i = 0; i < icount; i++) {
        tmp = (buffer_rx[6 + (2 * i)] & (0xFF >> (8 - bit))) | (buffer_rx[7 + (2 * i)] << bit) | (buffer_rx[8 + (2 * i)] << (bit + 8));
        if(i == icount - 1) tmp = (tmp & (0xFFFF >> (16 - ibit))) | (modbus->memory[reg + i] & (0xFFFF << ibit));
        if(reg + i < modbus->limit) modbus->memory[reg + i] = tmp;
      }
      if(FILE_Crc(modbus->file_tx, &crc16_modbus)) return MODBUS_Error_BufferSize;
      if(modbus->Write) modbus->Write(function_code, reg, icount);
      break;
    case MODBUS_Fnc_WriteRegisters:
      count = (buffer_rx[4] << 8) | (buffer_rx[5]);
      if(length < 11 || !(length % 2) || count != (length - 9) / 2 || count != buffer_rx[6] / 2) return MODBUS_Error_Length;
      for(uint16_t i = 0; i < 6; i++) FILE_Char(modbus->file_tx, buffer_rx[i]);
      start = (buffer_rx[2] << 8) | (buffer_rx[3]);
      for(uint16_t i = 0; i < count; i++) {
        if(start + i < modbus->limit) modbus->memory[start + i] = (buffer_rx[7 + (2 * i)] << 8) | (buffer_rx[8 + (2 * i)]);
      }
      if(FILE_Crc(modbus->file_tx, &crc16_modbus)) return MODBUS_Error_BufferSize;
      if(modbus->Write) modbus->Write(function_code, start, count);
      break;
    default:
      if(FILE_Array(modbus->file_tx, buffer_rx, length)) return MODBUS_Error_BufferSize;
      break;
  }
  if(UART_SendFile(modbus->uart, modbus->file_tx)) return MODBUS_Error_Sending;
  uint32_t wait_ms = UART_CalcTime(modbus->uart, modbus->file_tx->size) + 10;
  return MODBUS_Ok;
}
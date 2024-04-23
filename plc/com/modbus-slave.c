#include "modbus-slave.h"

MODBUS_Status_e MODBUS_Loop(MODBUS_Slave_t *modbus)
{
  if(UART_During(modbus->uart)) return MODBUS_Status_UartBusy;
  uint16_t size_rx = UART_ReadSize(modbus->uart);
  if(!size_rx) return MODBUS_Status_NoFrame;
  dloc((void **)&modbus->buffer_rx);
  modbus->buffer_rx = (uint8_t *)aloc(size_rx);
  size_rx = UART_ReadArray(modbus->uart, modbus->buffer_rx);
  if(size_rx <= 5) return MODBUS_Status_FrameTooShort;
  if(modbus->buffer_rx[0] != modbus->address) return MODBUS_Status_FrameNotForMe;
  if(CRC_Error(&crc16_modbus, modbus->buffer_rx, size_rx)) return MODBUS_Status_ErrorCRC;
  uint16_t reg, start, count, value;
  uint8_t bit;
  dloc((void **)&modbus->buffer_tx);
  uint16_t size_tx = 0;
  MODBUS_Fnc_e function_code = (MODBUS_Fnc_e)modbus->buffer_rx[1];
  switch(function_code) {
  //---------------------------------------------------------------------------------------------
    case MODBUS_Fnc_ReadBits:
    case MODBUS_Fnc_ReadOuts:
      if(size_rx != 8) return MODBUS_Status_FrameWrongSize;
      start = (modbus->buffer_rx[2] << 8) | modbus->buffer_rx[3];
      count = (modbus->buffer_rx[4] << 8) | modbus->buffer_rx[5];
      size_tx = ((count + 7) / 8) + 3;
      modbus->buffer_tx = (uint8_t *)aloc(size_tx + 2);
      modbus->buffer_tx[0] = modbus->buffer_rx[0];
      modbus->buffer_tx[1] = modbus->buffer_rx[1];
      modbus->buffer_tx[2] = size_tx - 3;
      reg = start / 16;
      bit = start % 16;
      uint16_t rcount = count / 8;
      if(count % 8) rcount++;
      uint16_t high, low;
      for(uint16_t i = 0; i < rcount; i++) {
        if(reg < modbus->regmap_size) {
          high = modbus->regmap[reg] >> bit;
          if(reg + 1 < modbus->regmap_size) low = modbus->regmap[reg + 1] << (16 - bit);
          else low = 0;
        }
        else {
          high = 0;
          low = 0;
        }
        modbus->buffer_tx[i + 3] = high | low;
        bit += 8;
        if(bit > 15) {
          bit -= 16;
          reg++;
        }
      }
      break;
    //---------------------------------------------------------------------------------------------
    case MODBUS_Fnc_ReadHoldingRegisters:
    case MODBUS_Fnc_ReadInputRegisters:
      if(size_rx != 8) return MODBUS_Status_FrameWrongSize;
      start = (modbus->buffer_rx[2] << 8) | modbus->buffer_rx[3];
      count = (modbus->buffer_rx[4] << 8) | modbus->buffer_rx[5];
      size_tx = 2 * count + 3;
      modbus->buffer_tx = (uint8_t *)aloc(size_tx + 2);
      modbus->buffer_tx[0] = modbus->buffer_rx[0];
      modbus->buffer_tx[1] = modbus->buffer_rx[1];
      modbus->buffer_tx[2] = size_tx - 3;
      for(uint16_t i = 0; i < count; i++) {
        if(start + i < modbus->regmap_size) value = modbus->regmap[start + i];
        else value = 0;
        modbus->buffer_tx[3 + (i * 2)] = (uint8_t)(value >> 8);
        modbus->buffer_tx[3 + (i * 2) + 1] = (uint8_t)value;
      }
      break;
    //---------------------------------------------------------------------------------------------
    case MODBUS_Fnc_PresetBit:
      if(size_rx != 8) return MODBUS_Status_FrameWrongSize;
      size_tx = 6;
      modbus->buffer_tx = (uint8_t *)aloc(size_tx + 2);
      for(uint16_t i = 0; i < size_tx; i++) modbus->buffer_tx[i] = modbus->buffer_rx[i];
      start = (modbus->buffer_rx[2] << 8) | (modbus->buffer_rx[3]);
      reg = start / 16;
      bit = start % 16;
      value = modbus->buffer_rx[4] ? modbus->regmap[reg] | (1 << bit) : modbus->regmap[reg] & ~(1 << bit);
      if(reg < modbus->regmap_size && modbus->write_mask[reg] && modbus->regmap[reg] != value) {
        modbus->regmap[reg] = value;
        modbus->update_flag[reg] = true;
        modbus->update_global_flag = true;
      }
      break;
    //---------------------------------------------------------------------------------------------
    case MODBUS_Fnc_PresetRegister:
      if(size_rx != 8) return MODBUS_Status_FrameWrongSize;
      size_tx = 6;
      modbus->buffer_tx = (uint8_t *)aloc(size_tx + 2);
      for(uint16_t i = 0; i < size_tx; i++) modbus->buffer_tx[i] = modbus->buffer_rx[i];
      reg = (modbus->buffer_rx[2] << 8) | (modbus->buffer_rx[3]);
      value = (modbus->buffer_rx[4] << 8) | (modbus->buffer_rx[5]);
      if(reg < modbus->regmap_size && modbus->write_mask[reg] && modbus->regmap[reg] != value) {
        modbus->regmap[reg] = value;
        modbus->update_flag[reg] = true;
        modbus->update_global_flag = true;
      }
      break;
    //---------------------------------------------------------------------------------------------
    case MODBUS_Fnc_WriteBits:
      if(size_rx < 10 || (size_rx != modbus->buffer_rx[6] + 9)) return MODBUS_Status_FrameWrongSize;
      size_tx = 6;
      modbus->buffer_tx = (uint8_t *)aloc(size_tx + 2);
      for(uint16_t i = 0; i < size_tx; i++) modbus->buffer_tx[i] = modbus->buffer_rx[i];
      start = (modbus->buffer_rx[2] << 8) | (modbus->buffer_rx[3]);
      count = (modbus->buffer_rx[4] << 8) | (modbus->buffer_rx[5]);
      reg = start / 16;
      bit = start % 16;
      uint16_t wcount = (count + bit) / 16;
      uint16_t ibit = (count + bit) % 16;
      if(ibit) wcount++;
      modbus->buffer_rx[7 + modbus->buffer_rx[6]] = 0;
      modbus->buffer_rx[6] = (uint8_t)(modbus->regmap[reg] >> 8);
      for(uint16_t i = 0; i < wcount; i++) {
        value = (modbus->buffer_rx[6 + (2 * i)] & (0xFF >> (8 - bit))) | (modbus->buffer_rx[7 + (2 * i)] << bit) | (modbus->buffer_rx[8 + (2 * i)] << (bit + 8));
        if(i == wcount - 1) value = (value & (0xFFFF >> (16 - ibit))) | (modbus->regmap[reg + i] & (0xFFFF << ibit));
        if(reg + i < modbus->regmap_size && modbus->write_mask[reg + i] && modbus->regmap[reg + i] != value) {
          modbus->regmap[reg + i] = value;
          modbus->update_flag[reg + i] = true;
          modbus->update_global_flag = true;
        }
      }
      break;
    //---------------------------------------------------------------------------------------------
    case MODBUS_Fnc_WriteRegisters:
    count = (modbus->buffer_rx[4] << 8) | modbus->buffer_rx[5];
      if(size_rx < 11 || !(size_rx % 2) || (count != (size_rx - 9) / 2) || count != modbus->buffer_rx[6] / 2) return MODBUS_Status_FrameWrongSize;
      size_tx = 6;
      modbus->buffer_tx = (uint8_t *)aloc(size_tx + 2);
      for(uint16_t i = 0; i < size_tx; i++) modbus->buffer_tx[i] = modbus->buffer_rx[i];
      start = (modbus->buffer_rx[2] << 8) | modbus->buffer_rx[3];
      for(uint16_t i = 0; i < count; i++) {
        value = (modbus->buffer_rx[7 + (2 * i)] << 8) | modbus->buffer_rx[8 + (2 * i)];
        if(start + i < modbus->regmap_size && modbus->write_mask[start + i] && modbus->regmap[start + i] != value) {
          modbus->regmap[start + i] = value;
          modbus->update_flag[start + i] = true;
          modbus->update_global_flag = true;
        }
      }
      break;
    //---------------------------------------------------------------------------------------------
    default:
      size_tx = size_rx;
      modbus->buffer_tx = (uint8_t *)aloc(size_tx + 2);
      for(uint16_t i = 0; i < size_tx; i++) modbus->buffer_tx[i] = modbus->buffer_rx[i];
      break;
  }
  if(size_tx) {
    size_tx = CRC_Append(&crc16_modbus, modbus->buffer_tx, size_tx);
    if(UART_Send(modbus->uart, modbus->buffer_tx, size_tx)) return MODBUS_Status_ErrorUartSend;
  }
  return MODBUS_Status_FrameForMe;
}

//-------------------------------------------------------------------------------------------------

bool MODBUS_IsUpdate(MODBUS_Slave_t *modbus)
{
  if(modbus->update_global_flag) {
    modbus->update_global_flag = false;
    return true;
  }
  return false;
}
#ifndef DOUT_H_
#define DOUT_H_

#include "spi-master.h"

typedef enum {
  MAX31865_Reg_Read_Configuration = 0x00,
  MAX31865_Reg_Read_RTD_MSBs = 0x01,
  MAX31865_Reg_Read_RTD_LSBs = 0x02,
  MAX31865_Reg_Read_HighFaultThreshold_MSB = 0x03,
  MAX31865_Reg_Read_HighFaultThreshold_LSB = 0x04,
  MAX31865_Reg_Read_LowFaultThreshold_MSB = 0x05,
  MAX31865_Reg_Read_LowFaultThreshold_LSB = 0x06,
  MAX31865_Reg_Read_FaultStatus = 0x07,
  MAX31865_Reg_Write_Configuration = 0x80,
  MAX31865_Reg_Write_HighFaultThreshold_MSB = 0x83,
  MAX31865_Reg_Write_HighFaultThreshold_LSB = 0x84,
  MAX31865_Reg_Write_LowFaultThreshold_MSB = 0x85,
  MAX31865_Reg_Write_LowFaultThreshold_LSB = 0x86,
} MAX31865_Reg_e;

typedef enum {
  MAX31865_Type_PT100 = 0,
  MAX31865_Type_PT1000 = 1
} MAX31865_Type_e;

typedef enum {
  MAX31865_Wire_2 = 0,
  MAX31865_Wire_3 = 1,
  MAX31865_Wire_4 = 0
} MAX31865_Wire_e;

typedef enum {
  MAX31865_Reject_60Hz = 0,
  MAX31865_Reject_50Hz = 1
} MAX31865_Reject_e;

typedef struct {
  SPI_Master_t *spi;
  GPIO_t *ready;
  MAX31865_Type_e type;
  MAX31865_Wire_e wire;
  MAX31865_Reject_e reject;
  float temperature;
} MAX31865_t;

#endif
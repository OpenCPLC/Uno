#include "max31865.h"

#define RTD_A 3.9083e-3
#define RTD_B -5.775e-7

/*!
    @brief Calculate the temperature in C from the RTD through calculation of
   the resistance. Uses
   http://www.analog.com/media/en/technical-documentation/application-notes/AN709_0.pdf
   technique
    @param raw The raw 16-bit value from the RTD_REG
    @param nominal The 'nominal' resistance of the RTD sensor, usually 100
    or 1000
    @param ref_resistor The value of the matching reference resistor, usually
    430 or 4300
    @returns Temperature in C
*/
static float RTD_Calc(uint16_t raw, float nominal, float ref_resistor)
{
  float Z1, Z2, Z3, Z4, Rt, temp;
  Rt = raw;
  Rt /= 32768;
  Rt *= ref_resistor;
  Z1 = -RTD_A;
  Z2 = RTD_A * RTD_A - (4 * RTD_B);
  Z3 = (4 * RTD_B) / nominal;
  Z4 = 2 * RTD_B;
  temp = Z2 + (Z3 * Rt);
  temp = (sqrt(temp) + Z1) / Z4;
  if(temp >= 0) return temp;
  Rt /= nominal;
  Rt *= 100;
  float rpoly = Rt;
  temp = -242.02;
  temp += 2.2228 * rpoly;
  rpoly *= Rt;
  temp += 2.5859e-3 * rpoly;
  rpoly *= Rt;
  temp -= 4.8260e-6 * rpoly;
  rpoly *= Rt;
  temp -= 2.8183e-8 * rpoly;
  rpoly *= Rt;
  temp += 1.5243e-10 * rpoly;
  return temp;
}

void RTD_Init(MAX31865_t *max)
{

}

float RTD_Temperature(MAX31865_t *max)
{
  uint8_t buffer[2];
  timeout(50, GPIO_In, max->ready);
  SPI_Master_ReadSequence(max->spi, MAX31865_Reg_Read_RTD_MSBs, &buffer, 2);
  timeout(50, SPI_Master_IsFree, max->spi);
  uint16_t raw = ((uint16_t)buffer[0] << 8) | buffer[1];
  if(max->type == MAX31865_Type_PT100) max->temperature = RTD_Calc(raw, 100, 400);
  else max->temperature = RTD_Calc(raw, 1000, 4000);
  return max->temperature;
}
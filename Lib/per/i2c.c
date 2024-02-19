#include "i2c.h"

//--------------------------------------------------------------------------

const GPIO_Map_t i2c_scl_map[] = {
  [I2C1_SCL_PA9] = { .port = GPIOA, .pin = 9, .alternate = 6 },
  [I2C1_SCL_PB6] = { .port = GPIOB, .pin = 6, .alternate = 6 },
  [I2C1_SCL_PB8] = { .port = GPIOB, .pin = 8, .alternate = 6 },
  [I2C2_SCL_PA11] = { .port = GPIOA, .pin = 11, .alternate = 6 },
  [I2C2_SCL_PB10] = { .port = GPIOB, .pin = 10, .alternate = 6 },
  [I2C2_SCL_PB13] = { .port = GPIOB, .pin = 13, .alternate = 6 }
};

const GPIO_Map_t i2c_sdc_map[] = {
  [I2C1_SDA_PA10] = { .port = GPIOA, .pin = 10, .alternate = 6 },
  [I2C1_SDA_PB7] = { .port = GPIOB, .pin = 7, .alternate = 6 },
  [I2C1_SDA_PB9] = { .port = GPIOB, .pin = 9, .alternate = 6 },
  [I2C2_SDA_PA12] = { .port = GPIOA, .pin = 12, .alternate = 6 },
  [I2C2_SDA_PB11] = { .port = GPIOB, .pin = 11, .alternate = 6 },
  [I2C2_SDA_PB14] = { .port = GPIOB, .pin = 14, .alternate = 6 }
};

void I2C_Reset(I2C_TypeDef *i2c_typedef)
{
  i2c_typedef->CR1 &= ~(I2C_CR1_PE);
  // TODO...
  switch((uint32_t)i2c_typedef) {
    case (uint32_t)I2C1: RCC->APBRSTR1 |= RCC_APBRSTR1_I2C1RST; break;
    case (uint32_t)I2C2: RCC->APBRSTR1 |= RCC_APBRSTR1_I2C2RST; break;
    default: break;
  }
}

//--------------------------------------------------------------------------

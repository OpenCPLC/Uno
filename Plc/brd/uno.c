#include "uno.h"

//------------------------------------------------------------------------------------------------- RELAY RO

EEPROM_t relay_eeprom[] = {
  { .page_a = 252, .page_b = 253 },
  { .page_a = 250, .page_b = 251 },
  { .page_a = 248, .page_b = 249 },
  { .page_a = 246, .page_b = 247 }
};

RELAY_t RO1 = { .gpio = { .port = GPIOB, .pin = 7 }, .eeprom = &relay_eeprom[0], .save = true };
RELAY_t RO2 = { .gpio = { .port = GPIOB, .pin = 6 }, .eeprom = &relay_eeprom[1], .save = true };
RELAY_t RO3 = { .gpio = { .port = GPIOB, .pin = 5 }, .eeprom = &relay_eeprom[2], .save = true };
RELAY_t RO4 = { .gpio = { .port = GPIOB, .pin = 4 }, .eeprom = &relay_eeprom[3], .save = true };

//------------------------------------------------------------------------------------------------- DOUT TO

EEPROM_t dout_eeprop = { .page_a = 244, . page_b = 245 };

PWM_t to_pwm = {
  .reg = TIM1,
  .prescaler = 49,
  .channel[TIM_CH1] = TIM1_CH1_PC8,
  .channel[TIM_CH2] = TIM1_CH2_PC9,
  .channel[TIM_CH3] = TIM1_CH3_PC10,
  .channel[TIM_CH4] = TIM1_CH4_PC11,
  .auto_reload = 1000,
  .center_aligned = true
};

DOUT_t TO1 = { .pwm = &to_pwm, .channel = TIM_CH4, .eeprom = &dout_eeprop, .save = false };
DOUT_t TO2 = { .pwm = &to_pwm, .channel = TIM_CH3, .eeprom = &dout_eeprop, .save = false };
DOUT_t TO3 = { .pwm = &to_pwm, .channel = TIM_CH2, .eeprom = &dout_eeprop, .save = false };
DOUT_t TO4 = { .pwm = &to_pwm, .channel = TIM_CH1, .eeprom = &dout_eeprop, .save = false };

void TO_Frequency(float frequency)
{
  PWM_Frequency(&to_pwm, frequency);
};

//------------------------------------------------------------------------------------------------- DOUT XO

PWM_t xo_pwm = {
  .reg = TIM15,
  .prescaler = 49,
  .channel[TIM_CH1] = TIM2_CH1_PA15,
  .channel[TIM_CH2] = TIM2_CH2_PB3,
  .auto_reload = 1000,
};

DOUT_t XO1 = { .pwm = &xo_pwm, .channel = TIM_CH2, .eeprom = &dout_eeprop, .save = false };
DOUT_t XO2 = { .pwm = &xo_pwm, .channel = TIM_CH1, .eeprom = &dout_eeprop, .save = false };

void XO_Frequency(float frequency)
{
  PWM_Frequency(&xo_pwm, frequency);
}

//------------------------------------------------------------------------------------------------- DIN

EEPROM_t din_eeprop = { .page_a = 242, . page_b = 243 };
EXTI_t din_trig3, din_trig4;

PWMI_t din_pwmi = {
  .reg = TIM3,
  .prescaler = 49,
  .input_prescaler = PWMI_Prescaler_1,
  .filter = TIM_Filter_NoFilter,
  .oversampling = 4
};

DIN_t DI1 = { .gpif = { .gpio = { .port = GPIOA, .pin = 6 } }, .eeprom = &din_eeprop };
DIN_t DI2 = { .gpif = { .gpio = { .port = GPIOA, .pin = 7 } }, .eeprom = &din_eeprop };
DIN_t DI3 = { .gpif = { .gpio = { .port = GPIOB, .pin = 0 } }, .eeprom = &din_eeprop };
DIN_t DI4 = { .gpif = { .gpio = { .port = GPIOB, .pin = 1 } }, .eeprom = &din_eeprop };

bool din_pwmi_init = false;

//------------------------------------------------------------------------------------------------- AIN

uint8_t ain_channels[] = {
  ADC_IN_PA0, ADC_IN_PA1
};

uint16_t ain_output[sizeof(ain_channels)];

ADC_t ain_adc = {
  .prescaler = ADC_Prescaler_2,
  .interrupt_level = 3,
  .measurements = {
    .channels = ain_channels,
    .count = sizeof(ain_channels),
    .output = ain_output,
    .sampling_time = ADC_SamplingTime_160,
    .oversampling_enable = true,
    .oversampling_ratio = ADC_OversamplingRatio_256,
    .oversampling_shift = 4
  }
};

AIN_t AI1 = { .adc = &ain_adc, .channel = 0, .type = AIN_Type_Volts };
AIN_t AI2 = { .adc = &ain_adc, .channel = 1, .type = AIN_Type_Volts };

//------------------------------------------------------------------------------------------------- RS485

uint8_t rs1_buff_buffer[PLC_RS485_BUFFER_SIZE];
BUFF_t rs1_buff = { .mem = rs1_buff_buffer, .size = PLC_RS485_BUFFER_SIZE };
GPIO_t rs1_gpio_direction = { .port = GPIOA, .pin = 4, .speed = GPIO_Speed_VeryHigh };
UART_t RS1 = {
  .reg = USART2,
  .tx_pin = UART2_TX_PA2,
  .rx_pin = UART2_RX_PA3,
  .dma_channel = 5,
  .UART_9600,
  .buff = &rs1_buff,
  .gpio_direction = &rs1_gpio_direction
};

uint8_t rs2_buff_buffer[PLC_RS485_BUFFER_SIZE];
BUFF_t rs2_buff = { .mem = rs2_buff_buffer, .size = PLC_RS485_BUFFER_SIZE };
GPIO_t rs2_gpio_direction = { .port = GPIOB, .pin = 2, .speed = GPIO_Speed_VeryHigh };
UART_t RS2 = {
  .reg = USART1,
  .tx_pin = UART1_TX_PC4,
  .rx_pin = UART1_RX_PC5,
  .dma_channel = 6,
  .UART_9600,
  .buff = &rs2_buff,
  .gpio_direction = &rs2_gpio_direction
};

//------------------------------------------------------------------------------------------------- DBG+Bash

uint8_t dbg_buff_buffer[2048];
BUFF_t dbg_buff = { .mem = dbg_buff_buffer, .size = sizeof(dbg_buff_buffer) };
#ifdef STM32G081xx
  TIM_t dbg_tim = { .reg = TIM6 };
#endif
UART_t dbg_uart = {
  .reg = USART3,
  .tx_pin = UART1_TX_PB6,
  .rx_pin = UART1_RX_PB7,
  .dma_channel = 4,
  .UART_115200,
  .buff = &dbg_buff,
  #ifdef STM32G081xx
    .tim = &dbg_tim
  #endif
};
uint8_t dbg_file_buffer[2048];
FILE_t dbg_file = { .name = "debug", .buffer = dbg_file_buffer, .limit = sizeof(dbg_file_buffer) };
#ifdef STM32G081xx
  EEPROM_t cache_eeprom = { .page_a = 62, .page_b = 63 };
#endif
#ifdef STM32G0C1xx
  EEPROM_t cache_eeprom = { .page_a = 254, .page_b = 255 };
#endif
uint8_t cache_file_buffer[2048];
FILE_t cache_file = { .name = "cache", .buffer = cache_file_buffer, .limit = sizeof(cache_file_buffer) };

STREAM_t dbg_stream = {
  .name = "debug",
  .modify = STREAM_Modify_Lowercase,
  .Size = DBG_ReadSize,
  .Read = DBG_ReadString
};

//------------------------------------------------------------------------------------------------- Functions PLC

void PLC_Init(void)
{
  // Konfiguracja systemowa
  SCB->VTOR = FLASH_BASE | 0x00000000U;
  RTC_Init();
  EEPROM_Cache(&cache_eeprom);
  DBG_Init(&dbg_uart, &dbg_file);
  BASH_AddFile(&cache_file);
  BASH_AddFile(&dbg_file);
  SYSTICK_Init(10);
  // Wyjścia cyfrowe przekaźnikowe (RO)
  RELAY_Init(&RO1);
  RELAY_Init(&RO2);
  RELAY_Init(&RO3);
  RELAY_Init(&RO4);
  // Wyjścia cyfrowe tranzystorowe (TO) i triakowe (XO)
  PWM_Init(&to_pwm);
  PWM_Init(&xo_pwm);
  // Wejścia cyfrowe (DI)
  if(DIN_Init(&DI1)) {
    din_pwmi.channel[TIM_CH1] = TIM3_CH1_PA6;
    DI1.frequency = &din_pwmi.frequency[0];
    DI1.fill = &din_pwmi.fill[0];
    din_pwmi_init = true;
  }
  if(DIN_Init(&DI2)) {
    din_pwmi.channel[TIM_CH2] = TIM3_CH2_PA7;
    DI1.frequency = &din_pwmi.frequency[1];
    DI1.fill = &din_pwmi.fill[1];
    din_pwmi_init = true;
  }
  if(DIN_Init(&DI3)) {
    din_pwmi.channel[TIM_CH3] = TIM3_CH3_PB0;
    DI1.frequency = &din_pwmi.frequency[2];
    DI1.fill = &din_pwmi.fill[2];
    din_pwmi_init = true;
    din_pwmi.trig3 = &din_trig3;
  }
  if(DIN_Init(&DI4)) {
    din_pwmi.channel[TIM_CH4] = TIM3_CH4_PB1;
    DI1.frequency = &din_pwmi.frequency[3];
    DI1.fill = &din_pwmi.fill[3];
    din_pwmi_init = true;
    din_pwmi.trig4 = &din_trig4;
  }
  if(din_pwmi_init) {
    PWMI_Init(&din_pwmi);
  }
  // Wejścia analogowe (AI)
  ADC_Init(&ain_adc);
  ADC_Measurements(&ain_adc);
  ADC_Wait(&ain_adc);
  // Interfejsy RS485
  UART_Init(&RS1);
  UART_Init(&RS2);
}

void PLC_Loop(void)
{
  // Wyjścia przekaźnikowe (RO)
  RELAY_Loop(&RO1);
  RELAY_Loop(&RO2);
  RELAY_Loop(&RO3);
  RELAY_Loop(&RO4);
  // Wejścia cyfrowe (DI)
  DIN_Loop(&DI1);
  DIN_Loop(&DI2);
  DIN_Loop(&DI3);
  DIN_Loop(&DI4);
  if(din_pwmi_init && PWMI_Loop(&din_pwmi)) {
    // PWMI_Print(&fan_inputs);
  }
  // Wejścia analogowe (AI)
  ADC_Measurements(&ain_adc);
  if(ain_adc.overrun) {
    // DBG_String("ADC overrun:");
    // DBG_uDec(ain_adc.overrun);
    // DBG_Enter();
    ain_adc.overrun = 0;
  }
  // Obsługa debugera i powłoki bash
  BASH_Loop(&dbg_stream);
  if(UART_IsFree(&dbg_uart)) {
    clear();
    if(dbg_file.size) {
      uint8_t *buffer = (uint8_t *)new(dbg_file.size);
      memcpy(buffer, dbg_file.buffer, dbg_file.size);
      UART_Send(&dbg_uart, buffer, dbg_file.size);
      FILE_Clear(&dbg_file);
    }
  }
  // Przełączanie wątku
  let();
}

//------------------------------------------------------------------------------------------------- RTD

GPIO_t rtd_gpio_drdy = { .port = GPIOB, .pin = 11, .reverse = true };
GPIO_t rtd_gpio_cs = { .port = GPIOB, .pin = 12, .reverse = true };

SPI_Master_t rtd_spi = {
  .spi_typedef = SPI2,
  .tx_dma_channel = 2,
  .rx_dma_channel = 3,
  .interrupt_level = 2,
  .miso_pin = SPI2_MISO_PB14,
  .mosi_pin = SPI2_MOSI_PB15,
  .sck_pin = SPI2_SCK_PB13,
  .cs_gpio = &rtd_gpio_cs,
  .cs_delay = 10,
  .prescaler = SPI_PS_16,
  .cpol = true,
  .cpha = true
};

//------------------------------------------------------------------------------------------------- MAX31865

MAX31865_t rtd_max31865 = {
  .spi = &rtd_spi,
  .ready = &rtd_gpio_drdy,
  .type = MAX31865_Type_PT100,
  .wire = MAX31865_Wire_3,
  .reject = MAX31865_Reject_60Hz
};

//------------------------------------------------------------------------------------------------- Functions RTD

void RTD_Init(void)
{
  MAX31865_Init(&rtd_max31865);
}

void RTD_Loop(void)
{
  MAX31865_Loop(&rtd_max31865);
  let();
}

float RTD_Temperature(void)
{
  return rtd_max31865.temperature;
}
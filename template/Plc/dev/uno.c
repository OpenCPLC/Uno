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

//------------------------------------------------------------------------------------------------- Functions

void PLC_Init(void)
{
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
}

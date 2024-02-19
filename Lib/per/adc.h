#ifndef PER_ADC_H_
#define PER_ADC_H_

#include "main.h"
#include "def.h"
#include "gpio.h"

#ifndef ADC_RECORD
  #define ADC_RECORD 0
#endif

#if(ADC_RECORD)
  #include "per/tim.h"
#endif

#define resistor_divider_factor(vcc, up, down, resolution) ((float)vcc * (up + down) / down / ((1 << resolution) - 1))
#define volts_factor(vcc, resolution) (float)(vcc / ((1 << resolution) - 1))

//-----------------------------------------------------------------------------

typedef enum {
  ADC_StatusFree = 0,
  ADC_StatusSingle = 1,
  ADC_StatusRecord = 2
} ADC_Status_e;

typedef enum {
  ADC_IN_PA0 = 0,
  ADC_IN_PA1 = 1,
  ADC_IN_PA2 = 2,
  ADC_IN_PA3 = 3,
  ADC_IN_PA4 = 4,
  ADC_IN_PA5 = 5,
  ADC_IN_PA6 = 6,
  ADC_IN_PA7 = 7,
  ADC_IN_PB0 = 8,
  ADC_IN_PB1 = 9,
  ADC_IN_PB2 = 10,
  ADC_IN_PB10 = 11,
  ADC_IN_TSEN = 12,
  ADC_IN_VREFEN = 13,
  ADC_IN_VBATEN = 14,
  ADC_IN_PB11 = 15,
  ADC_IN_PB12 = 16,
  ADC_IN_PC4 = 17,
  ADC_IN_PC5 = 18
} ADC_IN_e;

typedef enum {
	ADC_OversamplingRatio_2 = 0,
	ADC_OversamplingRatio_4 = 1,
	ADC_OversamplingRatio_8 = 2,
	ADC_OversamplingRatio_16 = 3,
	ADC_OversamplingRatio_32 = 4,
	ADC_OversamplingRatio_64 = 5,
	ADC_OversamplingRatio_128 = 6,
	ADC_OversamplingRatio_256 = 7
} ADC_OversamplingRatio_e;

typedef enum {
	ADC_SamplingTime_1 = 0,
	ADC_SamplingTime_3 = 1,
	ADC_SamplingTime_7 = 2,
	ADC_SamplingTime_12 = 3,
	ADC_SamplingTime_19 = 4,
	ADC_SamplingTime_39 = 5,
	ADC_SamplingTime_79 = 6,
	ADC_SamplingTime_160 = 7
} ADC_SamplingTime_e;

typedef enum {
	ADC_Prescaler_1 = 0,
	ADC_Prescaler_2 = 1,
	ADC_Prescaler_4 = 2,
	ADC_Prescaler_6 = 3,
	ADC_Prescaler_8 = 4,
	ADC_Prescaler_10 = 5,
	ADC_Prescaler_12 = 6,
	ADC_Prescaler_16 = 7,
	ADC_Prescaler_32 = 8,
	ADC_Prescaler_64 = 9,
	ADC_Prescaler_128 = 10,
	ADC_Prescaler_256 = 11
} ADC_Prescaler_e;

//-----------------------------------------------------------------------------

typedef struct {
  uint8_t *channels;
  uint8_t count;
  uint16_t *output;
  GPIO_t *gpio_enable;
  ADC_SamplingTime_e sampling_time;
  bool oversampling_enable;
  ADC_OversamplingRatio_e oversampling_ratio;
  uint8_t oversampling_shift; // 0-8 [bit]
  uint8_t _active;
} ADC_Measurements_t;

#if(ADC_RECORD)
typedef struct {
  uint8_t *channels;
  uint8_t count;
  TIM_t *tim;
  GPIO_t *gpio_enable;
  uint8_t dma_channel;
  ADC_SamplingTime_e sampling_time;
  bool oversampling_enable;
  ADC_OversamplingRatio_e oversampling_ratio;
  uint8_t oversampling_shift; // 0-8 [bit]
  uint16_t *buffer;
  uint16_t buffer_length;
  DMA_Channel_TypeDef* _dma;
  DMAMUX_Channel_TypeDef* _dmamux;
} ADC_Record_t;
#endif

typedef struct {
  uint8_t interrupt_level;
  ADC_Prescaler_e prescaler;
  ADC_Measurements_t measurements;
  #if(ADC_RECORD)
    ADC_Record_t record;
  #endif
  uint16_t overrun;
  uint8_t _busy_flag;
} ADC_t;

//-----------------------------------------------------------------------------

uint8_t ADC_Measurements(ADC_t *adc);
uint8_t ADC_Record(ADC_t *adc);

uint8_t ADC_IsBusy(ADC_t *adc);
uint8_t ADC_IsFree(ADC_t *adc);
void ADC_Wait(ADC_t *adc);
void ADC_Stop(ADC_t *adc);
void ADC_Enabled(void);
void ADC_Disabled(void);

void ADC_Init(ADC_t *adc);
uint8_t ADC_Configuration(ADC_t *adc);
uint16_t ADC_GetSingle(ADC_t *adc, uint8_t channel);

//-----------------------------------------------------------------------------

extern const uint8_t(*ANA_Single_ErtsVector[])(ADC_t *adc);
extern const uint16_t ANA_Single_ErtsTime[];

#if(ADC_RECORD)
  extern const uint8_t(*ANA_Record_ErtsVector[])(ADC_t *adc);
  extern const uint16_t ANA_Record_ErtsTime[];
#endif

//-----------------------------------------------------------------------------
#endif

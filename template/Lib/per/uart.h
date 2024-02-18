#ifndef PER_UART_H_
#define PER_UART_H_

#include "int.h"
#include "tim.h"
#include "file.h"
#include "buff.h"
#include "queue.h"
#include "main.h"

//---------------------------------------------------------------------------------------------------------------------

#define UART_115200 baud = 115200, .parity = UART_Parity_None, .stop_bits = UART_StopBits_1_0, .interrupt_level = 3
#define UART_57600 baud = 57600, .parity = UART_Parity_None, .stop_bits = UART_StopBits_1_0, .interrupt_level = 3
#define UART_19200 baud = 19200, .parity = UART_Parity_None, .stop_bits = UART_StopBits_1_0, .interrupt_level = 3
#define UART_9600 baud = 9600, .parity = 0, .stop_bits = 1, .interrupt_level = 3

//---------------------------------------------------------------------------------------------------------------------

typedef enum {
  UART1_TX_PA9, UART1_TX_PB6, UART1_TX_PC4,
  UART2_TX_PA2, UART2_TX_PA14, UART2_TX_PD5,
  UART3_TX_PB2, UART3_TX_PB8, UART3_TX_PB10, UART3_TX_PC4, UART3_TX_PC10, UART3_TX_PD8,
  UART4_TX_PA0, UART4_TX_PC10,
  LPUART1_TX_PA2, LPUART1_TX_PB11, LPUART1_TX_PC1
} UART_TX_e;

typedef enum {
  UART1_RX_PA10, UART1_RX_PB7, UART1_RX_PC5,
  UART2_RX_PA3, UART2_RX_PA15, UART2_RX_PD6,
  UART3_RX_PB0, UART3_RX_PB9, UART3_RX_PB11, UART3_RX_PC5, UART3_RX_PC11, UART3_RX_PD9,
  UART4_RX_PA1, UART4_RX_PC11,
  LPUART1_RX_PA3, LPUART1_RX_PB10, LPUART1_RX_PC0,
} UART_RX_e;

typedef enum {
  UART_Parity_None = 0,
  UART_Parity_Odd = 1,
  UART_Parity_Even = 2
} UART_Parity_e;

typedef enum {
  UART_StopBits_0_5 = 0,
  UART_StopBits_1_0 = 1,
  UART_StopBits_2_0 = 2,
  UART_StopBits_1_5 = 3
} UART_StopBits_e;

typedef enum {
  UART_Mode_Time = 0,
  UART_Mode_String = 1
} UART_Mode_e;

//---------------------------------------------------------------------------------------------------------------------

typedef struct {
  USART_TypeDef *reg;
  UART_TX_e tx_pin;
  UART_RX_e rx_pin;
  uint8_t dma_channel;
  uint8_t interrupt_level;
  uint32_t baud;
  uint8_t parity;
  uint8_t stop_bits;
  uint16_t timeout;
  GPIO_t *gpio_direction;
  TIM_t *tim;
  BUFF_t *rx_buff;
  QUEUE_t *tx_queue;
  UART_Mode_e mode; // TODO
  DMA_Channel_TypeDef *_tx_dma;
  DMAMUX_Channel_TypeDef *_tx_dmamux;
  uint8_t prefix;
  volatile bool _todo_dma;
  volatile bool _todo_tc;
  bool _busy_tx;
  bool _busy_tc;
} UART_t;

//---------------------------------------------------------------------------------------------------------------------

#define UART_TASK_TYPE_COUNT 1

typedef enum {
  UART_Task_Free = 0,
  UART_Task_Send = 1
} UART_Task_e;

#pragma pack(1)
typedef struct {
  UART_t *uart;
  uint8_t *buffer;
  uint16_t size;
  bool *mutex;
  uint8_t *temporary;
  bool copy;
} UART_Task_t;
#pragma pack()

//--------------------------------------------------------------------------------------------------------------------------------

void UART_Init(UART_t *uart);
void UART_ReInit(UART_t *uart);
bool UART_During(UART_t *uart);
bool UART_IsBusy(UART_t *uart);
bool UART_IsFree(UART_t *uart);
todo_t UART_Todo(UART_t *uart);

uint16_t UART_ReadSize(UART_t *uart);
uint16_t UART_ReadArray(UART_t *uart, uint8_t *array);
char *UART_ReadString(UART_t *uart);
uint8_t UART_ReadToFile(UART_t *uart, FILE_t *file);
void UART_ReadSkip(UART_t *uart);

task_t UART_SendArray(UART_t *uart, uint8_t *buffer, uint16_t size, bool *mutex, bool copy);
task_t UART_SendString(UART_t *uart, char *str, bool *mutex, bool copy);
task_t UART_SendFile(UART_t *uart, FILE_t *file, bool copy);

todo_t UART_Loop(UART_t *uart);

//--------------------------------------------------------------------------------------------------------------------------------

extern const uint8_t(*UART_ErtsVector[])(UART_t *uart);
extern const uint8_t uart_erts_jump[];

//--------------------------------------------------------------------------------------------------------------------------------
#endif

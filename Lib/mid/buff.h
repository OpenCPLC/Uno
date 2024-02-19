#ifndef LIB_BUFF_H_
#define LIB_BUFF_H_

#include <stdbool.h>
#include <string.h>
#include "mem.h"

#ifndef BUFF_MESSAGE_LIMIT
  #define BUFF_MESSAGE_LIMIT 32
#endif

//-------------------------------------------------------------------------------------------------

typedef struct {
  uint8_t *mem;
  uint16_t size;
  uint8_t *end;
  uint8_t *tail;
  volatile uint8_t *head;
  volatile uint16_t msg_counter;
  uint16_t msg_size[BUFF_MESSAGE_LIMIT];
  uint16_t msg_head;
  uint16_t msg_tail;
  volatile bool break_on;
} BUFF_t;

void BUFF_Init(BUFF_t *buff);
void BUFF_Push(BUFF_t *buff, uint8_t value);
void BUFF_Break(BUFF_t *buff);
uint16_t BUFF_Read(BUFF_t *buff, uint8_t *array);
uint16_t BUFF_Size(BUFF_t *buff);
uint16_t BUFF_Array(BUFF_t *buff, uint8_t *array);
char *BUFF_String(BUFF_t *buff);
void BUFF_Skip(BUFF_t *buff);

//-------------------------------------------------------------------------------------------------
#endif

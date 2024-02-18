#include "buff.h"

//-------------------------------------------------------------------------------------------------

inline void BUFF_Init(BUFF_t *buff)
{
  buff->head = buff->mem;
  buff->tail = buff->mem;
  buff->end = buff->mem + buff->size;
  buff->msg_head = 0;
  buff->msg_tail = 0;
}

inline void BUFF_Push(BUFF_t *buff, uint8_t value)
{
  *buff->head = value;
  buff->msg_counter++;
  buff->head++;
  if(buff->head >= buff->end) buff->head = buff->mem;
  buff->break_on = true;
}

inline void BUFF_Break(BUFF_t *buff)
{
  if(buff->break_on) {
    buff->msg_size[buff->msg_head] = buff->msg_counter;
    buff->msg_counter = 0;
    buff->msg_head++;
    if(buff->msg_head >= BUFF_MESSAGE_LIMIT) buff->msg_head = 0;
    buff->break_on = false;
  }
}

uint16_t BUFF_Size(BUFF_t *buff)
{
  if(buff->msg_head != buff->msg_tail) return buff->msg_size[buff->msg_tail];
  return 0;
}

uint16_t BUFF_Array(BUFF_t *buff, uint8_t *array)
{
  uint16_t size = BUFF_Size(buff);
  if(size) {
    while(buff->msg_size[buff->msg_tail]) {
      *array = *buff->tail;
      array++;
      buff->tail++;
      buff->msg_size[buff->msg_tail]--;
      if(buff->tail >= buff->end) buff->tail = buff->mem;
    }
    buff->msg_tail++;
    if(buff->msg_tail >= BUFF_MESSAGE_LIMIT) buff->msg_tail = 0;
  }
  return size;
}

char *BUFF_String(BUFF_t *buff)
{
  char *string = NULL;
  uint16_t size = BUFF_Size(buff);
  if(size) {
    string = new(size + 1);
    BUFF_Array(buff, (uint8_t *)string);
    string[size] = 0;
  }
  return string;
}

void BUFF_Skip(BUFF_t *buff)
{
  uint16_t size = BUFF_Size(buff);
  if(size) {
    while(buff->msg_size[buff->msg_tail]) {
      buff->tail++;
      buff->msg_size[buff->msg_tail]--;
      if(buff->tail >= buff->end) buff->tail = buff->mem;
    }
    buff->msg_tail++;
    if(buff->msg_tail >= BUFF_MESSAGE_LIMIT) buff->msg_tail = 0;
  }
}

//-------------------------------------------------------------------------------------------------

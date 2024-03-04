#include "new.h"

NEW_t *new_stacks[VRTS_THREAD_LIMIT];
uint16_t new_size;

//-------------------------------------------------------------------------------------------------

void *new_static(size_t size)
{
  new_size += size;
  return malloc(size);
}

void NEW_Init(uint8_t thread_nbr)
{
  NEW_t *new_stack = new_static(sizeof(NEW_t));
  new_stacks[thread_nbr] = new_stack;
}

static void MEM_ErrorHandler(void) {
  __disable_irq();
  volatile uint32_t i = 0;
  while(1) i++;
}

void *new(size_t size)
{
  void *pointer = NULL;
  if(!size) return pointer;
  uint8_t active_thread = VRTS_ActiveThread();
  NEW_t *stack = new_stacks[active_thread];
  if(stack->count >= NEW_COUNT_LIMIT) MEM_ErrorHandler();
  if((new_size + size) >= NEW_SIZE_LIMIT) MEM_ErrorHandler();
  pointer = malloc(size);
  stack->var[stack->count] = pointer;
  stack->count++;
  stack->size += size;
  new_size += size;
  return pointer;
}

void clear()
{
  uint8_t active_thread = VRTS_ActiveThread();
  NEW_t *stack = new_stacks[active_thread];
  if(stack->count) {
    for(int i = 0; i < stack->count; i++)
      free(stack->var[i]);
    stack->count = 0;
    new_size -= stack->size;
    stack->size = 0;
  }
}

//-------------------------------------------------------------------------------------------------

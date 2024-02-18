#include "mem.h"

struct {
  void *new_stack[MEM_NEW_STACK];
  uint16_t new_count;
  uint32_t new_size;
  void *loc_stack[MEM_LOC_STACK];
  uint32_t loc_stack_sizes[MEM_LOC_STACK];
  uint32_t loc_count;
  uint32_t loc_size;
} mem_state;

//-------------------------------------------------------------------------------------------------

static void MEM_ErrorHandler(void) {
  __disable_irq();
  volatile uint32_t i = 0;
  while(1) i++;
}

void *new(size_t size)
{
  void *pointer = NULL;
  if(!size) return pointer;
  if(mem_state.new_count >= MEM_NEW_STACK) MEM_ErrorHandler();
  else if((mem_state.new_size + mem_state.loc_size + size) >= MEM_LIMIT) MEM_ErrorHandler();
  else {
    pointer = malloc(size);
    mem_state.new_stack[mem_state.new_count] = pointer;
    mem_state.new_count++;
    mem_state.new_size += size;
  }
  return pointer;
}

void clear()
{
  if(mem_state.new_count) {
    for(int i = 0; i < mem_state.new_count; i++)
      free(mem_state.new_stack[i]);
    mem_state.new_count = 0;
    mem_state.new_size = 0;
  }
}

//--------------------------------------------------------------------------------------------------------------------------------

void *aloc(size_t size)
{
  uint16_t i = 0;
  void *pointer = 0;
  if(!size) return pointer;
  if((mem_state.new_size + mem_state.loc_size + size) < MEM_LIMIT) {
    while(mem_state.loc_stack[i]) {
      i++;
      if(i >= MEM_LOC_STACK) MEM_ErrorHandler();
    }
    pointer = malloc(size);
    mem_state.loc_stack[i] = pointer;
    mem_state.loc_stack_sizes[i] = size;
    mem_state.loc_size += size;
    mem_state.loc_count++;
  }
  else MEM_ErrorHandler();
  return pointer;
}

void dloc(void *pointer)
{
  if(!pointer) return;
  uint16_t i = 0;
  while(i < MEM_LOC_STACK) {
    if(pointer == mem_state.loc_stack[i]) {
      free(pointer);
      pointer = NULL;
      mem_state.loc_count--;
      mem_state.loc_size -= mem_state.loc_stack_sizes[i];
      mem_state.loc_stack_sizes[i] = 0;
      mem_state.loc_stack[i] = NULL;
      return;
    }
    i++;
  }
  MEM_ErrorHandler();
  return;
}

//---------------------------------------------------------------------------------------------------------------------

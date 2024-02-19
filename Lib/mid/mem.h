#ifndef MEM_H_
#define MEM_H_

#include <stdlib.h>
#include <stdint.h>
#include "stm32g0xx.h"
#include "main.h"

//-------------------------------------------------------------------------------------------------

#ifndef MEM_NEW_STACK
  #define MEM_NEW_STACK 128 // limit of variables allocated by the 'new' function
#endif
#ifndef MEM_LOC_STACK
  #define MEM_LOC_STACK 64 // limit of variables allocated by the 'aloc' function
#endif
#ifndef MEM_SIZE
  #define MEM_LIMIT 4096 // total allocated memory limit
#endif

//-------------------------------------------------------------------------------------------------

void *new(size_t size);
void clear();
void *aloc(size_t size);
void dloc(void *pointer);

//-------------------------------------------------------------------------------------------------

#endif

#ifndef NEW_H_
#define NEW_H_

#include <stdlib.h>
#include <stdint.h>
#include "stm32g0xx.h"
#include "vrts.h"
#include "main.h"

//-------------------------------------------------------------------------------------------------

#ifndef NEW_COUNT_LIMIT
  #define NEW_COUNT_LIMIT  32 // limit of variables allocated by the 'new' function
#endif
#ifndef NEW_SIZE_LIMIT
  #define NEW_SIZE_LIMIT 8000 // total allocated memory limit for function 'new'
#endif

//-------------------------------------------------------------------------------------------------

void *new(size_t size);
void clear();

typedef struct {
  void *var[NEW_COUNT_LIMIT];
  uint16_t count;
  uint16_t size;
} NEW_t;

//-------------------------------------------------------------------------------------------------

#endif

#ifndef BASH_H_
#define BASH_H_

#include "stream.h"
#include "file.h"
#include "main.h"

#ifndef BASH_FILE_LIMIT
  #define BASH_FILE_LIMIT 12
#endif

#ifndef BASH_DBG
  #define BASH_DBG 1
#endif

#ifndef BASH_RTC
  #define BASH_RTC 0
#endif

//-------------------------------------------------------------------------------------------------

void BASH_Init(FILE_t **files, void (*callback)(char **, uint16_t));
uint8_t BASH_Loop(STREAM_t *stream);

//-------------------------------------------------------------------------------------------------
#endif

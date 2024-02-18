#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "def.h"
#include "mem.h"
#include "main.h"

#ifndef QUEUE_LIMIT
  #define QUEUE_LIMIT 32 // Max:256
#endif

#if(INCLUDE_DBG)
  #ifndef QUEUE_DBG_PUSH
    #define QUEUE_DBG_PUSH 0
  #endif
  #ifndef QUEUE_DBG_START
    #define QUEUE_DBG_START 1
  #endif
  #ifndef QUEUE_DBG_END
    #define QUEUE_DBG_END 1
  #endif
  #ifndef QUEUE_DBG_ERROR
    #define QUEUE_DBG_ERROR 1
  #endif
#endif

#define TASK_FREE 0
typedef uint8_t task_t;

//-------------------------------------------------------------------------------------------------

typedef struct {
  const char *name;
  uint16_t object_size;
  void (*Print)(void *);
  status_t (*Start)(void *);
  status_t (*End)(void *);
} QUEUE_TaskService_t;

typedef struct {
  const char *name;
  const QUEUE_TaskService_t **services;
  bool debug;
  void *objects[QUEUE_LIMIT];
  task_t tasks[QUEUE_LIMIT];
  void (*gotos[QUEUE_LIMIT])(void *);
  void (*exceptions[QUEUE_LIMIT])(void *, void *);
  uint16_t head;
  uint16_t tail;
  uint16_t shadow;
  bool not_make_end;
} QUEUE_t;

uint8_t QUEUE_GetTask(QUEUE_t *queue);
void *QUEUE_GetObject(QUEUE_t *queue);
void *QUEUE_GetEndFunction(QUEUE_t *queue);

uint8_t QUEUE_Todo(QUEUE_t *queue);
uint8_t QUEUE_Push(QUEUE_t *queue, uint8_t task, void *object, void (*Goto)(void *), void (*E)(void *, void *));
uint8_t QUEUE_Skip(QUEUE_t *queue);
uint8_t QUEUE_Start(QUEUE_t *queue);
uint8_t QUEUE_End(QUEUE_t *queue);
uint8_t QUEUE_Exception(QUEUE_t *queue, void *e);
uint8_t QUEUE_Repeat(QUEUE_t *queue);
uint8_t QUEUE_Inject(QUEUE_t *queue, uint8_t task, void *object, void (*Goto)(void *), void (*E)(void *, void *));
void QUEUE_Clear(QUEUE_t *queue);

//-------------------------------------------------------------------------------------------------
#endif

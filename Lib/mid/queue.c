#include "queue.h"
#include "dbg.h"

//-------------------------------------------------------------------------------------------------

#if(INCLUDE_DBG)
static void _QUEUE_Print(QUEUE_t *queue, uint8_t task, void *object, char* string)
{
  DBG_String("* "); DBG_String((char *)queue->name); DBG_Char(' ');
  if(task) { DBG_String((char *)queue->services[task]->name); DBG_Char(' '); }
  DBG_String(string); DBG_Enter();
  if(task && queue->services[task]->Print) queue->services[task]->Print(object);
}
#endif

static inline bool _QUEUE_Uroboros(QUEUE_t *queue)
{
  uint16_t next = queue->head + 1;
  if(next >= QUEUE_LIMIT) next = 0;

  if(next == queue->shadow) return true;
  return false;
}

//-------------------------------------------------------------------------------------------------

task_t QUEUE_GetTask(QUEUE_t *queue)
{
  if(queue->head == queue->shadow) return TASK_FREE;
  return queue->tasks[queue->shadow];
}

void *QUEUE_GetObject(QUEUE_t *queue)
{
  if(queue->head == queue->shadow) return NULL;
  return queue->objects[queue->shadow];
}

void *QUEUE_GetEndFunction(QUEUE_t *queue)
{
  if(queue->head == queue->shadow) return NULL;
  return queue->gotos[queue->shadow];
}

todo_t QUEUE_Todo(QUEUE_t *queue)
{
  int16_t todo = (int16_t)queue->head - queue->shadow;
  if(todo < 0) todo += QUEUE_LIMIT;
  return (uint8_t)todo;
}

//-------------------------------------------------------------------------------------------------

task_t QUEUE_Push(QUEUE_t *queue, uint8_t task, void *object, void (*Goto)(void *), void (*E)(void *, void *))
{
  if(_QUEUE_Uroboros(queue)) {
    #if(QUEUE_DBG_ERROR)
      if(queue->debug) _QUEUE_Print(queue, task, object, "!uroboros");
    #endif
    return 0;
  }
  #if(QUEUE_DBG_PUSH)
    if(queue->debug) _QUEUE_Print(queue, task, queue->objects[queue->tail], "..");
  #endif
  queue->tasks[queue->head] = task;
  if(queue->services[task]->object_size) {
    queue->objects[queue->head] = aloc(queue->services[task]->object_size);
    if(object) memcpy(queue->objects[queue->head], (uint8_t *)object, queue->services[task]->object_size);
    else memset(queue->objects[queue->head], 0, queue->services[task]->object_size);
  }
  else queue->objects[queue->head] = NULL;
  if(Goto) queue->gotos[queue->head] = Goto;
  else queue->gotos[queue->head] = NULL;
  if(E) queue->exceptions[queue->head] = E;
  else queue->exceptions[queue->head] = NULL;
  queue->head++;
  if(queue->head >= QUEUE_LIMIT) queue->head = 0;
  return task;
}

task_t QUEUE_Skip(QUEUE_t *queue)
{
  if(queue->head == queue->shadow) return TASK_FREE;
  if(queue->tail == queue->shadow) {
    queue->tail++;
    if(queue->tail >= QUEUE_LIMIT) queue->tail = 0;
  }
  uint8_t task = queue->tasks[queue->shadow];
  if(queue->services[task]->object_size) dloc(queue->objects[queue->shadow]);
  queue->shadow++;
  if(queue->shadow >= QUEUE_LIMIT) queue->shadow = 0;
  return task;
}

task_t QUEUE_Start(QUEUE_t *queue)
{
  if(queue->head == queue->tail) return TASK_FREE;
  uint8_t task = queue->tasks[queue->tail];
  if(queue->services[task]->Start) {
    uint8_t response = queue->services[task]->Start(queue->objects[queue->tail]);
    if(response) {
      if(response == SKIP) {
        #if(QUEUE_DBG_START)
          if(queue->debug) _QUEUE_Print(queue, task, queue->objects[queue->tail], "<>");
        #endif
        QUEUE_Skip(queue);
      }
      return 0;
    }
  }
  #if(QUEUE_DBG_START)
    if(queue->debug) _QUEUE_Print(queue, task, queue->objects[queue->tail], "<");
  #endif
  queue->tail++;
  if(queue->tail >= QUEUE_LIMIT) queue->tail = 0;
  return task;
}

task_t QUEUE_End(QUEUE_t *queue)
{
  if(queue->tail == queue->shadow) return TASK_FREE;
  uint8_t task = queue->tasks[queue->shadow];
  if(queue->services[task]->End) {
    if(queue->services[task]->End(queue->objects[queue->shadow])) {
      #if(QUEUE_DBG_ERROR)
        if(queue->debug) _QUEUE_Print(queue, task, queue->objects[queue->shadow], "?>");
      #endif
    }
  }
  if(queue->gotos[queue->shadow])
    queue->gotos[queue->shadow](queue->objects[queue->shadow]);
  if(queue->services[task]->object_size) dloc(queue->objects[queue->shadow]);
  #if(QUEUE_DBG_END)
    if(queue->debug) _QUEUE_Print(queue, task, queue->objects[queue->shadow], ">");
  #endif
  queue->shadow++;
  if(queue->shadow >= QUEUE_LIMIT) queue->shadow = 0;
  return task;
}

task_t QUEUE_Exception(QUEUE_t *queue, void *e)
{
  if(queue->tail == queue->shadow) return TASK_FREE;
  uint8_t task = queue->tasks[queue->shadow];
  if(queue->services[task]->End && !queue->not_make_end) {
    if(queue->services[task]->End(queue->objects[queue->shadow])) {
      #if(QUEUE_DBG_ERROR)
        if(queue->debug) _QUEUE_Print(queue, task, queue->objects[queue->shadow], "?>");
      #endif
    }
  }
  if(queue->exceptions[queue->shadow])
    queue->exceptions[queue->shadow](queue->objects[queue->shadow], e);
  if(queue->services[task]->object_size) dloc(queue->objects[queue->shadow]);
  #if(QUEUE_DBG_START_END)
    if(queue->debug) _QUEUE_Print(queue, task, queue->objects[queue->shadow], "!>");
  #endif
  queue->shadow++;
  if(queue->shadow >= QUEUE_LIMIT) queue->shadow = 0;
  return task;
}

task_t QUEUE_Repeat(QUEUE_t *queue)
{
  if(queue->tail == queue->shadow) return TASK_FREE;
  uint8_t task = queue->tasks[queue->shadow];
  queue->tail = (queue->tail) ? queue->tail - 1 : QUEUE_LIMIT - 1;
  #if(QUEUE_DBG_START_END)
    if(queue->debug) _QUEUE_Print(queue, task, queue->objects[queue->shadow], "..>");
  #endif
  return task;
}

task_t QUEUE_Inject(QUEUE_t *queue, uint8_t task, void *object, void (*Goto)(void *), void (*E)(void *, void *))
{
  if(_QUEUE_Uroboros(queue)) {
    #if(QUEUE_DBG_ERROR)
      if(queue->debug) _QUEUE_Print(queue, task, object, "!uroboros");
    #endif
    return TASK_FREE;
  }
  if(queue->tail != queue->shadow) QUEUE_Repeat(queue);
  uint8_t move = QUEUE_Todo(queue);
  for(uint8_t i = 0; i < move; i++) {
    uint8_t from = (queue->head < i + 1) ? QUEUE_LIMIT - 1 : queue->head - i - 1;
    uint8_t to = (queue->head < i) ? queue->head - i : QUEUE_LIMIT;
    queue->objects[to] = queue->objects[from];
    queue->tasks[to] = queue->tasks[from];
    queue->gotos[to] = queue->gotos[from];
  }
  #if(QUEUE_DBG_PUSH)
    if(queue->debug) _QUEUE_Print(queue, task, object, "inject");
  #endif
  queue->tasks[queue->tail] = task;
  if(queue->services[task]->object_size) {
    queue->objects[queue->tail] = aloc(queue->services[task]->object_size);
    if(object) memcpy(queue->objects[queue->tail], (uint8_t *)object, queue->services[task]->object_size);
    else memset(queue->objects[queue->tail], 0, queue->services[task]->object_size);
  }
  else queue->objects[queue->tail] = NULL;
  if(Goto) queue->gotos[queue->tail] = Goto;
  else queue->gotos[queue->tail] = NULL;
  if(E) queue->exceptions[queue->tail] = E;
  else queue->exceptions[queue->tail] = NULL;
  queue->head++;
  if(queue->head >= QUEUE_LIMIT) queue->head = 0;
  return task;
}

void QUEUE_Clear(QUEUE_t *queue)
{
  while(QUEUE_Skip(queue));
}

//-------------------------------------------------------------------------------------------------

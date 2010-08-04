#include "mem.h"
#include "arena.h"
#ifndef _MEMORY_H_
#define _MEMORY_H_
typedef struct mem {
  int maxbytes;
  char *scalars;
  Arena_T arena;
}t_mem;
extern t_mem g_mem;
#endif

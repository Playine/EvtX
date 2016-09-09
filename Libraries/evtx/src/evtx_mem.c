#include <stdlib.h>
#include "evtx_mem.h"


// memory management initialization
void evtx_mem_init(void)
{

}

// alloc memory buffer
void *evtx_mem_alloc(uint32_t size)
{
  return malloc(size);
}

// free memory space
void evtx_mem_free(void *ptr)
{
  free(ptr);
}


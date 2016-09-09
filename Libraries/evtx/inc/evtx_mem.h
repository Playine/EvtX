#ifndef __EVTX_MEM_H
#define __EVTX_MEM_H

#include "evtx_common.h"

// -----------------------------------------------------------------------------
// pubic function declarations
// -----------------------------------------------------------------------------
// memory allocation
void evtx_mem_init(void);                   // memory management initialization
void *evtx_mem_alloc(uint32_t size);        // alloc memory buffer
void evtx_mem_free(void *ptr);              // free memory space

#endif

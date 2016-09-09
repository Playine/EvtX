/**
  ******************************************************************************
  * @file    evtx.h
  * @brief   EvtX (event-driven real time kernel) public header file
  *
  * $Id$
  ******************************************************************************
  */

#ifndef __EVTX_H
#define __EVTX_H

#include "evtx_common.h"
#include "evtx_mem.h"
#include "evtx_timer.h"
#include "evtx_task.h"
#include "evtx_event.h"
#include "evtx_msg.h"

// public constants


// -----------------------------------------------------------------------------
// pubic function declarations
// -----------------------------------------------------------------------------
evtx_status_t evtx_init(void);                  // os initialization 
void evtx_start(void);                          // run forever
void evtx_run(void);                            // one-shot run
evtx_task_id_t evtx_active_task(void);          // return current active task

void *evtx_memcpy( void *dst, const void *src, unsigned int len );

// interrupt management
uint8_t evtx_int_enable(uint8_t int_id);        // enable interrupt
uint8_t evtx_int_disable(uint8_t int_id);       // disable interrupt

#endif

/**
  ******************************************************************************
  * @file    evtx.c
  * @brief   EvtX core source file
  * 
  * $Id$ 
  ******************************************************************************
  */
#include "evtx.h"
#include "evtx_task.h"
#include "evtx_event.h"
#include "evtx_msg.h"
#include "evtx_mq.h"
#include "evtx_clock.h"
#include "hal_board.h"
#include "hal.h"

// private variables
static evtx_task_id_t active_task_id = EVTX_TASK_NONE;


// os initializaiton
evtx_status_t evtx_init(void)
{
  
  evtx_mem_init();                // initialize memory management
  
  evtx_mq_init();                 // initialize message queue
  
  evtx_clock_init();              // initialize clock    
  
  evtx_timer_init();              // initialize the timers

  evtx_init_tasks();              // initialize system tasks

  return EVTX_SUCCESS;
}

// start and run task loop forever
void evtx_start(void)
{
  for(;;)
  {
    evtx_run();
  }
}

// run task event loop
void evtx_run(void)
{
  uint8_t index = 0;
  
  hal_process_poll();
  
  // poll event flags of every task
  do{
    if(tasks_events[index] != 0)
    {
      break;
    }
    
  }while(++index < tasks_cnt);
  
  if(index < tasks_cnt)
  {
    evtx_event_t    events;
    hal_int_state_t hal_int_state;
    
    // read and clear events of a task
    // enter critical section
    HAL_CRITICAL_ENTER(hal_int_state);
    events = tasks_events[index];
    tasks_events[index] = 0;
    // exit critical section
    HAL_CRITICAL_EXIT(hal_int_state);

    active_task_id = index;
    events = (tasks_array[index])(index, events);
    
    // update with adding new unprocessed events
    // enter critical section
    HAL_CRITICAL_ENTER(hal_int_state);
    tasks_events[index] |= events;
    // exit critical section
    HAL_CRITICAL_EXIT(hal_int_state);
  }
  
}


// return current active task id
evtx_task_id_t evtx_active_task(void)
{
  return active_task_id;
}

//
//   Note: This function differs from the standard memcpy(), since
//         it returns the pointer to the next destination uint8. The
//         standard memcpy() returns the original destination address.
void *evtx_memcpy( void *dst, const void *src, unsigned int len )
{
  uint8_t *p_dst;
  const uint8_t  *p_src;

  p_src = src;
  p_dst = dst;

  while ( len-- )
    *p_dst++ = *p_src++;

  return p_dst;
}

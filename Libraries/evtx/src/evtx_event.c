#include "evtx_event.h"
#include "evtx_task.h"
#include "hal_board.h"


// Set the event flags for a task. The event passed in is OR'd into the task's 
// event variable
evtx_status_t evtx_event_set(evtx_task_id_t task_id, evtx_event_t event_flag)
{
  if(task_id < tasks_cnt)
  {
    hal_int_state_t int_state;
    
    HAL_CRITICAL_ENTER(int_state);
    tasks_events[task_id] |= event_flag;
    HAL_CRITICAL_EXIT(int_state);
    
    return EVTX_SUCCESS;
  }
  else
  {
    return EVTX_INVALID_TASK;
  }
}

// Clear the event flag for a task. The event passed in is masked out of the task's
// event variable.
evtx_status_t evtx_event_clear(evtx_task_id_t task_id, evtx_event_t event_flag)
{
  if(task_id < tasks_cnt)
  {
    hal_int_state_t int_state;
    
    HAL_CRITICAL_ENTER(int_state);
    tasks_events[task_id] &= event_flag;
    HAL_CRITICAL_EXIT(int_state);
    
    return EVTX_SUCCESS;
  }
  else
  {
    return EVTX_INVALID_TASK;
  }
}

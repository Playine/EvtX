#include "hal.h"
#include "evtx.h"

// public variables
evtx_task_id_t hal_task_id;



// HAL task initialization
void hal_init(evtx_task_id_t task_id)
{
  hal_task_id = task_id;
}




// process HAL polling task
void hal_process_poll(void)
{
  
  
}

// process HAL event task
evtx_event_t hal_process_event(evtx_task_id_t task_id, evtx_event_t event)
{
  
  return EVTX_EVENT_NONE;
}


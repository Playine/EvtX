#include "task2.h"
#include "task_common.h"

// public variables
evtx_task_id_t task2_id;



void task2_init(evtx_task_id_t task_id)
{
  task2_id = task_id;
}

// this task simulates a consumer to receive data from other task
evtx_event_t task2_eventloop(uint8_t task_id, evtx_event_t event)
{
  (void)task_id;
  
  if(event & EVTX_EVENT_SYS_MSG)
  {
    task_cmd_msg_t *p_msg;
    
    while( (p_msg = (task_cmd_msg_t *)evtx_msg_recv(task2_id)) != NULL)
    {      
      if(p_msg->hdr.event == TASK_EVENT_RX_DATA)
      {
        //process the command
      }

      evtx_msg_free((uint8_t *)p_msg);
    }
    
    // return unprocessed events
    return event ^ EVTX_EVENT_SYS_MSG;
  }
  
  // discard unknown events
  return 0;
}

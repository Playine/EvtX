#include "evtx.h"
#include "task1.h"
#include "task_common.h"

// extern variables
extern evtx_task_id_t task2_id;

// public variables
evtx_task_id_t task1_id;

// private variables
uint8_t tx_data[] = " Hello, task 2!";

void task1_init(evtx_task_id_t task_id)
{
  task1_id = task_id;
}

// trigger data transmission
void task1_trigger_transmission(void)
{
  //init a reload timer for sending message
  evtx_timer_reload_start(task1_id, TASK_EVENT_TX_DATA, 500);
}



// this task simulate a producer to generate data periodically
evtx_event_t task1_eventloop(uint8_t task_id, evtx_event_t event)
{
  (void)task_id;
  
  if(event & EVTX_EVENT_SYS_MSG)
  {
    
    
    return (event ^ EVTX_EVENT_SYS_MSG);
  }
  
  if(event & TASK_EVENT_TX_DATA)
  {
    size_t len = sizeof(tx_data);
    // send message to task2
    task_cmd_msg_t *p_msg = (task_cmd_msg_t *)evtx_msg_alloc(sizeof(task_cmd_msg_t) + len);
    
    // put data into msg
    if(p_msg != NULL)
    {
      p_msg->hdr.event = TASK_EVENT_RX_DATA;
      p_msg->cmd = 'H';
      evtx_memcpy(p_msg->data, tx_data, len);
      evtx_msg_send(task2_id, (uint8_t *)p_msg);
    }
    
    return (event ^ TASK_EVENT_TX_DATA);
  }
  
  return 0;
}

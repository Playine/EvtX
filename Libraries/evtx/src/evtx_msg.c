#include "evtx_msg.h"
#include "evtx_mq.h"
#include "evtx_task.h"
#include "evtx_mem.h"
#include "hal_board.h"

// private variables
static evtx_mq_t  p_q_head;


void evtx_msg_init(void)
{
  
}

// -----------------------------------------------------------------------------
// allocate buffer space for a message to be fulfilled
// -----------------------------------------------------------------------------
uint8_t *evtx_msg_alloc(uint16_t len)
{
  evtx_msg_hdr_t  *p_hdr;
  
  if(len == 0)
  {
    return NULL;
  }

  p_hdr = (evtx_msg_hdr_t *)evtx_mem_alloc( (uint16_t) (len + sizeof(evtx_msg_hdr_t)));
  
  if(p_hdr != NULL)
  {
    p_hdr->next = NULL;
    p_hdr->len = len;
    p_hdr->dst_task_id = EVTX_TASK_NONE;
    
    return (uint8_t *)(p_hdr + 1);
  }
  else
  {
    return NULL;
  }
}

// -----------------------------------------------------------------------------
// Deallocate a message buffer. It is called by task after it has finished
// processing a received message
// -----------------------------------------------------------------------------
evtx_status_t evtx_msg_free(uint8_t *p_msg)
{
  
  if(p_msg == NULL)
  {
    return EVTX_INVALID_MSG_POINTER;
  }
  
  // don't deallocate queued buffer
  if(EVTX_MSG_ID(p_msg) != EVTX_TASK_NONE)
  {
    return EVTX_MSG_BUFFER_NA;
  }
  
  evtx_mem_free((void *) ((uint8_t *)((uint8_t *)p_msg - sizeof(evtx_msg_hdr_t))));
  
  return EVTX_SUCCESS;
}

// -----------------------------------------------------------------------------
// This is called by a task to send a message to another task
// -----------------------------------------------------------------------------
evtx_status_t evtx_msg_send(evtx_task_id_t dst_task_id, uint8_t *p_msg)
{
  return evtx_msg_enqueue_push(dst_task_id, p_msg, false);
}

// -----------------------------------------------------------------------------
// This function is called by a task to push a command message
// to the head of the EVTX queue. The dst_task_id field
// must refer to a valid task, since the task ID will be used to
// send the message to. This function will also set a message
// ready event in the destination task's event list.
// -----------------------------------------------------------------------------
evtx_status_t evtx_msg_push_front(evtx_task_id_t dst_task_id, uint8_t *p_msg)
{
  return evtx_msg_enqueue_push(dst_task_id, p_msg, true);
}

// -----------------------------------------------------------------------------
// This function is called by a task to either enqueue (append to queue) or 
// push (prepend to queue) a command message to the EVTX queue.
// -----------------------------------------------------------------------------
evtx_status_t evtx_msg_enqueue_push(evtx_task_id_t dst_task_id, uint8_t *p_msg, bool push)
{
  if(p_msg == NULL)
  {
    return EVTX_INVALID_MSG_POINTER;
  }

  if(dst_task_id >= tasks_cnt)
  {
    evtx_msg_free(p_msg);
    return EVTX_INVALID_TASK;
  }

  // Check the message header
  if((EVTX_MSG_NEXT(p_msg) != NULL) ||
     (EVTX_MSG_ID(p_msg) != EVTX_TASK_NONE))
  {
    evtx_msg_free(p_msg);
    return EVTX_INVALID_MSG_POINTER;
  }

  EVTX_MSG_ID(p_msg) = dst_task_id;

  if(push)
  {
    evtx_mq_push(&p_q_head, p_msg);         // prepend the message
  }
  else
  {
    evtx_mq_enqueue(&p_q_head, p_msg);      // append the message
  }

  // Signal the task that a message is waiting
  evtx_event_set(dst_task_id, EVTX_EVENT_SYS_MSG);

  return EVTX_SUCCESS;
}


// -----------------------------------------------------------------------------
//    This function is called by a task to retrieve a received command
//    message. The calling task must deallocate the message buffer after
//    processing the message using the osal_msg_deallocate() call.
// -----------------------------------------------------------------------------
uint8_t *evtx_msg_recv(evtx_task_id_t task_id)
{
  evtx_msg_hdr_t *p_list_hdr;
  evtx_msg_hdr_t *p_prev_hdr = NULL;
  evtx_msg_hdr_t *p_found_hdr = NULL;
  hal_int_state_t int_state;

  // Hold off interrupts
  HAL_CRITICAL_ENTER(int_state);

  // Point to the top of the queue
  p_list_hdr = p_q_head;

  // Look through the queue for a message that belongs to the asking task
  while ( p_list_hdr != NULL )
  {
    if ( (p_list_hdr - 1)->dst_task_id == task_id)
    {
      if ( p_found_hdr == NULL )
      {
        // Save the first one
        p_found_hdr = p_list_hdr;
      }
      else
      {
        // Second msg found, stop looking
        break;
      }
    }
    if ( p_found_hdr == NULL )
    {
      p_prev_hdr = p_list_hdr;
    }
    p_list_hdr = EVTX_MSG_NEXT( p_list_hdr );
  }

  // Is there more than one?
  if ( p_list_hdr != NULL )
  {
    // Yes, Signal the task that a message is waiting
    evtx_event_set( task_id, EVTX_EVENT_SYS_MSG);
  }
  else
  {
    // No more
    evtx_event_clear( task_id, EVTX_EVENT_SYS_MSG);
  }

  // Did we find a message?
  if ( p_found_hdr != NULL )
  {
    // Take out of the link list
    evtx_mq_extract(&p_q_head, p_found_hdr, p_prev_hdr );
  }

  // Re-enable interrupts
  HAL_CRITICAL_EXIT(int_state);

  return (uint8_t *) p_found_hdr;
}

// This function finds in place an OSAL message matching the task_id and event
//             parameters.
evtx_event_hdr_t *evtx_msg_find(evtx_task_id_t task_id, evtx_event_t event)
{
  evtx_msg_hdr_t *p_hdr;
  hal_int_state_t int_state;

  HAL_CRITICAL_ENTER(int_state);  // Hold off interrupts.

  p_hdr = p_q_head;                // Point to the top of the queue.

  // Look through the queue for a message that matches the task_id and event parameters.
  while (p_hdr != NULL)
  {
    if (((p_hdr-1)->dst_task_id == task_id) && (((evtx_event_hdr_t *)p_hdr)->event == event))
    {
      break;
    }

    p_hdr = EVTX_MSG_NEXT(p_hdr);
  }

  HAL_CRITICAL_ENTER(int_state);  // Release interrupts.

  return (evtx_event_hdr_t *)p_hdr;
}

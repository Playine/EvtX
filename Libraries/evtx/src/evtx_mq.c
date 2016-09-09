#include "evtx_mq.h"
#include "evtx_msg.h"
#include "evtx_task.h"
#include "hal_board.h"


void evtx_mq_init(void)
{
  
}


//This function enqueues an EVTX message into an EVTX queue.
void evtx_mq_enqueue(evtx_mq_t *p_q, void *p_msg )
{
  void *list;
  hal_int_state_t int_state;

  // Hold off interrupts
  HAL_CRITICAL_ENTER(int_state);

  EVTX_MSG_NEXT(p_msg) = NULL;
  // If first message in queue
  if ( *p_q == NULL )
  {
    *p_q = p_msg;
  }
  else
  {
    // Find end of queue
    for ( list = *p_q;EVTX_MSG_NEXT(list) != NULL;list = EVTX_MSG_NEXT( list ) );

    // Add message to end of queue
    EVTX_MSG_NEXT(list) = p_msg;
  }

  // Re-enable interrupts
  HAL_CRITICAL_EXIT(int_state);
}


// This function dequeues an EVTX message from an EVTX queue.
void *evtx_mq_dequeue(evtx_mq_t *p_q)
{
  void *p_msg = NULL;
  hal_int_state_t int_state;

  // Hold off interrupts
  HAL_CRITICAL_ENTER(int_state);

  if(*p_q != NULL)
  {
    // Dequeue message
    p_msg = *p_q;
    *p_q = EVTX_MSG_NEXT(p_msg);
    EVTX_MSG_NEXT(p_msg) = NULL;
    EVTX_MSG_ID(p_msg) = EVTX_TASK_NONE;
  }

  // Re-enable interrupts
  HAL_CRITICAL_EXIT(int_state);

  return p_msg;
}

//This function pushes an EVTX message to the head of an EVTX queue.
void evtx_mq_push(evtx_mq_t *p_q, void *p_msg)
{
  hal_int_state_t int_state;

  // Hold off interrupts
  HAL_CRITICAL_ENTER(int_state);

  // Push message to head of queue
  EVTX_MSG_NEXT(p_msg) = *p_q;
  *p_q = p_msg;

  // Re-enable interrupts
  HAL_CRITICAL_EXIT(int_state);
}


// -----------------------------------------------------------------------------
//
//    This function extracts and removes an OSAL message from the
//    middle of an OSAL queue.
// -----------------------------------------------------------------------------
void evtx_mq_extract( evtx_mq_t *p_q, void *p_msg, void *p_prev)
{
  hal_int_state_t int_state;

  // Hold off interrupts
  HAL_CRITICAL_ENTER(int_state);

  if(p_msg == *p_q)
  {
    // remove from first
    *p_q = EVTX_MSG_NEXT(p_msg);
  }
  else
  {
    // remove from middle
    EVTX_MSG_NEXT(p_prev) = EVTX_MSG_NEXT(p_msg);
  }
  EVTX_MSG_NEXT(p_msg) = NULL;
  EVTX_MSG_ID(p_msg) = EVTX_TASK_NONE;

  // Re-enable interrupts
  HAL_CRITICAL_EXIT(int_state);
}

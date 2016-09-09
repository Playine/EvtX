#include "evtx_timer.h"
#include "evtx_mem.h"
#include "evtx_event.h"
#include "hal_board.h"

// private typedefs
typedef union{
  uint32_t time32;
  uint16_t time16[2];
  uint8_t  time8[4];
}evtx_time_t;

typedef struct{
  void *next;
  evtx_time_t     timeout;
  evtx_event_t    event_flag;
  evtx_task_id_t  task_id;
  uint32_t        reload_timeout;
}evtx_timer_rec_t;

// public variables
evtx_timer_rec_t *p_timer_head;

// local variables
static uint32_t evtx_sysclk;          // ms since last reboot

// local function declarations
static evtx_timer_rec_t *evtx_timer_add(evtx_task_id_t task_id, evtx_event_t event_flag, uint32_t timeout);
static evtx_timer_rec_t *evtx_timer_find(evtx_task_id_t task_id, evtx_event_t event_flag);
static void evtx_timer_delete(evtx_timer_rec_t *ptimer);


// EvtX timer management intialization
void evtx_timer_init(void)
{
  evtx_sysclk = 0;
}


// add a timer to the timer lists
static evtx_timer_rec_t *evtx_timer_add(evtx_task_id_t task_id, evtx_event_t event_flag, uint32_t timeout)
{
  evtx_timer_rec_t  *p_new_timer;
  evtx_timer_rec_t  *p_temp_timer;
  
  p_new_timer = evtx_timer_find(task_id, event_flag);
  if(p_new_timer != NULL)
  {
    p_new_timer->timeout.time32 = timeout;
    
    return p_new_timer;
  }
  else
  {
    p_new_timer = evtx_mem_alloc(sizeof(evtx_timer_rec_t));
    if(p_new_timer != NULL)
    {
      // fill in new timer
      p_new_timer->task_id = task_id;
      p_new_timer->event_flag = event_flag;
      p_new_timer->timeout.time32 = timeout;
      p_new_timer->next = (void *)NULL;
      p_new_timer->reload_timeout = 0;
      
      // check if timer list is empty
      if( p_timer_head == NULL)
      {
        p_timer_head = p_new_timer;
      }
      else
      {
        p_temp_timer = p_timer_head;
        
        while(p_temp_timer->next != NULL)
        {
          p_temp_timer = p_temp_timer->next;
        }
        
        // add new timer to list
        p_temp_timer->next = p_new_timer;
      }
      
      return p_new_timer;
    }
    else
    {
      return (evtx_timer_rec_t *)NULL;
    }
  }
}

// find a timer in timer list, interrupts must be disabled
static evtx_timer_rec_t *evtx_timer_find(evtx_task_id_t task_id, evtx_event_t event_flag)
{
  evtx_timer_rec_t  *p_temp_timer;
  
  p_temp_timer = p_timer_head;
  
  while(p_temp_timer != NULL)
  {
    if( (p_temp_timer->event_flag == event_flag) &&
        (p_temp_timer->task_id == task_id))
    {
      break;
    }
    
    p_temp_timer = p_temp_timer->next;
  }
  
  return p_temp_timer;
}

// delete a timer from timer list
static void evtx_timer_delete(evtx_timer_rec_t *p_timer)
{
  if(p_timer != NULL)
  {
    // clear the event flag and evtx_timer_update() will delete the timer from
    // the list
    p_timer->event_flag = 0;
  }
}

// Start a timer to expire in n ms. When the timer expires, the calling task
// will get the specified event
uint8_t evtx_timer_start_ex(uint8_t task_id, uint16_t event_id, uint32_t timeout_val)
{
  hal_int_state_t   int_state;
  evtx_timer_rec_t  *p_new_timer;
  
  HAL_CRITICAL_ENTER(int_state);
  
  p_new_timer = evtx_timer_add(task_id, event_id, timeout_val);
  
  HAL_CRITICAL_EXIT(int_state);
  
  return (p_new_timer != NULL) ? EVTX_SUCCESS : EVTX_TIMER_NA;
}


// Start a timer to expire in n ms. When the timer expires, the calling task will get the 
uint8_t evtx_timer_reload_start(uint8_t task_id, uint16_t event_id, uint32_t timeout_val)
{
  hal_int_state_t   int_state;
  evtx_timer_rec_t  *p_new_timer;
  
  HAL_CRITICAL_ENTER(int_state);
  
  p_new_timer = evtx_timer_add(task_id, event_id, timeout_val);
  if(p_new_timer != NULL)
  {
    p_new_timer->reload_timeout = timeout_val;
  }
  
  HAL_CRITICAL_EXIT(int_state);
  
  return (p_new_timer != NULL) ? EVTX_SUCCESS : EVTX_TIMER_NA;
}

// Stop a timer that has already been started. If success, the function
// will cancel the timer and prevent the event associated with the timer
// from being set for the calling task.
uint8_t evtx_timer_stop_ex(uint8_t task_id, uint16_t event_id)
{
  hal_int_state_t   int_state;
  evtx_timer_rec_t  *p_found_timer;
  
  HAL_CRITICAL_ENTER(int_state);
  
  p_found_timer = evtx_timer_find(task_id, event_id);
  if(p_found_timer != NULL)
  {
    evtx_timer_delete(p_found_timer);
  }
  
  HAL_CRITICAL_EXIT(int_state);
  
  return (p_found_timer != NULL) ? EVTX_SUCCESS : EVTX_INVALID_EVENT;
}


// update the timer record for a timer tick
void evtx_timer_update(uint32_t update_time)
{
  hal_int_state_t int_state;
  evtx_timer_rec_t  *p_temp_timer;
  evtx_timer_rec_t  *p_prev_timer;
  
  evtx_time_t       time_union;
  time_union.time32 = update_time;
  
  HAL_CRITICAL_ENTER(int_state);
  
  evtx_sysclk += update_time;             // update system time
  
  HAL_CRITICAL_EXIT(int_state);
  
  if(p_timer_head != NULL)
  {
    p_temp_timer = p_timer_head;
    p_prev_timer = (void*)NULL;
    
    while(p_temp_timer != NULL)
    {
      evtx_timer_rec_t  *p_free_timer = NULL;
      
      HAL_CRITICAL_ENTER(int_state);
      
      // notes: 
      // original osalTimerUpdate() avoid 32-bit math on 8051 platform,
      // for cortex-m3 core, it's unnecessary   
      if(p_temp_timer->timeout.time32 > time_union.time32)
      {
        p_temp_timer->timeout.time32 -= time_union.time32;
      }
      else
      {
        p_temp_timer->timeout.time32 = 0;
      }
      
      // check for reloading
      if( (p_temp_timer->timeout.time32 == 0) && 
          (p_temp_timer->reload_timeout != 0) &&
          (p_temp_timer->event_flag != EVTX_EVENT_NONE))
      {
        // notify the task of a timeout
        evtx_event_set(p_temp_timer->task_id, p_temp_timer->event_flag);
        
        // reload the timer timeout value
        p_temp_timer->timeout.time32 = p_temp_timer->reload_timeout;
      }
      
      // when timeout or delete (event_flag == EVTX_EVENT_NONE)
      if( (p_temp_timer->timeout.time32 == 0) ||
          (p_temp_timer->event_flag == EVTX_EVENT_NONE))
      {
        // remove from list
        if(p_prev_timer == NULL)
        {
          p_timer_head = p_temp_timer->next;
        }
        else
        {
          p_prev_timer->next = p_temp_timer->next;
        }
        
        // set to free memory
        p_free_timer = p_temp_timer;
        p_temp_timer = p_temp_timer->next;
      }
      else
      {
        // get next
        p_prev_timer = p_temp_timer;
        p_temp_timer = p_temp_timer->next;
      }
      
      HAL_CRITICAL_EXIT(int_state);
      
      if(p_free_timer != NULL)
      {
        if(p_free_timer->timeout.time32 == 0)
        {
          evtx_event_set(p_free_timer->task_id, p_free_timer->event_flag);
        }
        evtx_mem_free(p_free_timer);
      }
    }
  }
}

 // read system clock in ms
uint32_t evtx_sysclock_get(void)
{ 
  return evtx_sysclk;
}

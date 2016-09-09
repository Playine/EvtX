#include "evtx_clock.h"
#include "evtx_timer.h"
#include "hal_board.h"

// private macros
#define CONVERT_MS_TO_S_ELAPSED_REMAINDER( x, y, z ) st(     \
  y += x / 1000;                                             \
  z = x % 1000;                                              \
)
  
// private variables
static uint32_t time_ms = 0;

// number of seconds since 0 hrs, 0 minutes, 0 seconds, on the
// 1st of January 2000 UTC
utc_time_t evtx_time_secs = 0;

// private function declarations
static void evtx_clock_update(uint32_t elapsed_ms);

// EvtX clock module intialization
void evtx_clock_init(void)
{
  hal_systick_int_register(evtx_time_update);
}

// This is invoked to update clock and timers.
// this should be used with sys tick timer
void evtx_time_update(uint32_t ms)
{
  HAL_SYSTICK_INT_DISABLE();
  
  evtx_clock_update(ms);
  evtx_timer_update(ms);
  
  HAL_SYSTICK_INT_ENABLE();
}

// update the EvtX clock time wiht elasped ms
static void evtx_clock_update(uint32_t elapsed_ms)
{
  uint32_t tmp;
  hal_int_state_t int_state;
  
  HAL_CRITICAL_ENTER(int_state);
  // Add elapsed milliseconds to the saved millisecond portion of time
  time_ms += elapsed_ms;

  // Roll up milliseconds to the number of seconds
  if ( time_ms >= 1000 )
  {
    tmp = time_ms;
    CONVERT_MS_TO_S_ELAPSED_REMAINDER(tmp, evtx_time_secs, time_ms);
  }
  HAL_CRITICAL_EXIT(int_state);
}

// set current system time in second
void evtx_clock_set(utc_time_t new_time)
{
  hal_int_state_t int_state;
  
  HAL_CRITICAL_ENTER(int_state);
  
  evtx_time_secs = new_time;
  
  HAL_CRITICAL_EXIT(int_state);
}

// return number of secs
utc_time_t evtx_clock_get(void)
{
  return evtx_time_secs;
}

// convert utc_time_t value to utc_time_struct_t value
void evtx_utctime_convert(utc_time_struct_t *tm, utc_time_t sec)
{
  
}



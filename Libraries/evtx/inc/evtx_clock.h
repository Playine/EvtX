#ifndef __EVTX_CLOCK_H
#define __EVTX_CLOCK_H

#include "evtx_common.h"

// public typedefs
// number of seconds since 0 hrs, 0 minutes, 0 seconds, on the
// 1st of January 2000 UTC
typedef uint32_t utc_time_t;

// To be used with
typedef struct
{
  uint8_t   seconds;  // 0-59
  uint8_t   minutes;  // 0-59
  uint8_t   hour;     // 0-23
  uint8_t   day;      // 0-30
  uint8_t   month;    // 0-11
  uint16_t  year;    // 2000+
} utc_time_struct_t;


void evtx_clock_init(void);
void evtx_time_update(uint32_t ms);
#endif

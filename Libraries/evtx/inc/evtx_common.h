#ifndef __EVTX_COMMON_H
#define __EVTX_COMMON_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

// EVTX error codes
#define EVTX_SUCCESS                      0
#define EVTX_INVALID_MSG_POINTER          1
#define EVTX_MSG_BUFFER_NA                2 
#define EVTX_INVALID_TASK                 4       
#define EVTX_TIMER_NA                     8       // no timer available
#define EVTX_INVALID_EVENT                16      // invalid event id
#define EVTX_INVALID_INT                  32      // invalid interrupt

// Event constants
#define EVTX_EVENT_NONE                  0x0000   // none event
#define EVTX_EVENT_SYS_MSG               0x8000   // a message is waiting event

// public typedefs
typedef uint8_t     evtx_status_t;                // status 
typedef uint16_t    evtx_event_t;                 // event (flag) type
typedef uint8_t     evtx_task_id_t;               // task id


#endif

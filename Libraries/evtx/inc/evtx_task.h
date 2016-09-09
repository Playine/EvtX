#ifndef __EVTX_TASK_H
#define __EVTX_TASK_H

#include "evtx_common.h"

// public constants
#define EVTX_TASK_NONE        0xFF


// task eventloop handler
typedef evtx_event_t (*task_eventloop_t)(uint8_t task_id, evtx_event_t event);

// public variables
// following variables must be defined by application
extern const task_eventloop_t tasks_array[];
extern const uint8_t tasks_cnt;
extern evtx_event_t *tasks_events;

// public function declarations
extern void evtx_init_tasks(void);       // must be defined by application
#endif

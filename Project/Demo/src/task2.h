#ifndef __TASK2_H
#define __TASK2_H

#include "evtx.h"

void task2_init(evtx_task_id_t task_id);
evtx_event_t task2_eventloop(uint8_t task_id, evtx_event_t event);

#endif

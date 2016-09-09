#ifndef __TASK1_H
#define __TASK1_H

#include "evtx.h"

void task1_init(evtx_task_id_t task_id);
evtx_event_t task1_eventloop(uint8_t task_id, evtx_event_t event);
void task1_trigger_transmission(void);
#endif

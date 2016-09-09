#ifndef __HAL_H
#define __HAL_H

#include "evtx.h"

// public macros

// public variables
extern evtx_task_id_t hal_task_id;

// public function declarations
void hal_init(evtx_task_id_t task_id);
void hal_process_poll(void);
evtx_event_t hal_process_event(evtx_task_id_t task_id, evtx_event_t event);
#endif

#ifndef __EVTX_EVENT_H
#define __EVTX_EVENT_H

#include "evtx_common.h"

// Event Header
typedef struct{
  evtx_event_t    event;
  uint8_t         status;
}evtx_event_hdr_t;

// public function delcarations
evtx_status_t evtx_event_set(evtx_task_id_t task_id, evtx_event_t event_flag); 
evtx_status_t evtx_event_clear(evtx_task_id_t task_id, evtx_event_t event_flag);
#endif

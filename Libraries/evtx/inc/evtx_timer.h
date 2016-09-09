#ifndef __EVTX_TIMER_H
#define __EVTX_TIMER_H

#include "evtx_common.h"

// public function declarations
void evtx_timer_init(void);
uint8_t evtx_timer_start_ex(uint8_t task_id, uint16_t event_id, uint32_t timeout_val);
uint8_t evtx_timer_reload_start(uint8_t task_id, uint16_t event_id, uint32_t timeout_val);
uint8_t evtx_timer_stop_ex(uint8_t task_id, uint16_t event_id);
uint32_t evtx_sysclock_get(void);
void evtx_timer_update(uint32_t update_time);
#endif

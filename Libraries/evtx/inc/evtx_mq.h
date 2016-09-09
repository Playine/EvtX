
#ifndef __EVTX_MQ_H
#define __EVTX_MQ_H

#include "evtx_common.h"

// public typedefs
typedef void *evtx_mq_t;

// public function declarations
void evtx_mq_init(void);
void evtx_mq_enqueue(evtx_mq_t *p_q, void *p_msg );
void *evtx_mq_dequeue(evtx_mq_t *p_q);
void evtx_mq_push(evtx_mq_t *p_q, void *p_msg);
void evtx_mq_extract( evtx_mq_t *p_q, void *p_msg, void *p_prev);
#endif

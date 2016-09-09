#ifndef __EVTX_MSG_H
#define __EVTX_MSG_H

#include "evtx_common.h"
#include "evtx_event.h"
#include "evtx_mq.h"

// public macros
#if ( UINT_MAX == 65535 ) /* 8-bit and 16-bit devices */
  #define evtx_offsetof(type, member) ((uint16) &(((type *) 0)->member))
#else /* 32-bit devices */
  #define evtx_offsetof(type, member) ((uint32) &(((type *) 0)->member))
#endif

#define EVTX_MSG_NEXT(p_msg)      ((evtx_msg_hdr_t *)(p_msg) - 1)->next
#define EVTX_MSG_LEN(p_msg)       ((evtx_msg_hdr_t *)(p_msg) - 1)->len
#define EVTX_MSG_ID(p_msg)        ((evtx_msg_hdr_t *)(p_msg) - 1)->dst_task_id
#define EVTX_MSG_Q_INIT(p_q)      *(p_q) = NULL
#define EVTX_MSG_Q_EMPTY(p_q)     (*(p_q) == NULL)
#define EVTX_MSG_Q_HEAD(p_q)      (*(p_q))

// public typedefs
// Message Header
typedef struct{
  void            *next;
  uint16_t        len;
  evtx_task_id_t  dst_task_id;
}evtx_msg_hdr_t;


// public function delcarations
void evtx_msg_init(void);
uint8_t *evtx_msg_alloc(uint16_t len);
evtx_status_t evtx_msg_free(uint8_t *p_msg);          // free the message space after usage
evtx_status_t evtx_msg_send(evtx_task_id_t dst_task_id, uint8_t *p_msg);   // send a message to a task
evtx_status_t evtx_msg_enqueue_push(evtx_task_id_t dst_task_id, uint8_t *p_msg, bool push);
evtx_status_t evtx_msg_push_front(evtx_task_id_t dst_task_id, uint8_t *p_msg);
uint8_t *evtx_msg_recv(evtx_task_id_t task_id);       // calling task to receive a message
evtx_event_hdr_t *evtx_msg_find(evtx_task_id_t task_id, evtx_event_t event);
//void evtx_msg_push(evtx_mq_t *p_queue, void *p_msg);
//void evtx_msg_extract(evtx_mq_t *p_queue, void *p_msg, void *p_prev);
#endif

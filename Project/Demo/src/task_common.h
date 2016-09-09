#ifndef __TASK_COMMON_H
#define __TASK_COMMON_H

#include "evtx.h"

// task related event types
#define TASK_EVENT_RX_DATA        0x0010
#define TASK_EVENT_TX_DATA        0x0020


typedef struct
{
  evtx_event_hdr_t hdr;
  uint8_t cmd;
  uint8_t data[]; 
} task_cmd_msg_t;

#endif

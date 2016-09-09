#include "evtx.h"
#include "main.h"
#include "task1.h"
#include "task2.h"
#include "hal.h"
#include "hal_board.h"


// array of task event loop handlers
const task_eventloop_t tasks_array[] = {
  hal_process_event,
  task1_eventloop,
  task2_eventloop
};


// total count of tasks
const uint8_t tasks_cnt = sizeof(tasks_array) / sizeof(tasks_array[0]);

// pointer to task events array
evtx_event_t *tasks_events;

// function prototypes

// Main Entry Point
int main(void)
{
  hal_board_config();
  evtx_init();
  
  task1_trigger_transmission();
  
  evtx_start();
}

void evtx_init_tasks(void)
{
  uint8_t task_id = 0;
  
  tasks_events = (uint16_t *) evtx_mem_alloc(sizeof(uint16_t) * tasks_cnt);
  
  hal_init(task_id++);
  task1_init(task_id++);
  task2_init(task_id++);
}


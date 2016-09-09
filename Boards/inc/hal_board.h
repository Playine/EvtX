#ifndef __HAL_BOARD_H
#define __HAL_BOARD_H

#include "stm32f10x.h"
#include "core_cm3.h"

// typedefs
typedef uint8_t   hal_int_state_t;

// C statement formation macro
#define st(x)     do { x } while(__LINE__ == -1)


#define HAL_SYSTICK_INT_DISABLE()
#define HAL_SYSTICK_INT_ENABLE()
#define HAL_INT_DISABLE()         __disable_irq()
#define HAL_INT_ENABLE()          __enable_irq()

#define HAL_CRITICAL_ENTER(x)     st( x = x; HAL_INT_DISABLE();)
#define HAL_CRITICAL_EXIT(x)      st( x = x; HAL_INT_ENABLE();)


// public function declarations
void SysTick_Handler(void);
void hal_board_config(void);
void hal_board_init(void);
void hal_systick_int_register(void* cb);
void hal_systick_int_enable(void);
void hal_systick_enable(void);
#endif

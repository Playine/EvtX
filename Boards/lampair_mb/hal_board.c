#include <stddef.h>
#include "hal_board.h"


// private vars
static void (*systick_int_cb)(int ms)  = NULL;


static void RCC_ClockEnable(void)
{ 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | 
    RCC_APB1Periph_TIM6 | RCC_APB1Periph_TIM7 |	RCC_APB1Periph_USART2 | 
    RCC_APB1Periph_UART4 | RCC_APB1Periph_UART5 | RCC_APB1Periph_I2C1, ENABLE);				 
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM8 | RCC_APB2Periph_GPIOA | 
    RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | 
    RCC_APB2Periph_ADC1 | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE );	  	 
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE );
}

static void RCC_Configuration(void)
{
    ErrorStatus HSEStartUpStatus;
  
    RCC_DeInit();    //RCC system reset 

    RCC_HSEConfig(RCC_HSE_ON);  // Enable HSE

    HSEStartUpStatus = RCC_WaitForHSEStartUp();  // Wait till HSE is ready 

    if(HSEStartUpStatus == SUCCESS)
    {
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);  // Enable Prefetch Buffer 

        FLASH_SetLatency(FLASH_Latency_2);       // Flash 2 wait state 
        RCC_HCLKConfig(RCC_SYSCLK_Div1);        // HCLK = SYSCLK    //AHB
        RCC_PCLK2Config(RCC_HCLK_Div1);         // PCLK2 = HCLK 		//APB2 

        RCC_PCLK1Config(RCC_HCLK_Div8);         // PCLK1 = HCLK/8 	//APB1
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);    // PLLCLK = 8MHz * 9 = 72 MHz 
        RCC_PLLCmd(ENABLE); // Enable PLL 

        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)     // Wait till PLL is ready 
        {}

        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);      // Select PLL as system clock source 

        while(RCC_GetSYSCLKSource() != 0x08)            // Wait till PLL is used as system clock source 
        {}  
    }
}

void SysTick_Handler(void)
{
  if(systick_int_cb != NULL)
  {
    systick_int_cb(1);
  }
}

// HAL board initialization
void hal_board_init(void)
{

}

// HAL board configuration
void hal_board_config(void)
{
  // init systick
  if(SysTick_Config(SystemCoreClock / 1000))
  {
    // hal_assert(0);
    while(1);
  }
  
	RCC_Configuration();	
	RCC_ClockEnable();    
}

void hal_systick_int_register(void* cb)
{
  systick_int_cb = (void (*)(int))cb;
}

void hal_systick_int_enable(void)
{
  
}

void hal_systick_enable(void)
{
  
}


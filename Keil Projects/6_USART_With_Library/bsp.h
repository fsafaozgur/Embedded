#ifndef __BSP__H__
#define __BSP__H__

#include "stm32f4xx.h"



void USART_Conf(void);
//void USART_Send(volatile char* c);
void USART2_Send(volatile char* data);
void delay(uint32_t i);	
void USART2_IRQHandler(void);



#endif /* __BSP__H__ */



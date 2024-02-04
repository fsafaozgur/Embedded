#ifndef __BSP__H__
#define __BSP__H__

#include "stm32f4xx.h"                


void SPI_Tx(uint8_t adress, uint8_t data);
uint8_t SPI_Rx(uint8_t adress);
void USART_Puts (USART_TypeDef* USARTx, volatile char *s);
void Configuration (void);


#endif /* __BSP__H__ */
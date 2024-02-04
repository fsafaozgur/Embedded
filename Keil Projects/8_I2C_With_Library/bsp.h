#ifndef __BSP_H__
#define __BSP_H__

#include "stm32f4xx.h"                 


/*  I2C ile haberlesen Harici bir sensör olan MPU6050 ivme ölçer sensörü kullaniyoruz  */

#define SLAVE_ADRESS 0x68			/* I2C haberlesme protokolü iki hat üzerinden onlarca cihazla haberlesebildigi için
																 her Slave (sensör benzeri çevresel birimler) ile haberlesmede en basta o Slave nin adresi yollanir
																 böylece yalnizca haberlesmek istedigimiz sensörle iletisime geçmis oluruz */





void I2C_start (I2C_TypeDef* I2Cx, uint8_t adress, uint8_t direction);
void I2C_write(I2C_TypeDef* I2Cx, uint8_t data);
uint8_t I2C_read_ack(I2C_TypeDef* I2Cx);
uint8_t I2C_read_nack(I2C_TypeDef* I2Cx);
void I2C_stop(I2C_TypeDef* I2Cx);
void I2C_Configuration(void);
void USART_Puts (USART_TypeDef* USARTx, volatile char *s);
long map (long x, long in_min, long in_max, long out_min, long out_max);
void Configuration(void);
void delay(uint32_t i);	




#endif /* __BSP_H__ */
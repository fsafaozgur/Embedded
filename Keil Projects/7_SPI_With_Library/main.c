#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "bsp.h"

/*  stm32f407VG discoveryboard üzerinde bulunan ve SPI ile haberlesen LIS302DL ivme ölçer sensörü kullaniyoruz  */







int8_t x, y, z;								/* Sensörden X-Y-Z eksenleri için alacagimiz degerleri girecegimiz degiskenler */
char str[50];									/* USART ile seri porta gönderilecek karakterleri içeren dizi */
uint32_t i ;


int main()
{
	

	Configuration();



	GPIO_SetBits(GPIOE, GPIO_Pin_3);					/*Chip Select (Slave Select) pinini baslangiç olarak 1 konumuna aldik, yani pasif konuma çektik */
	
	SPI_Tx(0x20, 0x67);   										/* Sensörün datasheetinde yer alan G degerinin girildigi register adresi olan 0x20 adresine, 0x67 degerini yaz dedik (yani 100Hz 9.2g olarak ayarladik) */

	
	while (1)
	{
		x = SPI_Rx(0x29);												/* X eksenindeki ivme degerinin yazildigi sensörün içindeki registerin adresini yollayarak (sensörün datasheetinde register adresleri var), bu registerda ne varsa bize geri döndür dedik */
		y = SPI_Rx(0x2B);												/* Y eksenindeki ivme degerinin yazildigi sensörün içindeki registerin adresini yollayarak (sensörün datasheetinde register adresleri var, bu registerda ne varsa bize geri döndür dedik */
		z = SPI_Rx(0x2D);												/* Z eksenindeki ivme degerinin yazildigi sensörün içindeki registerin adresini yollayarak (sensörün datasheetinde register adresleri var, bu registerda ne varsa bize geri döndür dedik */
	
		if (x<-20) {
		GPIO_SetBits(GPIOD, GPIO_Pin_12);		
		} else if (x>20) {
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		GPIO_SetBits(GPIOD, GPIO_Pin_13);			
		} else {
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);
		}
		
		if (y<-20) {
		GPIO_SetBits(GPIOD, GPIO_Pin_14);		
		} else {
		GPIO_ResetBits(GPIOD, GPIO_Pin_14);			
		}
		
		if (z<-20) {
		GPIO_SetBits(GPIOD, GPIO_Pin_15);		
		} else {
		GPIO_ResetBits(GPIOD, GPIO_Pin_15);	
		}
		
		sprintf(str, "%d \n", x);
		USART_Puts(USART2, str);
		sprintf(str, "%d \n", y);
		USART_Puts(USART2, str);
		sprintf(str, "%d \n", z);
		USART_Puts(USART2, str);
		
		
		/* Asagidaki gibi tek satirda da yollayabilirdik
		
		sprintf(str, "x=%d , y=%d, z=%d \n", x, y, z);
		USART_Puts(USART2, str);  
		
		*/
		
		
		
		i=7000000;								/* Örnek amaçli kullanildi, islemciyi sürekli mesgul eden bu tarz delay fonksiyonlari kullanilmamali */
															/* Bu sürede güç tasarrufu yapilmalidir, RTOS mantigindaki Idle_Thread de kullanilarak yapilabilir */  
		while (i)									/* Ya da SysTick Timer kullanilabilir */
			i--;
		
	}

}






 

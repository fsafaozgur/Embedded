#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "bsp.h"

/*  stm32f407VG discoveryboard �zerinde bulunan ve SPI ile haberlesen LIS302DL ivme �l�er sens�r� kullaniyoruz  */







int8_t x, y, z;								/* Sens�rden X-Y-Z eksenleri i�in alacagimiz degerleri girecegimiz degiskenler */
char str[50];									/* USART ile seri porta g�nderilecek karakterleri i�eren dizi */
uint32_t i ;


int main()
{
	

	Configuration();



	GPIO_SetBits(GPIOE, GPIO_Pin_3);					/*Chip Select (Slave Select) pinini baslangi� olarak 1 konumuna aldik, yani pasif konuma �ektik */
	
	SPI_Tx(0x20, 0x67);   										/* Sens�r�n datasheetinde yer alan G degerinin girildigi register adresi olan 0x20 adresine, 0x67 degerini yaz dedik (yani 100Hz 9.2g olarak ayarladik) */

	
	while (1)
	{
		x = SPI_Rx(0x29);												/* X eksenindeki ivme degerinin yazildigi sens�r�n i�indeki registerin adresini yollayarak (sens�r�n datasheetinde register adresleri var), bu registerda ne varsa bize geri d�nd�r dedik */
		y = SPI_Rx(0x2B);												/* Y eksenindeki ivme degerinin yazildigi sens�r�n i�indeki registerin adresini yollayarak (sens�r�n datasheetinde register adresleri var, bu registerda ne varsa bize geri d�nd�r dedik */
		z = SPI_Rx(0x2D);												/* Z eksenindeki ivme degerinin yazildigi sens�r�n i�indeki registerin adresini yollayarak (sens�r�n datasheetinde register adresleri var, bu registerda ne varsa bize geri d�nd�r dedik */
	
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
		
		
		
		i=7000000;								/* �rnek ama�li kullanildi, islemciyi s�rekli mesgul eden bu tarz delay fonksiyonlari kullanilmamali */
															/* Bu s�rede g�� tasarrufu yapilmalidir, RTOS mantigindaki Idle_Thread de kullanilarak yapilabilir */  
		while (i)									/* Ya da SysTick Timer kullanilabilir */
			i--;
		
	}

}






 

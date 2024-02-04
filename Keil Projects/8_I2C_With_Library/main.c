#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bsp.h"





char str[50];									/* USART ile seri porta g�nderilecek karakterleri i�eren dizi */
uint32_t i ;
uint16_t a, b, c, a1, b1, c1;
uint8_t m, m1;
uint16_t d, d1;




int main()
{
	
	
	Configuration();
	I2C_Configuration();
	
	while (1)
	{
	
		/* Slave adresimiz 7  bit oldugundan onu sola kaydirdik ve arkasina Transmit yapacagimizi s�yleyen biti ekleyerek toplamda 8 bit elde ettik */
		I2C_start(I2C1, SLAVE_ADRESS<<1, I2C_Direction_Transmitter);  
		/* Slavenin PWR_MGMT_1 registerine 1 byte yazacagiz ve b�ylece MPU-6050 sens�r� aktif hale gelecek(uyandirilacak) */
		I2C_write(I2C1, 0x6B);				/* PWR_MGMT_1 registerinin adresi = 0x6B */
		/* PWR_MGMT_1 registerinin t�m bitlerini sifir yaparsak sens�r uyaniyor, 0x00 i yaziyoruz */
		I2C_write(I2C1, 0x00);				/* �nce veri yazilacak register adresini yolladik, sonrada yazilacak 1 byte lik veriyi */
		I2C_stop(I2C1);								/* Haberlestirmeyi durdurduk */
		
		
		
		/* Bu ayar sens�r�n G degerini ayarlamak i�in 0x1C adresli ACCEL_CONFIG registerine veri yaziyor, 0x00 yollarsak 2g se�ilir */
		I2C_start(I2C1, SLAVE_ADRESS<<1, I2C_Direction_Transmitter);
		I2C_write(I2C1, 0x1C);
		I2C_write(I2C1, 0x00);
		I2C_stop(I2C1);
		
		
		
		/* Bu ayar sens�r�n X ekseni ivme �ikisinin High Bitini ilgili register olan ACCEL_XOUT_H dan okumamizi sagliyor */
		/* High bitten kasit [15:8] bitleri */
		I2C_start(I2C1, SLAVE_ADRESS<<1, I2C_Direction_Transmitter);
		I2C_write(I2C1, 0x3B);
		I2C_stop(I2C1);
		I2C_start(I2C1, SLAVE_ADRESS<<1, I2C_Direction_Receiver);
		a = (I2C_read_nack(I2C1));		
		
		/* Bu ayar sens�r�n X ekseni ivme �ikisinin Low Bitini ilgili register olan ACCEL_XOUT_L dan okumamizi sagliyor */
		/* Low bitten kasit [7:0] bitleri */
		I2C_start(I2C1, SLAVE_ADRESS<<1, I2C_Direction_Transmitter);
		I2C_write(I2C1, 0x3C);
		I2C_stop(I2C1);	
		I2C_start(I2C1, SLAVE_ADRESS<<1, I2C_Direction_Receiver);
		b = (I2C_read_nack(I2C1));	




		/* Bu ayar sens�r�n Y ekseni ivme �ikisinin High Bitini ilgili register olan ACCEL_YOUT_H dan okumamizi sagliyor */
		/* High bitten kasit [15:8] bitleri */
		I2C_start(I2C1, SLAVE_ADRESS<<1, I2C_Direction_Transmitter);
		I2C_write(I2C1, 0x3D);
		I2C_stop(I2C1);
		I2C_start(I2C1, SLAVE_ADRESS<<1, I2C_Direction_Receiver);
		a1 = (I2C_read_nack(I2C1));		
		
		/* Bu ayar sens�r�n Y ekseni ivme �ikisinin Low Bitini ilgili register olan ACCEL_YOUT_L dan okumamizi sagliyor */
		/* Low bitten kasit [7:0] bitleri */
		I2C_start(I2C1, SLAVE_ADRESS<<1, I2C_Direction_Transmitter);
		I2C_write(I2C1, 0x3E);
		I2C_stop(I2C1);	
		I2C_start(I2C1, SLAVE_ADRESS<<1, I2C_Direction_Receiver);
		b1 = (I2C_read_nack(I2C1));	


		d = ( (a<<8) | b );								/* X ekseni i�in a high biti oldugundan 8 sola kaydirip lob bit olan b yi |(veya) operat�r�yle arkasina ekleyip 16 bitlik bir data elde edip 16 bitlik d degiskenine atiyoruz */
		d1 = ( (a1<<8) | b1 );						/* Ayni islemi Y ekseni i�in tekrarliyoruz */

		d=map(d, -17000, 17000, 0, 10);		/* Onluk sistemde bize -17000 ile 17000 arasi deger d�nd�r�yor bunu rahat okumak i�in 0 ile 10 arasinda deger d�nd�rmesini sagliyoruz */
		d1=map(d, -17000, 17000, 0, 10);	/* Ayni islemi d1 i�in de yaptik */
		
		sprintf(str, "X = %d", d);
		USART_Puts(USART1, str);
		sprintf(str, "Y = %d", d1);
		USART_Puts(USART1, str);
		
		
		if (d>=7 && 2<d1 && d1<7)		/* Kirmizi */
		{
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		GPIO_SetBits(GPIOD, GPIO_Pin_13 | GPIO_Pin_14 );	
		}
		if (d<=2 && 2<d1 && d1<7)		/* Yesil */
		{
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);
		GPIO_SetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_14 );	
		}
		if (d1>=7 && 2<d && d1<7)		/* Mavi */
		{
		GPIO_ResetBits(GPIOD, GPIO_Pin_14);
		GPIO_SetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 );	
		}
		if (d1<=2 && 2<d && d<7)		/* Mor */
		{
		GPIO_ResetBits(GPIOD, GPIO_Pin_12 |GPIO_Pin_14);
		GPIO_SetBits(GPIOD, GPIO_Pin_13 );	
		}
		if (2<d &&  d<7 && 2<d1 && d1<7)		/* Beyaz */
		{
		GPIO_ResetBits(GPIOD, GPIO_Pin_12 |GPIO_Pin_14 | GPIO_Pin_13);	
		}
		
		
		
		delay(600000);									/* �rnek ama�li kullanildi, islemciyi s�rekli mesgul eden bu tarz delay fonksiyonlari kullanilmamali */
																		/* Bu s�rede g�� tasarrufu yapilmalidir, RTOS mantigindaki Idle_Thread de kullanilarak yapilabilir */
																		/* Ya da SysTick Timer kullanilabilir */
			
	}
	
	
	
}


 
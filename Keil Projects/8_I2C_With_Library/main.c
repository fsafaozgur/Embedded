#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bsp.h"





char str[50];									/* USART ile seri porta gönderilecek karakterleri içeren dizi */
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
	
		/* Slave adresimiz 7  bit oldugundan onu sola kaydirdik ve arkasina Transmit yapacagimizi söyleyen biti ekleyerek toplamda 8 bit elde ettik */
		I2C_start(I2C1, SLAVE_ADRESS<<1, I2C_Direction_Transmitter);  
		/* Slavenin PWR_MGMT_1 registerine 1 byte yazacagiz ve böylece MPU-6050 sensörü aktif hale gelecek(uyandirilacak) */
		I2C_write(I2C1, 0x6B);				/* PWR_MGMT_1 registerinin adresi = 0x6B */
		/* PWR_MGMT_1 registerinin tüm bitlerini sifir yaparsak sensör uyaniyor, 0x00 i yaziyoruz */
		I2C_write(I2C1, 0x00);				/* Önce veri yazilacak register adresini yolladik, sonrada yazilacak 1 byte lik veriyi */
		I2C_stop(I2C1);								/* Haberlestirmeyi durdurduk */
		
		
		
		/* Bu ayar sensörün G degerini ayarlamak için 0x1C adresli ACCEL_CONFIG registerine veri yaziyor, 0x00 yollarsak 2g seçilir */
		I2C_start(I2C1, SLAVE_ADRESS<<1, I2C_Direction_Transmitter);
		I2C_write(I2C1, 0x1C);
		I2C_write(I2C1, 0x00);
		I2C_stop(I2C1);
		
		
		
		/* Bu ayar sensörün X ekseni ivme çikisinin High Bitini ilgili register olan ACCEL_XOUT_H dan okumamizi sagliyor */
		/* High bitten kasit [15:8] bitleri */
		I2C_start(I2C1, SLAVE_ADRESS<<1, I2C_Direction_Transmitter);
		I2C_write(I2C1, 0x3B);
		I2C_stop(I2C1);
		I2C_start(I2C1, SLAVE_ADRESS<<1, I2C_Direction_Receiver);
		a = (I2C_read_nack(I2C1));		
		
		/* Bu ayar sensörün X ekseni ivme çikisinin Low Bitini ilgili register olan ACCEL_XOUT_L dan okumamizi sagliyor */
		/* Low bitten kasit [7:0] bitleri */
		I2C_start(I2C1, SLAVE_ADRESS<<1, I2C_Direction_Transmitter);
		I2C_write(I2C1, 0x3C);
		I2C_stop(I2C1);	
		I2C_start(I2C1, SLAVE_ADRESS<<1, I2C_Direction_Receiver);
		b = (I2C_read_nack(I2C1));	




		/* Bu ayar sensörün Y ekseni ivme çikisinin High Bitini ilgili register olan ACCEL_YOUT_H dan okumamizi sagliyor */
		/* High bitten kasit [15:8] bitleri */
		I2C_start(I2C1, SLAVE_ADRESS<<1, I2C_Direction_Transmitter);
		I2C_write(I2C1, 0x3D);
		I2C_stop(I2C1);
		I2C_start(I2C1, SLAVE_ADRESS<<1, I2C_Direction_Receiver);
		a1 = (I2C_read_nack(I2C1));		
		
		/* Bu ayar sensörün Y ekseni ivme çikisinin Low Bitini ilgili register olan ACCEL_YOUT_L dan okumamizi sagliyor */
		/* Low bitten kasit [7:0] bitleri */
		I2C_start(I2C1, SLAVE_ADRESS<<1, I2C_Direction_Transmitter);
		I2C_write(I2C1, 0x3E);
		I2C_stop(I2C1);	
		I2C_start(I2C1, SLAVE_ADRESS<<1, I2C_Direction_Receiver);
		b1 = (I2C_read_nack(I2C1));	


		d = ( (a<<8) | b );								/* X ekseni için a high biti oldugundan 8 sola kaydirip lob bit olan b yi |(veya) operatörüyle arkasina ekleyip 16 bitlik bir data elde edip 16 bitlik d degiskenine atiyoruz */
		d1 = ( (a1<<8) | b1 );						/* Ayni islemi Y ekseni için tekrarliyoruz */

		d=map(d, -17000, 17000, 0, 10);		/* Onluk sistemde bize -17000 ile 17000 arasi deger döndürüyor bunu rahat okumak için 0 ile 10 arasinda deger döndürmesini sagliyoruz */
		d1=map(d, -17000, 17000, 0, 10);	/* Ayni islemi d1 için de yaptik */
		
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
		
		
		
		delay(600000);									/* Örnek amaçli kullanildi, islemciyi sürekli mesgul eden bu tarz delay fonksiyonlari kullanilmamali */
																		/* Bu sürede güç tasarrufu yapilmalidir, RTOS mantigindaki Idle_Thread de kullanilarak yapilabilir */
																		/* Ya da SysTick Timer kullanilabilir */
			
	}
	
	
	
}


 
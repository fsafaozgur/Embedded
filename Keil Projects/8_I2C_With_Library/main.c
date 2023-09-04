#include "stm32f4xx.h"                 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*  I2C ile haberlesen Harici bir sens�r olan MPU6050 ivme �l�er sens�r� kullaniyoruz  */

#define SLAVE_ADRESS 0x68			/* I2C haberlesme protokol� iki hat �zerinden onlarca cihazla haberlesebildigi i�in
																 her Slave (sens�r benzeri �evresel birimler) ile haberlesmede en basta o Slave nin adresi yollanir
																 b�ylece yalnizca haberlesmek istedigimiz sens�rle iletisime ge�mis oluruz */


void I2C_start (I2C_TypeDef* I2Cx, uint8_t adress, uint8_t direction);
void I2C_write(I2C_TypeDef* I2Cx, uint8_t data);
uint8_t I2C_read_ack(I2C_TypeDef* I2Cx);
uint8_t I2C_read_nack(I2C_TypeDef* I2Cx);
void I2C_stop(I2C_TypeDef* I2Cx);
void I2C_Configuration(void);
void USART_Puts (USART_TypeDef* USARTx, volatile char *s);
long map (long x, long in_min, long in_max, long out_min, long out_max);
void Configuration(void);




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
		
		
		i=600000;									/* �rnek ama�li kullanildi, islemciyi s�rekli mesgul eden bu tarz delay fonksiyonlari kullanilmamali */
															/* Bu s�rede g�� tasarrufu yapilmalidir, RTOS mantigindaki Idle_Thread de kullanilarak yapilabilir */
		while(i)									/* Ya da SysTick Timer kullanilabilir */
			i--;
	}
	
	
	
}





void I2C_Configuration()
{
	
	GPIO_InitTypeDef 				GPIO_Structure;
	I2C_InitTypeDef 				I2C_Structure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 									/* SCL ve SDA pinleri i�in clock hatti aktif edildi, sens�r buraya bagli */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);										/* Sens�r I2C1 hattina bagli, bus yolu aktif edildi */
		
	
	/**
   *	SCL ve SDA pin ayarlari
	 *  I2C1 'i iki farkli sekilde baglayabiliriz
	 *  
	 *  1. SCL yi PB6 ya, SDA yi PB7 ye
	 *  2. SCL yi PB8 ya, SDA yi PB9 ye
	 *
	 */
	
	/***************SCL_SDA_CONFIGURATION********************/
	

	GPIO_Structure.GPIO_Mode 			= GPIO_Mode_AF;    																								
	GPIO_Structure.GPIO_OType			= GPIO_OType_OD;						
	GPIO_Structure.GPIO_Pin 			= GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Structure.GPIO_Speed			= GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_PuPd			= GPIO_PuPd_UP;															/* Pull-Up diren� degerlerini degistirerek I2C1 hizini degistirebiliyoruz */
	GPIO_Init(GPIOB, &GPIO_Structure);
	
	
	/****************SPI_AlternatifFunction_CONFIGURATION***************/
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);   								/* SCL */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);										/* SDA */
	
	
	/****************I2C_CONFIGURATION***************/
	
	I2C_Structure.I2C_ClockSpeed				 		= 100000;  												/* Pull-Up direncimiz 10K oldugundan 100kHz de haberlesebiliyoruz(en d�s�k hiz), 
																																							 Pull-Up direncini dahili degilde disardan biz 2k olarak verseydik en y�ksek hiz yani 400 kHz de haberlesebilirdik */	
	I2C_Structure.I2C_Mode									= I2C_Mode_I2C;
	I2C_Structure.I2C_DutyCycle							= I2C_DutyCycle_2;								/* %50 Duty Cycle (standart) */
	I2C_Structure.I2C_OwnAddress1						= 0x00;														/* I2C nin kendi adresi, Slave adresi degil */
	I2C_Structure.I2C_Ack										= I2C_Ack_Disable;								/* Haberlesmeyi baslatirken bunu aktif hale getirecez */
	I2C_Structure.I2C_AcknowledgedAddress		= I2C_AcknowledgedAddress_7bit;		/* 7 veya 10 bit olabilir biz 7 se�tik */
	I2C_Init(I2C1, &I2C_Structure);
	I2C_Cmd(I2C1, ENABLE);																										/* I2C1 birimi aktif */
	
}



void Configuration()
{

	GPIO_InitTypeDef 				GPIO_Structure;
	USART_InitTypeDef 			USART_Structure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);				/* USART2 clock bus i aktif edildi */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 				/* USART2 nin Tx pini PA2 oldugundan A portu clock yolu aktif edildi */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 				/* LED ler i�in clock bus aktif edildi */		

	
	/***********************LED_CONFIGURATION***********************/
	
	GPIO_Structure.GPIO_Mode 			= GPIO_Mode_OUT;    																
	GPIO_Structure.GPIO_OType			= GPIO_OType_PP;		
	GPIO_Structure.GPIO_Pin 			= GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;							
	GPIO_Init(GPIOD, &GPIO_Structure);
	
	
	/****************USART_CONFIGURATION***************/
	
	GPIO_Structure.GPIO_Mode 			= GPIO_Mode_AF;    														      /* USART, Alternate Function modunda kullaniliyor */
	GPIO_Structure.GPIO_OType			= GPIO_OType_PP;		
	GPIO_Structure.GPIO_Pin 			= GPIO_Pin_2;
	GPIO_Structure.GPIO_Speed			= GPIO_Speed_100MHz;
	GPIO_Structure.GPIO_PuPd			= GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_Structure);


	/****************USART_AlternatifFunction_CONFIGURATION***************/
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);													
	

	/***********************USART_CONFIGURATION**************************/


	USART_Structure.USART_BaudRate 							= 115200;															/* 1 saniyede yollanilabilecek bit sayisi, istersek 9600 de yapardik ama hizli olsun istedik */
																																										/* Kablo 8-10 metre gibi uzun olacaksa  hizi d�s�k tutmak gerekli, yoksa g�r�lt� artabilir */
	USART_Structure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;			/* Verinin dogru gidip gitmedigini denetleyen Donanimsal Akis Kontrol� */
	USART_Structure.USART_Mode									= USART_Mode_Tx;											/* USART2 Tx(bit g�nderme) islemi yapacak */
	USART_Structure.USART_Parity								= USART_Parity_No;										/* Kontrol birimi aktif edilmedi */
	USART_Structure.USART_StopBits							= USART_StopBits_1;										/* Yollanilacak bilginin sonuna, karsi tarafin anlamasi i�in ka� bit konulacagini ayarladik */
	USART_Structure.USART_WordLength						= USART_WordLength_8b;								/* G�nderilecek data nin her birinin ka�ar bit olarak yollanacagini ayarladik */
	USART_Init(USART2, &USART_Structure);	
	USART_Cmd(USART2, ENABLE);																												/* USART2 birimi aktif */
	
	
}




 /**
	*
  * /brief					Haberlesme ilk olarak start bitiyle baslar, ilk durumda pin e Pull-Up bagli oldugundan ayni USART gibi bunda da Lojik-0 g�rd�g� anda haberlesme baslar yani ilk bit 0 olmali
  * 								0 bitinden sonra 7 bitlik adres ((2^7)-1= 127 tane adres yani 127 tane Slave cihaz baglanabilir) gelir, sonrada R/W islemini belirten  1 tane bit yollanir (Direction Bit)
  * 								
  */

void I2C_start (I2C_TypeDef* I2Cx, uint8_t adress, uint8_t direction)
{

	/* I2Cx nin mesguliyeti bitene kadar bekle */
	while( I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) ) {}
			
	/* I2C1 e baslangi� biti g�nderilerek	haberlesme basladi (Ilk ayarlarda "I2C_Structure.I2C_Ack" disable olarak ayarlamistik) */
	I2C_GenerateSTART(I2Cx, ENABLE);
	
	/* I2C1 EV5 bekleniyor (Slave nin baslangi� bitimizi anladigini belirten ACK d�nd�rme islemi ger�eklestimi?) */
	while (! I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) {}  	 /* Slave ACK d�nd�r�nce while dan �ikilacak */
	
	/* Slave adresi ve yapacagimiz islem R/W bilgisi yollaniyor */
	I2C_Send7bitAddress(I2Cx, adress, direction);

		
	/**
   *	I2C1 EV6 i�in bekleniyor
   *	Slave'in, Master'in Transmitter ya da Receiver modunda oldugunu anlamasi yani
   *  ACK degeri d�nd�rmesi bekleniyor (direction ile hangisini yolladiysak)
	 *
	 */
		
	
	if ( direction == I2C_Direction_Transmitter)
	{
	while (! I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}  	 /* Slave ACK d�nd�r�nce, yani mode (transmitter_mode) se�imi basariyla yapildiysa while dan �ikilacak ve haberlesme islemi devam edecek */
	}
	else if (direction == I2C_Direction_Receiver)
	{
	while (! I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {}  	 	 /* Slave ACK d�nd�r�nce, yani mode (receiver_mode) se�imi basariyla yapildiysa while dan �ikilacak ve haberlesme islemi devam edecek */
	}
	

}  




 /**
	* /brief					Bu fonksiyon Slave'e veriyi 1 er byte halinde g�nderecek 
  *  								Veri g�ndermek ya da almak i�in de SDA hatti kullaniliyor (SCL yalnizca cihazlarin senkron �alismasi i�in ortak Clock hatti)
  */

void I2C_write(I2C_TypeDef* I2Cx, uint8_t data)
{

	I2C_SendData(I2Cx, data);
	
	/*I2C1 EV8_2 bekleniyor (byte iletildimi kontrol� i�in) */
	while (! I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));							/* Byte'in iletildigine dair Slave'den ACK d�n�nce while d�ng�s�nden �ikilacak */

}



 /**
	* /brief					Bu fonksiyon Slave den 1 byte veri alir
  *  								Daha sonra Slaveye veriyi basariyla aldigina dair ACK yollar 
	*									Ard Arda veri okunacaksa baglantiyi sonlandirmadan okumaya devam etmek i�in kullanilir
  *									Veri g�ndermek ya da almak i�in de SDA hatti kullaniliyor (SCL yalnizca cihazlarin senkron �alismasi i�in ortak Clock hatti)
  */	
	
uint8_t I2C_read_ack(I2C_TypeDef* I2Cx)
{

	/* Gelen data nin anlasilmasi i�in aktif hale getirdik */
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	
	/* 1 byte gelene kadar bekletiyoruz*/
	while (! I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));							

	/* I2C 'nin registerinden datayi okuduk ve 1 byte lik data degiskeninin i�ine yazdik */
	uint8_t data = I2C_ReceiveData(I2Cx);
	return data;
}
	
	


 /**
	* /brief					Bu fonksiyon Slave den 1 byte veri alir
	*									Ard Arda veri okunmayacaksa, baglantiyi sonlandirir ve ardindan veriyi okuyarak return ile d�nd�r�r
  *									
  */	
	
uint8_t I2C_read_nack(I2C_TypeDef* I2Cx)
{

	/* Gelen data nin anlasilmasini pasif hale getirdik */
	I2C_AcknowledgeConfig(I2Cx, DISABLE);
	
	/* Son gelen bitten sonra durdurma biti yollayarak haberlesmeyi kesiyor (tekrar ilk ayarlardaki (I2C_Structure.I2C_Ack) gibi disable hale getirdik)*/
	I2C_GenerateSTOP(I2Cx, ENABLE);
	
	/* 1 byte gelene kadar bekletiyoruz*/
	while (! I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));							

	/* I2C nin registerinden datayi okuyarak ve 1 byte lik data degiskeninin i�ine yazdik */
	uint8_t data = I2C_ReceiveData(I2Cx);
	return data;
}



 /**
	* /brief					Haberlesmeyi durdurur
  *  								
  *									
  */	

void I2C_stop(I2C_TypeDef* I2Cx)
{

  /* I2C1 e STOP konumu g�nderildi yani haberlesmeyi durdur denildi */
	I2C_GenerateSTOP(I2Cx, ENABLE);
	
}



 /**
	* /brief					USART ile veri g�nderir
  *  								
  *									
  */	


void USART_Puts (USART_TypeDef* USARTx, volatile char *s)
{
	while(*s)
	{
		while(USARTx ->SR & 0X00000040) {}				/*  while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET) {} */
		USART_SendData(USARTx, *s);
		s++;
	
	}

}




long map (long x, long in_min, long in_max, long out_min, long out_max)
{
return (x-in_min)*(out_max-out_min)/(in_max-in_min)+out_min;	
}




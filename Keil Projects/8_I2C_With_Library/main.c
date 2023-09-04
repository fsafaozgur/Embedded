#include "stm32f4xx.h"                 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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
		
		
		i=600000;									/* Örnek amaçli kullanildi, islemciyi sürekli mesgul eden bu tarz delay fonksiyonlari kullanilmamali */
															/* Bu sürede güç tasarrufu yapilmalidir, RTOS mantigindaki Idle_Thread de kullanilarak yapilabilir */
		while(i)									/* Ya da SysTick Timer kullanilabilir */
			i--;
	}
	
	
	
}





void I2C_Configuration()
{
	
	GPIO_InitTypeDef 				GPIO_Structure;
	I2C_InitTypeDef 				I2C_Structure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 									/* SCL ve SDA pinleri için clock hatti aktif edildi, sensör buraya bagli */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);										/* Sensör I2C1 hattina bagli, bus yolu aktif edildi */
		
	
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
	GPIO_Structure.GPIO_PuPd			= GPIO_PuPd_UP;															/* Pull-Up direnç degerlerini degistirerek I2C1 hizini degistirebiliyoruz */
	GPIO_Init(GPIOB, &GPIO_Structure);
	
	
	/****************SPI_AlternatifFunction_CONFIGURATION***************/
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);   								/* SCL */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);										/* SDA */
	
	
	/****************I2C_CONFIGURATION***************/
	
	I2C_Structure.I2C_ClockSpeed				 		= 100000;  												/* Pull-Up direncimiz 10K oldugundan 100kHz de haberlesebiliyoruz(en düsük hiz), 
																																							 Pull-Up direncini dahili degilde disardan biz 2k olarak verseydik en yüksek hiz yani 400 kHz de haberlesebilirdik */	
	I2C_Structure.I2C_Mode									= I2C_Mode_I2C;
	I2C_Structure.I2C_DutyCycle							= I2C_DutyCycle_2;								/* %50 Duty Cycle (standart) */
	I2C_Structure.I2C_OwnAddress1						= 0x00;														/* I2C nin kendi adresi, Slave adresi degil */
	I2C_Structure.I2C_Ack										= I2C_Ack_Disable;								/* Haberlesmeyi baslatirken bunu aktif hale getirecez */
	I2C_Structure.I2C_AcknowledgedAddress		= I2C_AcknowledgedAddress_7bit;		/* 7 veya 10 bit olabilir biz 7 seçtik */
	I2C_Init(I2C1, &I2C_Structure);
	I2C_Cmd(I2C1, ENABLE);																										/* I2C1 birimi aktif */
	
}



void Configuration()
{

	GPIO_InitTypeDef 				GPIO_Structure;
	USART_InitTypeDef 			USART_Structure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);				/* USART2 clock bus i aktif edildi */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 				/* USART2 nin Tx pini PA2 oldugundan A portu clock yolu aktif edildi */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 				/* LED ler için clock bus aktif edildi */		

	
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
																																										/* Kablo 8-10 metre gibi uzun olacaksa  hizi düsük tutmak gerekli, yoksa gürültü artabilir */
	USART_Structure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;			/* Verinin dogru gidip gitmedigini denetleyen Donanimsal Akis Kontrolü */
	USART_Structure.USART_Mode									= USART_Mode_Tx;											/* USART2 Tx(bit gönderme) islemi yapacak */
	USART_Structure.USART_Parity								= USART_Parity_No;										/* Kontrol birimi aktif edilmedi */
	USART_Structure.USART_StopBits							= USART_StopBits_1;										/* Yollanilacak bilginin sonuna, karsi tarafin anlamasi için kaç bit konulacagini ayarladik */
	USART_Structure.USART_WordLength						= USART_WordLength_8b;								/* Gönderilecek data nin her birinin kaçar bit olarak yollanacagini ayarladik */
	USART_Init(USART2, &USART_Structure);	
	USART_Cmd(USART2, ENABLE);																												/* USART2 birimi aktif */
	
	
}




 /**
	*
  * /brief					Haberlesme ilk olarak start bitiyle baslar, ilk durumda pin e Pull-Up bagli oldugundan ayni USART gibi bunda da Lojik-0 gördügü anda haberlesme baslar yani ilk bit 0 olmali
  * 								0 bitinden sonra 7 bitlik adres ((2^7)-1= 127 tane adres yani 127 tane Slave cihaz baglanabilir) gelir, sonrada R/W islemini belirten  1 tane bit yollanir (Direction Bit)
  * 								
  */

void I2C_start (I2C_TypeDef* I2Cx, uint8_t adress, uint8_t direction)
{

	/* I2Cx nin mesguliyeti bitene kadar bekle */
	while( I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) ) {}
			
	/* I2C1 e baslangiç biti gönderilerek	haberlesme basladi (Ilk ayarlarda "I2C_Structure.I2C_Ack" disable olarak ayarlamistik) */
	I2C_GenerateSTART(I2Cx, ENABLE);
	
	/* I2C1 EV5 bekleniyor (Slave nin baslangiç bitimizi anladigini belirten ACK döndürme islemi gerçeklestimi?) */
	while (! I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) {}  	 /* Slave ACK döndürünce while dan çikilacak */
	
	/* Slave adresi ve yapacagimiz islem R/W bilgisi yollaniyor */
	I2C_Send7bitAddress(I2Cx, adress, direction);

		
	/**
   *	I2C1 EV6 için bekleniyor
   *	Slave'in, Master'in Transmitter ya da Receiver modunda oldugunu anlamasi yani
   *  ACK degeri döndürmesi bekleniyor (direction ile hangisini yolladiysak)
	 *
	 */
		
	
	if ( direction == I2C_Direction_Transmitter)
	{
	while (! I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}  	 /* Slave ACK döndürünce, yani mode (transmitter_mode) seçimi basariyla yapildiysa while dan çikilacak ve haberlesme islemi devam edecek */
	}
	else if (direction == I2C_Direction_Receiver)
	{
	while (! I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {}  	 	 /* Slave ACK döndürünce, yani mode (receiver_mode) seçimi basariyla yapildiysa while dan çikilacak ve haberlesme islemi devam edecek */
	}
	

}  




 /**
	* /brief					Bu fonksiyon Slave'e veriyi 1 er byte halinde gönderecek 
  *  								Veri göndermek ya da almak için de SDA hatti kullaniliyor (SCL yalnizca cihazlarin senkron çalismasi için ortak Clock hatti)
  */

void I2C_write(I2C_TypeDef* I2Cx, uint8_t data)
{

	I2C_SendData(I2Cx, data);
	
	/*I2C1 EV8_2 bekleniyor (byte iletildimi kontrolü için) */
	while (! I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));							/* Byte'in iletildigine dair Slave'den ACK dönünce while döngüsünden çikilacak */

}



 /**
	* /brief					Bu fonksiyon Slave den 1 byte veri alir
  *  								Daha sonra Slaveye veriyi basariyla aldigina dair ACK yollar 
	*									Ard Arda veri okunacaksa baglantiyi sonlandirmadan okumaya devam etmek için kullanilir
  *									Veri göndermek ya da almak için de SDA hatti kullaniliyor (SCL yalnizca cihazlarin senkron çalismasi için ortak Clock hatti)
  */	
	
uint8_t I2C_read_ack(I2C_TypeDef* I2Cx)
{

	/* Gelen data nin anlasilmasi için aktif hale getirdik */
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	
	/* 1 byte gelene kadar bekletiyoruz*/
	while (! I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));							

	/* I2C 'nin registerinden datayi okuduk ve 1 byte lik data degiskeninin içine yazdik */
	uint8_t data = I2C_ReceiveData(I2Cx);
	return data;
}
	
	


 /**
	* /brief					Bu fonksiyon Slave den 1 byte veri alir
	*									Ard Arda veri okunmayacaksa, baglantiyi sonlandirir ve ardindan veriyi okuyarak return ile döndürür
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

	/* I2C nin registerinden datayi okuyarak ve 1 byte lik data degiskeninin içine yazdik */
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

  /* I2C1 e STOP konumu gönderildi yani haberlesmeyi durdur denildi */
	I2C_GenerateSTOP(I2Cx, ENABLE);
	
}



 /**
	* /brief					USART ile veri gönderir
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




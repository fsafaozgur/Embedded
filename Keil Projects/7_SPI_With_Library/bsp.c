#include "bsp.h"




/**
	*
  * /brief					Peripheral ayarlari (Peripheral configurations)
  * 								
  * 								 
  */
void Configuration() 
{

	GPIO_InitTypeDef 				GPIO_Structure;
	USART_InitTypeDef 			USART_Structure;
	SPI_InitTypeDef 				SPI_Structure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOD, ENABLE); 	/* USART2 i�in A portu (2. pin), SPI i�in A portu (5. 6. ve 7. pinler), SPI in SS pini i�in E portu (3. pin), 
																																																					 LED ler i�in D portu bus yolu aktif edildi */
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);										/* Sens�r SPI1 hattina bagli, bus yolu aktif edildi */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);									/* USART2 nin Tx pini PA2, bus yolu aktif edildi */
	
	
	
		/***********************LED_CONFIGURATION***********************/
	
	GPIO_Structure.GPIO_Mode 			= GPIO_Mode_OUT;    																														
	GPIO_Structure.GPIO_OType			= GPIO_OType_PP;		
	GPIO_Structure.GPIO_Pin 			= GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;							
	GPIO_Init(GPIOE, &GPIO_Structure);
	
		
	
	/***************SCK_MISO_MOSI_CONFIGURATION********************/
	

	GPIO_Structure.GPIO_Mode 			= GPIO_Mode_AF;    													/* Pinler Alternate Function olarak kullanilacak */																				
	GPIO_Structure.GPIO_OType			= GPIO_OType_PP;		
	GPIO_Structure.GPIO_Pin 			= GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;														
	GPIO_Init(GPIOA, &GPIO_Structure);
	
	/**********ChipSelect(Slave_Select)_CONFIGURATION***************/
	
	GPIO_Structure.GPIO_Mode 			= GPIO_Mode_OUT;    												/* Haberlesmeyi baslatmak i�in gerekli pin, yalnizca RESET konumuna �ekmemiz yeterli oldugu i�in AF ye gerek yok */												
	GPIO_Structure.GPIO_OType			= GPIO_OType_PP;		
	GPIO_Structure.GPIO_Pin 			= GPIO_Pin_3;																										
	GPIO_Init(GPIOE, &GPIO_Structure);
	
	
	/****************SPI_ALTERNATE_FUNCTION_CONFIGURATION***************/
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	
	/****************SPI_CONFIGURATION***************/
	
	SPI_Structure.SPI_BaudRatePrescaler 		= SPI_BaudRatePrescaler_2;  								/* Hizi ayarladik */	
	SPI_Structure.SPI_Direction 						= SPI_Direction_2Lines_FullDuplex;  				/* Hem Read hem Write islemi yapacagimiz i�in 2 Line gerekiyor, FullDuplex demek ise ayni anda hem write ve read islemi yapabiliyor olmamiz, zaten 2 farkli hat olmasi bunu ayni anda yapmamizi sagliyor */
	SPI_Structure.SPI_Mode 									= SPI_Mode_Master;    											/* stm32f407VG burada Master oluyor, LIS302DL sens�r� ise Slave */
	SPI_Structure.SPI_DataSize 							= SPI_DataSize_8b;													/* 8 bitlik data haberlesmesi yapacaz */
	SPI_Structure.SPI_FirstBit 							= SPI_FirstBit_MSB;													/* Haberlesmesnin Most Significant Bit ile baslamasini tercih ettik */
	SPI_Structure.SPI_CPOL									= SPI_CPOL_High;														/* SCK'nin y�kselen kenarinda �rnekle */
	SPI_Structure.SPI_CPHA									= SPI_CPHA_2Edge;														/* SCK'nin 2. y�n degisiminde(kenarinda) �rnekle  */
	SPI_Structure.SPI_NSS										= SPI_NSS_Soft | SPI_NSSInternalSoft_Set;		/* SPI in yazilimsal olarak kontrol edilecegini belirttik */
	SPI_Init(SPI1, &SPI_Structure);
	SPI_Cmd(SPI1, ENABLE);																															/* SPI1 aktif */
	
	/****************USART_CONFIGURATION***************/
	
	GPIO_Structure.GPIO_Mode 			= GPIO_Mode_AF;    																		/* USART pini, Alternate Function modunda kullaniliyor */
	GPIO_Structure.GPIO_OType			= GPIO_OType_PP;		 
	GPIO_Structure.GPIO_Pin 			= GPIO_Pin_2;
	GPIO_Structure.GPIO_Speed			= GPIO_Speed_50MHz;
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
	USART_Structure.USART_StopBits							= USART_StopBits_1;										/* Yollanilacak datanin sonuna, karsi tarafin anlamasi i�in ka� bit konulacagini ayarladik */
	USART_Structure.USART_WordLength						= USART_WordLength_8b;								/* G�nderilecek data nin her birinin ka�ar bit olarak yollanacagini ayarladik */
	USART_Init(USART2, &USART_Structure);	
	USART_Cmd(USART2, ENABLE);																												/* USART2 aktif */


}





 /**
	*
  * /brief					Alternate Function Mapping e bakilirsa orada PE3 pininde Slave Select(SS) bacagi bagli 
  * 								Biz PE3 bacagini sifira �ekersek o bacaga SS pini bagli olan sens�r� se�mis oluruz ve haberlesmeye hazir hale gelir 
  * 								Ayni sekilde birden fazla sens�r olsaydi, o sens�rlerin SS bacaklarina Logic-0 verildiginde ilgili sens�r aktif olurdu 
  * 								Ayni anda yalnizca bir sens�r ile haberlesilebileceginden yalnizca bir sens�r�n SS bacagi Logic-0 yapilir 
  * 								Diger sens�rlerin SS pinlerine Logic-1 yollanmalidirki onlar iletisim kanalini dinlemesin ya da veri g�ndermesin 
	*									Bunun sebebi, MOSI(Master Output-Slave Input) ve MISO(Master Input-Slave Output) hatlarinin t�m sens�rlere paralel baglanmasi
	*									Yani Master-Slave arasi hatlar t�m Slave'ler i�in pararlel �ekilmistir, eger Slave Select yapilmazsa g�nderilen veri hepsine gider 
	*									Bu y�zden data g�nderip ya da alacagimiz sens�r neyse onun i�in belirledigimiz SS pinine Lojik-0 yollayarak aktif ederiz
  */
void SPI_Tx(uint8_t adress, uint8_t data)
{

	GPIO_ResetBits(GPIOE, GPIO_Pin_3); 										/* SS biti Lojik-0 konumuna �ekildi */

	/* Deger atamasi yapmak istedigimiz Register (sens�r�n registeri) adresinini yolluyoruz*/
	while (!SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE));				/* SR registerinin TXE biti 0 oldugu (flag kalkmadigi)s�rece DataRegister da yollanacak veri oldugunu anliyoruz, bunu while d�ng�s� ile DataRegister bosalana kadar bekletiyoruz  */
	SPI_SendData(SPI1, adress);														/* SR registerinin TXE biti 1 oldugu anda artik veri yollamaya m�sait demektir ve artik veri yollayabiliriz */
	
	/* Okudugumuz deger �nemsiz, senkron haberlesmenin geregi olarak okuma islemi yapiyoruz */
	while (!SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE));			/* SR registerinin RXNE biti 1 olana kadar yani DataRegister'a sens�rden veri gelene kadar bekliyoruz, bunu while d�ng�s� ile sagliyoruz*/
	SPI_ReceiveData(SPI1);																/* SR registerinin RXNE biti 1 d�nerse artik veri okumaya m�sait demektir ve veriyi okuyoruz */
	
	
	/* Register a yazmak istedigimiz datayi (�rnegin sens�r�n �alismasini istedigimiz G degerini yollayip ilgili registera onu yazmasini sagliyoruz) yolluyoruz */
	while (!SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE));				
	SPI_SendData(SPI1, data);	

	/* Okudugumuz deger �nemsiz, senkron haberlesmenin geregi olarak okuma islemi yapiyoruz */	
	while (!SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE));
	SPI_ReceiveData(SPI1);
	
	
	GPIO_SetBits(GPIOE, GPIO_Pin_3);											 /* Slave cihaz yani sens�rle isimiz bittigi i�in PE3 bacagini Lojik-1 yaparak haberlesmeyi kestik */
	
}







 /**
	*
  * /brief					SPI'dan veri okuma (Reading data from SPI)
  * 								
  * 								 
  */
uint8_t SPI_Rx(uint8_t adress)
{

	GPIO_ResetBits(GPIOE, GPIO_Pin_3); 
	adress = (0x80) | (adress);														/* (0x80) degeri binary olarak 1000 0000 demek oluyor, burada 8. bitin �nemi; SPI haberlesmede 8. bit 0 ise Write modda 1 ise Read modda haberlesiyoruz */
																												/* Biz okuma islemi yapacagimiz i�in cihaza yollayacagimiz datanin basina 1 koyarak okuma yapacagimizi belirtiyoruz */
	
	
	/* Sens�r�n hangi Registerini okumak istiyorusak o Registerin adresini yolluyoruz */
	while (!SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE));				
	SPI_SendData(SPI1, adress);		

	/* Okudugumuz deger �nemsiz, senkron haberlesmenin geregi olarak okuma islemi yapiyoruz */	
	while (!SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE));			
	SPI_ReceiveData(SPI1);																
	
	
	/* �ylesine bir veri yolladigimiz zaman (senkron haberlesme geregi veri okuyacaksak ilk olarak bir veri yollamaliyiz) Sens�r de bize Registerinde (yukarida adresini yolladigimiz) yazan degerini geri d�nd�r�yor */
	while (!SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE));				
	SPI_SendData(SPI1, 0x00);															/* 0x00 �ylesine bir deger, sens�r degerini okumak i�in bunu yollamak zorundayiz ��nk� SPI senkron �alisan bir haberlesme protokol� */
	
	while (!SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE));			/*Sens�rden d�necek veriyi bekliyoruz */		
																
																												/* USART asenkron �alisabildigi i�in veri okumak i�in yollamamiza gerek yoktu, okuma ve yazma senkron olmak zorunda degildi ancak SPI da veri okumak i�in mutlaka birsey yollamayliyiz */
																												/* Benzer sekilde SPI ile veri yolladigimizda karsidan anlamsiz da olsa bir veri g�nderilir ve biz bunu okuruz ancak kullanmayiz*/
	
	GPIO_SetBits(GPIOE, GPIO_Pin_3);											/* Slave cihaz yani sens�rle isimiz bittigi i�in PE3 pinini Lojik-1 yaparak haberlesmeyi kestik */
	return SPI_ReceiveData(SPI1);													/* Sens�rden gelen veriyi okuyoruz ve return ile d�nd�r�yoruz */ 

}



 /**
	*
  * /brief					USART ile data g�nderme (Sending data via USART)
  * 								
  * 								 
  */
void USART_Puts (USART_TypeDef* USARTx, volatile char *s)
{
	while(*s)
	{
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET) {}
		USART_SendData(USARTx, *s);
		s++;
	
	}

}


#include "bsp.h"


 /**
	*
  * /brief					Peripheral ayarlari (Peripheral configurations)
  * 								
  * 								 
  */
void USART_Conf(void)
{

/***************DEFINING_STRUCTRURES************************/	
	
GPIO_InitTypeDef USART_GPIO;
USART_InitTypeDef USART;
NVIC_InitTypeDef NVIC_Structure;



	
/***************GPIO_CONFIGURATION**************************/
	
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);   	//Blok Diyagramindan baktim
USART_GPIO.GPIO_Mode 			= GPIO_Mode_AF;    							//USART, Alternate Function modunda kullaniliyor
USART_GPIO.GPIO_OType			= GPIO_OType_PP;		
USART_GPIO.GPIO_Pin 			= GPIO_Pin_2 | GPIO_Pin_3;			//USART2 A portunun 2(TX) pinini ve 3(RX) pinini se�tik
USART_GPIO.GPIO_PuPd 			= GPIO_PuPd_UP;									//baslangi� biti sifir olacagindan, ilk konum Pull-Up olmali
USART_GPIO.GPIO_Speed 		= GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &USART_GPIO);
	
	
//Alternate Function kullaniminda, I�C, SPI, USART, vb. den hangisi amaciyla kullanacagimizi s�yleyecez

GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);  							//GPIO k�t�phanesinden aldim, Tx bacagi
GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);	 							//Rx bacagi


/***************USART_CONFIGURATION**************************/					//Bu ayarlar(baud_rate, parity, stop_bir, worldlength), veri transferi yapilacak ya da alinacak cihazda da ayni sekilde yapilmali

RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);    							//Blok Diyagramindan baktim

USART.USART_BaudRate 							= 9600;																//1 saniyede yollanilabilecek bit sayisi, istersek 115200 de yapardik 
																																				//Kablo 8-10 metre gibi uzun olacaksa  hizi d�s�k tutmak gerekli, yoksa g�r�lt� artabilir
USART.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;			//Verinin dogru gidip gitmedigini denetleyen Donanimsal Akis Kontrol�
USART.USART_Mode									= USART_Mode_Rx | USART_Mode_Tx;			//USART'i Tx(bit g�nderme) , Rx(bit okuma) islemi yapacak
USART.USART_Parity								= USART_Parity_No;										//Kontrol birimi aktif edilmedi
USART.USART_StopBits							= USART_StopBits_1;										//Yollanilacak bilginin sonuna, karsi tarafin anlamasi i�in ka� bit konulacagini ayarladik
USART.USART_WordLength						= USART_WordLength_8b;								//G�nderilecek data nin her birinin ka�ar bit olarak yollanacagini ayarladik
USART_Init(USART2, &USART);																							//Registerlere gerekli bilgiler girildi
	



USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 												  //Veri geldiginde kesme olusturmasi i�in, "Receive Data register not empty interrupt" aktif ediyoruz
 
 
/***************NVIC_CONFIGURATION**************************/
 
NVIC_Structure.NVIC_IRQChannel 											= USART2_IRQn;			//Hangi interrupt kaynaginin (USART2) kanalini aktif ettigimizi s�yl�yoruz
NVIC_Structure.NVIC_IRQChannelPreemptionPriority 		= 0;								//K���k rakam daha �nceliklidir, biz sifir vererek (var ise) diger t�m kesmelerden �ncelikli hale getirdik
NVIC_Structure.NVIC_IRQChannelSubPriority 					= 0;
NVIC_Structure.NVIC_IRQChannelCmd 									= ENABLE;						//Interrupt i aktif eden ayar
NVIC_Init(&NVIC_Structure);																							//Ayarlari resigtera isledik

NVIC_EnableIRQ(USART2_IRQn);


USART_Cmd(USART2, ENABLE);      											//USART'a veri g�ndermeye hazir olmasi s�ylendi yani aktif hale getirildi, USART k�kt�phanesinden baktim


}






void USART2_IRQHandler(void)
{
 


 while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);					// Okunan data registeri (gelen verinin yazildigi register) bos ise ilgili bit 0 d�ner ve while d�ng�s� i�inde kalinir
  
 
 char gelenVeri = USART_ReceiveData(USART2);														// Data registeri dolu, yani veri geldigi i�in okunan veriyi gelenVeri'ye kaydet
  

 while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);						// Aldigimiz veriyi g�nderecegimiz i�in, g�nderilecek verilerin yazilacagi registerin bos olup olmadigini kontrol ediyoruz
 
	
	/* Alternatif Kullanim
	
	
		USART2->SR resigteri(Status Register) �zerinden s�z konusu RXNE ve TXE bitlerinin set olup olmadigini yani flag kalkmismi diye kontrol edilebilir.
		
		#define USART_FLAG_TXE ((uint16_t)0x0080)	   ifadesini incelersek;
	
		" while(!(USART2->SR & 0x0080u)) " ifadesi aslinda " while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) " fonsiyonunun g�revini yerine getirir.
	
		Ancak okunus anlaminda kolaylik olsun ve DataSheet'e s�rekli bakilmak zorunda kalinmasin diye bu tarz fonksiyonlar kullanilmaktadir.
	
	*/
	
	
	
	USART_SendData(USART2, gelenVeri);																		//Data registeri bos, yani g�nderime hazir oldugu i�in veriyi g�nder
	
}




 /**
	*
  * /brief					USART ile veri g�nderme (Sending data via USART)
  * 								
  * 								 
  */
	void USART2_Send(volatile char* data)
{
	
	while(*data) 
	{
   while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);							//TXE (Transmit Data Register Empty) data registeri bos mu bilgisini tutan bir registerdir. 
   USART_SendData(USART2, *data);																						//Data registeri bos degilse yani hen�z yollanmamis veri varsa, USART_GetFlagStatus() fonksiyonu RESET degeri d�necek ve RESET==RESET sarti dogru oldugundan while d�ng�s�nde takili kalacaktir.  
   data++;																																	//Data Register bos ise, USART_GetFlagStatus() fonksiyonu geriye SET deger d�necek ve SET != RESET oldugundan while d�ng�s�nden cikilacaktir. 
  }																																					
																																						//Flag olarak incelersek; hen�z yollanmamis veri varsa TXE flag i "0" konumundadir, veriler yollanmissa "1" konumundadir
}																																						 





/*Karakter yollamanin bir diger fonksiyon sekli


void USART_Send(volatile char *c)   			//Char dizisinin pointerini buraya arg�man olarak verecegiz
{

	while(*c)
	{
		while(!(USART2 ->SR & 0x00000040));   //USART mod�l�n�n m�sait olup olmadigina bakilir, 
																					//USART m�sait degilse, sonraki g�nderilen data gitmez yokolur, o y�zden ilgili register biti "1" olana kadar while() ile bekleriz
																					//USART2'in SR registerindeki ilgili bit (7. bit) SET oldugu zaman, islemci artik m�saittir ve yeni data yollanabilir
																				
		
		USART_SendData(USART2, *c);						//Bu fonksiyon USART k�t�phanesinden alindi
		c++;																	//USART_SendData() fonksiyonu normalde tek byte data(tek karakter) yollar, biz d�ng�ye sokup birden �ok byte yolluyoruz
	
	}

}
*/




void delay(uint32_t i)									// �rnek ama�li kullanildi, islemciyi s�rekli mesgul eden bu tarz delay fonksiyonlari kullanilmamali 
																				// Bu s�rede g�� tasarrufu yapilmalidir, RTOS mantigindaki Idle_Thread de kullanilarak yapilabilir
{																				// Ya da SysTick Timer kullanilabilir 
while(--i);
}

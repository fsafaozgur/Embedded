#include "stm32f4xx.h"                 



void TIM2_IRQHandler(void);


int main()
{

	
	/**********************STRUCTURES************************/
	
	GPIO_InitTypeDef GPIO_Structure;
	TIM_TimeBaseInitTypeDef TIM_Structure;     //Timer ayarlarini d�zenleyecek yapi olusturuldu
	NVIC_InitTypeDef NVIC_Structure;					 //Interrupt ayarlarini d�zenleyecek yapi olusturuldu
	
	
	/*****************GPIO_CONFIGURATIONS********************/
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);						//Clock bus i aktif edildi
	
	GPIO_Structure.GPIO_Mode			= GPIO_Mode_OUT;
	GPIO_Structure.GPIO_OType			= GPIO_OType_PP;
	GPIO_Structure.GPIO_Pin				= GPIO_Pin_13;
	GPIO_Structure.GPIO_PuPd			= GPIO_PuPd_NOPULL;
	GPIO_Structure.GPIO_Speed			= GPIO_Speed_25MHz;
	
	GPIO_Init(GPIOD, &GPIO_Structure);
	
	
	/****************TIMER_CONFIGURATIONS*******************/
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);							//Clock bus i aktif edildi
	
	TIM_Structure.TIM_Prescaler 				= 8399;											  //Timer 16 bitlikse  0 and 65535 arasinda sayar, prescalara �rnegin 20 dersek her 20 de bir timer counter 1 artar
	TIM_Structure.TIM_CounterMode 			= TIM_CounterMode_Up;					//Timer'in aldigi input miktarini kullanir yani sayici g�revi g�r�r
	TIM_Structure.TIM_Period						= 9999;												//Prescalar degerine g�re timer counterin saydigi deger ka�a ulasinca sifirlansin ve tekrar bastan saymaya baslasin ayarlamasini yaptik
	TIM_Structure.TIM_ClockDivision 		= TIM_CKD_DIV4;								//APB1 bus indan gelen clock sinyalini b�lerek kullanir, 
	TIM_Structure.TIM_RepetitionCounter = 0;													//Timer counter ka� kere basa d�n�nce kesme olusturulsun
	
	/* TIM2 nin bagli oldugu APB1 bus i normalde 42 MHz ile �alissada kendi i�inde yer alan PLL ile Timer i�in bu frekansi �iftler yani 84MHz clock sinyali verir  */
	/* APB2 bus ina bagli bir Timer kullanilsaydi, APB2 default olarak 84 MHz ile �alisir,  ineternal PLL i sayesinde Timer i�in bu frekansi �iftler yani 168MHz clock sinyali verir  */
	/* Kesme Frekansi = (Timer_default_frequency) /  ( (Prescalar + 1) * (Periyod + 1) * (RepetitionCounter + 1) )   */
	/* Kesme Frekansi = (84000000) /  ( (8399 + 1) * (9999 + 1) * (0 + 1) ) = 1 Hz  yani saniyede 1 kez kesme veriyor  */
	
	/* Counterin saymasi olarak bakarsak; Her Sayma I�in Gerekli S�re = (Timer_default_frequency) /  ( (Prescalar + 1)  */
	/* (84000000) /  ( (8399 + 1) yani 1/10000 saniyede 1 kez sayiyor */
	/* Mesela TIM_GetCounter(TIM2) > 2500 oldugunda sunu yap dersen 0.25 saniye ge�tiginde sunu yap demek oluyor */
	/* 4 tane LED i�in ilk sunu yak sonra 2500 e gelince sunu 5000 e gelince sunu 7500 e gelince sunu yak dersen ve 9999 a gelince zaten 0 a geri d�n�p tekrar saymaya baslayacagindan
		 4 tane LED i 0.25 saniye araliklarla karasimsek gibi yakmis oluruz  */
	
	
	/* PWM Frekansi = (Timer_default_frequency) /  ( (Prescalar + 1) * (Periyod + 1) )   */
	
	TIM_TimeBaseInit(TIM2, &TIM_Structure);														//Register'lara ayarlar yazildi
	TIM_Cmd(TIM2, ENABLE);																						//Timer aktif edildi
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);												//Timer'a interrupt yaptirmak istedigimizi belirtiyoruz 
	
	/**************INTERRUPT_CONFIGURATIONS******************/
	
	
	NVIC_Structure.NVIC_IRQChannel 										= TIM2_IRQn;		//Hangi interrupt kaynaginin (TIM2) kaynagini aktif ettigimizi s�yl�yoruz
	NVIC_Structure.NVIC_IRQChannelPreemptionPriority  = 0x00;					//K���k rakam daha �nceliklidir, biz sifir vererek (var ise) diger t�m kesmelerden �ncelikli hale getirdik
	NVIC_Structure.NVIC_IRQChannelSubPriority					= 0x01;
	NVIC_Structure.NVIC_IRQChannelCmd									= ENABLE;				//Interrupt i aktif eden ayar
	
	NVIC_Init(&NVIC_Structure);																				//Ayarlari ilgili registerlara yazdik
	
	/*******************************************************/
	

}



	void TIM2_IRQHandler(void)																	//Bu fonsiyon startup k�t�phanesinden alindi, ismi degisemez
	{
	
		if( TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET )				//Timer in interrupt durumunun ne oldugunu sorduk, eger reset degilse yani interrupt olduysa
		{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);							//Interrupt oldugunu haber veren Flag i tutan TIM_IT_UPDATE registerindeki biti clear (yani Flag i reset ederek) yaparak interrupt durumundan �ikmamizi sagliyor
																															//Artik sistemi yeniden interrupt kabul edebilecek hale getirdik
			
			//TODO																									//Daha sonra yapilacak t�m islemler yazilir, m�mk�nse tek islem yapilmali, fonksiyon kullanimindan ka�inilmali 
																															//illa fonksiyon kullanilacaksa reentrant fonksiyonlar kullanilmali
			
			GPIO_ToggleBits(GPIOD, GPIO_Pin_13);										//Led in durumunu tersledik
			
		
		}
	
	}
	

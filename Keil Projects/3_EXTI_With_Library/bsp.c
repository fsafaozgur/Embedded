#include "bsp.h"




void Configuration(void)
{

	
	/*************DEFINING_STRUCTURE*****************/
	
	GPIO_InitTypeDef GPIO_EXTI_Structure;
	GPIO_InitTypeDef GPIO_LED_Structure;
	EXTI_InitTypeDef EXTI_Structure;
	NVIC_InitTypeDef NVIC_Structure;
	
	
	/**************GPIO_EXTI_CONFIGURATIONS****************/
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);						//A portu i�in clock aktif edildi
	
	GPIO_EXTI_Structure.GPIO_Mode 				= GPIO_Mode_IN;
	GPIO_EXTI_Structure.GPIO_OType 				= GPIO_OType_PP;
	GPIO_EXTI_Structure.GPIO_Pin 					= GPIO_Pin_0;					
	GPIO_EXTI_Structure.GPIO_PuPd					= GPIO_PuPd_DOWN;
	GPIO_EXTI_Structure.GPIO_Speed 				= GPIO_Speed_25MHz;
	
	GPIO_Init(GPIOA, &GPIO_EXTI_Structure);
	
	
		/**************GPIO_LED_CONFIGURATIONS****************/
	
		
	GPIO_LED_Structure.GPIO_Mode 					= GPIO_Mode_OUT;
	GPIO_LED_Structure.GPIO_OType 				= GPIO_OType_PP;
	GPIO_LED_Structure.GPIO_Pin 					= GPIO_Pin_13;					
	GPIO_LED_Structure.GPIO_PuPd					= GPIO_PuPd_NOPULL;
	GPIO_LED_Structure.GPIO_Speed 				= GPIO_Speed_25MHz;
	
	GPIO_Init(GPIOA, &GPIO_LED_Structure);
	
	
	/**************EXTI_CONFIGURATIONS****************/
	
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);						//SYSCFG i�in clock aktif edildi
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);			//Sisteme EXTI_Line0  i�in PA0 pinini kullanacagimizi s�yledik
	
	EXTI_Structure.EXTI_Line 							= EXTI_Line0;								//PA0, EXTI_Line0 hattina baglandi
																																		//Line0 t�m portlarin sadece 0 nolu pinlerinin EXTI hattini a�ar
																																		//Line0 ayni anda iki portta kullanilamaz, mesela PA0 kullaniliyorsa PC0 kullanilamaz
	EXTI_Structure.EXTI_LineCmd 					= ENABLE;										//Kesme aktif edildi
	EXTI_Structure.EXTI_Mode 							= EXTI_Mode_Interrupt;			//Kesme modu se�ildi
	EXTI_Structure.EXTI_Trigger 					= EXTI_Trigger_Rising;			//Y�kselen kenar yani ilk konum olarak Pull-Down direnci ile A0 pinine lojik-0 verilecek, butona basilinca lojik-1 geldigi an y�kselen kenar algilacak ve kesmeye gidecek
	
	EXTI_Init(&EXTI_Structure);																				//Yukaridaki ayarlar ilgili registerlara y�klendi
	
	
	/*************ADDING_IRQ_VECTOR_TO_NVIC**************/
	
	
	NVIC_Structure.NVIC_IRQChannel										= EXTI0_IRQn;			//EXTI0_IRQn vekt�r�ne sahip EXTI_Line0 hatti, PA0 pinine baglandi
	NVIC_Structure.NVIC_IRQChannelCmd									= ENABLE;					//Kesme aktif edildi
	NVIC_Structure.NVIC_IRQChannelPreemptionPriority	= 0x00;						//�ncelik ayari yapildi, EXTI t�r�nden baska kesme olsaysi(TIMI, USART, ADC, ... gibi) �ncelik siralamasi �nemli olurdu ama burda �nemsiz
	NVIC_Structure.NVIC_IRQChannelSubPriority					= 0x00;						//Birden �ok EXTI olsaydi bu siralama �nemli olurdu ama tek EXTI var ve sirasina sifir dedik			
	
	NVIC_Init(&NVIC_Structure);																					//Yukaridaki ayarlar ilgili registerlara y�klendi
																																			// NVIC �nceleri bir peripheral olsa da, daha sonra ARM tarafindan Core seviyeye �ekilmistir, bu sebeple islemcinin kendi DataSheet'inde yer almaktadir. 
	

}


void EXTI0_IRQHandler(void)
{

	if (EXTI_GetFlagStatus(EXTI_Line0) != RESET)
	{
		
		GPIO_ToggleBits(GPIOA, GPIO_Pin_13);
		EXTI_ClearITPendingBit(EXTI_Line0);
	}


}
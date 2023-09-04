#include "stm32f4xx.h"                 


void delay(int32_t time);
void Configuration(void);




int main()
{
	
	
	while (1) 
	{
		
		TIM4->CCR1	= 600;				/* CCR1 registerine atadigimiz degerler belli açilara karsilik geliyor */
		delay(41666);							/* Datasheete bakinca mesela 60 derece dönmesi için 0.17 saniyeye ihtiyaci var  o yüzden PWM  isaretinin degistirmeden önce belli açiya gelmesi beklenmeli */
		TIM4->CCR1	= 1000;				/* 50 Hz demek 1 periyodu 20ms olan dalga demek */
		delay(41666);							/* Mesela 20 ms lik peryotlu dalgada, motorun ilgili uçlarina yalnizca 1 ms lik Lojik-1 uygular geri 19ms Lojik-0 uygularsak motor 0 derece olur. */
		TIM4->CCR1	= 1500;				/* 20ms periyod içersinde uygulanan Lojik-1 süresine Duty Time (Duty Cycle) denir */
		delay(41666);							/* Örnek bir DataSheette söyle diyor; */
		TIM4->CCR1	= 2000;				/* 0.6 ms pulse 0 derece   */
		delay(41666);							/* 0.9 ms pulse 30 derece  */
		TIM4->CCR1	= 2200;				/* 1.2 ms pulse 60 derece  */
		delay(41666);							/* 1.5 ms pulse 90 derece  */
															/* 1.8 ms pulse 120 derece */
															/* 2.1 ms pulse 150 derece */
		
															/* Bizim sayicimiz 20ms yi 20000(TIM_Periyod) a böldügü için 
															mesela CCR1 e atadigimiz 1000 degerine yani counter 1000 e sayana kadar
															Lojik-1 verilerek 1 ms lik bir Lojik-1 sinyali elde edilir
															böylece servo motoru 40 derecelik açiya döndürmüs oluruz */
	
		
		
		
		
	/* Bu ayni zamanda asagidaki gibi de yapilabilirdi */
	
	/*
	 *
	 * TIM_OCStructure.TIM_Pulse= 600;											//CCR1 registerine yazilacak degeri structur a atadik
	 * TIM_OC1Init(TIM4,&TIM_OCStructure);									//CCR1 registerine degeri yazdik, yani Duty Cycle degerini atadik
	 * TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);			//CCR1 de verilen degere göre dalga üretilerek çikisa verildi
	 * delay(41666);
	 * TIM_OCStructure.TIM_Pulse= 1500;											//CCR1 registerine yazilacak degeri structur a atadik
	 * TIM_OC1Init(TIM4,&TIM_OCStructure);									//CCR1 registerine degeri yazdik, yani Duty Cycle degerini atadik
	 * TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);			//CCR1 de verilen degere göre dalga üretilerek çikisa verildi	 *
	 * delay(41666);
	 * ...
	 * ...
	 * ...
	 */
	
	
	}

}





void Configuration() 
{
	
	GPIO_InitTypeDef GPIO_Structure;
	TIM_TimeBaseInitTypeDef TIM_Structure;
	TIM_OCInitTypeDef TIM_OCStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	GPIO_Structure.GPIO_Mode 			= GPIO_Mode_AF;										
	GPIO_Structure.GPIO_OType		 	= GPIO_OType_PP;
	GPIO_Structure.GPIO_Pin 			= GPIO_Pin_12;
	GPIO_Structure.GPIO_PuPd 			= GPIO_PuPd_UP;
	GPIO_Structure.GPIO_Speed 		= GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_Structure);
	
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);				
	

	
	
	
	TIM_Structure.TIM_Prescaler 						= 83;											//84 cycle de 1 kere counterin 1 artmasini saglar
	TIM_Structure.TIM_CounterMode 					= TIM_CounterMode_Up;
	TIM_Structure.TIM_Period 								= 19999;    							//Counteri 0 dan 19999 kadar saydirip basa döndürmemizi saglayan register
	TIM_Structure.TIM_ClockDivision 				= TIM_CKD_DIV1;
	TIM_Structure.TIM_RepetitionCounter 		= 0;
	TIM_TimeBaseInit(TIM4, &TIM_Structure);
	TIM_Cmd(TIM4, ENABLE);
	
	
	/* PWM Frekansi = (Timer_default_frequency) /  ( (Prescalar + 1) * (Periyod + 1) )   */
	/* PWM Frekansi = 84000000 / ( (83 + 1) * (19999 + 1) ) = 50 Hz  */
	/* Servo motorun datasheetindeki çalisma frekansi olan 50 Hz elde edildi. */
	
	
	TIM_OCStructure.TIM_OCMode 							= TIM_OCMode_PWM1;						/* PWM moduna ayarladik, diger modlari görmek için DataSheet'e bak */
	TIM_OCStructure.TIM_OutputState 				= TIM_OutputState_Enable;
	TIM_OCStructure.TIM_Pulse								= 0;													/* CCR1 registerine ilk deger atadik, yani Duty Cycle degerini atadik */
																																				/* Mesela buraya 9999 yazsak %50 Duty Cycle ile PWM isareti olustururduk */
	TIM_OCStructure.TIM_OCPolarity					= TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM4,&TIM_OCStructure);																	  /* Channel 1 aktif edildi	*/			
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
	
		
		// Duty cycle hesabi:
    // TIM_Pulse = (((TIM_Period + 1) * duty_cycle) / 100) - 1
    // Ex. 25% duty cycle:
    //     TIM_Pulse = (((19999 + 1) * 25) / 100) - 1 = 4999
    //     TIM_Pulse = (((19999 + 1) * 50) / 100) - 1 = 9999
 

}






/* Asagidaki kod 4 cycle den olusuyor, 1 cycle 6 nanosaniye dersek time=41666 yazarsak 1 ms elde ederiz */
/* Ancak bu delay mantigi yanlistir, islemciyi sürekli mesgul eder ve enerji kaybina sebep olur */
/* Bu sürede güç tasarrufu yapilmalidir, RTOS mantigindaki Idle_Thread de kullanilarak yapilabilir*/
/* Ya da SysTick Timer kullanilabilir */
void delay(int32_t time) {

while (time--) {}


}

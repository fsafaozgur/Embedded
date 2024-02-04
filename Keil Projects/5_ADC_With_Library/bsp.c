#include "bsp.h"



/**
	* brief					Tüm GPIO ve ADC ayarlari (All GPIO and ADC Configurations)
	*								
	*/
void Configuration(void) {

	ADC_InitTypeDef 				ADC_Structure;
	ADC_CommonInitTypeDef 	ADC_CommonStructure;
	GPIO_InitTypeDef 				GPIO_Structure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);				/* LED pinleri için clock aktif */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);				/* ADC pini için clock aktif */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);				/* ADC clock aktif*/

	
	/*******************LED_CONFIGURATION******************/
	
	GPIO_Structure.GPIO_Mode 			= GPIO_Mode_OUT;
	GPIO_Structure.GPIO_OType 		= GPIO_OType_PP;
	GPIO_Structure.GPIO_Pin 			= GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Structure.GPIO_PuPd 			= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_Structure);
	
	/*******************ADC_PIN_CONFIGURATION******************/
	
	
	GPIO_Structure.GPIO_Mode 			= GPIO_Mode_AN;
	GPIO_Structure.GPIO_OType 		= GPIO_OType_PP;
	GPIO_Structure.GPIO_Pin 			= GPIO_Pin_0;
	GPIO_Structure.GPIO_PuPd 			= GPIO_PuPd_NOPULL;    							/* Analog sinyal alinacak elemanin direnci yüksekse, daha yüksek olup veri bozulmamasi için NOPULL yaptik */ 
	GPIO_Init(GPIOA, &GPIO_Structure);
	
	
	/*******************ADC_COMMON_CONFIGURATION******************/
	
	
	ADC_CommonStructure.ADC_Mode 					= ADC_Mode_Independent;			
	ADC_CommonStructure.ADC_Prescaler 		= ADC_Prescaler_Div4;				/* Clock sinyalini bölerek kullaniyoruz */
	ADC_CommonInit(&ADC_CommonStructure);
	
	/*******************ADC_CONFIGURATION******************/
	
	ADC_Structure.ADC_Resolution 					= ADC_Resolution_8b;				/* Analog sinyal,8 bit yani 256 parçaya bölündü */
	ADC_Init(ADC1, &ADC_Structure);
	
	ADC_Cmd(ADC1, ENABLE);


}





/**
  * brief					ADC modülü ile verileri okuma (Reading data with ADC module)
  *								
  */
uint8_t Read_ADC_Data(void)
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_56Cycles);		/* Analog sinyalin hesaplanmasini kaç cycle de yapilacagini söylüyor */
																																								/* Cycle sayisi düstükçe islem hizlanir ama harcanan enerji artar */

	ADC_SoftwareStartConv(ADC1);																									/* ADC islemini baslattik */
	
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET) {}											/* End Of Conversion Flag'i bekliyoruz, çevrim tamamlaninca flag kalkacak ve while döngüsünden çikilacak */ 
																																								/* böylece 56 cycle bitene kadar return satirina geçmesini engellemis oluyoruz*/
	return ADC_GetConversionValue(ADC1);
}		


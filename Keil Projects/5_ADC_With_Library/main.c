#include "stm32f4xx.h"                  


uint8_t Read_ADC_Data(void);
void Configuration(void);																																						



int main()
{

	
	uint8_t adc_data;
	
	Configuration();
	
	
	/**
	  * brief					ADC pin gerilim degerine g�re ledleri yakma (Blink Leds according to ADC pin voltage value)
	  *						  	
	  */
	
	while (1)
	{
		adc_data = Read_ADC_Data();					/* En fazla 255 degeri d�necek (8-bit) */
	
		if (adc_data < 100) {
		
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);
		GPIO_ResetBits(GPIOD, GPIO_Pin_14);
		GPIO_ResetBits(GPIOD, GPIO_Pin_15);
		
		} else if (adc_data >99 && adc_data < 200) {
		
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
		GPIO_ResetBits(GPIOD, GPIO_Pin_15);
		
		} else {
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
		}
		
		
	}
	
	/* Kodlar �rnek olarak yazilmistir, bu tarz s�reklilik arz eden (sens�rden s�rekli veri okuma gibi) islemleri DMA ile yapmak islemciyi mesgul etmemek a�isindan �nemlidir */ 
	/* Yani ADC ile alinan degeri g�steren register adresini (SourceAdress) kullanarak , DMA yardimi ile adc_data degiskeninin adresini (DestinationAdress) de kullanarak veriyi direk olarak adc_data i�erisine yazabilirdik*/ 
}



/**
	* brief					T�m GPIO ve ADC ayarlari (All GPIO and ADC Configurations)
	*								
	*/


void Configuration(void) {

	ADC_InitTypeDef 				ADC_Structure;
	ADC_CommonInitTypeDef 	ADC_CommonStructure;
	GPIO_InitTypeDef 				GPIO_Structure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);				/* LED pinleri i�in clock aktif */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);				/* ADC pini i�in clock aktif */
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
	GPIO_Structure.GPIO_PuPd 			= GPIO_PuPd_NOPULL;    							/* Analog sinyal alinacak elemanin direnci y�ksekse, daha y�ksek olup veri bozulmamasi i�in NOPULL yaptik */ 
	GPIO_Init(GPIOA, &GPIO_Structure);
	
	
	/*******************ADC_COMMON_CONFIGURATION******************/
	
	
	ADC_CommonStructure.ADC_Mode 					= ADC_Mode_Independent;			
	ADC_CommonStructure.ADC_Prescaler 		= ADC_Prescaler_Div4;				/* Clock sinyalini b�lerek kullaniyoruz */
	ADC_CommonInit(&ADC_CommonStructure);
	
	/*******************ADC_CONFIGURATION******************/
	
	ADC_Structure.ADC_Resolution 					= ADC_Resolution_8b;				/* Analog sinyal,8 bit yani 256 par�aya b�l�nd� */
	ADC_Init(ADC1, &ADC_Structure);
	
	ADC_Cmd(ADC1, ENABLE);


}





/**
  * brief					ADC mod�l� ile verileri okuma (Reading data with ADC module)
  *								
  */


uint8_t Read_ADC_Data(void)
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_56Cycles);		/* Analog sinyalin hesaplanmasini ka� cycle de yapilacagini s�yl�yor */
																																								/* Cycle sayisi d�st�k�e islem hizlanir ama harcanan enerji artar */

	ADC_SoftwareStartConv(ADC1);																									/* ADC islemini baslattik */
	
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET) {}											/* End Of Conversion Flag'i bekliyoruz, �evrim tamamlaninca flag kalkacak ve while d�ng�s�nden �ikilacak */ 
																																								/* b�ylece 56 cycle bitene kadar return satirina ge�mesini engellemis oluyoruz*/
	return ADC_GetConversionValue(ADC1);
}		


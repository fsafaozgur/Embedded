#include "bsp.h"



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




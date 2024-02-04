#include "bsp.h"                 



int main()
{
	
	
	while (1) 
	{
		
		TIM4->CCR1	= 600;				/* CCR1 registerine atadigimiz degerler belli a�ilara karsilik geliyor */
		delay(41666);							/* Datasheete bakinca mesela 60 derece d�nmesi i�in 0.17 saniyeye ihtiyaci var  o y�zden PWM  isaretinin degistirmeden �nce belli a�iya gelmesi beklenmeli */
		TIM4->CCR1	= 1000;				/* 50 Hz demek 1 periyodu 20ms olan dalga demek */
		delay(41666);							/* Mesela 20 ms lik peryotlu dalgada, motorun ilgili u�larina yalnizca 1 ms lik Lojik-1 uygular geri 19ms Lojik-0 uygularsak motor 0 derece olur. */
		TIM4->CCR1	= 1500;				/* 20ms periyod i�ersinde uygulanan Lojik-1 s�resine Duty Time (Duty Cycle) denir */
		delay(41666);							/* �rnek bir DataSheette s�yle diyor; */
		TIM4->CCR1	= 2000;				/* 0.6 ms pulse 0 derece   */
		delay(41666);							/* 0.9 ms pulse 30 derece  */
		TIM4->CCR1	= 2200;				/* 1.2 ms pulse 60 derece  */
		delay(41666);							/* 1.5 ms pulse 90 derece  */
															/* 1.8 ms pulse 120 derece */
															/* 2.1 ms pulse 150 derece */
		
															/* Bizim sayicimiz 20ms yi 20000(TIM_Periyod) a b�ld�g� i�in 
															mesela CCR1 e atadigimiz 1000 degerine yani counter 1000 e sayana kadar
															Lojik-1 verilerek 1 ms lik bir Lojik-1 sinyali elde edilir
															b�ylece servo motoru 40 derecelik a�iya d�nd�rm�s oluruz */
	
		
		
		
		
	/* Bu ayni zamanda asagidaki gibi de yapilabilirdi */
	
	/*
	 *
	 * TIM_OCStructure.TIM_Pulse= 600;											//CCR1 registerine yazilacak degeri structur a atadik
	 * TIM_OC1Init(TIM4,&TIM_OCStructure);									//CCR1 registerine degeri yazdik, yani Duty Cycle degerini atadik
	 * TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);			//CCR1 de verilen degere g�re dalga �retilerek �ikisa verildi
	 * delay(41666);
	 * TIM_OCStructure.TIM_Pulse= 1500;											//CCR1 registerine yazilacak degeri structur a atadik
	 * TIM_OC1Init(TIM4,&TIM_OCStructure);									//CCR1 registerine degeri yazdik, yani Duty Cycle degerini atadik
	 * TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);			//CCR1 de verilen degere g�re dalga �retilerek �ikisa verildi	 *
	 * delay(41666);
	 * ...
	 * ...
	 * ...
	 */
	
	
	}

}






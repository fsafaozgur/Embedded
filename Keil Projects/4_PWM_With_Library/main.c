#include "bsp.h"                 



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






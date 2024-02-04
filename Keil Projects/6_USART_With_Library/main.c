#include "bsp.h"





char str[50]="fatih safa ozgur";	      //Eger bu olmazsa sprintf(str, "fatih safa ozgur") seklinde yapilacak
																				//Kütüphane olarak <stdio.h> kütüphanesi eklenecek



int main ()															//PC tarafi USB olan entegrenin, TTL tarafinin Rx bacagini bizim Tx bacagimiz olan A2 portuna, Tx bacagini da bizim Rx bacagimiz olan A3 portuna baglamamiz gerek
{
	
	USART_Conf();

	while (1)
	{
		USART2_Send(str);
		delay(1000000);													//USART2 her müsait oldugunda tekrar tekrar bu veri yollanirsa mavi ekran verme durumu olabilecegi için küçük bir gecikme konuldu
										
	}
										
	return 0;
}




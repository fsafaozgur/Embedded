#include "stm32f4xx.h"

void delay(int a);


int main ()
{


	RCC->AHB1ENR |= 0x0000001F;      // C portunun clock bus yolunu açtik 
	GPIOC->MODER |= 0x55550000;			 // 1010 1010 0000 0000 0000 0000 0000 0000  /----/  4 pini "Output Mode" yaptik
	GPIOC->OTYPER |= 0x0;						 // Default olarak "Pull-Push" biraktik
	GPIOC->OSPEEDR |=0x0;						 // Hizi default biraktik
	GPIOC->PUPDR |= 0x0;						 // "None_Pullup_PullDown" yani default biraktik
	
	
	while(1) {
		GPIOC->ODR |= 0xF000;  					 // 0xb1111 0000 0000 0000   // 12, 13, 14, 15 pinlerini enerjilendir(SET konumuna getir)
		delay(1000000);
		GPIOC->ODR &= ~0xF000;  			   // 0xb0000 0000 0000 0000   // 12, 13, 14, 15 pinlerini RESET konumuna getir
	}

	
	return 0;
}



void delay(int a)
{
	while (--a);
}
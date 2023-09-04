#include "config.h"


#define LED_ON() GPIO_SetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
#define LED_OFF() GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
#define Button_Pressed GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)==1

void delay(uint16_t time);


int main()
{

	LED_Init();
	Button_Init();
	
	while(1)
	{
	
		if ( Button_Pressed )
		{
			LED_ON();	
		}
		else
		{
			LED_OFF();
		}	
	}

}



void delay(uint16_t time)							/* Örnek amaçli kullanildi, islemciyi sürekli mesgul eden bu tarz delay fonksiyonlari kullanilmamali */
{																			/* Bu sürede güç tasarrufu yapilmalidir, RTOS mantigindaki Idle_Thread de kullanilarak yapilabilir */
	time *= 1000;												/* Ya da SysTick Timer kullanilabilir */
	while(--time);
}






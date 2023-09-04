#include "config.h"


void LED_Init()
{
GPIO_InitTypeDef LED;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

LED.GPIO_Mode = GPIO_Mode_OUT;
LED.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
LED.GPIO_OType = GPIO_OType_PP;
LED.GPIO_Speed = GPIO_Speed_2MHz;
LED.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	
GPIO_Init(GPIOD, &LED);
	
}


void Button_Init()
{

GPIO_InitTypeDef Button;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	
Button.GPIO_Mode = GPIO_Mode_IN;
Button.GPIO_Pin = GPIO_Pin_2;
Button.GPIO_OType = GPIO_OType_PP;
Button.GPIO_Speed = GPIO_Speed_2MHz;
Button.GPIO_PuPd = GPIO_PuPd_DOWN;
	
GPIO_Init(GPIOA, &Button);
	
}



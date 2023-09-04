#include "config.h"


#define LED_ON() GPIO_SetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
#define LED_OFF() GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
#define Button_Pressed GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)==1
void delay(uint16_t time);

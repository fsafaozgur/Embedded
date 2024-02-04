#include "bsp.h"



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










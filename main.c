/*
 * main.c
 *
 * Runs button flash
 *
 * Created: 2/15/2018 2:10:36 PM
 * Author : meloanmm
 */ 

#include "ButtonFlash.h"

int main(void)
{
	init_ports();
	char light_state = OFF;
		
	while (1)
	{	
		switch(light_state) {
			case OFF:
				light_state = doOff();
				break;
			case ROTATE:
				light_state = doRotate();
				break;
			case FLASH:
				light_state = doFlash();
				break;
			default:
				break;
		}
	}
}

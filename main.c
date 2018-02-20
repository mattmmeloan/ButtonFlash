/*
 * main.c
 *
 * Runs button flash
 *
 * Created: 2/15/2018 2:10:36 PM
 * Author : meloanmm
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "ButtonFlash.h"


int main(void)
{
	init_ports();
	
	/* state transition table - state_table[i][j] is the
	 * next state after state i when button j is pressed.
	 */
	unsigned char state_table[3][2] = {
		{ ROTATE,	FLASH },    // OFF state
		{ OFF,		FLASH },    // ROTATE state
		{ ROTATE,	OFF   }     // FLASH state
	};
	
	unsigned char light_state = OFF;
	
	// both buttons start unpressed
	unsigned char b0_state = 1;
	unsigned char b1_state = 1;
	
	
	
	while (1)
	{
		_delay_ms(4);
		
		
		if (PINF & B0_PIN) {
			//button 0 is unpressed
			if (!b0_state) {
				//button was previously pressed, so change state
				light_state = state_table[light_state][B0];
				b0_state = 1;
			} 
			
		} else {
			//button 0 is pressed
			b0_state = 0;
		}
		
		if (PINF & B1_PIN) {
			//button 1 is unpressed
			if (!b1_state) {
				//button was previously pressed, so change state
				light_state = state_table[light_state][B1];
				b1_state = 1;
			}
			
		} else {
			//button 1 is pressed
			b1_state = 0;
		}
		
		
		switch(light_state) {
			default:
			case OFF:
				doOff();
				break;
			case ROTATE:
				doRotate();
				break;
			case FLASH:
				doFlash();
				break;
		}
	}
}


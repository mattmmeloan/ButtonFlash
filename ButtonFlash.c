/*
 * ButtonFlash.c
 *
 * Created: 2/15/2018 3:02:29 PM
 *  Author: meloanmm
 */ 

#include <util/delay.h>
#include <avr/io.h>
#include "ButtonFlash.h"

/* state transition table - state_table[i][j] is the
 * next state after state i when button j is pressed.
 */
unsigned char state_table[3][2] = {
	{ ROTATE,	FLASH },    // OFF state
	{ OFF,		FLASH },    // ROTATE state
	{ ROTATE,	OFF   }     // FLASH state
};

char doOff() {
	char state;
	
	while (1) {
		//all lights off
		PORTF &= 0xF0;
	
		state = sample_buttons(OFF, ~(1 << 15));
		if (state != OFF) return state;
	}
}

char doRotate() {
	char state;
	
	while (1) {
		//all lights off
		PORTF &= 0xF0;

		// turn each light on in succession
		for (char led = PF0; led < PF4; led++) {
			PORTF |= (1 << led);	// turn on current LED
			state = sample_buttons(ROTATE, 1000);
			if (state != ROTATE) return state;
			PORTF &= ~(1 << led);	// turn off current LED
		}
	}
}

char doFlash() {
	char state;
	
	while (1) {
		for (int i = 0; i < 4; i++) {
			// turn on all LEDs for 200 ms
			PORTF |= 0x0F;
			state = sample_buttons(FLASH, 200);
			if (state != FLASH) return state;
		
			// turn off all LEDs for 1 second
			PORTF &= 0xF0;
			state = sample_buttons(FLASH, 1000);
			if (state != FLASH) return state;
		}
	}
}

void init_ports(void) {
	// configure button pins for input with pull-up enabled
	DDRF &= 0x3F;
	PORTF |= 0xC0;
	
	// configure led output pins
	DDRF |= 0x0F;
}

char sample_buttons(char curr_state, int ms) {
	static int b0 = 1;
	static int b1 = 1;
	int old_b0, old_b1, delayed = 0;

	// delay 4ms before sampling buttons
	while (delayed < ms) {
		old_b0 = b0;
		old_b1 = b1;
		
		_delay_ms(4);
		delayed += 4;
		
		b0 = PINF & B0_PIN;
		b1 = PINF & B1_PIN;
		
		// change state if button has been released
		if (!old_b0 && b0)
			return state_table[(int) curr_state][B0];
		if (!old_b1 && b1)
			return state_table[(int) curr_state][B1];
	}

	return curr_state;
}

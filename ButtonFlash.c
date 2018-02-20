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
	char b0_state, b1_state, pushed_pin;
	int pushed_button, delayed = 0;
	
	// wait until a button is pushed or the specified number of ms elapses
	while ((b0_state = PINF & B0_PIN) && (b1_state = PINF & B1_PIN)) {
		delayed++;
		_delay_ms(1);
		if (delayed == ms) return curr_state;
	}
	
	// get pin for button that was pushed
	if (!b0_state) {
		pushed_pin = B0_PIN;
		pushed_button = B0;
	}
	else {
		pushed_pin = B1_PIN;
		pushed_button = B1;
	}
	
	// delay until button is released
	do {
		_delay_ms(4);
		delayed += 4;
		if (delayed >= ms) return curr_state;
	} while (!(PINF & pushed_pin));
	
	return state_table[(int) curr_state][pushed_button];
}

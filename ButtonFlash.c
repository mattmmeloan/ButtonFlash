/*
 * ButtonFlash.c
 *
 * Created: 2/15/2018 3:02:29 PM
 *  Author: meloanmm
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "ButtonFlash.h"

/* state transition table - state_table[i][j] is the
 * next state after state i when button j is pressed.
 */
unsigned char state_table[3][2] = {
	{ ROTATE,	FLASH },    // OFF state
	{ OFF,		FLASH },    // ROTATE state
	{ ROTATE,	OFF   }     // FLASH state
};

volatile int button0pressed;
volatile int button1pressed;

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
	DDRK &= 0xFC;
	PORTK |= 0x03;
	
	// enable interrupts
	sei();
	
	// enable interrupts for pins A8 and A9
	PCICR |= 0x04;
	PCMSK2 |= 0x03;
	
	// configure led output pins
	DDRF |= 0x0F;
}

char sample_buttons(char curr_state, int ms) {
	int delayed = 0;

	while (delayed < ms) {
		// change state if button has been released
		if (button0pressed) {
			button0pressed = 0;
			return state_table[(int) curr_state][B0];
		}
		if (button1pressed) {
			button1pressed = 0;
			return state_table[(int) curr_state][B1];
		}
		
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			_delay_ms(4);
			delayed += 4;	
		}
	}

	return curr_state;
}


ISR (PCINT2_vect) {
	static char b0 = 1;
	static char b1 = 1;
	
	char old_b0 = b0;
	char old_b1 = b1;
	
	b0 = PINK & B0_PIN;
	b1 = PINK & B1_PIN;
	
	// change state if button has been released
	if (!old_b0 && b0)
		button0pressed = 1;
	if (!old_b1 && b1)
		button1pressed = 1;
}


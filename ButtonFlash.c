/*
 * ButtonFlash.c
 *
 * Created: 2/15/2018 3:02:29 PM
 *  Author: meloanmm
 */ 

#include <util/delay.h>
#include <avr/io.h>
#include "ButtonFlash.h"

void doOff() {
	//all lights off
	PORTF &= 0xF0;
}

void doRotate() {
	//all lights off
	PORTF &= 0xF0;

	// turn each light on in succession
	for (char led = PF0; led < PF4; led++) {
		PORTF |= (1 << led);	// turn on current LED
		_delay_ms(1000);
		PORTF &= ~(1 << led);	// turn off current LED
	}
}

void doFlash() {
	for (int i = 0; i < 4; i++) {
		PORTF |= 0x40;	// turn on every LED
		_delay_ms(200);
		
		PORTF &= 0x40;	// turn off every LED
		_delay_ms(1000);
	}
}

void init_ports(void) {
	// configure button pins for input with pull-up enabled
	DDRF &= 0x3F;
	PORTF |= 0xC0;
	
	// configure led output pins
	DDRF |= 0x0F;
}


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
	
}

void doRotate() {
	//all lights off
	
	
	//repeat for each light {
	//	light on
	//	delay 100
	//	light off
	//}
}

void doFlash() {
	// all on
	// delay 200
	// all off 
	// delay 1000
}

void init_ports(void) {
	// configure button pins for input with pull-up enabled
	DDRF &= 0x3F;
	PORTF |= 0xC0;
	
	// configure led output pins
	DDRF |= 0x0F;
}


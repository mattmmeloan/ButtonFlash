/*
 * ButtonFlash.h
 *
 * Created: 2/15/2018 3:02:45 PM
 *  Author: meloanmm
 */ 


#ifndef BUTTONFLASH_H_
#define BUTTONFLASH_H_

/* transition table row indices corresponding to LED states */
#define OFF	0
#define ROTATE 1
#define FLASH  2

/* transition table column indices corresponding to buttons 0 and 1 */
#define B0 	0
#define B1 	1

/* pin masks for button 0 and button 1 */
#define B0_PIN (1 << PF6)
#define B1_PIN (1 << PF7)

/* pin masks for LEDs 0 through 3 */
#define LED_0 (1 << PF0)
#define LED_1 (1 << PF1)
#define LED_2 (1 << PF2)
#define LED_3 (1 << PF3)


char doOff();
char doRotate();
char doFlash();
void init_ports(void);
char sample_buttons(char curr_state, int ms);



#endif /* BUTTONFLASH_H_ */
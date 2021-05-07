/* Author: Karan Bhogal
 * Lab Section: 21
 * Assignment: Lab #8  Exercise #1
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code is my own original work.
 *
 *  Demo Link: https://drive.google.com/file/d/1xJacBLv2Ws8OBmvVZ0gLZyQz-ggSFy6X/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency){
		if (!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }

		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) { OCR3A = 0x0000; }

		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on(){
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}

void PWM_off(){
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}
/*
enum LA_State {Wait, S1, S2, S3} State;

void LED_Latch ()
{
	unsigned char num = ~PINA;
	
	switch (State){
	   case Wait:
		 if ( num == 0x01 ){
			State = S1;
		 }
		 else if ( num == 0x02 ){
			State = S2;
		 }
		 else if ( num == 0x04 ){
			State = S3;
		 }
		 else{
			State = Wait;
		 }
		 break;

	   case S1:
		 if ( num == 0x01 ){
			State = S1;
		 }
		 else{
			State = Wait;
		 }
		 break;

	   case S2:
		if ( num == 0x02 ){
			State = S2;
		}
		else {
			State = Wait;
		}
		break;
	
	   case S3:
		if ( num == 0x04 ){
			State = S3;
		}
		else {
			State = Wait;
		}
		break;
	
	   default:
		State = Wait;
		break;
	}

	switch (State){

	   case Wait:
		set_PWM(0);
		break;

	   case S1:
		set_PWM(261.63);
		break;

	   case S2:
		set_PWM(293.66);
		break;

	   case S3:
		set_PWM(329.66);
		break;

	   default:
		break;
	}
}

*/

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
	PWM_on();
	unsigned char check;
    while (1) {

//	LED_Latch(); 
	check = (~PINA) & 0x07;
	
	if ( check == 0x01 ){
		set_PWM(261.63);
	}	
	else if ( check == 0x02 ){
		set_PWM(293.66);
	}
	else if ( check == 0x04 ){
		set_PWM(329.66);
	}
	else {
		set_PWM(0);
	}
	
    }
    return 1;
}

/* Author: Karan Bhogal
 * Lab Section: 21
 * Assignment: Lab #8  Exercise #2
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code is my own original work.
 *
 *  Demo Link: https://drive.google.com/file/d/17b42RlzYSIu83QN3QZzT5CiE93alasnq/view?usp=sharing
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

unsigned int sc = 1;

enum LA_State {Wait, Inc, Dec, Wait2 } State;

void LED_Latch()
{
	unsigned char num = ~PINA;
	
	switch (State){
	   case Wait:
		
		if ( num == 0x02 ){
			State = Dec;
		 }
		 else if ( num == 0x04 ){
			State = Inc;
		 }
		 else{
			State = Wait;
		 }
		 break;

	   case Dec:
		if (num == 0x02){
			State = Wait2;
		}
		else{
			State = Dec;
		}
		break;
	
	   case Inc:
		if (num == 0x04){
			State = Wait2;
		}
		else{
			State = Inc;
		}
		break;

	case Wait2:
		if (num == 0x04){
			State = Wait2;
		}
		else if (num == 0x02){
			State = Wait2;
		}
		else{
			State = Wait;
		}
		break;
	
	   default:
		State = Wait;
		break;
	}

	switch (State){

	   case Wait:
		break;

	   case Dec:
		if (sc > 1){
			sc = sc - 1;
		}
		else {
			sc = 1;
		}
		break;

	   case Inc:
		if (sc < 8){
			sc = sc + 1;
		}
		else{
			sc = 8;
		}
		break;

	   default:
		break;
	}
}

unsigned char loc = 1;

enum LA_States { On, Hold, Off} States;

void LED()
{

	unsigned char tmpA = ~PINA;

	switch (States){
		case On:
		 loc = 1;
		 if (tmpA == 0x01){
			States = Hold;
		 } 
		 else{
			States = On;
		 }
		 break;
		 
		case Hold:
		 if (loc == 1){
			States = Off;
		 }
		 else {
			States = On;
		 }
		break;

		case Off:
		 loc = 0;
		 if (tmpA == 0x01){
			States = Hold;
		 }
		 else{
			States = Off;
		 }
		 break;

		default:
		 States = On;
		 break;
	}

	switch(States){
		case On:
		 if (sc == 1){
			set_PWM(261.63);
		 }
		 else if (sc == 2){
			set_PWM(293.66);
		 }
		 else if (sc == 3){
			set_PWM(329.63);
		 }
		 else if (sc == 4){
			set_PWM(349.23);
		 }
		 else if (sc == 5){
                        set_PWM(392.00);
                 }
                 else if (sc == 6){
                        set_PWM(440.00);
                 }
                 else if (sc == 7){
                        set_PWM(493.88);
                 }
		 else if (sc == 8){
			set_PWM(523.25);
		 }
		 break;

		case Off:
		 set_PWM(0);
		 break;

		default:
		 set_PWM(261.63);
		 break;
	}

}



int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
	PWM_on();
	
	State = Wait;
	States = On;

    while (1) {

	LED();
	LED_Latch(); 
	
	
	
    }
    return 1;
}

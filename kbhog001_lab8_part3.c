/* Author: Karan Bhogal
 * Lab Section: 21
 * Assignment: Lab #8  Exercise #2
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code is my own original work.
 *
 *  Demo Link:
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;

unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;

	_avr_timer_cntcurr = _avr_timer_M;

	SREG |= 0x80;
}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

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

double ar[11] = { 293.66, 349.23, 293.66, 261.63, 293.66, 349.23, 392.00, 329.63, 293.66, 392.00, 349.23};
unsigned int i = 0;
enum LA_State { Start, Song, Stop } State;

void LED_Latch()
{
	unsigned char num = ~PINA;
	
	switch (State){
	   case Start:
		if ( num == 0x01 ){
			State = Song;
		}
		else{
			State = Start;
		}
		break;
	
	  case Song:
		if ( i != 11 ){
			i = i + 1;
			State = Song;
		}
		else {
			i = 0;
			State = Stop;
		}
		break;

	case Stop:
		if ( num == 0x01 ){
			State = Stop;
		}
		else{
			State = Start;
		}
		break;

		

	   default:
		State = Start;
		break;
	}


	switch (State){
	   case Start:
		break;
	
	case Song:
		set_PWM(ar[i]);
		break;

	case Stop:
		set_PWM(0);
		break;

	   default:
		break;
	}
}



int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
	PWM_on();
	
	TimerSet(700);
	TimerOn();
	
	State = Start;

    while (1) {

	while (!TimerFlag);
	TimerFlag = 0;
	LED_Latch(); 	

    }
    return 1;
}

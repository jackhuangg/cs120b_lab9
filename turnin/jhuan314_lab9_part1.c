/*	Author: Jack Huang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #9  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *	https://drive.google.com/drive/folders/1JBIqqJb-m900203LVLXI8yLaMciH493w?usp=sharing
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
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

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
unsigned char one;
unsigned char two;
enum states1 {bit1,bit2,bit3} state1;
enum states2 {bit3on,bit3off} state2;
enum states3 {combine} state3;

void ThreeLEDsSM(){
	switch(state1){
		case bit1:
			state1 = bit2;
			break;
		case bit2:
			state1 = bit3;
			break;
		case bit3:
			state1 = bit1;
			break;
	}
	switch(state1){
		case bit1:
			one=1;
			break;
		case bit2:
			one=2;
			break;
		case bit3:
			one=4;
			break;	
	}
}
void BlinkingLEDSM(){
	switch(state2){
		case bit3on:
			state2 = bit3off;
			break;
		case bit3off:
			state2 = bit3on;
			break;
	}
	switch(state2){
		case bit3on:
			two = 8;
			break;
		case bit3off:
			two=0;
			break;
	}
}
void CombineLEDsSM(){
	switch(state3){
		case combine:
			state3 = combine;
			break;
	}
	switch(state3){
		case combine:
			PORTB = one + two;
			break;
	}
}



int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
    TimerSet(1000);
    TimerOn();
    while (1) {
	ThreeLEDsSM();
	BlinkingLEDSM();
	CombineLEDsSM();
	while(!TimerFlag);
	TimerFlag=0;
    }
    return 1;
}

/*
    Start/Stop CarPC with your Car Key
*/

#define MCU=atmega8
#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 8000000

#define IGNIT() (PINC & 0b00100000)				//Pin for Ignition
#define trigger_on() PORT |= 0b00000010				//Pin for PC
#define trigger_off() PORT &= ~(0b00000010)
#define battery_on() PORTD |= 0b1000000				//Pin for Batteryrelay (K1)
#define battery_off() PORTD &= ~0b10000000
#define relay2_on() PORTD |= 0b01000000				//Pin for Relay2 (K6)
#define relay2_off() PORTD &= ~0b01000000
#define relay3_on() PORTD |= 0b00100000				//Pin for Relay3 ()
#define relay3_off() PORTD &= ~0b00100000

void init_adc () {
        ADMUX = 0b01000000;                                     //Reference=AVCC, Right Adjust, MUX=ADC0
        SFIOR &= ~(1<<ADTS2) | ~(1<<ADTS1) | ~(1<<ADTS0);       //Free running Mode
        ADCSRA |= (1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); //Enabele ADC & autotrigger, prescale=128
}

int main(void) {
	DDRC = 0x00;
	DDRD = 0b11100010;
	DDRB = 0b00000000;
	return 0;
}


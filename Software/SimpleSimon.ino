/*
	Author: Eric Conner
	Project: Simple Simon
	Date: 01-09-2017
	File: SimpleSimon.ino
	Copyright (c) 2017, Eric Conner Apps. All right Reserved.
*/

#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay_basic.h>

const uint8_t buttons[4] = {
	0b00001010, 0b00000110, 0b00000011, 0b00010010
};

const uint8_t note[4] = {
	239, 179, 143, 119
};

uint8_t lastKey;
uint8_t lvl = 0;
uint8_t maxLvl;
uint16_t ctx;
uint16_t seed;
uint8_t sound;
volatile uint8_t nrot = 8;
volatile uint16_t time;

void setup() {
	DDRB = 0b00000000;
	PORTB = 0b00011101;
	
	ADCSRA |= (1 << ADEN); // enable ADC
	ADCSRA |= (1 << ADSC); // start the conversion on unconnected ADC0 (ADMUX = 0b00000000 by default)
	while(ADCSRA & (1 << ADSC)); // ADSC is cleared when the conversion finishes
	seed = ADCL; // set seed to lower ADC byte
	ADCSRA = 0b00000000; // turn off ADC

	if(MCUSR & _BV(WDRF)){ // If a reset was caused by the Watchdog Timer...
		MCUSR &= ~_BV(WDRF); // Clear the WDT reset flag
		WDTCR |= (_BV(WDCE) | _BV(WDE)); // Enable the WD Change Bit
		WDTCR = 0x00; // Disable the WDT
	}
	
	wdt_disable();
	WDTCR |= (_BV(WDCE) | _BV(WDE)); // Enable the WD Change Bit
	WDTCR = _BV(WDIE); // start watchdog timer with 16ms prescaler (interrupt mode)
	sei();
	
	while(nrot);
	
	TCCR0B = (1 << CS00); // Timer0 in normal mode (no prescaler)
	TCCR0A = (1 << COM0B1) | (0 << COM0B0) | (0 << WGM01) | (1 << WGM00); // set Timer0 to phase correct PWM
	
	maxLvl = ~eeprom_read_byte((uint8_t*) 0);
	sound = ~eeprom_read_byte((uint8_t*) 1);
	
	switch(PINB & 0b00011101) {
		case 0b00010101: // Red button is pressed during reset
			eeprom_write_byte((uint8_t*) 0, 255); // reset best score
			maxLvl = 0;
			break;
		case 0b00011001: // Green button is pressed during reset
			lvl = 255; // play random tones in an infinite loop
			break;
		case 0b00011100: // Blue button is pressed during reset
			lvl = maxLvl; // start from max level
			break;
		case 0b00001101: // Yellow button is pressed during reset
			if(sound) { // Toggle sound on/off
				sound = 0;
				eeprom_write_byte((uint8_t*) 1, ~sound);
			} else {
				sound = 1;
				eeprom_write_byte((uint8_t*) 1, ~sound);
			}
			break;
	}
}

ISR(WDT_vect) {
	time++; // increase each 16 ms
	if(nrot) { // random seed generation
		nrot--;
		seed = (seed << 1) ^ TCNT0;
	}
}

void sleepNow() {
	PORTB = 0b00000000; // disable all pull-up resistors
	cli(); // disable all interrupts
	WDTCR = 0; // turn off the Watchdog timer
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sleep_cpu();
}

void resetCtx() {
	ctx = seed;
}

uint8_t random4() {
	ctx = 2053 * ctx + 13849;
	uint8_t temp = ctx ^ (ctx >> 8); // XOR two bytes
	temp ^= (temp >> 4); // XOR two nibbles
	return (temp ^ (temp >> 2)) & 0b00000011; // XOR two pairs of bits and return remainder after division by 4
}

void play(uint8_t i, uint16_t t = 45000) {
	PORTB = 0b00000000; // set all button pins low or disable pull-up resistors
	if(sound) {
		DDRB = buttons[i]; // set speaker and #i button pin as output
		OCR0A = note[i];
		OCR0B = note[i] >> 1;
		TCCR0B = (1 << WGM02) | (1 << CS01); // prescaler /8
		_delay_loop_2(t);
		TCCR0B = 0b00000000; // no clock source (Timer0 stopped)
	} else {
		DDRB = buttons[i] & 0b00011101;
		_delay_loop_2(t);
	}
	DDRB = 0b00000000;
	PORTB = 0b00011101;
}

void levelUp() {
	for(uint8_t i = 0; i < 4; i++) {
		play(i, 25000);
	}
}

void gameOver() {
	for(uint8_t i = 0; i < 4; i++) {
		play(3 - i, 25000);
	}
	
	if(lvl > maxLvl) {
		eeprom_write_byte((uint8_t*) 0, ~lvl); // write best score
		for(uint8_t i = 0; i < 3; i++) { // play best score melody
			levelUp();
		}
	}
	
	sleepNow();
}

void loop() {
	resetCtx();
	
	for(uint8_t cnt = 0; cnt <= lvl; cnt++) { // never ends if lvl == 255
		_delay_loop_2(4400 + 489088 / (8 + lvl));
		play(random4());
	}
	
	time = 0;
	lastKey = 5;
	resetCtx();
	
	for(uint8_t cnt = 0; cnt <= lvl; cnt++) {
		bool pressed = false;
		while(!pressed) {
			for(uint8_t i = 0; i < 4; i++) {
				if(!(PINB & buttons[i] & 0b00011101)) {
					if(time > 1 || i != lastKey) {
						play(i);
						pressed = true;
						uint8_t correct = random4();
						if(i != correct) {
							for(uint8_t j = 0; j < 3; j++) {
								_delay_loop_2(10000);
								play(correct, 20000);
							}
							_delay_loop_2(65536);
							gameOver();
						}
						time = 0;
						lastKey = i;
						break;
					}
					time = 0;
				}
				if(time > 1000) {
					gameOver();
				}
			}
		}
	}

	_delay_loop_2(65536);

	if(lvl < 254) {
		lvl++;
		levelUp(); // animation for completed level
		_delay_loop_2(45000);
	} else { // special animation for highest allowable (255th) level
		levelUp();
		gameOver(); // then turn off
	}
}

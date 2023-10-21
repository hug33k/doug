#include <avr/io.h>
#include <avr/interrupt.h>
#include "usi_i2c_slave.h"

#define DELAY_1 500
#define DELAY_2 250

#define OUTPUT	1
#define INPUT	0
#define HIGH	1
#define LOW	0


void wait(int delay_ms) {
	for(volatile int i=0; i < delay_ms; i++) {
		for (volatile int j=0; j<50; j++) {}
	}
}

void pinMode(int pin, int value) {
	if (value == OUTPUT)
		DDRB |= (1 << pin);
	else
		DDRB &= ~(1 << pin);
}

void digitalWrite(int pin, int value) {
	if (value == HIGH)
		PORTB |= (1 << pin);
	else
		PORTB &= ~(1 << pin);
}

int main() {
	USI_I2C_Init(I2C_SLAVE_ADDRESS);
	sei();
	pinMode(PB4, OUTPUT);
	while (1) {
		digitalWrite(PB4, HIGH);
		i2c_registers[0]++;
		wait(DELAY_1);
		digitalWrite(PB4, LOW);
		wait(DELAY_2);
	}
}

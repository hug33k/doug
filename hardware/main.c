#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <inttypes.h>
#include "usiTwiSlave.h"

volatile uint8_t data = 0x00;

#define LED_PIN		PB4
#define BTN_PIN		PB1
#define DELAY_1		100
#define DELAY_2		200

void wait(int delay_ms) {
	for(volatile int i=0; i < delay_ms; i++) {
		for (volatile int j=0; j<50; j++) {}
	}
}

void blink(byte count) {
	for (byte i=0; i < count; i++) {
		digitalWrite(LED_PIN, HIGH);
		wait(DELAY_1);
		digitalWrite(LED_PIN, LOW);
		wait(DELAY_2);
	}
}

ISR(PCINT0_vect) {
	if (PINB & _BV(BTN_PIN)) {
		data = 0x05;
	}
}

int main(void) {
	pinMode(LED_PIN, OUTPUT);
	pinMode(BTN_PIN, INPUT);
	blink(2);
	wait(DELAY_1);
	usiTwiSlaveInit(I2C_SLAVE_ADDR);

	PCMSK |= _BV(PB1);
	MCUCR = _BV(ISC01) | _BV(ISC00);
	GIMSK |= _BV(PCIE);

	sei();
	blink(2);
	wait(DELAY_1);
	usiTwiSlaveEnable();

//	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	while (1) {
		if (data != 0x00) {
			blink(1);
//			wait(DELAY_1);
			usiTwiTransmitByte(data);
//			wait(DELAY_1);
			blink(1);
			data = 0x00;
		}
	}
//		sleep_mode();
}

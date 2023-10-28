#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <inttypes.h>
#include "usiTwiSlave.h"

volatile uint8_t data = 0x00;

void wait(int delay_ms) {
	for(volatile int i=0; i < delay_ms; i++) {
		for (volatile int j=0; j<50; j++) {}
	}
}

void blink(byte count) {
	for (byte i=0; i < count; i++) {
		digitalWrite(PB4, HIGH);
		wait(DELAY_1);
		digitalWrite(PB4, LOW);
		wait(DELAY_2);
	}
}

ISR(PCINT0_vect) {
	if (PINB & _BV(PB1)) {
		data = 0x05;
	}
}

int main(void) {
	pinMode(PB4, OUTPUT);
	pinMode(PB1, INPUT);
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
/*

	while (1) {
		byte byteRcvd = 0;
		if (usiTwiDataInReceiveBuffer()) {
			blink(2);
			wait(DELAY_1);
			byteRcvd = usiTwiReceiveByte();
			blink(byteRcvd);
			byteRcvd += 10;
			wait(DELAY_1);
			usiTwiTransmitByte(byteRcvd);
			blink(1);
		}

	}
*/
}

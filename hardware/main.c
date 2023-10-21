#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "usiTwiSlave.h"


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

void blink2(byte count) {
	for (byte i=0; i < count; i++) {
		digitalWrite(PB3, HIGH);
		wait(DELAY_1);
		digitalWrite(PB3, LOW);
		wait(DELAY_2);
	}
}

void blink3(byte count) {
	for (byte i=0; i < count; i++) {
		digitalWrite(PB1, HIGH);
		wait(DELAY_1);
		digitalWrite(PB1, LOW);
		wait(DELAY_2);
	}
}

int main(void) {
	pinMode(PB4, OUTPUT);
	pinMode(PB3, OUTPUT);
	pinMode(PB1, OUTPUT);
	blink(2);
	wait(DELAY_1);
	usiTwiSlaveInit(I2C_SLAVE_ADDR);
	sei();
	blink(2);
	wait(DELAY_1);
	usiTwiSlaveEnable();
	blink2(1);
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
}

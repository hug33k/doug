#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "usiTwiSlave.h"

#define LED_PIN		PB4
#define DELAY_1 	250
#define DELAY_2 	100

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

int main(void) {
	pinMode(LED_PIN, OUTPUT);
	blink(1);
	usiTwiSlaveInit(I2C_SLAVE_ADDR);
	sei();
	blink(2);
	usiTwiSlaveEnable();
	blink(3);
	while (1) {
		byte byteRcvd = 0;
		if (usiTwiDataInReceiveBuffer()) {
			blink(1);
			wait(DELAY_1);
			byteRcvd = usiTwiReceiveByte();
			blink(byteRcvd);
			byteRcvd += 10;
			usiTwiTransmitByte(byteRcvd);
			wait(DELAY_1);
			blink(1);
			wait(DELAY_1);
		}
	}
}

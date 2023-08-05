#include <avr/io.h>
#define LED_PIN PORTB4
#define LED_TOGGLE PORTB ^= ( 1 << LED_PIN)
#define DELAY_1 1000
#define DELAY_2 250

void wait(int delay_ms) {
	for(volatile int i=0; i < delay_ms; i++) {
		for (volatile int j=0; j<50; j++) {}
	}
}

int main() {
	DDRB |= (1 << LED_PIN);
	while (1) {
		LED_TOGGLE;
		wait(DELAY_1);
		LED_TOGGLE;
		wait(DELAY_2);
	}
}


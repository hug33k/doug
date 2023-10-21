#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED_PIN PB1
#define I2C_ADDRESS 0x12

void i2c_init(uint8_t address) {
    // Set up TWI (I2C) slave address
    TWAR = (address << 1);
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
}

void turn_on_led() {
    PORTB |= (1 << LED_PIN);
}

void turn_off_led() {
    PORTB &= ~(1 << LED_PIN);
}

ISR(TWI_vect) {
    switch (TWSR & 0xF8) {
        case 0x60:  // Own SLA+W received, ACK returned
        case 0x68:  // Arbitration lost in SLA+R/W as master, own SLA+W received, ACK returned
        case 0x70:  // General call address received, ACK returned
        case 0x78:  // Arbitration lost in SLA+R/W as master, general call received, ACK returned
            TWCR |= (1 << TWINT) | (1 << TWEA);
            break;

        case 0x80:  // Data received, ACK returned
            if (TWDR == 1) {
                turn_on_led();
            } else if (TWDR == 0) {
                turn_off_led();
            }
            TWCR |= (1 << TWINT) | (1 << TWEA);
            break;

        case 0xA0:  // Stop or repeated start condition received
            TWCR |= (1 << TWINT) | (1 << TWEA);
            break;

        default:
            // Unknown status, reset TWI
            TWCR |= (1 << TWINT) | (1 << TWEA);
    }
}

int main() {
    DDRB |= (1 << LED_PIN);  // Set LED_PIN as output

    // Set up TWI (I2C)
    i2c_init(I2C_ADDRESS);

    // Enable global interrupts
    sei();

    while (1) {
        // Continue normal operation
    }

    return 0;
}

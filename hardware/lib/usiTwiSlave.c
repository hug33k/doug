/********************************************************************************

USI TWI Slave driver.

Created by Donald R. Blake
donblake at worldnet.att.net

Modified by Nels D. Pearson to match twiSlave.c TWI code.
chip at gameactive.org

---------------------------------------------------------------------------------

Created from Atmel source files for Application Note AVR312: Using the USI Module
as an I2C slave.

This program is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

---------------------------------------------------------------------------------

Change Activity:

	Date       Description
   ------      -------------
  16 Mar 2007  Created.
  27 Mar 2007  Added support for ATtiny261, 461 and 861.
  26 Apr 2007  Fixed ACK of slave address on a read.
  27 May 2015  Added support for ATtiny24/44/84 and ATtiny24A/44A/84A devices.(ndp)
  23 Jan 2016  Added support functions used by twiSlave.c for interchangeability.(ndp)
  29 Jan 2016  Add TxBuf[] test to support single read example. (ndp)

********************************************************************************/

/********************************************************************************

                                    includes

********************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "usiTwiSlave.h"

/********************************************************************************

							custom

********************************************************************************/

void	pinMode(int pin, int value) {
	if (value == OUTPUT)
		DDRB |= (1 << pin);
	else
		DDRB &= ~(1 << pin);
}

void	digitalWrite(int pin, int value) {
	if (value == HIGH)
		PORTB |= (1 << pin);
	else
		PORTB &= ~(1 << pin);
}

/********************************************************************************

								local variables

********************************************************************************/

static uint8_t					slaveAddress;
static volatile overflowState_t	overflowState;

static uint8_t					rxBuf[ TWI_RX_BUFFER_SIZE ];
static volatile uint8_t			rxHead;
static volatile uint8_t			rxTail;

static uint8_t					txBuf[ TWI_TX_BUFFER_SIZE ];
static volatile uint8_t			txHead;
static volatile uint8_t			txTail;

/********************************************************************************

								local functions

********************************************************************************/

static void	flushTwiBuffers(void) {	// flushes the TWI buffers
	rxTail = 0;
	rxHead = 0;
	txTail = 0;
	txHead = 0;
}

/********************************************************************************

								public functions

********************************************************************************/

void	usiTwiSlaveInit(uint8_t ownAddress) {	// initialise USI for TWI slave mode
  flushTwiBuffers();

  slaveAddress = ownAddress;

	// In Two Wire mode (USIWM1, USIWM0 = 1X), the slave USI will pull SCL
	// low when a start condition is detected or a counter overflow (only
	// for USIWM1, USIWM0 = 11).  This inserts a wait state.  SCL is released
	// by the ISRs (USI_START_vect and USI_OVERFLOW_vect).

	pinMode(PORT_USI_SCL, OUTPUT);		// Set SCL as output
	pinMode(PORT_USI_SDA, OUTPUT);		// Set SDA as output

	digitalWrite(PORT_USI_SCL, HIGH);	// set SCL high
	digitalWrite(PORT_USI_SDA, HIGH);	// set SDA high

	pinMode(PORT_USI_SCL, OUTPUT);		// set SCL as output
	pinMode(PORT_USI_SDA, INPUT);		// Set SDA as input
}

void	usiTwiSlaveEnable(void)																	// Enable I2C Slave, this is called to make the Slave ready to receive commands.
{
	USICR = ( 1 << USISIE ) |																	// enable Start Condition Interrupt
			( 0 << USIOIE ) |																	// disable Overflow Interrupt
			( 1 << USIWM1 ) | ( 0 << USIWM0 ) |													// set USI in Two-wire mode, no USI Counter overflow hold
			( 1 << USICS1 ) | ( 0 << USICS0 ) | ( 0 << USICLK ) |								// Shift Register Clock Source = external, positive edge / 4-Bit Counter Source = external, both edges
			( 0 << USITC );																		// no toggle clock-port pin

	USISR = ( 1 << USI_START_COND_INT ) | ( 1 << USIOIF ) | ( 1 << USIPF ) | ( 1 << USIDC );	// clear all interrupt flags and reset overflow counter
}

void	usiTwiTransmitByte(uint8_t data) {			// put data in the transmission buffer, wait if buffer is full
	uint8_t tmphead;

	tmphead = ( txHead + 1 ) & TWI_TX_BUFFER_MASK;	// calculate buffer index

	while ( tmphead == txTail );					// wait for free space in buffer

	txBuf[ tmphead ] = data;						// store data in buffer

	txHead = tmphead;								// store new index
}

uint8_t	usiTwiReceiveByte(void) {					// return a byte from the receive buffer, wait if buffer is empty
	while ( rxHead == rxTail );						// wait for Rx data

	rxTail = ( rxTail + 1 ) & TWI_RX_BUFFER_MASK;	// calculate buffer index

	return rxBuf[ rxTail ];							// return data from the buffer.
}

bool	usiTwiDataInReceiveBuffer(void) {			// check if there is data in the receive buffer
	return rxHead != rxTail;						// return 0 (false) if the receive buffer is empty
}

bool	usiTwiDataInTransmitBuffer(void) {	// Check that prior data was been sent. / This function should return FALSE when a Read request is received.
	return txHead != txTail;				// return 0 (false) if the transmit buffer is empty
}


/********************************************************************************

							USI Start Condition ISR

********************************************************************************/

ISR(USI_START_VECTOR) {
	// set default starting conditions for new TWI package
	overflowState = USI_SLAVE_CHECK_ADDRESS;

	// set SDA as input
	pinMode(PORT_USI_SDA, INPUT);

	// wait for SCL to go low to ensure the Start Condition has completed (the
	// start detector will hold SCL low ) - if a Stop Condition arises then leave
	// the interrupt to prevent waiting forever - don't use USISR to test for Stop
	// Condition as in Application Note AVR312 because the Stop Condition Flag is
	// going to be set from the last TWI sequence
	while (
		// SCL his high
		( PIN_USI & ( 1 << PIN_USI_SCL ) ) &&
		// and SDA is low
		!( ( PIN_USI & ( 1 << PIN_USI_SDA ) ) )
	);


	if ( !( PIN_USI & ( 1 << PIN_USI_SDA ) ) )
	{

		// a Stop Condition did not occur

		USICR =
			// keep Start Condition Interrupt enabled to detect RESTART
			( 1 << USISIE ) |
			// enable Overflow Interrupt
			( 1 << USIOIE ) |
			// set USI in Two-wire mode, hold SCL low on USI Counter overflow
			( 1 << USIWM1 ) | ( 1 << USIWM0 ) |
			// Shift Register Clock Source = External, positive edge
			// 4-Bit Counter Source = external, both edges
			( 1 << USICS1 ) | ( 0 << USICS0 ) | ( 0 << USICLK ) |
			// no toggle clock-port pin
			( 0 << USITC );

	}
	else
	{

		// a Stop Condition did occur
		USICR =
			// enable Start Condition Interrupt
			( 1 << USISIE ) |
			// disable Overflow Interrupt
			( 0 << USIOIE ) |
			// set USI in Two-wire mode, no USI Counter overflow hold
			( 1 << USIWM1 ) | ( 0 << USIWM0 ) |
			// Shift Register Clock Source = external, positive edge
			// 4-Bit Counter Source = external, both edges
			( 1 << USICS1 ) | ( 0 << USICS0 ) | ( 0 << USICLK ) |
			// no toggle clock-port pin
			( 0 << USITC );

	} // end if

	USISR =
		// clear interrupt flags - resetting the Start Condition Flag will
		// release SCL
		( 1 << USI_START_COND_INT ) | ( 1 << USIOIF ) |
		( 1 << USIPF ) |( 1 << USIDC ) |
		// set USI to sample 8 bits (count 16 external SCL pin toggles)
		( 0x0 << USICNT0);

}

/********************************************************************************

								USI Overflow ISR

Handles all the communication.

Only disabled when waiting for a new Start Condition.

********************************************************************************/

ISR(USI_OVERFLOW_VECTOR)	{
	switch(overflowState) {

		// Address mode: check address and send ACK (and next USI_SLAVE_SEND_DATA) if OK,
		// else reset USI
		case USI_SLAVE_CHECK_ADDRESS:
			if ( ( USIDR == 0 ) ||  (( USIDR >> 1 ) == slaveAddress )) {
				if ( USIDR & 0x01 )
					overflowState = USI_SLAVE_SEND_DATA;
				else
					overflowState = USI_SLAVE_REQUEST_DATA;
				SET_USI_TO_SEND_ACK();
			}
			else
				SET_USI_TO_TWI_START_CONDITION_MODE();
			break;

		// Master write data mode: check reply and goto USI_SLAVE_SEND_DATA if OK,
		// else reset USI
		case USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA:
			if (USIDR) {
				// if NACK, the master does not want more data
				SET_USI_TO_TWI_START_CONDITION_MODE();
				return;
			}
		// from here we just drop straight into USI_SLAVE_SEND_DATA if the
		// master sent an ACK

		// copy data from buffer to USIDR and set USI to shift byte
		// next USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA
		case USI_SLAVE_SEND_DATA:
			// Get data from Buffer
			if ( txHead != txTail ) {
				txTail = ( txTail + 1 ) & TWI_TX_BUFFER_MASK;
				USIDR = txBuf[ txTail ];
			} else {
				// the buffer is empty
				SET_USI_TO_TWI_START_CONDITION_MODE();
				return;
			} // end if
			overflowState = USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA;
			SET_USI_TO_SEND_DATA();
			break;

		// set USI to sample reply from master
		// next USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA
		case USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA:
			overflowState = USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA;
			SET_USI_TO_READ_ACK();
			break;

		// Master read data mode: set USI to sample data from master, next
		// USI_SLAVE_GET_DATA_AND_SEND_ACK
		case USI_SLAVE_REQUEST_DATA:
			overflowState = USI_SLAVE_GET_DATA_AND_SEND_ACK;
			SET_USI_TO_READ_DATA();
			break;

		// copy data from USIDR and send ACK
		// next USI_SLAVE_REQUEST_DATA
		case USI_SLAVE_GET_DATA_AND_SEND_ACK:
			// put data into buffer
			// Not necessary, but prevents warnings
			rxHead = ( rxHead + 1 ) & TWI_RX_BUFFER_MASK;
			rxBuf[ rxHead ] = USIDR;
			// next USI_SLAVE_REQUEST_DATA
			overflowState = USI_SLAVE_REQUEST_DATA;
			SET_USI_TO_SEND_ACK();
			break;
	}
}

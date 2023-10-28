/********************************************************************************

Header file for the USI TWI Slave driver.

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
  15 Mar 2007  Created.
  23 Jan 2016  Added support functions used by twiSlave.c for interchangeability.(ndp)
  29 Jan 2016  Add TxBuf[] test to support single read example. (ndp)

********************************************************************************/

#ifndef _USI_TWI_SLAVE_H_
#define _USI_TWI_SLAVE_H_

	/********************************************************************************

										includes

	********************************************************************************/

	#include <stdbool.h>

	typedef	unsigned char	byte ;

	#define I2C_SLAVE_ADDR  0x24
	#define OUTPUT			1
	#define INPUT			0
	#define HIGH			1
	#define LOW				0

	/********************************************************************************

									prototypes

	********************************************************************************/

	void	pinMode(int, int);
	void 	digitalWrite(int, int);

	void	usiTwiSlaveInit(uint8_t);
	void	usiTwiSlaveEnable(void);			// added ndp
	void	usiTwiTransmitByte(uint8_t);
	uint8_t	usiTwiReceiveByte(void);
	bool	usiTwiDataInReceiveBuffer(void);
	bool	usiTwiDataInTransmitBuffer(void);

	/********************************************************************************

							driver buffer definitions

	********************************************************************************/

	// permitted RX buffer sizes: 1, 2, 4, 8, 16, 32, 64, 128 or 256

	#define TWI_RX_BUFFER_SIZE	(32) // jjg was 16
	#define TWI_RX_BUFFER_MASK	(TWI_RX_BUFFER_SIZE - 1)

	#if (TWI_RX_BUFFER_SIZE & TWI_RX_BUFFER_MASK)
		#  error TWI RX buffer size is not a power of 2
	#endif

	// permitted TX buffer sizes: 1, 2, 4, 8, 16, 32, 64, 128 or 256

	#define TWI_TX_BUFFER_SIZE	(32) // jjg was 16
	#define TWI_TX_BUFFER_MASK	(TWI_TX_BUFFER_SIZE - 1)

	#if (TWI_TX_BUFFER_SIZE & TWI_TX_BUFFER_MASK)
		#  error TWI TX buffer size is not a power of 2
	#endif

	/********************************************************************************

								device dependent defines

	********************************************************************************/

	#if defined( __AVR_ATtiny2313__ )
		#define DDR_USI				DDRB
		#define PORT_USI			PORTB
		#define PIN_USI				PINB
		#define PORT_USI_SDA		PORTB5
		#define PORT_USI_SCL		PORTB7
		#define PIN_USI_SDA			PINB5
		#define PIN_USI_SCL			PINB7
		#define USI_START_COND_INT	USISIF
		#define USI_START_VECTOR	USI_START_vect
		#define USI_OVERFLOW_VECTOR	USI_OVERFLOW_vect
	#endif

	#if defined( __AVR_ATtiny25__ ) | \
		defined( __AVR_ATtiny45__ ) | \
		defined( __AVR_ATtiny85__ )
		#define DDR_USI				DDRB
		#define PORT_USI			PORTB
		#define PIN_USI				PINB
		#define PORT_USI_SDA		PB0
		#define PORT_USI_SCL		PB2
		#define PIN_USI_SDA			PINB0
		#define PIN_USI_SCL			PINB2
		#define USI_START_COND_INT	USISIF //was USICIF jjg
		#define USI_START_VECTOR	USI_START_vect
		#define USI_OVERFLOW_VECTOR	USI_OVF_vect
	#endif

	#if defined( __AVR_ATtiny26__ )
		#define DDR_USI				DDRB
		#define PORT_USI			PORTB
		#define PIN_USI				PINB
		#define PORT_USI_SDA		PB0
		#define PORT_USI_SCL		PB2
		#define PIN_USI_SDA			PINB0
		#define PIN_USI_SCL			PINB2
		#define USI_START_COND_INT	USISIF
		#define USI_START_VECTOR	USI_STRT_vect
		#define USI_OVERFLOW_VECTOR	USI_OVF_vect
	#endif

	#if defined( __AVR_ATtiny261__ ) | \
		defined( __AVR_ATtiny461__ ) | \
		defined( __AVR_ATtiny861__ )
		#define DDR_USI				DDRB
		#define PORT_USI			PORTB
		#define PIN_USI				PINB
		#define PORT_USI_SDA		PB0
		#define PORT_USI_SCL		PB2
		#define PIN_USI_SDA			PINB0
		#define PIN_USI_SCL			PINB2
		#define USI_START_COND_INT	USISIF
		#define USI_START_VECTOR	USI_START_vect
		#define USI_OVERFLOW_VECTOR	USI_OVF_vect
	#endif

	#if defined( __AVR_ATmega165__ ) | \
		defined( __AVR_ATmega325__ ) | \
		defined( __AVR_ATmega3250__ ) | \
		defined( __AVR_ATmega645__ ) | \
		defined( __AVR_ATmega6450__ ) | \
		defined( __AVR_ATmega329__ ) | \
		defined( __AVR_ATmega3290__ )
		#define DDR_USI				DDRE
		#define PORT_USI			PORTE
		#define PIN_USI				PINE
		#define PORT_USI_SDA		PE5
		#define PORT_USI_SCL		PE4
		#define PIN_USI_SDA			PINE5
		#define PIN_USI_SCL			PINE4
		#define USI_START_COND_INT	USISIF
		#define USI_START_VECTOR	USI_START_vect
		#define USI_OVERFLOW_VECTOR	USI_OVERFLOW_vect
	#endif

	#if defined( __AVR_ATmega169__ )
		#define DDR_USI				DDRE
		#define PORT_USI			PORTE
		#define PIN_USI				PINE
		#define PORT_USI_SDA		PE5
		#define PORT_USI_SCL		PE4
		#define PIN_USI_SDA			PINE5
		#define PIN_USI_SCL			PINE4
		#define USI_START_COND_INT	USISIF
		#define USI_START_VECTOR	USI_START_vect
		#define USI_OVERFLOW_VECTOR	USI_OVERFLOW_vect
	#endif

	/********************************************************************************

							functions implemented as macros
							TODO : NEED TO REPLACE THEM WITH FUNCTIONS !!!!!!!!!!!!

	********************************************************************************/

	#define SET_USI_TO_SEND_ACK()	{				\
		USIDR = 0;									/* prepare ACK */\
		DDR_USI |= ( 1 << PORT_USI_SDA );			/* set SDA as output */\
		USISR = ( 0 << USI_START_COND_INT ) | 		/* clear all interrupt flags, except Start Cond */\
				( 1 << USIOIF ) | ( 1 << USIPF ) |	\
				( 1 << USIDC )|						\
				( 0x0E << USICNT0 );				/* set USI counter to shift 1 bit */\
	}

	#define SET_USI_TO_READ_ACK()	{			\
		DDR_USI &= ~( 1 << PORT_USI_SDA );		/* set SDA as input */\
		USIDR = 0;								/* prepare ACK */\
		USISR = ( 0 << USI_START_COND_INT ) |	/* clear all interrupt flags, except Start Cond */\
				( 1 << USIOIF ) |				\
				( 1 << USIPF ) |				\
				( 1 << USIDC ) |				\
				( 0x0E << USICNT0 );			/* set USI counter to shift 1 bit */\
	}

	#define SET_USI_TO_TWI_START_CONDITION_MODE()	{								\
		USICR = ( 1 << USISIE ) | ( 0 << USIOIE ) |									/* enable Start Condition Interrupt, disable Overflow Interrupt */\
				( 1 << USIWM1 ) | ( 0 << USIWM0 ) |									/* set USI in Two-wire mode, no USI Counter overflow hold */\
				( 1 << USICS1 ) | ( 0 << USICS0 ) | ( 0 << USICLK ) |				/* Shift Register Clock Source = External, positive edge / 4-Bit Counter Source = external, both edges / no toggle clock-port pin */\
				( 0 << USITC );														\
		USISR = ( 0 << USI_START_COND_INT ) | ( 1 << USIOIF ) | ( 1 << USIPF ) |	/* clear all interrupt flags, except Start Cond */\
				( 1 << USIDC ) | ( 0x0 << USICNT0 );								\
	}

	#define SET_USI_TO_SEND_DATA()	{												\
		DDR_USI |= ( 1 << PORT_USI_SDA );											/* set SDA as output */\
		USISR = ( 0 << USI_START_COND_INT ) | ( 1 << USIOIF ) | ( 1 << USIPF ) |	/* clear all interrupt flags, except Start Cond */\
				( 1 << USIDC) |														\
				( 0x0 << USICNT0 );													/* set USI to shift out 8 bits */\
	}

	#define SET_USI_TO_READ_DATA()	{							\
		DDR_USI &= ~( 1 << PORT_USI_SDA );						/* set SDA as input */\
		USISR = ( 0 << USI_START_COND_INT ) | ( 1 << USIOIF ) |	/* clear all interrupt flags, except Start Cond */\
				( 1 << USIPF ) | ( 1 << USIDC ) |				\
				( 0x0 << USICNT0 );								/* set USI to shift out 8 bits */\
	}

	/********************************************************************************

									typedef's

	********************************************************************************/

	typedef enum
	{
		USI_SLAVE_CHECK_ADDRESS					= 0x00,
		USI_SLAVE_SEND_DATA						= 0x01,
		USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA	= 0x02,
		USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA	= 0x03,
		USI_SLAVE_REQUEST_DATA					= 0x04,
		USI_SLAVE_GET_DATA_AND_SEND_ACK			= 0x05
	} overflowState_t;

#endif  // ifndef _USI_TWI_SLAVE_H_

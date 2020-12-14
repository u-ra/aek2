/*
	Copyright (C) 2011  Andrew Cotter

	This program is free software: you can redistribute it and/or modify it
	under the terms of the GNU General Public License as published by the Free
	Software Foundation, either version 3 of the License, or (at your option)
	any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
	FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
	more details.

	You should have received a copy of the GNU General Public License along
	with this program. If not, see <http://www.gnu.org/licenses/>.
*/


/**
	\file pins.c
	\brief Pin allocation function definitions
*/




#include "pins.h"

#include <avr/io.h>
#include <avr/pgmspace.h>

#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>




enum {
#if ( defined( PINA ) && defined( DDRA ) && defined( PORTA ) )
	INDEX_A,    ///< index of port A
#endif    // A
#if ( defined( PINB ) && defined( DDRB ) && defined( PORTB ) )
	INDEX_B,    ///< index of port B
#endif    // B
#if ( defined( PINC ) && defined( DDRC ) && defined( PORTC ) )
	INDEX_C,    ///< index of port C
#endif    // C
#if ( defined( PIND ) && defined( DDRD ) && defined( PORTD ) )
	INDEX_D,    ///< index of port D
#endif    // D
#if ( defined( PINE ) && defined( DDRE ) && defined( PORTE ) )
	INDEX_E,    ///< index of port E
#endif    // E
#if ( defined( PINF ) && defined( DDRF ) && defined( PORTF ) )
	INDEX_F,    ///< index of port F
#endif    // F
#if ( defined( PING ) && defined( DDRG ) && defined( PORTG ) )
	INDEX_G,    ///< index of port G
#endif    // G
#if ( defined( PINH ) && defined( DDRH ) && defined( PORTH ) )
	INDEX_H,    ///< index of port H
#endif    // H
	INDEX_END,         ///< number of port indices
	INDEX_BEGIN = 0    ///< first port index
};

/// array of addresses of port input, direction and output registers
static PROGMEM uint8_t volatile* const g_pointers[ INDEX_END ][ 3 ] = {
#if ( defined( PINA ) && defined( DDRA ) && defined( PORTA ) )
	{ &PINA, &DDRA, &PORTA },
#endif    // A
#if ( defined( PINB ) && defined( DDRB ) && defined( PORTB ) )
	{ &PINB, &DDRB, &PORTB },
#endif    // B
#if ( defined( PINC ) && defined( DDRC ) && defined( PORTC ) )
	{ &PINC, &DDRC, &PORTC },
#endif    // C
#if ( defined( PIND ) && defined( DDRD ) && defined( PORTD ) )
	{ &PIND, &DDRD, &PORTD },
#endif    // D
#if ( defined( PINE ) && defined( DDRE ) && defined( PORTE ) )
	{ &PINE, &DDRE, &PORTE },
#endif    // E
#if ( defined( PINF ) && defined( DDRF ) && defined( PORTF ) )
	{ &PINF, &DDRF, &PORTF },
#endif    // F
#if ( defined( PING ) && defined( DDRG ) && defined( PORTG ) )
	{ &PING, &DDRG, &PORTG },
#endif    // G
#if ( defined( PINH ) && defined( DDRH ) && defined( PORTH ) )
	{ &PINH, &DDRH, &PORTH },
#endif    // H
};

/// array of bit flags marking which pins have been allocated
static uint8_t g_allocated[ INDEX_END ] = { 0 };




/**
	\brief Helper function for PinAllocate() and PinFree()

	Finds the index of a port within the internal arrays g_pointers and
	g_allocated, returning the one-past-the-end index on failure.

	\param name   port name ('a'-'h', 'A'-'H')
	\result  index
*/
static uint8_t const PinGetIndex( char const name ) {

	uint8_t index = INDEX_END;

	switch( name ) {
#if ( defined( PINA ) && defined( DDRA ) && defined( PORTA ) )
		case 'a': case 'A': index = INDEX_A; break;
#endif    // A
#if ( defined( PINB ) && defined( DDRB ) && defined( PORTB ) )
		case 'b': case 'B': index = INDEX_B; break;
#endif    // B
#if ( defined( PINC ) && defined( DDRC ) && defined( PORTC ) )
		case 'c': case 'C': index = INDEX_C; break;
#endif    // C
#if ( defined( PIND ) && defined( DDRD ) && defined( PORTD ) )
		case 'd': case 'D': index = INDEX_D; break;
#endif    // D
#if ( defined( PINE ) && defined( DDRE ) && defined( PORTE ) )
		case 'e': case 'E': index = INDEX_E; break;
#endif    // E
#if ( defined( PINF ) && defined( DDRF ) && defined( PORTF ) )
		case 'f': case 'F': index = INDEX_F; break;
#endif    // F
#if ( defined( PING ) && defined( DDRG ) && defined( PORTG ) )
		case 'g': case 'G': index = INDEX_G; break;
#endif    // G
#if ( defined( PINH ) && defined( DDRH ) && defined( PORTH ) )
		case 'h': case 'H': index = INDEX_H; break;
#endif    // H
		default: break;
	}

	return index;
}


bool const PinAllocate(
	uint8_t volatile** const pPin,
	uint8_t volatile** const pDdr,
	uint8_t volatile** const pPort,
	char const name,
	uint8_t const bit
)
{
	bool success = false;

	uint8_t const index = PinGetIndex( name );
	uint8_t const mask = ( 1u << bit );
	if ( ( index < INDEX_END ) && ( mask > 0 ) && ( ( g_allocated[ index ] & mask ) == 0 ) ) {

		uint8_t volatile* pointers[ 3 ] = { NULL, NULL, NULL };
		memcpy_P( pointers, g_pointers[ index ], sizeof( pointers ) );

		*pPin  = pointers[ 0 ];
		*pDdr  = pointers[ 1 ];
		*pPort = pointers[ 2 ];

		g_allocated[ index ] |= mask;

		success = true;
	}
	else {

		*pPin  = NULL;
		*pDdr  = NULL;
		*pPort = NULL;
	}

	return success;
}


void PinFree(
	char const name,
	uint8_t const bit
)
{
	uint8_t const index = PinGetIndex( name );
	if ( index < INDEX_END )
		g_allocated[ index ] &= ~( 1u << bit );
}

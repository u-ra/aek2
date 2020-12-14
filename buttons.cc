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
	\file buttons.cc
	\brief Buttons implementation
*/




#include "buttons.hh"

#include <util/delay.h>




//============================================================================
//    Buttons methods
//============================================================================


Buttons::Buttons( char const* const buttonString, bool const activeHigh ) :
	m_buttons( 0 ),
	m_activeHigh( activeHigh ),
	m_debouncingIterations( 1 ),
	m_state( 0 )
{
	for ( unsigned int ii = 0; ( buttonString[ ii * 2 ] != '\0' ) && ( buttonString[ ii * 2 + 1 ] != '\0' ) && ( m_buttons < MAXIMUM_BUTTONS ); ++ii ) {

		char const name = buttonString[ ii * 2     ];
		char const bit  = buttonString[ ii * 2 + 1 ] - '0';

		uint8_t volatile* ddr  = NULL;
		uint8_t volatile* port = NULL;
		uint8_t volatile* pin  = NULL;
		if ( PinAllocate( &pin, &ddr, &port, name, bit ) ) {    /// \todo handle errors

			*ddr  &= ~( 1u << bit );    // direction = input
			*port |=  ( 1u << bit );    // value = high (pull-up resistor)

			m_buttonPinNames[ m_buttons ] = name;
			m_buttonPinBits[  m_buttons ] = bit;
			m_buttonPins[     m_buttons ] = pin;

			++m_buttons;
		}
	}
}


Buttons::~Buttons() {

	for ( unsigned int ii = 0; ii < m_buttons; ++ii )
		PinFree( m_buttonPinNames[ ii ], m_buttonPinBits[ ii ] );
}


bool const Buttons::Update() {

	StateType const oldState = m_state;

	m_state = ( static_cast< StateType >( 1 ) << m_buttons ) - 1;
	for ( unsigned int ii = 0; ii < m_debouncingIterations; ++ii ) {

		_delay_us( 10 );

		for ( unsigned int ii = 0; ii < m_buttons; ++ii )
			if ( ( ( *m_buttonPins[ ii ] & ( 1u << m_buttonPinBits[ ii ] ) ) != 0 ) != m_activeHigh )    // is this button pressed?
				m_state &= ~( static_cast< StateType >( 1 ) << ii );
	}

	return( m_state != oldState );
}

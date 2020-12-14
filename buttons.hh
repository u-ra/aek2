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
	\file buttons.hh
	\brief Buttons implementation
*/




#ifndef __BUTTONS_HH__
#define __BUTTONS_HH__

#ifdef __cplusplus




#include "pins.h"
#include "helpers.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include <assert.h>
#include <stdlib.h>
#include <inttypes.h>




//============================================================================
//    Buttons class
//============================================================================


struct Buttons {

	typedef uint32_t StateType;

	enum { MAXIMUM_BUTTONS = sizeof( StateType ) * 8 };


	Buttons( char const* const buttonString, bool const activeHigh );
	~Buttons();


	inline uint8_t const GetButtons() const;


	inline unsigned int const GetDebouncing() const;
	inline void SetDebouncing( unsigned int const debouncingIterations );


	inline bool const GetPressed( uint8_t const index ) const;


	bool const Update();


private:

	uint8_t m_buttons;
	char m_buttonPinNames[          MAXIMUM_BUTTONS ];
	uint8_t m_buttonPinBits[        MAXIMUM_BUTTONS ];
	uint8_t volatile* m_buttonPins[ MAXIMUM_BUTTONS ];

	bool m_activeHigh;

	unsigned int m_debouncingIterations;

	StateType m_state;
};




//============================================================================
//    Buttons inline methods
//============================================================================


unsigned int const Buttons::GetDebouncing() const {

	return m_debouncingIterations;
}


void Buttons::SetDebouncing( unsigned int const debouncingIterations ) {

	assert( debouncingIterations > 0 );

	// save and clear the interrupt flag
	uint8_t const sreg = SREG;
	cli();

	m_debouncingIterations = debouncingIterations;

	// restore the interrupt flag
	SREG = sreg;
}


uint8_t const Buttons::GetButtons() const {

	return m_buttons;
}


bool const Buttons::GetPressed( uint8_t const index ) const {

	return( ( m_state & ( static_cast< StateType >( 1 ) << index ) ) != 0 );
}




#endif    /* __cplusplus */

#endif    /* __BUTTONS_HH__ */

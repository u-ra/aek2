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
	\file usb_hid_keyboard.cc
	\brief USB::HID::Keyboard implementation
*/




#include "usb.hh"
#include "helpers.h"

#include <inttypes.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>




//============================================================================
//    HID keyboard report descriptor macros
//============================================================================


// between 1 and 8
#define LEDS ( 5 )




namespace {




//============================================================================
//    HID keyboard report descriptor
//============================================================================


static_assert( ( LEDS >  0 ), "LEDS must be at least 1" );
static_assert( ( LEDS <= 8 ), "LEDS must be at most 8"  );


extern uint8_t const g_HIDReportDescriptor[] __attribute__(( __progmem__ ));
uint8_t const g_HIDReportDescriptor[] = {

// ----  modifiers  -----------------------------------------------------------
	0x05, 0x07,            // usage page = keyboard
	0x19, 0xe0,            // usage minimum = 224 (left control)
	0x29, 0xe7,            // usage maximum = 231 (right GUI)
	0x15, 0x00,            // logical minimum = 0
	0x25, 0x01,            // logical maximum = 1
	0x75, 0x01,            // report size = 1
	0x95, 0x08,            // report count = 8
	0x81, 0x02,            // input (data, variable, absolute, no wrap, linear, preferred state, no null position, non volatile, bitfield)
// ----------------------------------------------------------------------------

// ----  padding  -------------------------------------------------------------
	//0x75, 0x01,            // report size = 1
	//0x95, 0x08,            // report count = 8
	0x81, 0x01,            // input (constant, array, absolute, no wrap, linear, preferred state, no null position, non volatile, bitfield)
// ----------------------------------------------------------------------------

// ----  LEDs  ----------------------------------------------------------------
	0x05, 0x08,            // usage page = LEDs
	0x19, 0x01,            // usage minimum = 1
	0x29, LEDS,            // usage maximum
	//0x15, 0x00,            // logical minimum = 0
	//0x25, 0x01,            // logical maximum = 1
	//0x75, 0x01,            // report size = 1
	0x95, LEDS,            // report count
	0x91, 0x02,            // output (data, variable, absolute, no wrap, linear, preferred state, no null position, non volatile, bitfield)
// ----------------------------------------------------------------------------

// ----  padding  -------------------------------------------------------------
#if ( LEDS < 8 )
	//0x75, 0x01,            // report size = 1
	0x95, ( 8 - LEDS ),    // report count
	0x91, 0x01,            // output (constant, array, absolute, no wrap, linear, preferred state, no null position, non volatile, bitfield)
#endif    // ( LEDS < 8 )
// ----------------------------------------------------------------------------

// ----  keys  ----------------------------------------------------------------
	0x05, 0x07,            // usage page = keyboard
	0x19, 0x00,            // usage minimum = 0
	0x2a, 0xff, 0x00,      // usage maximum = 255
	//0x15, 0x00,            // logical minimum = 0
	0x26, 0xff, 0x00,      // logical maximum = 255
	0x75, 0x08,            // report size = 8
	0x95, 0x06,            // report count = 6
	0x81, 0x00,            // input (data, array, absolute, no wrap, linear, preferred state, no null position, non volatile, bitfield)
// ----------------------------------------------------------------------------

};




}    // anomymous namespace




namespace USB {


namespace HID {




//============================================================================
//    USB::HID::Keyboard methods
//============================================================================


Keyboard::Keyboard( uint8_t const interfaceString ) :
	Interface(
		0x03,    // HID
		0x01,    // boot
		0x01,    // keyboard
		interfaceString,
		8,
		1
	),
	m_modifiers( 0 ),
	m_leds( 0 ),
	m_oldModifiers( 0 )
{
	memset( m_keys,    0, ARRAYLENGTH( m_keys    ) );
	memset( m_oldKeys, 0, ARRAYLENGTH( m_oldKeys ) );

	m_report = RegisterReportProgmem(
		g_HIDReportDescriptor,
		ARRAYLENGTH( g_HIDReportDescriptor ),
		( REPORT_FLAG_SEND | REPORT_FLAG_RECEIVE | REPORT_FLAG_IDLE ),
		0x01,    // usage page = generic desktop controls
		0x06,    // usage = keyboard
		125
	);
}


Keyboard::~Keyboard() {
}


bool const Keyboard::GetLED( LED const led ) const {

	bool found = false;

	// save and clear the interrupt flag
	uint8_t const sreg = SREG;
	cli();

	if ( ( led >= 1 ) && ( led <= LEDS ) )
		found = ( ( m_leds & ( 1 << ( led - 1 ) ) ) != 0 );

	// restore the interrupt flag
	SREG = sreg;

	return found;
}


bool const Keyboard::GetKey( Key const key ) const {

	bool found = false;
	if ( ( key >= 0xe0 ) && ( key <= 0xe7 ) )
		found = ( ( m_modifiers & ( 1 << ( key - 0xe0 ) ) ) != 0 );
	else if ( ( key >= 0x00 ) && ( key <= 0xff ) ) {

		for ( unsigned int ii = 0; ( ! found ) && ( ii < ARRAYLENGTH( m_keys ) ); ++ii )
			found = ( m_keys[ ii ] == key );
	}
	return found;
}


bool const Keyboard::PressKey( Key const key ) {

	bool found = false;

	// save and clear the interrupt flag
	uint8_t const sreg = SREG;
	cli();

	if ( ( key >= 0xe0 ) && ( key <= 0xe7 ) ) {

		m_modifiers |= ( 1 << ( key - 0xe0 ) );
		found = true;
	}
	else if ( ( key >= 0x00 ) && ( key <= 0xff ) ) {

		for ( unsigned int ii = 0; ( ! found ) && ( ii < ARRAYLENGTH( m_keys ) ); ++ii )
			found = ( m_keys[ ii ] == key );

		if ( ! found ) {

			for ( unsigned int ii = 0; ii < ARRAYLENGTH( m_keys ); ++ii ) {

				if ( m_keys[ ii ] == 0 ) {

					m_keys[ ii ] = key;
					found = true;
					break;
				}
			}

			/*
				The spec says that we should handle more than 6 keypresses by
				doing something involving KEY_ERROR_ROLLOVER. Since we want to
				be able to fall back to KeyboardExtension, we don't do this,
				and instead return false.
			*/
		}
	}

	// restore the interrupt flag
	SREG = sreg;

	return found;
}


void Keyboard::ReleaseKey( Key const key ) {

	// save and clear the interrupt flag
	uint8_t const sreg = SREG;
	cli();

	if ( ( key >= 0xe0 ) && ( key <= 0xe7 ) )
		m_modifiers &= ~( 1 << ( key - 0xe0 ) );
	else if ( ( key >= 0x00 ) && ( key <= 0xff ) ) {

		for ( unsigned int ii = 0; ii < ARRAYLENGTH( m_keys ); ++ii )
			if ( m_keys[ ii ] == key )
				m_keys[ ii ] = 0;
	}

	// restore the interrupt flag
	SREG = sreg;
}


bool const Keyboard::IsChangedReport( uint8_t const report ) const {

	bool result = false;
	if ( report == m_report ) {

		result = ( m_modifiers != m_oldModifiers );
		for ( unsigned int ii = 0; ( ! result ) && ( ii < ARRAYLENGTH( m_keys ) ); ++ii )
			result = ( m_keys[ ii ] != m_oldKeys[ ii ] );
	}
	return result;
}


void Keyboard::SendReport( uint8_t const report ) {

	if ( report == m_report ) {

		UEDATX = m_modifiers;
		m_oldModifiers = m_modifiers;

		UEDATX = 0;

		for ( unsigned int ii = 0; ii < ARRAYLENGTH( m_keys ); ++ii ) {

			UEDATX = m_keys[ ii ];
			m_oldKeys[ ii ] = m_keys[ ii ];
		}
	}
}


void Keyboard::ReceiveReport( uint8_t const report ) {

	if ( report == m_report )
		m_leds = UEDATX;
}




}    // namespace HID


}    // namespace USB

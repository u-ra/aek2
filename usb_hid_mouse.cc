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
	\file usb_hid_mouse.cc
	\brief USB::HID::Mouse implementation
*/




#include "usb.hh"
#include "helpers.h"

#include <inttypes.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>




//============================================================================
//    HID mouse report descriptor macros
//============================================================================


// between 1 and 8
#define BUTTONS ( 3 )

// use 16 bits for x and y, instead of 8 bits?
//#define LONG_AXES

// use 16 bits for wheel, instead of 8 bits?
//#define LONG_WHEEL




namespace {




//============================================================================
//    HID mouse report descriptor
//============================================================================


static_assert( ( BUTTONS >  0 ), "BUTTONS must be at least 1" );
static_assert( ( BUTTONS <= 8 ), "BUTTONS must be at most 8"  );


extern uint8_t const g_HIDReportDescriptor[] __attribute__(( __progmem__ ));
uint8_t const g_HIDReportDescriptor[] = {

	//0x05, 0x01,               // usage page = generic desktop controls
	0x09, 0x01,               // usage = pointer
	0xa1, 0x00,               // collection = physical

// ----  buttons  -------------------------------------------------------------
	0x05, 0x09,               //     usage page = buttons
	0x19, 0x01,               //     usage minimum = button 1
	0x29, BUTTONS,            //     usage maximum
	0x15, 0x00,               //     logical minimum = 0
	0x25, 0x01,               //     logical maximum = 1
	0x75, 0x01,               //     report size = 1
	0x95, BUTTONS,            //     report count
	0x81, 0x02,               //     input (data, variable, absolute, no wrap, linear, preferred state, no null position, non volatile, bitfield)
// ----------------------------------------------------------------------------

// ----  padding  -------------------------------------------------------------
#if ( BUTTONS < 8 )
	//0x75, 0x01,               //     report size = 1
	0x95, ( 8 - BUTTONS ),    //     report count
	0x81, 0x01,               //     input (constant, array, absolute, no wrap, linear, preferred state, no null position, non volatile, bitfield)
#endif    // ( BUTTONS < 8 )
// ----------------------------------------------------------------------------

// ----  x and y  -------------------------------------------------------------
	0x05, 0x01,               //     usage page = generic desktop controls
	0x09, 0x30,               //     usage = direction-x
	0x09, 0x31,               //     usage = direction-y
#ifdef LONG_AXES
	0x16, 0x01, 0x80,         //     logical minimum = -32767
	0x26, 0xff, 0x7f,         //     logical maximum = 32767
	0x75, 0x10,               //     report size = 16
#else         // LONG_AXES
	0x15, 0x81,               //     logical minimum = -127
	0x25, 0x7f,               //     logical maximum = 127
	0x75, 0x08,               //     report size = 8
#endif         // LONG_AXES
	0x95, 0x02,               //     report count = 2
	0x81, 0x06,               //     input (data, variable, relative, no wrap, linear, preferred state, no null position, non volatile, bitfield)
// ----------------------------------------------------------------------------

// ----  wheel  ---------------------------------------------------------------
	//0x05, 0x01,               //     usage page = generic desktop controls
	0x09, 0x38,               //     usage = wheel
#if ( defined( LONG_AXES ) != defined( LONG_WHEEL ) )
#ifdef LONG_WHEEL
	0x16, 0x01, 0x80,         //     logical minimum = -32767
	0x26, 0xff, 0x7f,         //     logical maximum = 32767
	0x75, 0x10,               //     report size = 16
#else         // LONG_WHEEL
	0x15, 0x81,               //     logical minimum = -127
	0x25, 0x7f,               //     logical maximum = 127
	0x75, 0x08,               //     report size = 8
#endif         // LONG_WHEEL
#endif    // ( defined( LONG_AXES ) != defined( LONG_WHEEL ) )
	0x95, 0x01,               //     report count = 1
	0x81, 0x06,               //     input (data, variable, relative, no wrap, linear, preferred state, no null position, non volatile, bitfield)
// ----------------------------------------------------------------------------

	0xc0,                     // end collection

};




}    // anomymous namespace




namespace USB {


namespace HID {




//============================================================================
//    USB::HID::Mouse methods
//============================================================================


Mouse::Mouse( uint8_t const interfaceString ) :
	Interface(
		0x03,    // HID
		0x01,    // boot
		0x02,    // mouse
		interfaceString,
		(
			1
			+
#ifdef LONG_AXES
			4
#else    // LONG_AXES
			2
#endif    // LONG_AXES
			+
#ifdef LONG_WHEEL
			2
#else    // LONG_WHEEL
			1
#endif    // LONG_WHEEL
		),
		1
	),
	m_buttons( 0 ),
	m_xx( 0 ),
	m_yy( 0 ),
	m_wheel( 0 ),
	m_oldButtons( 0 )
{
	m_report = RegisterReportProgmem(
		g_HIDReportDescriptor,
		ARRAYLENGTH( g_HIDReportDescriptor ),
		( REPORT_FLAG_SEND | REPORT_FLAG_IDLE ),
		0x01,    // usage page = generic desktop controls
		0x02,    // usage = mouse
		0
	);
}


Mouse::~Mouse() {
}


bool const Mouse::GetButton( Button const button ) const {

	bool found = false;
	if ( ( button >= 1 ) && ( button <= BUTTONS ) )
		found = ( ( m_buttons & ( 1 << ( button - 1 ) ) ) != 0 );
	return found;
}


bool const Mouse::PressButton( Button const button ) {

	// save and clear the interrupt flag
	uint8_t sreg = SREG;
	cli();

	bool found = false;
	if ( ( button >= 1 ) && ( button <= BUTTONS ) ) {

		m_buttons |= ( 1 << ( button - 1 ) );
		found = true;
	}

	// restore the interrupt flag
	SREG = sreg;

	return found;
}


void Mouse::ReleaseButton( Button const button ) {

	// save and clear the interrupt flag
	uint8_t sreg = SREG;
	cli();

	if ( ( button >= 1 ) && ( button <= BUTTONS ) )
		m_buttons &= ~( 1 << ( button - 1 ) );

	// restore the interrupt flag
	SREG = sreg;
}


void Mouse::Move( int const xx, int const yy, int const wheel ) {

	// save and clear the interrupt flag
	uint8_t sreg = SREG;
	cli();

	m_xx    += xx;
	m_yy    += yy;
	m_wheel += wheel;

	// restore the interrupt flag
	SREG = sreg;
}


bool const Mouse::IsChangedReport( uint8_t const report ) const {

	bool result = false;
	if ( report == m_report ) {

		if ( GetProtocol() == 0 )
			result = ( ( ( m_buttons & 0x07 ) != ( m_oldButtons & 0x07 ) ) || ( m_xx != 0 ) || ( m_yy != 0 ) );
		else
			result = ( ( m_buttons != m_oldButtons ) || ( m_xx != 0 ) || ( m_yy != 0 ) || ( m_wheel != 0 ) );
	}
	return result;
}


void Mouse::SendReport( uint8_t const report ) {

	if ( report == m_report ) {

		if ( GetProtocol() == 0 ) {

			uint8_t const buttons = ( m_buttons & 0x07 );
			UEDATX = buttons;
			m_oldButtons = buttons;

			int8_t const xx = ( ( m_xx < -127 ) ? -127 : ( ( m_xx > 127 ) ? 127 : m_xx ) );
			int8_t const yy = ( ( m_yy < -127 ) ? -127 : ( ( m_yy > 127 ) ? 127 : m_yy ) );
			UEDATX = xx;
			UEDATX = yy;
			m_xx -= xx;
			m_yy -= yy;

			// **TODO: according to the spec, we're allowed to send more bytes, but should expect them to be ignored. Would there be any point in sticking the wheel in?
		}
		else {

			UEDATX = m_buttons;
			m_oldButtons = m_buttons;

#ifdef LONG_AXES
			int16_t const xx = ( ( m_xx < -32767 ) ? -32767 : ( ( m_xx > 32767 ) ? 32767 : m_xx ) );
			int16_t const yy = ( ( m_yy < -32767 ) ? -32767 : ( ( m_yy > 32767 ) ? 32767 : m_yy ) );
			UEDATX = LSB( xx );
			UEDATX = MSB( xx );
			UEDATX = LSB( yy );
			UEDATX = MSB( yy );
#else    // LONG_AXES
			int8_t const xx = ( ( m_xx < -127 ) ? -127 : ( ( m_xx > 127 ) ? 127 : m_xx ) );
			int8_t const yy = ( ( m_yy < -127 ) ? -127 : ( ( m_yy > 127 ) ? 127 : m_yy ) );
			UEDATX = xx;
			UEDATX = yy;
#endif    // LONG_AXES
			m_xx -= xx;
			m_yy -= yy;

#ifdef LONG_WHEEL
			int16_t const wheel = ( ( m_wheel < -32767 ) ? -32767 : ( ( m_wheel > 32767 ) ? 32767 : m_wheel ) );
			UEDATX = LSB( wheel );
			UEDATX = MSB( wheel );
#else    // LONG_WHEEL
			int8_t const wheel = ( ( m_wheel < -127 ) ? -127 : ( ( m_wheel > 127 ) ? 127 : m_wheel ) );
			UEDATX = wheel;
#endif    // LONG_WHEEL
			m_wheel -= wheel;
		}
	}
}


void Mouse::ReceiveReport( uint8_t const report ) {
}




}    // namespace HID


}    // namespace USB

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
	\file usb_hid_keyboard_extension.cc
	\brief USB::HID::KeyboardExtension implementation
*/




#include "usb.hh"
#include "helpers.h"

#include <inttypes.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>




namespace {




//============================================================================
//    HID report descriptors
//============================================================================


extern uint8_t const g_HIDKeyboard1ReportDescriptor[] __attribute__(( __progmem__ ));
uint8_t const g_HIDKeyboard1ReportDescriptor[] = {

// ----  keys  ----------------------------------------------------------------
	0x05, 0x07,          // usage page = keyboard
	0x19, 0x04,          // usage minimum = 4 (A)
	0x29, 0x3b,          // usage maximum = 59
	0x15, 0x00,          // logical minimum = 0
	0x25, 0x01,          // logical maximum = 1
	0x75, 0x01,          // report size = 1
	0x95, 0x38,          // report count = 56
	0x81, 0x02,          // input (data, variable, absolute, no wrap, linear, preferred state, no null position, non volatile, bitfield)
// ----------------------------------------------------------------------------

};


extern uint8_t const g_HIDKeyboard2ReportDescriptor[] __attribute__(( __progmem__ ));
uint8_t const g_HIDKeyboard2ReportDescriptor[] = {

// ----  keys  ----------------------------------------------------------------
	0x05, 0x07,          // usage page = keyboard
	0x19, 0x3c,          // usage minimum = 60
	0x29, 0x73,          // usage maximum = 115 (F24)
	0x15, 0x00,          // logical minimum = 0
	0x25, 0x01,          // logical maximum = 1
	0x75, 0x01,          // report size = 1
	0x95, 0x38,          // report count = 56
	0x81, 0x02,          // input (data, variable, absolute, no wrap, linear, preferred state, no null position, non volatile, bitfield)
// ----------------------------------------------------------------------------

};


extern uint8_t const g_HIDConsumerReportDescriptor[] __attribute__(( __progmem__ ));
uint8_t const g_HIDConsumerReportDescriptor[] = {

// ----  consumer  ------------------------------------------------------------
	//0x05, 0x0c,          // usage page = consumer
	0x19, 0x00,          // usage minimum = 0
	0x2a, 0xff, 0x0f,    // usage maximum = 4095
	//0x15, 0x00,          // logical minimum = 0
	0x26, 0xff, 0x0f,    // logical maximum = 4095
	0x75, 0x10,          // report size = 16
	0x95, 0x01,          // report count = 1
	0x81, 0x00,          // input (data, array, absolute, no wrap, linear, preferred state, no null position, non volatile, bitfield)
// ----------------------------------------------------------------------------

};




}    // anomymous namespace




namespace USB {


namespace HID {




//============================================================================
//    USB::HID::KeyboardExtension methods
//============================================================================


KeyboardExtension::KeyboardExtension( uint8_t const interfaceString ) :
	Interface(
		0x03,    // HID
		0x00,    // none
		0x00,    // none
		interfaceString,
		8,
		1
	),
	m_consumer( 0 ),
	m_oldConsumer( 0 )
{
	memset( m_keys,    0, ARRAYLENGTH( m_keys    ) );
	memset( m_oldKeys, 0, ARRAYLENGTH( m_oldKeys ) );

	m_keyboard1Report = RegisterReportProgmem(
		g_HIDKeyboard1ReportDescriptor,
		ARRAYLENGTH( g_HIDKeyboard1ReportDescriptor ),
		( REPORT_FLAG_SEND | REPORT_FLAG_IDLE ),
		0x01,    // usage page = generic desktop controls
		0x06,    // usage = keyboard
		125
	);

	m_keyboard2Report = RegisterReportProgmem(
		g_HIDKeyboard2ReportDescriptor,
		ARRAYLENGTH( g_HIDKeyboard2ReportDescriptor ),
		( REPORT_FLAG_SEND | REPORT_FLAG_IDLE ),
		0x01,    // usage page = generic desktop controls
		0x06,    // usage = keyboard
		125
	);

	m_consumerReport = RegisterReportProgmem(
		g_HIDConsumerReportDescriptor,
		ARRAYLENGTH( g_HIDConsumerReportDescriptor ),
		( REPORT_FLAG_SEND | REPORT_FLAG_IDLE ),
		0x0c,    // usage page = consumer
		0x01,    // usage = consumer control
		125
	);
}


KeyboardExtension::~KeyboardExtension() {
}


bool const KeyboardExtension::GetKey( Key const key ) const {

	bool found = false;
	if ( ( key >= 0x04 ) && ( key <= 0x73 ) ) {

		uint8_t const index = ( key - 0x04 );
		found = ( ( m_keys[ index >> 3 ] & ( 1 << ( index & 7 ) ) ) != 0 );
	}
	return found;
}


bool const KeyboardExtension::PressKey( Key const key ) {

	bool found = false;

	// save and clear the interrupt flag
	uint8_t const sreg = SREG;
	cli();

	if ( ( key >= 0x04 ) && ( key <= 0x73 ) ) {

		if ( ( key != KEY_CAPS_LOCK ) && ( key != KEY_SCROLL_LOCK ) && ( key != KEY_NUM_LOCK ) ) {    // **NOTE: workaround for linux bug which prevents LED state from being updated when locking keys are pressed from this interface

			uint8_t const index = ( key - 0x04 );
			m_keys[ index >> 3 ] |= ( 1 << ( index & 7 ) );
			found = true;
		}
	}

	// restore the interrupt flag
	SREG = sreg;

	return found;
}


void KeyboardExtension::ReleaseKey( Key const key ) {

	// save and clear the interrupt flag
	uint8_t const sreg = SREG;
	cli();

	if ( ( key >= 0x04 ) && ( key <= 0x73 ) ) {

		uint8_t const index = ( key - 0x04 );
		m_keys[ index >> 3 ] &= ~( 1 << ( index & 7 ) );
	}

	// restore the interrupt flag
	SREG = sreg;
}


bool const KeyboardExtension::GetConsumer( Consumer const consumer ) const {

	return( m_consumer == static_cast< uint16_t >( consumer ) );
}


bool const KeyboardExtension::PressConsumer( Consumer const consumer ) {

	bool found = false;

	// save and clear the interrupt flag
	uint8_t const sreg = SREG;
	cli();

	if ( ( consumer >= 0x00 ) && ( consumer <= 0x0fff ) ) {

		if ( m_consumer == 0 )
			m_consumer = consumer;

		found = ( m_consumer == static_cast< uint16_t >( consumer ) );
	}

	// restore the interrupt flag
	SREG = sreg;

	return found;
}


void KeyboardExtension::ReleaseConsumer( Consumer const consumer ) {

	// save and clear the interrupt flag
	uint8_t const sreg = SREG;
	cli();

	if ( m_consumer == static_cast< uint16_t >( consumer ) )
		m_consumer = 0;

	// restore the interrupt flag
	SREG = sreg;
}


bool const KeyboardExtension::IsChangedReport( uint8_t const report ) const {

	bool result = false;
	if ( report == m_keyboard1Report ) {

		for ( unsigned int ii = 0; ( ! result ) && ( ii < 7 ); ++ii )
			result = ( m_keys[ ii ] != m_oldKeys[ ii ] );
	}
	if ( report == m_keyboard2Report ) {

		for ( unsigned int ii = 7; ( ! result ) && ( ii < 14 ); ++ii )
			result = ( m_keys[ ii ] != m_oldKeys[ ii ] );
	}
	else if ( report == m_consumerReport )
		result = ( m_consumer != m_oldConsumer );
	return result;
}


void KeyboardExtension::SendReport( uint8_t const report ) {

	if ( report == m_keyboard1Report ) {

		for ( unsigned int ii = 0; ii < 7; ++ii ) {

			UEDATX = m_keys[ ii ];
			m_oldKeys[ ii ] = m_keys[ ii ];
		}
	}
	if ( report == m_keyboard2Report ) {

		for ( unsigned int ii = 7; ii < 14; ++ii ) {

			UEDATX = m_keys[ ii ];
			m_oldKeys[ ii ] = m_keys[ ii ];
		}
	}
	else if ( report == m_consumerReport ) {

		UEDATX = LSB( m_consumer );
		UEDATX = MSB( m_consumer );
		m_oldConsumer = m_consumer;
	}
}


void KeyboardExtension::ReceiveReport( uint8_t const report ) {
}




}    // namespace HID


}    // namespace USB

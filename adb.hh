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
	\file adb.hh
	\brief ADB implementation
*/




#ifndef __ADB_HH__
#define __ADB_HH__

#ifdef __cplusplus




#include "timer.hh"
#include "pins.h"
#include "helpers.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdlib.h>
#include <inttypes.h>




//============================================================================
//    ADB class
//============================================================================


struct ADB {

	enum {
		KEY_A = 0x00,
		KEY_S,
		KEY_D,
		KEY_F,
		KEY_H,
		KEY_G,
		KEY_Z,
		KEY_X,
		KEY_C,
		KEY_V,
		KEY_B = 0x0b,
		KEY_Q,
		KEY_W,
		KEY_E,
		KEY_R,
		KEY_Y,
		KEY_T,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_6,
		KEY_5,
		KEY_EQUALS,
		KEY_9,
		KEY_7,
		KEY_DASH,
		KEY_8,
		KEY_0,
		KEY_RIGHT_BRACKET,
		KEY_O,
		KEY_U,
		KEY_LEFT_BRACKET,
		KEY_I,
		KEY_P,
		KEY_ENTER,
		KEY_L,
		KEY_J,
		KEY_QUOTE,
		KEY_K,
		KEY_SEMICOLON,
		KEY_BACKSLASH,
		KEY_COMMA,
		KEY_SLASH,
		KEY_N,
		KEY_M,
		KEY_PERIOD,
		KEY_TAB,
		KEY_SPACEBAR,
		KEY_BACKQUOTE,
		KEY_BACKSPACE,
		KEY_ESCAPE = 0x35,
		KEY_LEFT_CONTROL,
		KEY_COMMAND,
		KEY_LEFT_SHIFT,
		KEY_LOCKING_CAPS_LOCK,
		KEY_LEFT_OPTION,
		KEY_LEFT_ARROW,
		KEY_RIGHT_ARROW,
		KEY_DOWN_ARROW,
		KEY_UP_ARROW,
		KEY_FUNCTION,
		KEY_KEYPAD_PERIOD = 0x41,
		KEY_KEYPAD_STAR = 0x43,
		KEY_KEYPAD_PLUS = 0x45,
		KEY_KEYPAD_CLEAR = 0x47,
		KEY_KEYPAD_SLASH = 0x4b,
		KEY_KEYPAD_ENTER,
		KEY_KEYPAD_DASH = 0x4e,
		KEY_KEYPAD_EQUALS = 0x51,
		KEY_KEYPAD_0,
		KEY_KEYPAD_1,
		KEY_KEYPAD_2,
		KEY_KEYPAD_3,
		KEY_KEYPAD_4,
		KEY_KEYPAD_5,
		KEY_KEYPAD_6,
		KEY_KEYPAD_7,
		KEY_KEYPAD_8 = 0x5b,
		KEY_KEYPAD_9,
		KEY_F5 = 0x60,
		KEY_F6,
		KEY_F7,
		KEY_F3,
		KEY_F8,
		KEY_F9,
		KEY_F11 = 0x67,
		KEY_F13 = 0x69,
		KEY_F14 = 0x6b,
		KEY_F10 = 0x6d,
		KEY_F12 = 0x6f,
		KEY_F15 = 0x71,
		KEY_INSERT,
		KEY_HOME,
		KEY_PAGE_UP,
		KEY_DELETE,
		KEY_F4,
		KEY_END,
		KEY_F2,
		KEY_PAGE_DOWN,
		KEY_F1,
		KEY_RIGHT_SHIFT,
		KEY_RIGHT_OPTION,
		KEY_RIGHT_CONTROL,
		KEY_POWER = 0x7f
	};

	enum {
		LED_NUM_LOCK    = 0x01,
		LED_CAPS_LOCK   = 0x02,
		LED_SCROLL_LOCK = 0x04
	};


	ADB( char const* const adbString );
	~ADB();


	bool const UpdateKeyboard();

	bool const GetPressed( uint8_t const key ) const;

	void SetLEDs( uint8_t const leds );


	void Reset();


private:

	enum ResultCode {
		RESULT_FAILURE = 0,
		RESULT_SUCCESS,
		RESULT_SERVICE
	};

	enum CommandCode {
		COMMAND_RESET = 0,
		COMMAND_FLUSH = 1
	};


	inline void WriteBit( Timer const* const pTimer, uint16_t const lowTicks, uint16_t const highTicks ) const;
	inline void WriteByte( uint8_t const byte, Timer const* const pTimer, uint16_t const shortTicks, uint16_t const longTicks ) const;

	inline bool const WaitLow( Timer const* const pTimer, uint16_t const ticks ) const;
	inline bool const WaitHigh( Timer const* const pTimer, uint16_t const ticks ) const;

	inline bool const ReadBit( bool* const bit, Timer const* const pTimer, uint16_t const bitTicks ) const;
	inline bool const ReadByte( uint8_t* const byte, Timer const* const pTimer, uint16_t const bitTicks ) const;


	ResultCode const WriteRegister( uint8_t const address, uint8_t const index, uint8_t const* const data, uint8_t const size ) const;
	ResultCode const ReadRegister( uint8_t const address, uint8_t const index, uint8_t* const data, uint8_t const size ) const;

	ResultCode const SendCommand( uint8_t const address, CommandCode const command ) const;


	char m_adbPinName;
	uint8_t m_adbPinBit;
	uint8_t volatile* m_adbPin;
	uint8_t volatile* m_adbDDR;
	uint8_t volatile* m_adbPort;

	uint16_t m_keys[ 8 ];    ///< ADB supports 128 keyboard scan codes
	uint8_t m_leds;
};




//============================================================================
//    ADB inline methods
//============================================================================


void ADB::WriteBit( Timer const* const pTimer, uint16_t const lowTicks, uint16_t const highTicks ) const {

	*m_adbDDR  |=  ( 1u << m_adbPinBit );    // direction = output
	*m_adbPort &= ~( 1u << m_adbPinBit );    // value = low

	pTimer->DelayTicks( lowTicks );

	*m_adbPort |=  ( 1u << m_adbPinBit );    // value = high
	*m_adbDDR  &= ~( 1u << m_adbPinBit );    // direction = input

	pTimer->DelayTicks( highTicks );
}


void ADB::WriteByte( uint8_t const byte, Timer const* const pTimer, uint16_t const shortTicks, uint16_t const longTicks ) const {

	for ( int ii = 7; ii >= 0; --ii ) {

		if ( ( byte & ( 1u << ii ) ) == 0 )
			WriteBit( pTimer, longTicks, shortTicks );
		else
			WriteBit( pTimer, shortTicks, longTicks );
	}
}


bool const ADB::WaitLow( Timer const* const pTimer, uint16_t const ticks ) const {

	bool success = false;

	for ( uint16_t const startTicks = pTimer->GetTicks(); ( pTimer->GetTicks() - startTicks ) < ticks; ) {

		if ( ( *m_adbPin & ( 1u << m_adbPinBit ) ) == 0 ) {

			success = true;
			break;
		}
	}

	return success;
}


bool const ADB::WaitHigh( Timer const* const pTimer, uint16_t const ticks ) const {

	bool success = false;

	for ( uint16_t const startTicks = pTimer->GetTicks(); ( pTimer->GetTicks() - startTicks ) < ticks; ) {

		if ( ( *m_adbPin & ( 1u << m_adbPinBit ) ) != 0 ) {

			success = true;
			break;
		}
	}

	return success;
}


bool const ADB::ReadBit( bool* const bit, Timer const* const pTimer, uint16_t const bitTicks ) const {

	bool success = false;

	if ( ( *m_adbPin & ( 1u << m_adbPinBit ) ) == 0 ) {

		uint16_t const startTicks = pTimer->GetTicks();

		uint16_t lowTicks = startTicks;
		do {

			if ( ( *m_adbPin & ( 1u << m_adbPinBit ) ) != 0 )
				break;

			lowTicks = pTimer->GetTicks();

		} while ( ( lowTicks - startTicks ) < bitTicks );

		if ( ( *m_adbPin & ( 1u << m_adbPinBit ) ) != 0 ) {

			success = false;

			uint16_t highTicks = lowTicks;
			do {

				if ( ( *m_adbPin & ( 1u << m_adbPinBit ) ) == 0 )
					break;

				highTicks = pTimer->GetTicks();

			} while ( ( highTicks - startTicks ) < bitTicks );

			if ( ( *m_adbPin & ( 1u << m_adbPinBit ) ) == 0 ) {

				if ( ( lowTicks - startTicks ) >= ( highTicks - lowTicks ) )
					*bit = false;
				else
					*bit = true;

				success = true;
			}
		}
	}

	return success;
}


bool const ADB::ReadByte( uint8_t* const byte, Timer const* const pTimer, uint16_t const bitTicks ) const {

	bool success = true;

	uint8_t result = 0;
	for ( int ii = 7; success && ( ii >= 0 ); --ii ) {

		bool bit = false;
		success = ReadBit( &bit, pTimer, bitTicks );
		if ( bit )
			result |= ( 1u << ii );
	}
	if ( success )
		*byte = result;

	return success;
}




#endif    /* __cplusplus */

#endif    /* __ADB_HH__ */

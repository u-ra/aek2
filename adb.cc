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
	\file adb.cc
	\brief ADB implementation
*/




#include "adb.hh"

#include <util/delay.h>




//============================================================================
//    ADB methods
//============================================================================


ADB::ADB( char const* const adbString ) :
	m_adbPinName( '\0' ),
	m_adbPinBit( 0 ),
	m_adbPin(  NULL ),
	m_adbDDR(  NULL ),
	m_adbPort( NULL ),
	m_leds( 0xff )
{
	if ( ( adbString[ 0 ] != '\0' ) && ( adbString[ 1 ] != '\0' ) && ( adbString[ 2 ] == '\0' ) ) {

		char const name = adbString[ 0 ];
		char const bit  = adbString[ 1 ] - '0';

		uint8_t volatile* ddr  = NULL;
		uint8_t volatile* port = NULL;
		uint8_t volatile* pin  = NULL;

		if ( PinAllocate( &pin, &ddr, &port, name, bit ) ) {    /// \todo handle errors

			*port |=  ( 1u << bit );    // value = high
			*ddr  &= ~( 1u << bit );    // direction = input

			m_adbPinName = name;
			m_adbPinBit  = bit;
			m_adbPin     = pin;
			m_adbDDR     = ddr;
			m_adbPort    = port;
		}
	}

	Reset();
}


ADB::~ADB() {

	if ( m_adbPinName != '\0' )
		PinFree( m_adbPinName, m_adbPinBit );
}


bool const ADB::UpdateKeyboard() {

	bool changed = false;

	uint8_t data[] = { 0x7f, 0xff };
	while ( ReadRegister( 2, 0, data, sizeof( data ) ) == RESULT_SUCCESS ) {

		if ( ( data[ 0 ] == 0x7f ) && ( data[ 1 ] == 0x7f ) )
			m_keys[ 0x7f >> 4 ] |= ( 1u << ( 0x7f & 15 ) );
		else if ( ( data[ 0 ] == 0xff ) && ( data[ 1 ] == 0xff ) )
			m_keys[ 0x7f >> 4 ] &= ~( 1u << ( 0x7f & 15 ) );
		else {

			for ( unsigned int ii = 0; ii < 2; ++ii ) {

				uint8_t const key = ( data[ ii ] & 0x7f );
				if ( key != 0x7f ) {

					if ( ( data[ ii ] & 0x80 ) == 0 )
						m_keys[ key >> 4 ] |= ( 1u << ( key & 15 ) );
					else
						m_keys[ key >> 4 ] &= ~( 1u << ( key & 15 ) );
				}
			}
		}

		changed = true;
	}

	return changed;
}


bool const ADB::GetPressed( uint8_t const key ) const {

	return( ( m_keys[ key >> 4 ] & ( 1u << ( key & 15 ) ) ) != 0 );
}


void ADB::SetLEDs( uint8_t const leds ) {

	uint8_t const newLEDs = ( leds & 0x07 );
	if ( newLEDs != m_leds ) {

		uint8_t const data[] = { 0, ( newLEDs ^ 0x07 ) };
		if ( ADB::WriteRegister( 2, 2, data, sizeof( data ) ) == RESULT_SUCCESS )
			m_leds = newLEDs;
	}
}


void ADB::Reset() {

	SendCommand( 2, COMMAND_RESET );

	for ( unsigned int ii = 0; ii < ARRAYLENGTH( m_keys ); ++ii )
		m_keys[ ii ] = 0;
	m_leds = 0xff;

	// give the device time to reset
	_delay_ms( 100 );
}


ADB::ResultCode const ADB::WriteRegister( uint8_t const address, uint8_t const index, uint8_t const* const data, uint8_t const size ) const {

	ResultCode result = RESULT_FAILURE;

	if ( m_adbPinName != '\0' ) {

		Timer const* const pTimer = Timer::Instance();

		uint16_t const shortTicks     = Timer::MicrosecondsToTicks( 30  );    // 35
		uint16_t const longTicks      = Timer::MicrosecondsToTicks( 60  );    // 65
		uint16_t const attentionTicks = Timer::MicrosecondsToTicks( 800 );    // 720
		uint16_t const stopStartTicks = Timer::MicrosecondsToTicks( 200 );    // 128

		uint16_t const maximumStopStartTicks = Timer::MicrosecondsToTicks( 260 );

		// save and clear the interrupt flag
		uint8_t const sreg = SREG;
		cli();

		// wait until bus is clear (**FIXME **TODO **HACK: potential infinite loop)
		while ( WaitLow( pTimer, maximumStopStartTicks ) );

		// attention & sync
		WriteBit( pTimer, attentionTicks, longTicks );

		// listen
		WriteByte( ( ( address << 4 ) | ( index & 0x03 ) | 0x08 ), pTimer, shortTicks, longTicks );

		// stop bit
		WriteBit( pTimer, longTicks, shortTicks );

		// bus low = service request
		if ( ( *m_adbPin & ( 1u << m_adbPinBit ) ) == 0 )
			result = RESULT_SERVICE;
		else {

			// stop-to-start time
			pTimer->DelayTicks( stopStartTicks );

			// start bit
			WriteBit( pTimer, shortTicks, longTicks );

			// data
			for ( unsigned int ii = 0; ii < size; ++ii )
				WriteByte( data[ ii ], pTimer, shortTicks, longTicks );

			// stop bit
			WriteBit( pTimer, longTicks, shortTicks );

			result = RESULT_SUCCESS;
		}

		// restore the interrupt flag
		SREG = sreg;
	}

	return result;
}


ADB::ResultCode const ADB::ReadRegister( uint8_t const address, uint8_t const index, uint8_t* const data, uint8_t const size ) const {

	ResultCode result = RESULT_FAILURE;

	if ( m_adbPinName != '\0' ) {

		Timer const* const pTimer = Timer::Instance();

		uint16_t const shortTicks     = Timer::MicrosecondsToTicks( 30  );    // 35
		uint16_t const longTicks      = Timer::MicrosecondsToTicks( 60  );    // 65
		uint16_t const attentionTicks = Timer::MicrosecondsToTicks( 800 );    // 720

		uint16_t const maximumStopStartTicks = Timer::MicrosecondsToTicks( 260 );
		uint16_t const maximumBitTicks       = Timer::MicrosecondsToTicks( 130 );
		uint16_t const maximumHalfBitTicks   = Timer::MicrosecondsToTicks( 91  );

		// save and clear the interrupt flag
		uint8_t const sreg = SREG;
		cli();

		// wait until bus is clear (**FIXME **TODO **HACK: potential infinite loop)
		while ( WaitLow( pTimer, maximumStopStartTicks ) );

		// attention & sync
		WriteBit( pTimer, attentionTicks, longTicks );

		// talk
		WriteByte( ( ( address << 4 ) | ( index & 0x03 ) | 0x0c ), pTimer, shortTicks, longTicks );

		// stop bit
		WriteBit( pTimer, longTicks, shortTicks );

		// bus low = service request
		if ( ( *m_adbPin & ( 1u << m_adbPinBit ) ) == 0 )
			result = RESULT_SERVICE;
		else {

			// stop-to-start time
			bool success = WaitLow( pTimer, maximumStopStartTicks );
			if ( success ) {

				// start bit
				bool bit = false;
				success = ReadBit( &bit, pTimer, maximumBitTicks );
				success &= bit;
				if ( success ) {

					// data
					for ( unsigned int ii = 0; success && ( ii < size ); ++ii )
						success = ReadByte( data + ii, pTimer, maximumBitTicks );

					if ( success ) {

						// stop bit
						if ( WaitHigh( pTimer, maximumHalfBitTicks ) )
							result = RESULT_SUCCESS;
					}
				}
			}
		}

		// restore the interrupt flag
		SREG = sreg;
	}

	return result;
}


ADB::ResultCode const ADB::SendCommand( uint8_t const address, CommandCode const command ) const {

	ResultCode result = RESULT_FAILURE;

	if ( m_adbPinName != '\0' ) {

		Timer const* const pTimer = Timer::Instance();

		uint16_t const shortTicks     = Timer::MicrosecondsToTicks( 30  );    // 35
		uint16_t const longTicks      = Timer::MicrosecondsToTicks( 60  );    // 65
		uint16_t const attentionTicks = Timer::MicrosecondsToTicks( 800 );    // 720

		uint16_t const maximumStopStartTicks = Timer::MicrosecondsToTicks( 260 );

		// save and clear the interrupt flag
		uint8_t const sreg = SREG;
		cli();

		// wait until bus is clear (**FIXME **TODO **HACK: potential infinite loop)
		while ( WaitLow( pTimer, maximumStopStartTicks ) );

		// attention & sync
		WriteBit( pTimer, attentionTicks, longTicks );

		// reset
		WriteByte( ( ( address << 4 ) | command ), pTimer, shortTicks, longTicks );

		// stop bit
		WriteBit( pTimer, longTicks, shortTicks );

		// bus low = service request
		if ( ( *m_adbPin & ( 1u << m_adbPinBit ) ) == 0 )
			result = RESULT_SERVICE;
		else
			result = RESULT_SUCCESS;

		// restore the interrupt flag
		SREG = sreg;
	}

	return result;
}

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


/*
	This file is partly derived from PJRC's usb_serial, usb_keyboard and
	usb_mouse example programs, which I received under the following license:

		Copyright (c) 2008 PJRC.COM, LLC

		Permission is hereby granted, free of charge, to any person obtaining a
		copy of this software and associated documentation files (the
		"Software"), to deal in the Software without restriction, including
		without limitation the rights to use, copy, modify, merge, publish,
		distribute, sublicense, and/or sell copies of the Software, and to
		permit persons to whom the Software is furnished to do so, subject to
		the following conditions:

		The above copyright notice and this permission notice shall be included
		in all copies or substantial portions of the Software.

		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
		OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
		MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
		IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
		CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
		TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
		SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


/**
	\file usb_hid_interface.cc
	\brief USB::HID::Interface implementation
*/




#include "usb.hh"
#include "helpers.h"

#include <inttypes.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>




namespace USB {


namespace HID {




//============================================================================
//    USB::HID::Interface methods
//============================================================================


Interface::~Interface() {
}


bool const Interface::IsChanged() {

	bool changed = false;
	for ( unsigned int ii = 0; ( ! changed ) && ( ii < m_nextReport ); ++ii ) {

		if ( m_reports[ ii ].reportType & REPORT_FLAG_SEND )
			changed |= this->IsChangedReport( ii );
	}
	return changed;
}


Interface::Interface(
	uint8_t const interfaceClass,
	uint8_t const interfaceSubclass,
	uint8_t const interfaceProtocol,
	uint8_t const interfaceString,
	uint16_t const endpointSize,
	uint8_t const endpointInterval
) :
	m_nextReport( 0 ),
	m_protocol( 1 )
{
	memset( m_reports, 0, sizeof( m_reports ) );

	m_interface = Device::Instance()->RegisterInterface(
		this,
		interfaceClass,
		interfaceSubclass,
		interfaceProtocol,
		interfaceString
	);
	if ( m_interface != 0xff ) {

		m_endpoint = Device::Instance()->RegisterEndpoint(
			m_interface,
			( Device::ENDPOINT_FLAG_INTERRUPT | Device::ENDPOINT_FLAG_IN | Device::ENDPOINT_FLAG_DOUBLE_BUFFERED ),
			endpointSize,
			endpointInterval
		);
		if ( m_endpoint == 0xff )
			m_interface = Device::Instance()->UnregisterInterface( m_interface );
	}
}


uint8_t const Interface::RegisterReportProgmem(
	uint8_t const* const descriptor,
	uint8_t const descriptorSize,
	uint8_t const reportType,
	uint8_t const usagePage,
	uint8_t const usage,
	uint8_t const idle
)
{
	uint8_t result = 0xff;
	if ( ( ! Device::Instance()->IsStarted() ) && ( m_nextReport < MAXIMUM_REPORTS ) ) {

		result = m_nextReport;
		m_reports[ m_nextReport ].descriptor     = descriptor;
		m_reports[ m_nextReport ].descriptorSize = descriptorSize;
		m_reports[ m_nextReport ].reportType     = reportType;
		m_reports[ m_nextReport ].usagePage      = usagePage;
		m_reports[ m_nextReport ].usage          = usage;
		m_reports[ m_nextReport ].idle           = idle;
		m_reports[ m_nextReport ].idleCount      = 0;
		++m_nextReport;
	}
	return result;
}


uint8_t const Interface::UnregisterReport( uint8_t const report ) {

	uint8_t result = report;
	if ( ( ! Device::Instance()->IsStarted() ) && ( report + 1 == m_nextReport ) ) {

		--m_nextReport;

		m_reports[ m_nextReport ].descriptor     = NULL;
		m_reports[ m_nextReport ].descriptorSize = 0;
		m_reports[ m_nextReport ].reportType     = 0;
		m_reports[ m_nextReport ].usagePage      = 0;
		m_reports[ m_nextReport ].usage          = 0;
		m_reports[ m_nextReport ].idle           = 0;
		m_reports[ m_nextReport ].idleCount      = 0;

		result = 0xff;
	}
	return result;
}


bool const Interface::ForceSendReport( uint8_t const report, uint16_t const timeout ) {

	bool success = false;
	if ( ( Device::Instance()->GetConfiguration() != 0 ) && ( report < m_nextReport ) && ( m_reports[ report ].reportType & REPORT_FLAG_SEND ) ) {

		// save and clear the interrupt flag
		uint8_t sreg = SREG;
		cli();

		UENUM = m_endpoint;
		uint8_t ueintx = UEINTX;
		if ( ! ( ueintx & ( 1 << RWAL ) ) ) {

			if ( timeout > 0 ) {

				uint8_t const frameStartLow  = UDFNUML;
				uint8_t const frameStartHigh = UDFNUMH;
				uint16_t const frameStart = ( ( uint16_t )frameStartLow | ( ( uint16_t )frameStartHigh << 8 ) );

				do {

					//UEINTX = ( ( 1 << RWAL ) | ( 1 << NAKOUTI ) | ( 1 << RXSTPI ) | ( 1 << STALLEDI ) );

					uint8_t const frameLow  = UDFNUML;
					uint8_t const frameHigh = UDFNUMH;
					uint16_t const frame = ( ( uint16_t )frameLow | ( ( uint16_t )frameHigh << 8 ) );
					if ( ( Device::Instance()->GetConfiguration() == 0 ) || ( frame - frameStart >= timeout ) )
						break;

					// restore the interrupt flag
					SREG = sreg;

					// **TODO: sleep?

					// save and clear the interrupt flag
					sreg = SREG;
					cli();

					UENUM = m_endpoint;
					ueintx = UEINTX;

				} while ( ! ( ueintx & ( 1 << RWAL ) ) );
			}
		}

		if ( ueintx & ( 1 << RWAL ) ) {

			if ( m_nextReport > 1 )
				UEDATX = report + 1;
			this->SendReport( report );
			m_reports[ report ].idleCount = 0;

			UEINTX = ( ( 1 << RWAL ) | ( 1 << NAKOUTI ) | ( 1 << RXSTPI ) | ( 1 << STALLEDI ) );

			success = true;
		}

		// restore the interrupt flag
		SREG = sreg;
	}
	return success;
}


void Interface::HandleIdle( uint8_t const interface ) {

	for ( unsigned int ii = 0; ii < m_nextReport; ++ii ) {

		if ( ( m_reports[ ii ].reportType & REPORT_FLAG_SEND ) && ( m_reports[ ii ].reportType & REPORT_FLAG_IDLE ) ) {

			uint8_t const idle = m_reports[ ii ].idle;

			if ( m_reports[ ii ].idleCount < idle * 4u )
				++m_reports[ ii ].idleCount;

			if ( this->IsChangedReport( ii ) || ( ( idle > 0 ) && ( m_reports[ ii ].idleCount >= idle * 4u ) ) ) {

				UENUM = m_endpoint;
				if ( UEINTX & ( 1 << RWAL ) ) {

					if ( m_nextReport > 1 )
						UEDATX = ii + 1;
					this->SendReport( ii );
					m_reports[ ii ].idleCount = 0;

					UEINTX = ( ( 1 << RWAL ) | ( 1 << NAKOUTI ) | ( 1 << RXSTPI ) | ( 1 << STALLEDI ) );
				}
			}
		}
	}
}


bool const Interface::HandleRequest( uint8_t const bmRequestType, uint8_t const bRequest, uint16_t const wValue, uint16_t const wIndex, uint16_t const wLength ) {

	bool result = false;
	switch( bmRequestType ) {

		case 0x01: {    // standard host to interface

			if ( bRequest == 5 ) {    // set address

				ClearIn();
				WaitIn();
				UDADDR = ( wValue | ( 1 << ADDEN ) );

				result = true;
			}
			break;
		}

		case 0x81: {    // standard interface to host

			if ( bRequest == 0 ) {    // get status

				WaitIn();
				UEDATX = 0;
				UEDATX = 0;
				ClearIn();

				result = true;
			}
			else if ( bRequest == 6 ) {    // get descriptor

				uint8_t* buffer = NULL;
				unsigned int length = 0;

				if ( ( wValue >> 8 ) == 0x21 ) {    // HID configuration descriptor

					length = this->GetConfigurationDescriptor( m_interface, NULL );
					if ( length > 0 ) {

						buffer = static_cast< uint8_t* >( malloc( length ) );
						this->GetConfigurationDescriptor( m_interface, buffer );
					}
				}
				else if ( ( wValue >> 8 ) == 0x22 ) {    // HID report descriptor

					length = 0;
					if ( m_nextReport == 1 )
						length += 6 + m_reports[ 0 ].descriptorSize + 1;
					else if ( m_nextReport > 1 ) {

						for ( unsigned int ii = 0; ii < m_nextReport; ++ii )
							length += 8 + m_reports[ ii ].descriptorSize + 1;
					}

					if ( length > 0 ) {

						uint8_t* pBuffer = static_cast< uint8_t* >( malloc( length ) );
						buffer = pBuffer;

						if ( m_nextReport == 1 ) {

							*( pBuffer++ ) = 0x05; *( pBuffer++ ) = m_reports[ 0 ].usagePage;    // usage page
							*( pBuffer++ ) = 0x09; *( pBuffer++ ) = m_reports[ 0 ].usage;        // usage
							*( pBuffer++ ) = 0xa1; *( pBuffer++ ) = 0x01;                        // collection = application

							uint8_t const* const descriptor = m_reports[ 0 ].descriptor;
							unsigned int const descriptorSize = m_reports[ 0 ].descriptorSize;
							for ( unsigned int ii = 0; ii < descriptorSize; ++ii )
								*( pBuffer++ ) = pgm_read_byte( descriptor + ii );

							*( pBuffer++ ) = 0xc0;    // end collection
						}
						else if ( m_nextReport > 1 ) {

							for ( unsigned int ii = 0; ii < m_nextReport; ++ii ) {

								*( pBuffer++ ) = 0x05; *( pBuffer++ ) = m_reports[ ii ].usagePage;    // usage page
								*( pBuffer++ ) = 0x09; *( pBuffer++ ) = m_reports[ ii ].usage;        // usage
								*( pBuffer++ ) = 0xa1; *( pBuffer++ ) = 0x01;                         // collection = application
								*( pBuffer++ ) = 0x85; *( pBuffer++ ) = ii + 1;                       // report ID

								uint8_t const* const descriptor = m_reports[ ii ].descriptor;
								unsigned int const descriptorSize = m_reports[ ii ].descriptorSize;
								for ( unsigned int jj = 0; jj < descriptorSize; ++jj )
									*( pBuffer++ ) = pgm_read_byte( descriptor + jj );

								*( pBuffer++ ) = 0xc0;    // end collection
							}
						}
					}
				}

				if ( buffer != NULL ) {

					SendBuffer( buffer, Min( length, wLength, 255u ) );
					free( buffer );

					result = true;
				}
			}
			break;
		}

		case 0x21: {    // class host to interface

			switch( bRequest ) {

				case 9: {    // set report

					uint8_t const report = ( wValue & 255 );
					if ( m_nextReport == 1 ) {

						if ( ( report == 0 ) && ( m_reports[ 0 ].reportType & REPORT_FLAG_RECEIVE ) ) {

							WaitOut();
							this->ReceiveReport( 0 );
							ClearOut();
							ClearIn();

							result = true;
						}
					}
					else if ( m_nextReport > 1 ) {

						if ( ( report > 0 ) && ( report <= m_nextReport ) && ( m_reports[ report - 1 ].reportType & REPORT_FLAG_RECEIVE ) ) {

							WaitOut();
							uint8_t const reportAgain = UEDATX;
							if ( reportAgain == report )
								this->ReceiveReport( report - 1 );
							ClearOut();
							ClearIn();

							result = true;
						}
					}
					break;
				}

				case 10: {    // set idle

					uint8_t const report = ( wValue & 255 );
					if ( report == 0 ) {

						uint8_t const idle = ( wValue >> 8 );
						for ( unsigned int ii = 0; ii < m_nextReport; ++ii ) {

							if ( m_reports[ ii ].reportType & REPORT_FLAG_IDLE ) {

								m_reports[ ii ].idle = idle;
								m_reports[ ii ].idleCount = 0;
							}
						}
						ClearIn();

						result = true;
					}
					else if ( ( report <= m_nextReport ) && ( m_reports[ report - 1 ].reportType & REPORT_FLAG_IDLE ) ) {

						m_reports[ report - 1 ].idle = ( wValue >> 8 );
						m_reports[ report - 1 ].idleCount = 0;
						ClearIn();

						result = true;
					}
					break;
				}

				case 11: {    // set protocol

					m_protocol = wValue;
					for ( unsigned int ii = 0; ii < m_nextReport; ++ii )
						m_reports[ ii ].idleCount = 0xffff;    // send a report ASAP
					ClearIn();

					result = true;
					break;
				}
			}
			break;
		}

		case 0xa1: {    // class interface to host

			switch( bRequest ) {

				case 1: {    // get report

					uint8_t const report = ( wValue & 255 );
					if ( m_nextReport == 1 ) {

						if ( ( report == 0 ) && ( m_reports[ 0 ].reportType & REPORT_FLAG_SEND ) ) {

							WaitIn();
							this->SendReport( 0 );
							m_reports[ 0 ].idleCount = 0;
							ClearIn();

							result = true;
						}
					}
					else if ( m_nextReport > 1 ) {

						if ( ( report > 0 ) && ( report <= m_nextReport ) && ( m_reports[ report - 1 ].reportType & REPORT_FLAG_SEND ) ) {

							WaitIn();
							UEDATX = report;
							this->SendReport( report - 1 );
							m_reports[ report - 1 ].idleCount = 0;
							ClearIn();

							result = true;
						}
					}
					break;
				}

				case 2: {    // get idle

					uint8_t const report = ( wValue & 255 );
					if ( m_nextReport == 1 ) {

						if ( ( report == 0 ) && ( m_reports[ 0 ].reportType & REPORT_FLAG_IDLE ) ) {

							WaitIn();
							UEDATX = m_reports[ 0 ].idle;
							ClearIn();

							result = true;
						}
					}
					else if ( m_nextReport > 1 ) {

						if ( ( report > 0 ) && ( report <= m_nextReport ) && ( m_reports[ report - 1 ].reportType & REPORT_FLAG_IDLE ) ) {

							WaitIn();
							UEDATX = report;
							UEDATX = m_reports[ report - 1 ].idle;
							ClearIn();

							result = true;
						}
					}
					break;
				}

				case 3: {    // get protocol

					WaitIn();
					UEDATX = m_protocol;
					ClearIn();

					result = true;
					break;
				}
			}
			break;
		}
		break;
	}
	return result;
}


unsigned int const Interface::GetConfigurationDescriptor( uint8_t const interface, uint8_t* const buffer ) const {

	if ( buffer != NULL ) {

		uint8_t* pBuffer = buffer;

		uint16_t reportLength = 0;
		if ( m_nextReport == 1 )
			reportLength += 6 + m_reports[ 0 ].descriptorSize + 1;
		else if ( m_nextReport > 1 ) {

			for ( unsigned int ii = 0; ii < m_nextReport; ++ii )
				reportLength += 8 + m_reports[ ii ].descriptorSize + 1;
		}

		*( pBuffer++ ) = 9;                      // bLength
		*( pBuffer++ ) = 0x21;                   // bDescriptorType
		*( pBuffer++ ) = 0x11;                   // bcdHID
		*( pBuffer++ ) = 0x01;                   // bcdHID
		*( pBuffer++ ) = 0;                      // bCountryCode
		*( pBuffer++ ) = 1;                      // bNumDescriptors
		*( pBuffer++ ) = 0x22;                   // bDescriptorType
		*( pBuffer++ ) = LSB( reportLength );    // wDescriptorLength
		*( pBuffer++ ) = MSB( reportLength );    // wDescriptorLength
	}
	return 9;
}




}    // namespace HID


}    // namespace USB

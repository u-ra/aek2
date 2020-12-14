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
	\file usb_device.cc
	\brief USB::Device implementation
*/




#include "usb.hh"
#include "helpers.h"

#include <inttypes.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>




namespace USB {




namespace {




//============================================================================
//    USB device hardware helpers
//============================================================================


#if defined( __AVR_AT90USB162__ )
inline void HWConfig()
inline void PLLConfig() { PLLCSR = ( ( 1 << PLLE ) | ( 1 << PLLP0 ) ); }
inline void USBConfig() { USBCON = ( 1 << USBE ); }
inline void USBFreeze() { USBCON = ( ( 1 << USBE ) | ( 1 << FRZCLK ) ); }
#elif defined( __AVR_ATmega32U4__ )
inline void HWConfig() { UHWCON = ( 1 << UVREGE ); }
inline void PLLConfig() { PLLCSR = ( ( 1 << PLLP2 ) | ( 1 << PLLE ) ); }
inline void USBConfig() { USBCON = ( ( 1 << USBE ) | ( 1 << OTGPADE ) ); }
inline void USBFreeze() { USBCON = ( ( 1 << USBE ) | ( 1 << FRZCLK ) ); }
#elif defined( __AVR_AT90USB646__ )
inline void HWConfig() { UHWCON = ( ( 1 << UIMOD ) | ( 1 << UVREGE ) ); }
inline void PLLConfig() { PLLCSR = ( ( 1 << PLLP2 ) | ( 1 << PLLP1 ) | ( 1 << PLLE ) ); }
inline void USBConfig() { USBCON = ( ( 1 << USBE ) | ( 1 << OTGPADE ) ); }
inline void USBFreeze() { USBCON = ( ( 1 << USBE ) | ( 1 << FRZCLK ) ); }
#elif defined( __AVR_AT90USB1286__ )
inline void HWConfig() { UHWCON = ( ( 1 << UIMOD ) | ( 1 << UVREGE ) ); }
inline void PLLConfig() { PLLCSR = ( ( 1 << PLLP2 ) | ( 1 << PLLP0 ) | ( 1 << PLLE ) ); }
inline void USBConfig() { USBCON = ( ( 1 << USBE ) | ( 1 << OTGPADE ) ); }
inline void USBFreeze() { USBCON = ( ( 1 << USBE ) | ( 1 << FRZCLK ) ); }
#endif


uint8_t const GetCFG1Bits( uint16_t const endpointSize, uint8_t const endpointType ) {

	// bits 6-4 = endpoint size bits
	//     000 = 8 bytes
	//     001 = 16 bytes
	//     ...
	//     110 = 512 bytes
	//     111 = reserved
	// bits 3-2 = endpoint bank bits
	//     00 = one bank
	//     01 = two banks
	//     1? = reserved
	//  bit 1 = endpoint allocation bit

	uint8_t bits = ( endpointType & Device::ENDPOINT_MASK_BUFFER );
	if ( endpointSize > 32 ) {

		if ( endpointSize > 128 ) {

			if ( endpointSize > 256 )
				bits |= 0x60;
			else
				bits |= 0x50;
		}
		else {

			if ( endpointSize > 64 )
				bits |= 0x40;
			else
				bits |= 0x30;
		}
	}
	else if ( endpointSize > 8 ) {

		if ( endpointSize > 16 )
			bits |= 0x20;
		else
			bits |= 0x10;
	}

	return bits;
}




}    // anomymous namespace




//============================================================================
//    USB::Device methods
//============================================================================


void Device::Start(
	uint16_t const vendorID,
	uint16_t const productID,
	uint16_t const releaseNumber,
	uint8_t const manufacturerString,
	uint8_t const productString,
	uint8_t const serialString,
	uint8_t const configurationString
)
{
	if ( ! m_started ) {

		m_vendorID      = vendorID;
		m_productID     = productID;
		m_releaseNumber = releaseNumber;

		m_manufacturerString  = ( ( manufacturerString  <= m_nextString ) ? manufacturerString  : 0 );
		m_productString       = ( ( productString       <= m_nextString ) ? productString       : 0 );
		m_serialString        = ( ( serialString        <= m_nextString ) ? serialString        : 0 );
		m_configurationString = ( ( configurationString <= m_nextString ) ? configurationString : 0 );

		HWConfig();
		USBFreeze();
		PLLConfig();
		while ( ! ( PLLCSR & ( 1 << PLOCK ) ) );
		USBConfig();
		UDCON = 0;

		m_configuration = 0;
		for ( unsigned int ii = 0; ii < m_nextInterface; ++ii )
			m_interfaces[ ii ].pCallbacks->Configured( ii, m_configuration );

		UDIEN = ( ( 1 << EORSTE ) | ( 1 << SOFE ) );
		sei();

		m_started = true;
	}
}


uint8_t const Device::GetConfiguration() const {

	uint8_t configuration = 0;

	// save and clear the interrupt flag
	uint8_t const sreg = SREG;
	cli();

	configuration = m_configuration;

	// restore the interrupt flag
	SREG = sreg;

	return configuration;
}


uint8_t const Device::RegisterStringProgmem(
	wchar_t const* const string,
	uint8_t const length
)
{
	uint8_t result = 0xff;
	if ( ( ! m_started ) && ( m_nextString < MAXIMUM_STRINGS ) ) {

		result = m_nextString + 1;    // string 0 is the language identifier
		m_strings[ m_nextString ].string = string;
		m_strings[ m_nextString ].length = length;
		++m_nextString;
	}
	return result;
}


uint8_t const Device::UnregisterString( uint8_t const string ) {

	uint8_t result = string;
	if ( ( ! m_started ) && ( m_nextString > 0 ) && ( string == m_nextString ) ) {

		--m_nextString;

		m_strings[ m_nextString ].string = NULL;
		m_strings[ m_nextString ].length = 0;

		result = 0xff;
	}
	return result;
}


uint8_t const Device::RegisterInterface(
	Callbacks* const pCallbacks,
	uint8_t const interfaceClass,
	uint8_t const interfaceSubclass,
	uint8_t const interfaceProtocol,
	uint8_t const interfaceString
)
{
	uint8_t result = 0xff;
	if ( ( ! m_started ) && ( m_nextInterface < MAXIMUM_INTERFACES ) ) {

		result = m_nextInterface;
		m_interfaces[ m_nextInterface ].pCallbacks        = pCallbacks;
		m_interfaces[ m_nextInterface ].interfaceClass    = interfaceClass;
		m_interfaces[ m_nextInterface ].interfaceSubclass = interfaceSubclass;
		m_interfaces[ m_nextInterface ].interfaceProtocol = interfaceProtocol;
		m_interfaces[ m_nextInterface ].interfaceString   = ( ( interfaceString <= m_nextString ) ? interfaceString : 0 );
		++m_nextInterface;
	}
	return result;
}


uint8_t const Device::UnregisterInterface( uint8_t const interface ) {

	uint8_t result = interface;
	if ( ( ! m_started ) && ( interface + 1 == m_nextInterface ) ) {

		--m_nextInterface;

		m_interfaces[ m_nextInterface ].pCallbacks        = NULL;
		m_interfaces[ m_nextInterface ].interfaceClass    = 0;
		m_interfaces[ m_nextInterface ].interfaceSubclass = 0;
		m_interfaces[ m_nextInterface ].interfaceProtocol = 0;
		m_interfaces[ m_nextInterface ].interfaceString   = 0;

		for ( unsigned int ii = 0; ii < ARRAYLENGTH( m_endpoints ); ++ii ) {

			if ( ( m_endpoints[ ii ].endpointType != 0 ) && ( m_endpoints[ ii ].interface == m_nextInterface ) ) {

				m_endpoints[ ii ].interface        = 0;
				m_endpoints[ ii ].endpointType     = 0;
				m_endpoints[ ii ].endpointSize     = 0;
				m_endpoints[ ii ].endpointInterval = 0;
			}
		}

		result = 0xff;
	}
	return result;
}


uint8_t const Device::RegisterEndpoint(
	uint8_t const interface,
	uint8_t const endpointType,
	uint16_t const endpointSize,
	uint8_t const endpointInterval
)
{
	static uint8_t const addresses[] = { 0, 1, 4, 5, 2, 3 };

	uint8_t result = 0xff;
	if ( ( ! m_started ) && ( interface < m_nextInterface ) ) {

		// **FIXME **TODO **HACK: only endpoints 3 and 4 support bulk transfers, but this is platform dependant!
		uint8_t index = 0;
		if ( ( endpointType & ENDPOINT_MASK_TYPE ) == ENDPOINT_FLAG_BULK )
			index += 4;

		for ( ; index < ARRAYLENGTH( addresses ); ++index ) {

			uint8_t const address = addresses[ index ];
			if ( m_endpoints[ address ].endpointType == 0 ) {

				m_endpoints[ address ].interface        = interface;
				m_endpoints[ address ].endpointType     = endpointType;
				m_endpoints[ address ].endpointSize     = endpointSize;
				m_endpoints[ address ].endpointInterval = endpointInterval;

				result = address + 1;    // the control endpoint (0) is not stored in m_endpoints
				break;
			}
		}
	}
	return result;
}


uint8_t const Device::UnregisterEndpoint( uint8_t const endpoint ) {

	uint8_t result = endpoint;
	if ( ( ! m_started ) && ( endpoint > 0 ) ) {

		uint8_t address = endpoint - 1;
		if ( ( address < ARRAYLENGTH( m_endpoints ) ) && ( m_endpoints[ address ].endpointType != 0 ) ) {

			m_endpoints[ address ].interface        = 0;
			m_endpoints[ address ].endpointType     = 0;
			m_endpoints[ address ].endpointSize     = 0;
			m_endpoints[ address ].endpointInterval = 0;

			result = 0xff;
		}
	}
	return result;
}


void Device::DeviceInterrupt() {

	uint8_t const udint = UDINT;
	UDINT = 0;
	if ( udint & ( 1 << EORSTI ) ) {

		UENUM = 0;

		// bit 5 = stall request handshake bit
		// bit 4 = stall equest clear handshake bit
		// bit 3 = reset data toggle bit
		// bit 0 = endpoint enable bit
		UECONX = 1;

		// bits 7-6 = endpoint type bits
		//     00 = bulk
		//     01 = isochronous
		//     11 = interrupt
		// bit 0 = endpoint direction bit
		//     0 = out
		//     1 = in
		UECFG0X = ENDPOINT_FLAG_CONTROL;

		// bits 6-4 = endpoint size bits
		//     000 = 8 bytes
		//     001 = 16 bytes
		//     ...
		//     110 = 512 bytes
		//     111 = reserved
		// bits 3-2 = endpoint bank bits
		//     00 = one bank
		//     01 = two banks
		//     1? = reserved
		//  bit 1 = endpoint allocation bit
		UECFG1X = GetCFG1Bits( CONTROL_ENDPOINT_SIZE, ENDPOINT_FLAG_SINGLE_BUFFERED );

		// bit 4 (NAKOUTE) = NAK out interrupt enable bit
		// bit 3 (RXSTPE) = received setup interrupt enable flag
		// bit 2 (RXOUTE) = received out data interrupt enable flag
		// bit 1 (STALLEDE) = stalled interrupt enable flag
		// bit 0 (TXINE) = transmitter ready interrupt enable flag
		UEIENX = ( 1 << RXSTPE );

		m_configuration = 0;
		for ( unsigned int ii = 0; ii < m_nextInterface; ++ii )
			m_interfaces[ ii ].pCallbacks->Configured( ii, m_configuration );
	}
	if ( m_configuration && ( udint & ( 1 << SOFI ) ) ) {    // start of frame

		for ( unsigned int ii = 0; ii < m_nextInterface; ++ii )
			m_interfaces[ ii ].pCallbacks->HandleIdle( ii );
	}
}


void Device::CommunicationInterrupt() {

	bool success = false;

	UENUM = 0;
	uint8_t const ueintx = UEINTX;
	if ( ueintx & ( 1 << RXSTPI ) ) {

		uint8_t const bmRequestType = UEDATX;

		uint8_t const bRequest = UEDATX;

		uint8_t const wValueLow  = UEDATX;
		uint8_t const wValueHigh = UEDATX;
		uint16_t const wValue = ( ( uint16_t )wValueLow | ( ( uint16_t )wValueHigh << 8 ) );

		uint8_t const wIndexLow  = UEDATX;
		uint8_t const wIndexHigh = UEDATX;
		uint16_t const wIndex = ( ( uint16_t )wIndexLow | ( ( uint16_t )wIndexHigh << 8 ) );

		uint8_t const wLengthLow  = UEDATX;
		uint8_t const wLengthHigh = UEDATX;
		uint16_t const wLength = ( ( uint16_t )wLengthLow | ( ( uint16_t )wLengthHigh << 8 ) );

		UEINTX = ~( ( 1 << RXSTPI ) | ( 1 << RXOUTI ) | ( 1 << TXINI ) );

		/*
			bit 7 = direction:
				0 = host to device
				1 = device to host
			bits 6-5 = type:
				0 = standard
				1 = class
				2 = vendor
			bits 4-0 = recipient:
				0 = device
				1 = interface
				2 = endpoint
				3 = other
		*/
		switch( bmRequestType & 0x1f ) {

			case 0: {    // device

				if ( bmRequestType == 0x00 ) {    // standard host to device

					switch( bRequest ) {

						case 5: {    // set address

							ClearIn();
							WaitIn();
							UDADDR = ( wValue | ( 1 << ADDEN ) );

							success = true;
							break;
						}

						case 9: {    // set configuration

							m_configuration = wValue;
							for ( unsigned int ii = 0; ii < m_nextInterface; ++ii )
								m_interfaces[ ii ].pCallbacks->Configured( ii, m_configuration );
							ClearIn();
							for ( unsigned int ii = 0; ii < ARRAYLENGTH( m_endpoints ); ++ii ) {

								UENUM = ii + 1;
								if ( m_endpoints[ ii ].endpointType == 0 ) {

									// bit 5 = stall request handshake bit
									// bit 4 = stall equest clear handshake bit
									// bit 3 = reset data toggle bit
									// bit 0 = endpoint enable bit
									UECONX = 0;
								}
								else {

									// bit 5 = stall request handshake bit
									// bit 4 = stall equest clear handshake bit
									// bit 3 = reset data toggle bit
									// bit 0 = endpoint enable bit
									UECONX = 1;

									// bits 7-6 = endpoint type bits
									//     00 = bulk
									//     01 = isochronous
									//     11 = interrupt
									// bit 0 = endpoint direction bit
									//     0 = out
									//     1 = in
									UECFG0X = ( m_endpoints[ ii ].endpointType & ENDPOINT_MASK_INOUT_TYPE );

									// bits 6-4 = endpoint size bits
									//     000 = 8 bytes
									//     001 = 16 bytes
									//     ...
									//     110 = 512 bytes
									//     111 = reserved
									// bits 3-2 = endpoint bank bits
									//     00 = one bank
									//     01 = two banks
									//     1? = reserved
									//  bit 1 = endpoint allocation bit
									UECFG1X = GetCFG1Bits( m_endpoints[ ii ].endpointSize, m_endpoints[ ii ].endpointType );
								}
							}
							UERST = 0x1e;
							UERST = 0;

							success = true;
							break;
						}
					}
				}
				else if ( bmRequestType == 0x80 ) {    // standard device to host

					switch( bRequest ) {

						case 0: {    // get status

							WaitIn();
							UEDATX = 0;
							UEDATX = 0;
							ClearIn();

							success = true;
							break;
						}

						case 6: {    // get descriptor

							uint8_t* buffer = NULL;
							unsigned int length = 0;

							switch( wValueHigh ) {

								case 0x01: {    // device descriptor

									if ( wValueLow == 0 ) {

										length = 18;
										uint8_t* pBuffer = static_cast< uint8_t* >( malloc( length ) );
										buffer = pBuffer;

										*( pBuffer++ ) = 18;                        // bLength
										*( pBuffer++ ) = 1;                         // bDescriptorType
										*( pBuffer++ ) = 0x00;                      // bcdUSB
										*( pBuffer++ ) = 0x02;                      // bcdUSB
										*( pBuffer++ ) = 0;                         // bDeviceClass    // **TODO **FIXME **HACK: 0=composite, 2=communication. If we don't choose "communication", then USB Serial doesn't work on Mac or Windows, and if we do choose "communication", then the HID interfaces won't work on Windows!
										*( pBuffer++ ) = 0;                         // bDeviceSubClass
										*( pBuffer++ ) = 0;                         // bDeviceProtocol
										*( pBuffer++ ) = CONTROL_ENDPOINT_SIZE;     // bMaxPacketSize0
										*( pBuffer++ ) = LSB( m_vendorID );         // idVendor
										*( pBuffer++ ) = MSB( m_vendorID );         // idVendor
										*( pBuffer++ ) = LSB( m_productID );        // idProduct
										*( pBuffer++ ) = MSB( m_productID );        // idProduct
										*( pBuffer++ ) = LSB( m_releaseNumber );    // bcdDevice
										*( pBuffer++ ) = MSB( m_releaseNumber );    // bcdDevice
										*( pBuffer++ ) = m_manufacturerString;      // iManufacturer
										*( pBuffer++ ) = m_productString;           // iProduct
										*( pBuffer++ ) = m_serialString;            // iSerialNumber
										*( pBuffer++ ) = 1;                         // bNumConfigurations
									}
									break;
								}

								case 0x02: {    // configuration descriptor

									if ( wValueLow == 0 ) {

										length = 9;
										for ( unsigned int ii = 0; ii < m_nextInterface; ++ii )
											length += 9 + m_interfaces[ ii ].pCallbacks->GetConfigurationDescriptor( ii, NULL );
										for ( unsigned int ii = 0; ii < ARRAYLENGTH( m_endpoints ); ++ii )
											if ( m_endpoints[ ii ].endpointType != 0 )
												length += 7;

										uint8_t* pBuffer = static_cast< uint8_t* >( malloc( length ) );
										buffer = pBuffer;

										// configuration descriptor
										*( pBuffer++ ) = 9;                        // bLength
										*( pBuffer++ ) = 2;                        // bDescriptorType
										*( pBuffer++ ) = LSB( length );            // wTotalLength
										*( pBuffer++ ) = MSB( length );            // wTotalLength
										*( pBuffer++ ) = m_nextInterface;          // bNumInterfaces
										*( pBuffer++ ) = 1;                        // bConfigurationValue
										*( pBuffer++ ) = m_configurationString;    // iConfiguration
										*( pBuffer++ ) = 0xc0;                     // bmAttributes
										*( pBuffer++ ) = 50;                       // bMaxPower

										for ( unsigned int ii = 0; ii < m_nextInterface; ++ii ) {

											uint8_t endpoints = 0;
											for ( unsigned int jj = 0; jj < ARRAYLENGTH( m_endpoints ); ++jj )
												if ( ( m_endpoints[ jj ].endpointType != 0 ) && ( m_endpoints[ jj ].interface == ii ) )
													++endpoints;

											// interface descriptor
											*( pBuffer++ ) = 9;                                       // bLength
											*( pBuffer++ ) = 4;                                       // bDescriptorType
											*( pBuffer++ ) = ii;                                      // bInterfaceNumber
											*( pBuffer++ ) = 0;                                       // bAlternateSetting
											*( pBuffer++ ) = endpoints;                               // bNumEndpoints
											*( pBuffer++ ) = m_interfaces[ ii ].interfaceClass;       // bInterfaceClass
											*( pBuffer++ ) = m_interfaces[ ii ].interfaceSubclass;    // bInterfaceSubClass
											*( pBuffer++ ) = m_interfaces[ ii ].interfaceProtocol;    // bInterfaceProtocol
											*( pBuffer++ ) = m_interfaces[ ii ].interfaceString;      // iInterface

											pBuffer += m_interfaces[ ii ].pCallbacks->GetConfigurationDescriptor( ii, pBuffer );

											for ( unsigned int jj = 0; jj < ARRAYLENGTH( m_endpoints ); ++jj ) {

												if ( ( m_endpoints[ jj ].endpointType != 0 ) && ( m_endpoints[ jj ].interface == ii ) ) {

													uint8_t const index = ( ( m_endpoints[ jj ].endpointType & ENDPOINT_FLAG_IN ) ? ( jj + 0x81 ) : ( jj + 0x01 ) );
													uint8_t const attributes = ( ( m_endpoints[ jj ].endpointType & ENDPOINT_MASK_TYPE ) >> ENDPOINT_SHIFT_TYPE );

													// endpoint descriptor
													*( pBuffer++ ) = 7;                                        // bLength
													*( pBuffer++ ) = 5;                                        // bDescriptorType
													*( pBuffer++ ) = index;                                    // bEndpointAddress
													*( pBuffer++ ) = attributes;                               // bmAttributes
													*( pBuffer++ ) = LSB( m_endpoints[ jj ].endpointSize );    // wMaxPacketSize
													*( pBuffer++ ) = MSB( m_endpoints[ jj ].endpointSize );    // wMaxPacketSize
													*( pBuffer++ ) = m_endpoints[ jj ].endpointInterval;       // bInterval
												}
											}
										}
									}
									break;
								}

								case 0x03: {    // string descriptor

									if ( wValueLow == 0 ) {

										length = 4;
										uint8_t* pBuffer = static_cast< uint8_t* >( malloc( length ) );
										buffer = pBuffer;

										*( pBuffer++ ) = 4;       // bLength
										*( pBuffer++ ) = 3;       // bDescriptorType
										*( pBuffer++ ) = 0x09;    // wLANGID
										*( pBuffer++ ) = 0x04;    // wLANGID
									}
									else if ( wValueLow <= m_nextString ) {

										wchar_t const* const string = m_strings[ wValueLow - 1 ].string;
										unsigned int const stringLength = Min( m_strings[ wValueLow - 1 ].length, ( uint8_t )126 );

										length = 2 + stringLength * 2;
										uint8_t* pBuffer = static_cast< uint8_t* >( malloc( length ) );
										buffer = pBuffer;

										*( pBuffer++ ) = length;    // bLength
										*( pBuffer++ ) = 3;         // bDescriptorType
										for ( unsigned int ii = 0; ii < stringLength; ++ii ) {

											uint16_t const character = pgm_read_word( string + ii );
											*( pBuffer++ ) = LSB( character );
											*( pBuffer++ ) = MSB( character );
										}
									}
									break;
								}
							}

							if ( buffer != NULL ) {

								SendBuffer( buffer, Min( length, wLength, 255u ) );
								free( buffer );

								success = true;
							}
							break;
						}

						case 8: {    // get configuration

							WaitIn();
							UEDATX = m_configuration;
							ClearIn();

							success = true;
							break;
						}
					}
				}
				break;
			}

			case 1: {    // interface

				if ( wIndex < m_nextInterface )
					success = m_interfaces[ wIndex ].pCallbacks->HandleRequest( bmRequestType, bRequest, wValue, wIndex, wLength );
				break;
			}

			case 2: {    // endpoint

				if ( wIndex > 0 ) {

					uint16_t index = wIndex - 1;
					if ( ( index < ARRAYLENGTH( m_endpoints ) ) && ( m_endpoints[ index ].endpointType != 0 ) )
						success = m_interfaces[ m_endpoints[ index ].interface ].pCallbacks->HandleRequest( bmRequestType, bRequest, wValue, wIndex, wLength );
				}
				break;
			}
		}
	}

	if ( ! success )
		Stall();
}




}    // namespace USB




//============================================================================
//    USB device interrupts
//============================================================================


ISR( USB_GEN_vect ) {

	USB::_Private::DeviceInterrupt();
}


ISR( USB_COM_vect ) {

	USB::_Private::CommunicationInterrupt();
}

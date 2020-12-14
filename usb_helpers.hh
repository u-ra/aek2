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
	\file usb_helpers.hh
	\brief USB helper functions
*/




#ifndef __USB_HELPERS_HH__
#define __USB_HELPERS_HH__

#ifdef __cplusplus




#include <avr/io.h>
#include <avr/interrupt.h>




namespace USB {




//============================================================================
//    USB hardware helpers
//============================================================================


inline void WaitIn() {

	while ( ! ( UEINTX & ( 1 << TXINI ) ) );
}


inline void WaitOut() {

	while ( ! ( UEINTX & ( 1 << RXOUTI ) ) );
}


inline uint8_t const WaitInOut() {

	uint8_t ueintx;
	do { ueintx = UEINTX; } while ( ! ( ueintx & ( ( 1 << TXINI ) | ( 1 << RXOUTI ) ) ) );
	return ueintx;
}


inline void ClearIn() {

	UEINTX = ~( 1 << TXINI );
}


inline void ClearOut() {

	UEINTX = ~( 1 << RXOUTI );
}


inline void Stall() {

	UECONX = ( ( 1 << STALLRQ ) | ( 1 << EPEN ) );
}




//============================================================================
//    USB memory transfer helpers
//============================================================================


unsigned int const SendBuffer( uint8_t const* const buffer, unsigned int const length, unsigned int const endpointSize = Device::CONTROL_ENDPOINT_SIZE );


unsigned int const SendBufferProgmem( uint8_t const* const buffer, unsigned int const length, unsigned int const endpointSize = Device::CONTROL_ENDPOINT_SIZE );




}    // namespace USB




#endif    /* __cplusplus */

#endif    /* __USB_HELPERS_HH__ */

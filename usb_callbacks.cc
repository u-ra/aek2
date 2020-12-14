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
	\file usb_callbacks.cc
	\brief USB::Callbacks implementation
*/




#include "usb.hh"

#include <inttypes.h>
#include <stdlib.h>




namespace USB {




//============================================================================
//    USB::Callbacks methods
//============================================================================


Callbacks::~Callbacks() {
}


void Callbacks::Configured( uint8_t const interface, uint8_t const configuration ) {
}


void Callbacks::HandleIdle( uint8_t const interface ) {
}


bool const Callbacks::HandleRequest( uint8_t const bmRequestType, uint8_t const bRequest, uint16_t const wValue, uint16_t const wIndex, uint16_t const wLength ) {

	return false;
}




}    // namespace USB

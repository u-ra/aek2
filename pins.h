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
	\file pins.h
	\brief Pin allocation function declarations
*/




#ifndef __PINS_H__
#define __PINS_H__

#ifdef __cplusplus
extern "C" {
#endif    // __cplusplus




#include <avr/io.h>

#include <stdbool.h>
#include <inttypes.h>




/**
	\brief Called to take possession of a pin

	Given a port name, places the addresses of the corresponding input,
	direction, and output registers in *pPin, *pDdr and *pPort, respectively.
	If the pin has already been allocated, but not yet freed (with PinFree()),
	then this function will fail.

	\param pPin   pointer to input register result
	\param pDdr   pointer to direction register result
	\param pPort  pointer to output register result
	\param name   port name ('a'-'h', 'A'-'H')
	\param bit    bit index (0-7)
*/
bool const PinAllocate(
	uint8_t volatile** const pPin,
	uint8_t volatile** const pDdr,
	uint8_t volatile** const pPort,
	char const name,
	uint8_t const bit
);


/**
	\brief Called to release ownership of a pin

	\param name  port name ('a'-'h', 'A'-'H')
	\param bit   bit index (0-7)
*/
extern void PinFree(
	char const name,
	uint8_t const bit
);




#ifdef __cplusplus
}    // extern "C"
#endif    // __cplusplus

#endif    /* __PINS_H__ */

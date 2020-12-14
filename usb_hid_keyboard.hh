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
	\file usb_hid_keyboard.hh
	\brief USB::HID::Keyboard interface
*/




#ifndef __USB_HID_KEYBOARD_HH__
#define __USB_HID_KEYBOARD_HH__

#ifdef __cplusplus




#include "usb_hid_interface.hh"
#include "usb_hid_tables.hh"
#include "usb_device.hh"
#include "helpers.h"

#include <inttypes.h>




namespace USB {


namespace HID {




//============================================================================
//    USB::HID::Keyboard class
//============================================================================


/**
	\brief HID keyboard class

	This class represents a boot-protocol keyboard. It stores the state of
	keyboard LEDs (send by the host), 8 modifier keys, and up to 6 other keys.
	To handle more than six simultaneous non-modifier keypresses, use the
	KeyboardExtension class.
*/
struct Keyboard : public Interface {

	/**
		\brief Constructor

		\param interfaceString    8-bit string number for the interface, 0 if none
	*/
	Keyboard( uint8_t const interfaceString = 0 );

	/// \brief Destructor
	virtual ~Keyboard();


	/**
		\brief Gets the state of an LED
		\param LED  LED number (from HID::LED table)
		\result  true if lit, false otherwise
	*/
	bool const GetLED( LED const led ) const;


	/**
		\brief Gets the state of a key
		\param key  key number (from HID::Key table)
		\result  true if pressed, false otherwise
	*/
	bool const GetKey( Key const key ) const;

	/**
		\brief Sets the state of a key to "pressed"
		\param key  key number (from HID::Key table)
		\result  true on success, false on failure
	*/
	bool const PressKey( Key const key );

	/**
		\brief Sets the state of a key to "released"
		\param key  key number (from HID::Key table)
	*/
	void ReleaseKey( Key const key );


private:

	virtual bool const IsChangedReport( uint8_t const report ) const;

	virtual void SendReport( uint8_t const report );
	virtual void ReceiveReport( uint8_t const report );


	uint8_t m_report;

	uint8_t m_modifiers;
	uint8_t m_leds;
	uint8_t m_keys[ 6 ];

	uint8_t m_oldModifiers;
	uint8_t m_oldKeys[ 6 ];


	inline Keyboard( Keyboard const& other );
	inline Keyboard const& operator=( Keyboard const& other );
};




}    // namespace HID


}    // namespace USB




#endif    /* __cplusplus */

#endif    /* __USB_HID_KEYBOARD_HH__ */

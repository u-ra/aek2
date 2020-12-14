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
	\file usb_hid_keyboard_extension.hh
	\brief USB::HID::KeyboardExtension interface
*/




#ifndef __USB_HID_KEYBOARD_EXTENSION_HH__
#define __USB_HID_KEYBOARD_EXTENSION_HH__

#ifdef __cplusplus




#include "usb_hid_interface.hh"
#include "usb_hid_tables.hh"
#include "usb_device.hh"
#include "helpers.h"

#include <inttypes.h>




namespace USB {


namespace HID {




//============================================================================
//    USB::HID::KeyboardExtension class
//============================================================================


/**
	\brief HID N-key rollover keyboard class

	This class stores the state of many useful keys ("A" through "F24") in a
	112-element bitfield, permitting any combination of them to be pressed
	simultaneously. It does not handle modifiers, so should be used <em>in
	addition to</em> the Keyboard class, not instead of it. Whenever you want
	to register a keypress, first try to send it to an instance of the Keyboard
	class, and only fall back to this class if it fails.

	While there are standard keys to control the volume, more advanced media
	controls (play, pause, stop, etc) can be found in the consumer page. This
	is the reason for stopping at 112 bits (14 bytes) of standard keyboard
	keys--this leaves 2 unused bytes in a 16-byte request. These remaining
	bytes are used, as a one-key-rollover element storing 16-bit consumer
	controls.

	\note Under Linux, if a caps lock keypress goes through the Keyboard class,
	then the caps lock LED will state will only be sent to the Keyboard class,
	<em>not</em> the KeyboardExtension class. This also applies to other locking
	keys, and is, in my opinion, a bug in linux. The workaround implemented in
	this class is to never report a locking key. As a result, we're limited to
	the 6-key rollover of the Keyboard class on these keys.
*/
struct KeyboardExtension : public Interface {

	/**
		\brief Constructor

		\param interfaceString    8-bit string number for the interface, 0 if none
	*/
	KeyboardExtension( uint8_t const interfaceString = 0 );

	/// \brief Destructor
	virtual ~KeyboardExtension();


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


	/**
		\brief Gets the state of a consumer control
		\param consumer  consumer control number (from HID::Consumer table)
		\result  true if pressed, false otherwise
	*/
	bool const GetConsumer( Consumer const consumer ) const;

	/**
		\brief Sets the state of a consumer control to "pressed"
		\param consumer  consumer control number (from HID::Consumer table)
		\result  true on success, false on failure
	*/
	bool const PressConsumer( Consumer const consumer );

	/**
		\brief Sets the state of a consumer control to "released"
		\param consumer  consumer control number (from HID::Consumer table)
	*/
	void ReleaseConsumer( Consumer const consumer );


private:

	virtual bool const IsChangedReport( uint8_t const report ) const;

	virtual void SendReport( uint8_t const report );
	virtual void ReceiveReport( uint8_t const report );


	uint8_t m_keyboard1Report;
	uint8_t m_keyboard2Report;
	uint8_t m_consumerReport;

	uint8_t m_keys[ 14 ];
	uint16_t m_consumer;

	uint8_t m_oldKeys[ 14 ];
	uint16_t m_oldConsumer;


	inline KeyboardExtension( KeyboardExtension const& other );
	inline KeyboardExtension const& operator=( KeyboardExtension const& other );
};




}    // namespace HID


}    // namespace USB




#endif    /* __cplusplus */

#endif    /* __USB_HID_KEYBOARD_EXTENSION_HH__ */

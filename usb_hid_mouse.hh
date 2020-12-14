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
	\file usb_hid_mouse.hh
	\brief USB::HID::Mouse interface
*/




#ifndef __USB_HID_MOUSE_HH__
#define __USB_HID_MOUSE_HH__

#ifdef __cplusplus




#include "usb_hid_interface.hh"
#include "usb_hid_tables.hh"
#include "usb_device.hh"

#include <inttypes.h>




namespace USB {


namespace HID {




//============================================================================
//    USB::HID::Mouse class
//============================================================================


/**
	\brief HID mouse class

	This class represents a boot-protocol mouse with three movement axes
	(x,y,wheel).
*/
struct Mouse : public Interface {

	/**
		\brief Constructor

		\param interfaceString    8-bit string number for the interface, 0 if none
	*/
	Mouse( uint8_t const interfaceString = 0 );

	/// \brief Destructor
	virtual ~Mouse();


	/**
		\brief Gets the state of a button
		\param button  button number (from HID::Button table)
		\result  true if pressed, false otherwise
	*/
	bool const GetButton( Button const button ) const;

	/**
		\brief Sets the state of a button to "pressed"
		\param button  button number (from HID::Button table)
		\result  true on success, false on failure
	*/
	bool const PressButton( Button const button );

	/**
		\brief Sets the state of a button to "released"
		\param button  button number (from HID::Button table)
	*/
	void ReleaseButton( Button const button );


	/**
		\brief Moves the mouse
		\param xx     movement in X-direction
		\param yy     movement in Y-direction
		\param wheel  wheel movement
	*/
	void Move( int const xx, int const yy, int const wheel = 0 );


private:

	virtual bool const IsChangedReport( uint8_t const report ) const;

	virtual void SendReport( uint8_t const report );
	virtual void ReceiveReport( uint8_t const report );


	uint8_t m_report;

	uint8_t m_buttons;
	int m_xx;
	int m_yy;
	int m_wheel;

	uint8_t volatile m_oldButtons;


	inline Mouse( Mouse const& other );
	inline Mouse const& operator=( Mouse const& other );
};




}    // namespace HID


}    // namespace USB




#endif    /* __cplusplus */

#endif    /* __USB_HID_MOUSE_HH__ */

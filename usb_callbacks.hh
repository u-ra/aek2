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
	\file usb_callbacks.hh
	\brief USB::Callbacks interface
*/




#ifndef __USB_CALLBACKS_HH__
#define __USB_CALLBACKS_HH__

#ifdef __cplusplus




#include <inttypes.h>




namespace USB {




//============================================================================
//    USB::Callbacks class
//============================================================================


/**
	\brief USB interface/endpoint callbacks class

	Instances of this class are registered with the USB device via calls to
	Device::RegisterInterface(). It will then receive events intended for the
	registered interface, as well as any associated endpoints.
*/
struct Callbacks {

	/// \brief Pure virtual destructor
	virtual ~Callbacks() = 0;


protected:

	/// \brief Default constructor
	inline Callbacks();


	/**
		\brief Notification that the USB device has been configured

		This callback will be called on all registered interfaces by
		USB::Device when the device is configured or deconfigured. Note that,
		if you registered the same instance of this class on multiple
		interfaces, then this callback will be called multiple times per event.

		\param interface  interface for which this message is intended
		\param configuration  nonzero if configured, zero if deconfigured
	*/
	virtual void Configured( uint8_t const interface, uint8_t const configuration );


	/**
		\brief Idle event handler

		Called by USB::Device whenever a start of frame interrupt occurs.

		\param interface  Interface for which this message is intended
	*/
	virtual void HandleIdle( uint8_t const interface );


	/**
		\brief Idle event handler

		Called by USB::Device whenever for every message intended for an
		interface or endpoint assigned to this Callbacks instance.

		\param bmRequestType
		\param bRequest
		\param wValue
		\param wIndex
		\param wLength
		\result  true if the request was handled, false otherwise
	*/
	virtual bool const HandleRequest( uint8_t const bmRequestType, uint8_t const bRequest, uint16_t const wValue, uint16_t const wIndex, uint16_t const wLength );


	/**
		\brief Returns the configuration descriptor for this interface

		What I'm calling the "configuration descriptor" is "everything between
		the interface and endpoint descriptors, exclusive", and might in fact
		be multiple descriptors. Examples are HID descriptors and functional
		descriptors.

		If buffer is NULL, then obviously no data should be written. However,
		this function should still return the correct descriptor length (which
		will then be used to allocate a buffer of the appropriate size).

		\param interface  interface for which this message is intended
		\param buffer     buffer to which to write the descriptor (may be NULL)
		\result  size of descriptor
	*/
	virtual unsigned int const GetConfigurationDescriptor( uint8_t const interface, uint8_t* const buffer ) const = 0;


private:

	friend struct Device;


	inline Callbacks( Callbacks const& other );
	inline Callbacks const& operator=( Callbacks const& other );
};




//============================================================================
//    USB::Callbacks inline methods
//============================================================================


Callbacks::Callbacks() {
}




}    // namespace USB




#endif    /* __cplusplus */

#endif    /* __USB_CALLBACKS_HH__ */

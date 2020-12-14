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
	\file usb_device.hh
	\brief USB::Device implementation
*/




#ifndef __USB_DEVICE_HH__
#define __USB_DEVICE_HH__

#ifdef __cplusplus




#include "usb_callbacks.hh"

#include <inttypes.h>
#include <string.h>




namespace USB {




namespace _Private {




//============================================================================
//    USB device interrupts
//============================================================================


/**
	\brief Device interrupt passthrough

	The interrupt service routine for USB_GEN_vect calls this function, which
	in turn retrieves the global Device instance, and passes control to
	Device::DeviceInterrupt().
*/
inline void DeviceInterrupt();


/**
	\brief Communication interrupt passthrough

	The interrupt service routine for USB_COM_vect calls this function, which
	in turn retrieves the global Device instance, and passes control to
	Device::ControlInterrupt().
*/
inline void CommunicationInterrupt();




}    // namespace _Private




//============================================================================
//    USB::Device class
//============================================================================


/**
	\brief USB device class

	Singleton class which handles the USB device. The global class instance may
	be retrieved by calling the Instance() function.

	Everything having to do with USB goes through this class. All interfaces
	must register an instance of the Callbacks class using the
	RegisterInterface method, and must register their endpoints using the
	RegisterEndpoint method.

	This class responds directly to some USB requests. Others are passed
	through to the appropriate Callbacks instance. Registered
	interfaces/endpoints will receive start-of-frame interrupts via the
	Callbacks::HandleIdle() method, and control interrupts via
	Callbacks::HandleRequest().
*/
struct Device {

	enum { MAXIMUM_STRINGS = 8 };
	enum { MAXIMUM_INTERFACES = 6 };
	enum { CONTROL_ENDPOINT_SIZE = 32 };

	enum {

		// bits 7-6 = endpoint type
		ENDPOINT_FLAG_CONTROL     = 0x00,
		ENDPOINT_FLAG_ISOCHRONOUS = 0x40,
		ENDPOINT_FLAG_BULK        = 0x80,
		ENDPOINT_FLAG_INTERRUPT   = 0xc0,

		// bits 5-4 = unused

		// bits 3-1 = buffering
		ENDPOINT_FLAG_SINGLE_BUFFERED = 0x02,
		ENDPOINT_FLAG_DOUBLE_BUFFERED = 0x06,

		// bit 0 = in/out
		ENDPOINT_FLAG_OUT = 0x00,
		ENDPOINT_FLAG_IN  = 0x01,

		ENDPOINT_MASK_TYPE       = 0xc0,
		ENDPOINT_MASK_INOUT_TYPE = 0xc1,
		ENDPOINT_MASK_BUFFER     = 0x0e,
		ENDPOINT_MASK_INOUT      = 0x01,

		ENDPOINT_SHIFT_TYPE   = 6,
		ENDPOINT_SHIFT_BUFFER = 1,
		ENDPOINT_SHIFT_INOUT  = 0

	};


	/**
		\brief Get the global class instance
		\result  instance
	*/
	static inline Device* const Instance();


	/**
		\brief Starts the USB device

		After calling this function, no new interfaces or callbacks may be
		registered or unregistered, and events will start being handled.

		\param vendorID             16-bit vendor identifier
		\param productID            16-bit product identifier
		\param releaseNumber        16-bit version number
		\param manufacturerString   8-bit string number for the manufacturer, 0 if none
		\param productString        8-bit string number for the product, 0 if none
		\param serialString         8-bit string number for the serial number, 0 if none
		\param configurationString  8-bit string number for the configuration, 0 if none
	*/
	void Start(
		uint16_t const vendorID,
		uint16_t const productID,
		uint16_t const releaseNumber = 0x0100,
		uint8_t const manufacturerString = 0,
		uint8_t const productString = 0,
		uint8_t const serialString = 0,
		uint8_t const configurationString = 0
	);

	/**
		\brief Checks whether the device has been started

		\result  true if started, false otherwise
	*/
	inline bool const IsStarted() const;


	/**
		\brief Gets the USB configuration

		We don't support multiple configurations, so this function is only
		really useful for determining whether or not the device has been
		configured. If the returned configuration value is nonzero (it should
		be 1), then the device has been configured.

		\result  configuration
	*/
	uint8_t const GetConfiguration() const;


	/**
		\brief Registers a new string

		If no string could be created (for example, because MAXIMUM_STRINGS
		have already been allocated), then this function will return 255.
		Otherwise, it will return the string number.

		This function cannot be called after a call to Start().

		\param string  buffer containing the string (in program memory)
		\param length  length of the string (characters, not including NULL-termination)
		\result  String number, or 255 on error
	*/
	uint8_t const RegisterStringProgmem(
		wchar_t const* const string,
		uint8_t const length
	);

	/**
		\brief Unregisters a string

		This function provides a <em>limited</em> facility for unregistering
		strings, because it will <em>only</em> succeed in unregistering strings
		in the opposite order that they were registered.

		This function cannot be called after a call to Start().

		\param string  string number
		\result  255 on success, old string number on failure
	*/
	uint8_t const UnregisterString( uint8_t const string );


	/**
		\brief Registers a new interface

		The passed Callbacks instance will receive start-of-frame interrupts
		via Callbacks::HandleIdle() and control interrupts via
		Callbacks::HandleRequest(), after Start() has been called.

		If no interface could be created (for example, because
		MAXIMUM_INTERFACES have already been allocated), then this function
		will return 255. Otherwise, it will return the interface number.

		This function cannot be called after a call to Start().

		\param pCallbacks         Callbacks class for this interface
		\param interfaceClass     8-bit interface class
		\param interfaceSubclass  8-bit interface subclass
		\param interfaceProtocol  8-bit interface protocol
		\param interfaceString    8-bit string number for the interface, 0 if none
		\result  Interface number, or 255 on error
	*/
	uint8_t const RegisterInterface(
		Callbacks* const pCallbacks,
		uint8_t const interfaceClass,
		uint8_t const interfaceSubclass,
		uint8_t const interfaceProtocol,
		uint8_t const interfaceString
	);

	/**
		\brief Unregisters an interface

		This function provides a <em>limited</em> facility for unregistering
		interfaces and their associated endpoints, because it will
		<em>only</em> succeed in unregistering interfaces in the opposite order
		that they were registered.

		This function cannot be called after a call to Start().

		\param interface  interface number
		\result  255 on success, old interface number on failure
	*/
	uint8_t const UnregisterInterface( uint8_t const interface );


	/**
		\brief Registers a new endpoint

		The endpoint type is some combination of the endpoint type flags
		ENDPOINT_FLAG_BULK and ENDPOINT_FLAG_INTERRUPT, the endpoint buffering
		flags ENDPOINT_FLAG_SINGLE_BUFFERED and ENDPOINT_FLAG_DOUBLE_BUFFERED,
		and the endpoint direction flags ENDPOINT_FLAG_OUT and
		ENDPOINT_FLAG_IN. Isochronous endpoints are not supported.

		The passed Callbacks instance associated with the interface owning this
		endpoint will receive control interrupts for this endpoint via
		Callbacks::HandleRequest(), after Start() has been called.

		This function cannot be called after a call to Start().

		\param interface  interface owning this endpoint
		\param endpointType      8-bit endpoint type
		\param endpointSize      16-bit endpoint size
		\param endpointInterval  8-bit polling interval
		\result  Endpoint number, or 255 on error
	*/
	uint8_t const RegisterEndpoint(
		uint8_t const interface,
		uint8_t const endpointType,
		uint16_t const endpointSize,
		uint8_t const endpointInterval
	);

	/**
		\brief Unregisters an endpoint

		In contrast to UnregisterInterface(), endpoints may be unregistered in
		any order.

		This function cannot be called after a call to Start().

		\param endpoint  endpoint number
		\result  255 on success, old endpoint number on failure
	*/
	uint8_t const UnregisterEndpoint( uint8_t const endpoint );


private:

	inline Device();

	void DeviceInterrupt();
	void CommunicationInterrupt();


	struct StringData {
		wchar_t const* string;
		uint8_t length;
	};

	struct InterfaceData {
		Callbacks* pCallbacks;
		uint8_t interfaceClass;
		uint8_t interfaceSubclass;
		uint8_t interfaceProtocol;
		uint8_t interfaceString;
	};

	struct EndpointData {
		uint8_t interface;
		uint8_t endpointType;
		uint16_t endpointSize;
		uint8_t endpointInterval;
	};


	StringData m_strings[ MAXIMUM_STRINGS ];
	uint8_t m_nextString;

	InterfaceData m_interfaces[ MAXIMUM_INTERFACES ];
	uint8_t m_nextInterface;

	EndpointData m_endpoints[ 6 ];

	uint16_t m_vendorID;
	uint16_t m_productID;
	uint16_t m_releaseNumber;

	uint8_t m_manufacturerString;
	uint8_t m_productString;
	uint8_t m_serialString;
	uint8_t m_configurationString;

	bool m_started;
	uint8_t m_configuration;


	friend void _Private::DeviceInterrupt();
	friend void _Private::CommunicationInterrupt();


	inline Device( Device const& other );
	inline Device const& operator=( Device const& other );
};




//============================================================================
//    USB::Device inline methods
//============================================================================


Device* const Device::Instance() {

	static Device device;
	return &device;
}


bool const Device::IsStarted() const {

	return m_started;
}


Device::Device() :
	m_nextString( 0 ),
	m_nextInterface( 0 ),
	m_vendorID( 0 ),
	m_productID( 0 ),
	m_releaseNumber( 0 ),
	m_manufacturerString( 0 ),
	m_productString( 0 ),
	m_serialString( 0 ),
	m_configurationString( 0 ),
	m_started( false ),
	m_configuration( 0 )
{
	memset( m_strings,    0, sizeof( m_strings    ) );
	memset( m_interfaces, 0, sizeof( m_interfaces ) );
	memset( m_endpoints,  0, sizeof( m_endpoints  ) );
}




namespace _Private {




//============================================================================
//    USB device interrupts
//============================================================================


inline void DeviceInterrupt() {

	Device::Instance()->DeviceInterrupt();
}


inline void CommunicationInterrupt() {

	Device::Instance()->CommunicationInterrupt();
}




}    // namespace _Private




}    // namespace USB




#endif    /* __cplusplus */

#endif    /* __USB_DEVICE_HH__ */

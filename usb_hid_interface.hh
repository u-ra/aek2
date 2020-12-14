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
	\file usb_hid_interface.hh
	\brief USB::HID::Base interface
*/




#ifndef __USB_HID_INTERFACE_HH__
#define __USB_HID_INTERFACE_HH__

#ifdef __cplusplus




#include "usb_callbacks.hh"
#include "usb_device.hh"

#include <inttypes.h>




namespace USB {


namespace HID {




//============================================================================
//    USB::HID::Interface class
//============================================================================


/**
	\brief Base class for HID device

	This class represents a single basic HID device, with a single interface, a
	single HID report descriptor, and a single double-buffered interrupt IN
	endpoint. All HID requests are handled by this class, often by passing
	through to the virtual IsChangedReport(), SendReport() and ReceiveReport()
	methods.
*/
struct Interface : public Callbacks {

	/// \brief Pure virtual descructor
	virtual ~Interface() = 0;


	/**
		\brief Have any HID input reports changed?
		\result  true if changed, false otherwise
	*/
	bool const IsChanged();


protected:

	enum { MAXIMUM_REPORTS = 4 };

	enum {
		REPORT_FLAG_SEND    = 1,
		REPORT_FLAG_RECEIVE = 2,
		REPORT_FLAG_IDLE    = 4     // we only *send* on idle, never receive
	};


	/**
		\brief Constructor

		We only support a single double-buffered interrupt IN endpoint, so the
		endpointType parameter is absent, but otherwise the parameters to this
		function are simply passed through to Device::RegisterInterface and
		Device::RegisterEndpoint.

		\param interfaceClass     8-bit interface class
		\param interfaceSubclass  8-bit interface subclass
		\param interfaceProtocol  8-bit interface protocol
		\param interfaceString    8-bit string number for the interface, 0 if none
		\param endpointSize       16-bit endpoint size
		\param endpointInterval   8-bit polling interval
	*/
	Interface(
		uint8_t const interfaceClass,
		uint8_t const interfaceSubclass,
		uint8_t const interfaceProtocol,
		uint8_t const interfaceString,
		uint16_t const endpointSize,
		uint8_t const endpointInterval
	);


	/**
		\brief Registers a new report

		If no report could be created (for example, because MAXIMUM_REPORTS
		have already been allocated), then this function will return 255.
		Otherwise, it will return the report number.

		This function cannot be called after a call to Device::Start().

		\param descriptor      buffer containing the report descriptor (in program memory)
		\param descriptorSize  size of the report descriptor (bytes)
		\param reportType      flags indicating which functions are supported
		\param usagePage       8-bit usage page of the report
		\param usage           8-bit usage of the report
		\param idle            idle duration (4 USB frames)
		\result  Report number, or 255 on error
	*/
	uint8_t const RegisterReportProgmem(
		uint8_t const* const descriptor,
		uint8_t const descriptorSize,
		uint8_t const reportType,
		uint8_t const usagePage,
		uint8_t const usage,
		uint8_t const idle = 0
	);

	/**
		\brief Unregisters a report

		This function provides a <em>limited</em> facility for unregistering
		reports, because it will <em>only</em> succeed in unregistering reports
		in the opposite order that they were registered.

		This function cannot be called after a call to Device::Start().

		\param report  report number
		\result  255 on success, old report number on failure
	*/
	uint8_t const UnregisterReport( uint8_t const report );


	/**
		\brief Sends a HID input report

		This class handles the task of sending reports at start-of-frame either
		when the report changes, or when the idle interval has elapsed, and
		also in response to control interrupts. In certain circumstances,
		however, the user may wish to send a report <em>immediately</em>. This
		function waits until the USB host is ready to receive a report, and
		then sends it.

		\param report   report number
		\param timeout  how long to wait before giving up (USB frames)
		\result  true if report was sent, false otherwise
	*/
	bool const ForceSendReport( uint8_t const report, uint16_t const timeout = 0 );


	/**
		\brief Has the HID input report changed?

		This function is used to determine whether a report should be sent,
		inside the idle interrupt. It should return true only if the report has
		changed since the last call to SendReport().

		\param report  report number
		\result  true if changed, false otherwise
	*/
	virtual bool const IsChangedReport( uint8_t const report ) const = 0;

	/**
		\brief Sends the HID input report

		This function should do nothing but write its report to UEDATX. No
		other handling is necessary, and no other USB registers should be
		accessed.

		\param report  report number
	*/
	virtual void SendReport( uint8_t const report ) = 0;

	/**
		\brief Receives the HID output report

		This function should do nothing but read its report from UEDATX. No
		other handling is necessary, and no other USB registers should be
		accessed.

		\param report  report number
	*/
	virtual void ReceiveReport( uint8_t const report ) = 0;


	/**
		\brief Accessor for interface number
		\result interface number
	*/
	inline uint8_t const GetInterface() const;

	/**
		\brief Accessor for endpoint number
		\result endpoint number
	*/
	inline uint8_t const GetEndpoint() const;


	/**
		\brief Accessor for HID protocol

		If the protocol is 0, then the device is in boot mode, which should be
		handled appropriately by IsChanged, SendReport and ReceiveReport.
		Otherwise, we're in report mode.

		\result HID protocol
	*/
	inline uint8_t const GetProtocol() const;

	/**
		\brief Accessor for HID idle duration

		\param report  report number
		\result HID idle duration (4 USB frames)
	*/
	inline uint8_t const GetIdle( uint8_t const report ) const;


private:

	virtual void HandleIdle( uint8_t const interface );
	virtual bool const HandleRequest( uint8_t const bmRequestType, uint8_t const bRequest, uint16_t const wValue, uint16_t const wIndex, uint16_t const wLength );

	virtual unsigned int const GetConfigurationDescriptor( uint8_t const interface, uint8_t* const buffer ) const;


	struct ReportData {
		uint8_t const* descriptor;
		uint16_t descriptorSize;
		uint8_t reportType;
		uint8_t usagePage;
		uint8_t usage;
		uint8_t idle;
		uint16_t idleCount;
	};


	ReportData m_reports[ MAXIMUM_REPORTS ];
	uint8_t m_nextReport;

	uint8_t m_interface;
	uint8_t m_endpoint;

	uint8_t m_protocol;


	inline Interface( Interface const& other );
	inline Interface const& operator=( Interface const& other );
};




//============================================================================
//    USB::HID::Interface inline methods
//============================================================================


uint8_t const Interface::GetInterface() const {

	return m_interface;
}


uint8_t const Interface::GetEndpoint() const {

	return m_endpoint;
}


uint8_t const Interface::GetProtocol() const {

	return m_protocol;
}


uint8_t const Interface::GetIdle( uint8_t const report ) const {

	uint8_t idle = 0;
	if ( report < m_nextReport )
		idle = m_reports[ report ].idle;
	return idle;
}




}    // namespace HID


}    // namespace USB




#endif    /* __cplusplus */

#endif    /* __USB_HID_INTERFACE_HH__ */

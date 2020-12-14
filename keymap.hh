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
	\file keymap.hh
	\brief Keymap implementation
*/




#ifndef __KEYMAP_HH__
#define __KEYMAP_HH__

#ifdef __cplusplus




#include "usb.hh"
#include "helpers.h"

#include <inttypes.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>




//============================================================================
//    Base keymap
//============================================================================


enum {

	KEYMAP_BASE_BEGIN = 0,

	KEYMAP_NONE = KEYMAP_BASE_BEGIN,

	KEYMAP_KEYBOARD_A,
	KEYMAP_KEYBOARD_B,
	KEYMAP_KEYBOARD_C,
	KEYMAP_KEYBOARD_D,
	KEYMAP_KEYBOARD_E,
	KEYMAP_KEYBOARD_F,
	KEYMAP_KEYBOARD_G,
	KEYMAP_KEYBOARD_H,
	KEYMAP_KEYBOARD_I,
	KEYMAP_KEYBOARD_J,
	KEYMAP_KEYBOARD_K,
	KEYMAP_KEYBOARD_L,
	KEYMAP_KEYBOARD_M,
	KEYMAP_KEYBOARD_N,
	KEYMAP_KEYBOARD_O,
	KEYMAP_KEYBOARD_P,
	KEYMAP_KEYBOARD_Q,
	KEYMAP_KEYBOARD_R,
	KEYMAP_KEYBOARD_S,
	KEYMAP_KEYBOARD_T,
	KEYMAP_KEYBOARD_U,
	KEYMAP_KEYBOARD_V,
	KEYMAP_KEYBOARD_W,
	KEYMAP_KEYBOARD_X,
	KEYMAP_KEYBOARD_Y,
	KEYMAP_KEYBOARD_Z,
	KEYMAP_KEYBOARD_1,
	KEYMAP_KEYBOARD_2,
	KEYMAP_KEYBOARD_3,
	KEYMAP_KEYBOARD_4,
	KEYMAP_KEYBOARD_5,
	KEYMAP_KEYBOARD_6,
	KEYMAP_KEYBOARD_7,
	KEYMAP_KEYBOARD_8,
	KEYMAP_KEYBOARD_9,
	KEYMAP_KEYBOARD_0,
	KEYMAP_KEYBOARD_ENTER,
	KEYMAP_KEYBOARD_ESCAPE,
	KEYMAP_KEYBOARD_BACKSPACE,
	KEYMAP_KEYBOARD_TAB,
	KEYMAP_KEYBOARD_SPACEBAR,
	KEYMAP_KEYBOARD_DASH,
	KEYMAP_KEYBOARD_EQUALS,
	KEYMAP_KEYBOARD_LEFT_BRACKET,
	KEYMAP_KEYBOARD_RIGHT_BRACKET,
	KEYMAP_KEYBOARD_BACKSLASH,
	KEYMAP_KEYBOARD_NON_US_HASH,
	KEYMAP_KEYBOARD_SEMICOLON,
	KEYMAP_KEYBOARD_QUOTE,
	KEYMAP_KEYBOARD_BACKQUOTE,
	KEYMAP_KEYBOARD_COMMA,
	KEYMAP_KEYBOARD_PERIOD,
	KEYMAP_KEYBOARD_SLASH,
	KEYMAP_KEYBOARD_CAPS_LOCK,
	KEYMAP_KEYBOARD_F1,
	KEYMAP_KEYBOARD_F2,
	KEYMAP_KEYBOARD_F3,
	KEYMAP_KEYBOARD_F4,
	KEYMAP_KEYBOARD_F5,
	KEYMAP_KEYBOARD_F6,
	KEYMAP_KEYBOARD_F7,
	KEYMAP_KEYBOARD_F8,
	KEYMAP_KEYBOARD_F9,
	KEYMAP_KEYBOARD_F10,
	KEYMAP_KEYBOARD_F11,
	KEYMAP_KEYBOARD_F12,
	KEYMAP_KEYBOARD_PRINT_SCREEN,
	KEYMAP_KEYBOARD_SCROLL_LOCK,
	KEYMAP_KEYBOARD_PAUSE,
	KEYMAP_KEYBOARD_INSERT,
	KEYMAP_KEYBOARD_HOME,
	KEYMAP_KEYBOARD_PAGE_UP,
	KEYMAP_KEYBOARD_DELETE,
	KEYMAP_KEYBOARD_END,
	KEYMAP_KEYBOARD_PAGE_DOWN,
	KEYMAP_KEYBOARD_RIGHT_ARROW,
	KEYMAP_KEYBOARD_LEFT_ARROW,
	KEYMAP_KEYBOARD_DOWN_ARROW,
	KEYMAP_KEYBOARD_UP_ARROW,
	KEYMAP_KEYBOARD_NUM_LOCK,
	KEYMAP_KEYBOARD_KEYPAD_SLASH,
	KEYMAP_KEYBOARD_KEYPAD_STAR,
	KEYMAP_KEYBOARD_KEYPAD_DASH,
	KEYMAP_KEYBOARD_KEYPAD_PLUS,
	KEYMAP_KEYBOARD_KEYPAD_ENTER,
	KEYMAP_KEYBOARD_KEYPAD_1,
	KEYMAP_KEYBOARD_KEYPAD_2,
	KEYMAP_KEYBOARD_KEYPAD_3,
	KEYMAP_KEYBOARD_KEYPAD_4,
	KEYMAP_KEYBOARD_KEYPAD_5,
	KEYMAP_KEYBOARD_KEYPAD_6,
	KEYMAP_KEYBOARD_KEYPAD_7,
	KEYMAP_KEYBOARD_KEYPAD_8,
	KEYMAP_KEYBOARD_KEYPAD_9,
	KEYMAP_KEYBOARD_KEYPAD_0,
	KEYMAP_KEYBOARD_KEYPAD_PERIOD,
	KEYMAP_KEYBOARD_NON_US_BACKSLASH,
	KEYMAP_KEYBOARD_APPLICATION,
	KEYMAP_KEYBOARD_POWER,
	KEYMAP_KEYBOARD_KEYPAD_EQUALS,
	KEYMAP_KEYBOARD_F13,
	KEYMAP_KEYBOARD_F14,
	KEYMAP_KEYBOARD_F15,

	KEYMAP_KEYBOARD_LEFT_CONTROL,
	KEYMAP_KEYBOARD_LEFT_SHIFT,
	KEYMAP_KEYBOARD_LEFT_ALT,
	KEYMAP_KEYBOARD_LEFT_GUI,
	KEYMAP_KEYBOARD_RIGHT_CONTROL,
	KEYMAP_KEYBOARD_RIGHT_SHIFT,
	KEYMAP_KEYBOARD_RIGHT_ALT,
	KEYMAP_KEYBOARD_RIGHT_GUI,

	KEYMAP_VIRTUAL_LOCKING_CAPS_LOCK,
	KEYMAP_VIRTUAL_LOCKING_NUM_LOCK,
	KEYMAP_VIRTUAL_LOCKING_SCROLL_LOCK,

	KEYMAP_BASE_END

};




//============================================================================
//    Keymap class
//============================================================================


struct Keymap {

	enum { MAXIMUM_LAYERS = 16 };

	enum {

		KEYCODE_CODE_MASK = 0x0fff,
		KEYCODE_TYPE_MASK = 0xf000,

		KEYCODE_TYPE_KEYBOARD = 0x0000,
		KEYCODE_TYPE_CONSUMER = 0x1000,
		KEYCODE_TYPE_VIRTUAL  = 0xf000

	};

	enum {

		VIRTUAL_LAYER_SELECT,
		VIRTUAL_LAYER_LOCK = VIRTUAL_LAYER_SELECT + MAXIMUM_LAYERS,

		VIRTUAL_LOCKING_CAPS_LOCK = VIRTUAL_LAYER_LOCK + MAXIMUM_LAYERS,
		VIRTUAL_LOCKING_NUM_LOCK,
		VIRTUAL_LOCKING_SCROLL_LOCK,

		VIRTUAL_STICKY_LEFT_CONTROL,
		VIRTUAL_STICKY_LEFT_SHIFT,
		VIRTUAL_STICKY_LEFT_ALT,
		VIRTUAL_STICKY_LEFT_GUI,
		VIRTUAL_STICKY_RIGHT_CONTROL,
		VIRTUAL_STICKY_RIGHT_SHIFT,
		VIRTUAL_STICKY_RIGHT_ALT,
		VIRTUAL_STICKY_RIGHT_GUI,

		VIRTUAL_MOUSE_N,
		VIRTUAL_MOUSE_NE,
		VIRTUAL_MOUSE_E,
		VIRTUAL_MOUSE_SE,
		VIRTUAL_MOUSE_S,
		VIRTUAL_MOUSE_SW,
		VIRTUAL_MOUSE_W,
		VIRTUAL_MOUSE_NW,
		VIRTUAL_MOUSE_WHEEL_UP,
		VIRTUAL_MOUSE_WHEEL_DOWN,
		VIRTUAL_MOUSE_BUTTON_1_PRESS,
		VIRTUAL_MOUSE_BUTTON_2_PRESS,
		VIRTUAL_MOUSE_BUTTON_3_PRESS,
		VIRTUAL_MOUSE_BUTTON_1_TOGGLE,
		VIRTUAL_MOUSE_BUTTON_2_TOGGLE,
		VIRTUAL_MOUSE_BUTTON_3_TOGGLE,

	};


	Keymap(
		USB::HID::Mouse* const pMouse,
		USB::HID::Keyboard* const pKeyboard,
		USB::HID::KeyboardExtension* const pKeyboardExtension,
		float const mouseMoveMinimumTime,
		float const mouseMoveMaximumTime,
		float const mouseMoveMinimumVelocity,
		float const mouseMoveMaximumVelocity,
		float const mouseWheelMinimumTime,
		float const mouseWheelMaximumTime,
		float const mouseWheelMinimumVelocity,
		float const mouseWheelMaximumVelocity,
		uint16_t const* const extendedKeymap = NULL
	);


	inline void SetLayer( uint8_t const index, uint8_t const* const layer = NULL );


	void Press( uint8_t const code );
	void Release( uint8_t const code );


	void Update();


private:

	void VirtualMouseMoveStart( uint8_t const index );
	void VirtualMouseMove(
		uint8_t const index,
		float const duration,
		float const minimumTime,
		float const maximumTime,
		float const minimumVelocity,
		float const maximumVelocity
	);

	void VirtualTap( uint16_t const mappedCode, uint8_t const code );
	void VirtualPress( uint16_t const mappedCode );
	void VirtualRelease( uint16_t const mappedCode );


	USB::HID::Mouse* m_pMouse;
	USB::HID::Keyboard* m_pKeyboard;
	USB::HID::KeyboardExtension* m_pKeyboardExtension;

	uint16_t const* m_extendedKeymap;
	uint8_t const* m_layers[ MAXIMUM_LAYERS ];

	uint16_t m_oldPressed[ 16 ];
	uint16_t m_pressed[ 16 ];

	uint8_t m_assignments[ 256 ];
	uint16_t m_active[ 16 ];

	uint8_t m_layerSelected;
	uint8_t m_layerLocked;

	uint16_t m_normalKeypresses;
	uint8_t m_pressedModifiers;
	uint8_t m_pressedStickyModifiers;
	uint8_t m_stickyModifiers;
	uint8_t m_unstickyModifiers;
	uint8_t m_modifiers;
	uint8_t m_oldModifiers;

	float m_mouseMoveMinimumTime;
	float m_mouseMoveMaximumTime;
	float m_mouseMoveMinimumVelocity;
	float m_mouseMoveMaximumVelocity;

	float m_mouseWheelMinimumTime;
	float m_mouseWheelMaximumTime;
	float m_mouseWheelMinimumVelocity;
	float m_mouseWheelMaximumVelocity;

	float m_mouseMoveTime[ 10 ];
	float m_mouseCoordinates[ 3 ];
	uint8_t m_mouseMovement;

	uint16_t m_pressedMouseDirections;
	uint8_t m_pressedMouseButtons;
	uint8_t m_toggledMouseButtons;
	uint8_t m_mouseButtons;
	uint8_t m_oldMouseButtons;
};




//============================================================================
//    Keymap inline methods
//============================================================================


void Keymap::SetLayer( uint8_t const index, uint8_t const* const layer ) {

	if ( index < MAXIMUM_LAYERS )
		m_layers[ index ] = layer;
}




//============================================================================
//    Keymap macros
//============================================================================


#define KEYCODE_KEYBOARD( nn ) ( ( nn ) | Keymap::KEYCODE_TYPE_KEYBOARD )
#define KEYCODE_CONSUMER( nn ) ( ( nn ) | Keymap::KEYCODE_TYPE_CONSUMER )
#define KEYCODE_VIRTUAL(  nn ) ( ( nn ) | Keymap::KEYCODE_TYPE_VIRTUAL  )




#endif    /* __cplusplus */

#endif    /* __KEYMAP_HH__ */

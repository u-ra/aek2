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
	\file keymap.cc
	\brief Keymap implementation
*/




#include "keymap.hh"
#include "timer.hh"

#include <math.h>

#include <avr/pgmspace.h>




//============================================================================
//    Base keymap
//============================================================================


extern uint16_t const g_baseKeymap[] __attribute__(( __progmem__ ));
uint16_t const g_baseKeymap[] = {

	0,

	KEYCODE_KEYBOARD( USB::HID::KEY_A ),
	KEYCODE_KEYBOARD( USB::HID::KEY_B ),
	KEYCODE_KEYBOARD( USB::HID::KEY_C ),
	KEYCODE_KEYBOARD( USB::HID::KEY_D ),
	KEYCODE_KEYBOARD( USB::HID::KEY_E ),
	KEYCODE_KEYBOARD( USB::HID::KEY_F ),
	KEYCODE_KEYBOARD( USB::HID::KEY_G ),
	KEYCODE_KEYBOARD( USB::HID::KEY_H ),
	KEYCODE_KEYBOARD( USB::HID::KEY_I ),
	KEYCODE_KEYBOARD( USB::HID::KEY_J ),
	KEYCODE_KEYBOARD( USB::HID::KEY_K ),
	KEYCODE_KEYBOARD( USB::HID::KEY_L ),
	KEYCODE_KEYBOARD( USB::HID::KEY_M ),
	KEYCODE_KEYBOARD( USB::HID::KEY_N ),
	KEYCODE_KEYBOARD( USB::HID::KEY_O ),
	KEYCODE_KEYBOARD( USB::HID::KEY_P ),
	KEYCODE_KEYBOARD( USB::HID::KEY_Q ),
	KEYCODE_KEYBOARD( USB::HID::KEY_R ),
	KEYCODE_KEYBOARD( USB::HID::KEY_S ),
	KEYCODE_KEYBOARD( USB::HID::KEY_T ),
	KEYCODE_KEYBOARD( USB::HID::KEY_U ),
	KEYCODE_KEYBOARD( USB::HID::KEY_V ),
	KEYCODE_KEYBOARD( USB::HID::KEY_W ),
	KEYCODE_KEYBOARD( USB::HID::KEY_X ),
	KEYCODE_KEYBOARD( USB::HID::KEY_Y ),
	KEYCODE_KEYBOARD( USB::HID::KEY_Z ),
	KEYCODE_KEYBOARD( USB::HID::KEY_1 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_2 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_3 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_4 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_5 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_6 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_7 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_8 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_9 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_0 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_ENTER ),
	KEYCODE_KEYBOARD( USB::HID::KEY_ESCAPE ),
	KEYCODE_KEYBOARD( USB::HID::KEY_BACKSPACE ),
	KEYCODE_KEYBOARD( USB::HID::KEY_TAB ),
	KEYCODE_KEYBOARD( USB::HID::KEY_SPACEBAR ),
	KEYCODE_KEYBOARD( USB::HID::KEY_DASH ),
	KEYCODE_KEYBOARD( USB::HID::KEY_EQUALS ),
	KEYCODE_KEYBOARD( USB::HID::KEY_LEFT_BRACKET ),
	KEYCODE_KEYBOARD( USB::HID::KEY_RIGHT_BRACKET ),
	KEYCODE_KEYBOARD( USB::HID::KEY_BACKSLASH ),
	KEYCODE_KEYBOARD( USB::HID::KEY_NON_US_HASH ),
	KEYCODE_KEYBOARD( USB::HID::KEY_SEMICOLON ),
	KEYCODE_KEYBOARD( USB::HID::KEY_QUOTE ),
	KEYCODE_KEYBOARD( USB::HID::KEY_BACKQUOTE ),
	KEYCODE_KEYBOARD( USB::HID::KEY_COMMA ),
	KEYCODE_KEYBOARD( USB::HID::KEY_PERIOD ),
	KEYCODE_KEYBOARD( USB::HID::KEY_SLASH ),
	KEYCODE_KEYBOARD( USB::HID::KEY_CAPS_LOCK ),
	KEYCODE_KEYBOARD( USB::HID::KEY_F1 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_F2 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_F3 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_F4 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_F5 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_F6 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_F7 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_F8 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_F9 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_F10 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_F11 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_F12 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_PRINT_SCREEN ),
	KEYCODE_KEYBOARD( USB::HID::KEY_SCROLL_LOCK ),
	KEYCODE_KEYBOARD( USB::HID::KEY_PAUSE ),
	KEYCODE_KEYBOARD( USB::HID::KEY_INSERT ),
	KEYCODE_KEYBOARD( USB::HID::KEY_HOME ),
	KEYCODE_KEYBOARD( USB::HID::KEY_PAGE_UP ),
	KEYCODE_KEYBOARD( USB::HID::KEY_DELETE ),
	KEYCODE_KEYBOARD( USB::HID::KEY_END ),
	KEYCODE_KEYBOARD( USB::HID::KEY_PAGE_DOWN ),
	KEYCODE_KEYBOARD( USB::HID::KEY_RIGHT_ARROW ),
	KEYCODE_KEYBOARD( USB::HID::KEY_LEFT_ARROW ),
	KEYCODE_KEYBOARD( USB::HID::KEY_DOWN_ARROW ),
	KEYCODE_KEYBOARD( USB::HID::KEY_UP_ARROW ),
	KEYCODE_KEYBOARD( USB::HID::KEY_NUM_LOCK ),
	KEYCODE_KEYBOARD( USB::HID::KEY_KEYPAD_SLASH ),
	KEYCODE_KEYBOARD( USB::HID::KEY_KEYPAD_STAR ),
	KEYCODE_KEYBOARD( USB::HID::KEY_KEYPAD_DASH ),
	KEYCODE_KEYBOARD( USB::HID::KEY_KEYPAD_PLUS ),
	KEYCODE_KEYBOARD( USB::HID::KEY_KEYPAD_ENTER ),
	KEYCODE_KEYBOARD( USB::HID::KEY_KEYPAD_1 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_KEYPAD_2 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_KEYPAD_3 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_KEYPAD_4 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_KEYPAD_5 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_KEYPAD_6 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_KEYPAD_7 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_KEYPAD_8 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_KEYPAD_9 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_KEYPAD_0 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_KEYPAD_PERIOD ),
	KEYCODE_KEYBOARD( USB::HID::KEY_NON_US_BACKSLASH ),
	KEYCODE_KEYBOARD( USB::HID::KEY_APPLICATION ),
	KEYCODE_KEYBOARD( USB::HID::KEY_POWER ),
	KEYCODE_KEYBOARD( USB::HID::KEY_KEYPAD_EQUALS ),
	KEYCODE_KEYBOARD( USB::HID::KEY_F13 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_F14 ),
	KEYCODE_KEYBOARD( USB::HID::KEY_F15 ),

	KEYCODE_KEYBOARD( USB::HID::KEY_LEFT_CONTROL ),
	KEYCODE_KEYBOARD( USB::HID::KEY_LEFT_SHIFT ),
	KEYCODE_KEYBOARD( USB::HID::KEY_LEFT_ALT ),
	KEYCODE_KEYBOARD( USB::HID::KEY_LEFT_GUI ),
	KEYCODE_KEYBOARD( USB::HID::KEY_RIGHT_CONTROL ),
	KEYCODE_KEYBOARD( USB::HID::KEY_RIGHT_SHIFT ),
	KEYCODE_KEYBOARD( USB::HID::KEY_RIGHT_ALT ),
	KEYCODE_KEYBOARD( USB::HID::KEY_RIGHT_GUI ),

	KEYCODE_VIRTUAL( Keymap::VIRTUAL_LOCKING_CAPS_LOCK ),
	KEYCODE_VIRTUAL( Keymap::VIRTUAL_LOCKING_NUM_LOCK ),
	KEYCODE_VIRTUAL( Keymap::VIRTUAL_LOCKING_SCROLL_LOCK )

};
static_assert( ARRAYLENGTH( g_baseKeymap ) == KEYMAP_BASE_END, "g_baseKeymap must have length KEYMAP_BASE_END" );




//============================================================================
//    Mouse directions
//============================================================================


extern float const g_mouseMoveDirection[] __attribute__(( __progmem__ ));
float const g_mouseMoveDirection[] = {
	 0,         -1,          0,    // N
	 0.7071068, -0.7071068,  0,    // NE
	 1,          0,          0,    // E
	 0.7071068,  0.7071068,  0,    // SE
	 0,          1,          0,    // S
	-0.7071068,  0.7071068,  0,    // SW
	-1,          0,          0,    // W
	-0.7071068, -0.7071068,  0,    // NW
	 0,          0,          1,    // Wheel+
	 0,          0,         -1     // Wheel-
};




//============================================================================
//    Keymap methods
//============================================================================


Keymap::Keymap(
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
	uint16_t const* const extendedKeymap
) :
	m_pMouse( pMouse ),
	m_pKeyboard( pKeyboard ),
	m_pKeyboardExtension( pKeyboardExtension ),
	m_extendedKeymap( extendedKeymap ),
	m_layerSelected( 0xff ),
	m_layerLocked( 0 ),
	m_normalKeypresses( 0 ),
	m_pressedModifiers( 0 ),
	m_pressedStickyModifiers( 0 ),
	m_stickyModifiers( 0 ),
	m_unstickyModifiers( 0 ),
	m_modifiers( 0 ),
	m_oldModifiers( 0 ),
	m_mouseMoveMinimumTime( mouseMoveMinimumTime ),
	m_mouseMoveMaximumTime( mouseMoveMaximumTime ),
	m_mouseMoveMinimumVelocity( mouseMoveMinimumVelocity ),
	m_mouseMoveMaximumVelocity( mouseMoveMaximumVelocity ),
	m_mouseWheelMinimumTime( mouseWheelMinimumTime ),
	m_mouseWheelMaximumTime( mouseWheelMaximumTime ),
	m_mouseWheelMinimumVelocity( mouseWheelMinimumVelocity ),
	m_mouseWheelMaximumVelocity( mouseWheelMaximumVelocity ),
	m_mouseMovement( 0 ),
	m_pressedMouseDirections( 0 ),
	m_pressedMouseButtons( 0 ),
	m_toggledMouseButtons( 0 ),
	m_mouseButtons( 0 ),
	m_oldMouseButtons( 0 )
{
	memset( m_layers, 0, sizeof( m_layers ) );

	memset( m_oldPressed, 0, sizeof( m_oldPressed ) );
	memset( m_pressed, 0, sizeof( m_pressed ) );

	memset( m_assignments, 0, sizeof( m_assignments ) );
	memset( m_active, 0, sizeof( m_active ) );

	memset( m_mouseMoveTime, 0, sizeof( m_mouseMoveTime ) );
	memset( m_mouseCoordinates, 0, sizeof( m_mouseCoordinates ) );
}


void Keymap::Press( uint8_t const code ) {

	if ( m_assignments[ code ] == 0 ) {

		uint8_t assignment = 0;
		if ( m_layerSelected < MAXIMUM_LAYERS ) {

			uint8_t const* const layer = m_layers[ m_layerSelected ];
			if ( layer != NULL ) {

				assignment = pgm_read_byte( layer + code );
				uint16_t const mapped = ( ( assignment < KEYMAP_BASE_END ) ? pgm_read_word( g_baseKeymap + assignment ) : ( ( m_extendedKeymap != NULL ) ? pgm_read_word( m_extendedKeymap + assignment - KEYMAP_BASE_END ) : 0 ) );
				if ( ( mapped & KEYCODE_TYPE_MASK ) == KEYCODE_TYPE_VIRTUAL ) {

					uint16_t const mappedCode = ( mapped & KEYCODE_CODE_MASK );
					if ( ( mappedCode >= VIRTUAL_LAYER_SELECT ) && ( mappedCode < VIRTUAL_LAYER_SELECT + MAXIMUM_LAYERS ) )    // ignore new layer selection keypresses
						assignment = 0;
				}
			}
		}
		else {

			uint8_t const* const layer = m_layers[ m_layerLocked ];
			if ( layer != NULL )
				assignment = pgm_read_byte( layer + code );
		}

		if ( assignment != 0 ) {

			m_assignments[ code ] = assignment;
			m_active[ code >> 4 ] |= ( 1u << ( code & 15 ) );
		}
	}
}


void Keymap::Release( uint8_t const code ) {

	uint8_t const assignment = m_assignments[ code ];

	uint8_t const index = ( assignment >> 4 );
	uint16_t const mask = ( 1u << ( assignment & 15 ) );
	if ( ! ( m_oldPressed[ index ] & mask ) )
		m_pressed[ index ] |= mask;

	m_assignments[ code ] = 0;
	m_active[ code >> 4 ] &= ~( 1u << ( code & 15 ) );
}


void Keymap::Update() {

	static uint8_t lastCode = 0;
	static uint8_t lastAssignment = 0;
	static uint16_t oldOverflows = 0;

	if ( ! ( ( ( m_pMouse != NULL ) && m_pMouse->IsChanged() ) || ( ( m_pKeyboard != NULL ) && m_pKeyboard->IsChanged() ) || ( ( m_pKeyboardExtension != NULL ) && m_pKeyboardExtension->IsChanged() ) ) ) {

		uint16_t const overflows = Timer::Instance()->GetOverflows();
		float const duration = ( ( overflows - oldOverflows ) / 256.0 );

		if ( lastCode != 0 ) {

			if ( ! ( m_active[ lastCode >> 4 ] & ( 1u << ( lastCode & 15 ) ) ) ) {

				uint16_t const mapped = ( ( lastAssignment < KEYMAP_BASE_END ) ? pgm_read_word( g_baseKeymap + lastAssignment ) : ( ( m_extendedKeymap != NULL ) ? pgm_read_word( m_extendedKeymap + lastAssignment - KEYMAP_BASE_END ) : 0 ) );
				if ( ( mapped & KEYCODE_TYPE_MASK ) == KEYCODE_TYPE_VIRTUAL )
					VirtualTap( mapped & KEYCODE_CODE_MASK, lastCode );
				lastCode = 0;
				lastAssignment = 0;
			}
		}

		for ( unsigned int ii = 0; ii < ARRAYLENGTH( m_active ); ++ii ) {

			if ( m_active[ ii ] != 0 ) {

				uint8_t code = ii * 16;
				for ( unsigned int jj = 0; jj < 16; ++jj ) {

					if ( m_active[ ii ] & ( 1u << jj ) ) {

						uint8_t const assignment = m_assignments[ code ];
						if ( assignment != 0 ) {

							uint8_t const index = ( assignment >> 4 );
							uint16_t const mask = ( 1u << ( assignment & 15 ) );
							if ( ! ( m_oldPressed[ index ] & mask ) ) {

								lastCode = code;
								lastAssignment = assignment;
							}
							m_pressed[ index ] |= mask;
						}
					}

					++code;
				}
			}
		}

		for ( unsigned int ii = 0; ii < ARRAYLENGTH( m_pressed ); ++ii ) {

			if ( m_oldPressed[ ii ] != m_pressed[ ii ] ) {

				uint8_t assignment = ii * 16;
				for ( unsigned int jj = 0; jj < 16; ++jj ) {

					uint16_t const mask = ( 1u << jj );
					bool const oldPressed = ( ( m_oldPressed[ ii ] & mask ) != 0 );
					bool const pressed = ( ( m_pressed[ ii ] & mask ) != 0 );
					if ( oldPressed != pressed ) {

						uint16_t const mapped = ( ( assignment < KEYMAP_BASE_END ) ? pgm_read_word( g_baseKeymap + assignment ) : ( ( m_extendedKeymap != NULL ) ? pgm_read_word( m_extendedKeymap + assignment - KEYMAP_BASE_END ) : 0 ) );
						if ( ( mapped & KEYCODE_TYPE_MASK ) == KEYCODE_TYPE_VIRTUAL ) {

							uint16_t const mappedCode = ( mapped & KEYCODE_CODE_MASK );
							if ( pressed )
								VirtualPress( mappedCode );
							else
								VirtualRelease( mappedCode );
						}
					}

					++assignment;
				}
			}
		}

		for ( unsigned int ii = 0; ii < ARRAYLENGTH( m_pressed ); ++ii ) {

			if ( m_oldPressed[ ii ] != m_pressed[ ii ] ) {

				uint8_t assignment = ii * 16;
				for ( unsigned int jj = 0; jj < 16; ++jj ) {

					uint16_t const mask = ( 1u << jj );
					bool const oldPressed = ( ( m_oldPressed[ ii ] & mask ) != 0 );
					bool const pressed = ( ( m_pressed[ ii ] & mask ) != 0 );
					if ( oldPressed != pressed ) {

						uint16_t const mapped = ( ( assignment < KEYMAP_BASE_END ) ? pgm_read_word( g_baseKeymap + assignment ) : ( ( m_extendedKeymap != NULL ) ? pgm_read_word( m_extendedKeymap + assignment - KEYMAP_BASE_END ) : 0 ) );
						switch( mapped & KEYCODE_TYPE_MASK ) {

							case KEYCODE_TYPE_KEYBOARD: {

								USB::HID::Key const mappedCode = static_cast< USB::HID::Key >( mapped & KEYCODE_CODE_MASK );
								if ( pressed ) {

									switch( mappedCode ) {

										case USB::HID::KEY_LEFT_CONTROL:  { m_pressedModifiers |= ( 1u << 0 ); break; }
										case USB::HID::KEY_LEFT_SHIFT:    { m_pressedModifiers |= ( 1u << 1 ); break; }
										case USB::HID::KEY_LEFT_ALT:      { m_pressedModifiers |= ( 1u << 2 ); break; }
										case USB::HID::KEY_LEFT_GUI:      { m_pressedModifiers |= ( 1u << 3 ); break; }
										case USB::HID::KEY_RIGHT_CONTROL: { m_pressedModifiers |= ( 1u << 4 ); break; }
										case USB::HID::KEY_RIGHT_SHIFT:   { m_pressedModifiers |= ( 1u << 5 ); break; }
										case USB::HID::KEY_RIGHT_ALT:     { m_pressedModifiers |= ( 1u << 6 ); break; }
										case USB::HID::KEY_RIGHT_GUI:     { m_pressedModifiers |= ( 1u << 7 ); break; }

										default: {

											if ( ! ( ( m_pKeyboard != NULL ) && m_pKeyboard->PressKey( mappedCode ) ) )
												if ( m_pKeyboardExtension != NULL )
													m_pKeyboardExtension->PressKey( mappedCode );
											++m_normalKeypresses;

											break;
										}
									}
								}
								else {

									switch( mappedCode ) {

										case USB::HID::KEY_LEFT_CONTROL:  { m_pressedModifiers &= ~( 1u << 0 ); break; }
										case USB::HID::KEY_LEFT_SHIFT:    { m_pressedModifiers &= ~( 1u << 1 ); break; }
										case USB::HID::KEY_LEFT_ALT:      { m_pressedModifiers &= ~( 1u << 2 ); break; }
										case USB::HID::KEY_LEFT_GUI:      { m_pressedModifiers &= ~( 1u << 3 ); break; }
										case USB::HID::KEY_RIGHT_CONTROL: { m_pressedModifiers &= ~( 1u << 4 ); break; }
										case USB::HID::KEY_RIGHT_SHIFT:   { m_pressedModifiers &= ~( 1u << 5 ); break; }
										case USB::HID::KEY_RIGHT_ALT:     { m_pressedModifiers &= ~( 1u << 6 ); break; }
										case USB::HID::KEY_RIGHT_GUI:     { m_pressedModifiers &= ~( 1u << 7 ); break; }

										default: {

											if ( m_pKeyboard != NULL )
												m_pKeyboard->ReleaseKey( mappedCode );
											if ( m_pKeyboardExtension != NULL )
												m_pKeyboardExtension->ReleaseKey( mappedCode );
											--m_normalKeypresses;

											break;
										}
									}
								}

								break;
							}

							case KEYCODE_TYPE_CONSUMER: {

								USB::HID::Consumer const mappedCode = static_cast< USB::HID::Consumer >( mapped & KEYCODE_CODE_MASK );
								if ( pressed ) {

									if ( m_pKeyboardExtension != NULL )
										m_pKeyboardExtension->PressConsumer( mappedCode );
									++m_normalKeypresses;
								}
								else {

									if ( m_pKeyboardExtension != NULL )
										m_pKeyboardExtension->ReleaseConsumer( mappedCode );
									--m_normalKeypresses;
								}

								break;
							}
						}
					}

					++assignment;
				}
			}
		}

		if ( m_normalKeypresses > 0 )
			m_unstickyModifiers |= m_stickyModifiers;
		else {

			m_stickyModifiers &= ~m_unstickyModifiers;
			m_unstickyModifiers = 0;
		}
		m_modifiers = ( m_pressedModifiers | m_pressedStickyModifiers | m_stickyModifiers );
		if ( m_oldModifiers != m_modifiers ) {

			for ( unsigned int ii = 0; ii < 8; ++ii ) {

				uint8_t const mask = ( 1u << ii );
				bool const oldPressed = ( ( m_oldModifiers & mask ) != 0 );
				bool const pressed = ( ( m_modifiers & mask ) != 0 );
				if ( oldPressed != pressed ) {

					USB::HID::Key const mappedCode = static_cast< USB::HID::Key >( USB::HID::KEY_LEFT_CONTROL + ii );
					if ( pressed ) {

						if ( ! ( ( m_pKeyboard != NULL ) && m_pKeyboard->PressKey( mappedCode ) ) )
							if ( m_pKeyboardExtension != NULL )
								m_pKeyboardExtension->PressKey( mappedCode );
					}
					else {

						if ( m_pKeyboard != NULL )
							m_pKeyboard->ReleaseKey( mappedCode );
						if ( m_pKeyboardExtension != NULL )
							m_pKeyboardExtension->ReleaseKey( mappedCode );
					}
				}
			}
			m_oldModifiers = m_modifiers;
		}

		memcpy( m_oldPressed, m_pressed, sizeof( m_oldPressed ) );
		memset( m_pressed, 0, sizeof( m_pressed ) );

		if ( m_pressedMouseDirections != 0 ) {

			for ( unsigned int ii = 0; ii < 8; ++ii )
				if ( m_pressedMouseDirections & ( 1u << ii ) )
					VirtualMouseMove( ii, duration, m_mouseMoveMinimumTime, m_mouseMoveMaximumTime, m_mouseMoveMinimumVelocity, m_mouseMoveMaximumVelocity );
			for ( unsigned int ii = 8; ii < 10; ++ii )
				if ( m_pressedMouseDirections & ( 1u << ii ) )
					VirtualMouseMove( ii, duration, m_mouseWheelMinimumTime, m_mouseWheelMaximumTime, m_mouseWheelMinimumVelocity, m_mouseWheelMaximumVelocity );
		}

		m_mouseButtons = ( m_pressedMouseButtons | m_toggledMouseButtons );
		if ( m_pMouse != NULL ) {

			if ( m_mouseButtons != m_oldMouseButtons ) {

				if ( ( m_mouseButtons & 1 ) != ( m_oldMouseButtons & 1 ) ) {

					if ( m_mouseButtons & 1 )
						m_pMouse->PressButton( USB::HID::BUTTON_1 );
					else
						m_pMouse->ReleaseButton( USB::HID::BUTTON_1 );
				}
				if ( ( m_mouseButtons & 2 ) != ( m_oldMouseButtons & 2 ) ) {

					if ( m_mouseButtons & 2 )
						m_pMouse->PressButton( USB::HID::BUTTON_2 );
					else
						m_pMouse->ReleaseButton( USB::HID::BUTTON_2 );
				}
				if ( ( m_mouseButtons & 4 ) != ( m_oldMouseButtons & 4 ) ) {

					if ( m_mouseButtons & 4 )
						m_pMouse->PressButton( USB::HID::BUTTON_3 );
					else
						m_pMouse->ReleaseButton( USB::HID::BUTTON_3 );
				}
				m_oldMouseButtons = m_mouseButtons;
			}
		}

		if ( m_pMouse != NULL ) {

			for ( unsigned int ii = 0; ii < 3; ++ii ) {

				if ( ! ( m_mouseMovement & ( 1u << ii ) ) )
					m_mouseCoordinates[ ii ] = 0;
			}
			int16_t const mouseXX    = trunc( m_mouseCoordinates[ 0 ] );
			int16_t const mouseYY    = trunc( m_mouseCoordinates[ 1 ] );
			int16_t const mouseWheel = trunc( m_mouseCoordinates[ 2 ] );
			if ( ( mouseXX != 0 ) || ( mouseYY != 0 ) || ( mouseWheel != 0 ) ) {

				m_pMouse->Move( mouseXX, mouseYY, mouseWheel );
				m_mouseCoordinates[ 0 ] -= mouseXX;
				m_mouseCoordinates[ 1 ] -= mouseYY;
				m_mouseCoordinates[ 2 ] -= mouseWheel;
			}
			m_mouseMovement = 0;
		}

		oldOverflows = overflows;
	}
}


void Keymap::VirtualMouseMoveStart( uint8_t const index ) {

	m_mouseMoveTime[ index ] = 0;
	for ( unsigned int ii = 0; ii < 3; ++ii ) {

		float const direction = pgm_read_float( g_mouseMoveDirection + index * 3 + ii );
		if ( direction != 0 ) {

			if ( direction > 0 )
				++m_mouseCoordinates[ ii ];
			else
				--m_mouseCoordinates[ ii ];
			m_mouseMovement |= ( 1u << ii );
		}
	}
}


void Keymap::VirtualMouseMove(
	uint8_t const index,
	float const duration,
	float const minimumTime,
	float const maximumTime,
	float const minimumVelocity,
	float const maximumVelocity
)
{
	float oldMouseTime = m_mouseMoveTime[ index ];
	float newMouseTime = oldMouseTime + duration;

	float delta = 0;
	if ( oldMouseTime < minimumTime )
		delta += ( Min( newMouseTime, minimumTime ) - oldMouseTime ) * minimumVelocity;
	if ( ( newMouseTime > minimumTime ) && ( oldMouseTime < maximumTime ) ) {

		float const lowerTime = Max( oldMouseTime, minimumTime );
		float const upperTime = Min( newMouseTime, maximumTime );
		delta += ( upperTime - lowerTime ) * ( minimumVelocity + ( 0.5 * ( lowerTime + upperTime ) - minimumTime ) * ( maximumVelocity - minimumVelocity ) / ( maximumTime - minimumTime ) );
	}
	if ( newMouseTime > maximumTime )
		delta += ( newMouseTime - Max( oldMouseTime, maximumTime ) ) * maximumVelocity;

	for ( unsigned int ii = 0; ii < 3; ++ii ) {

		float const direction = pgm_read_float( g_mouseMoveDirection + index * 3 + ii );
		if ( direction != 0 ) {

			m_mouseCoordinates[ ii ] += direction * delta;
			m_mouseMovement |= ( 1u << ii );
		}
	}

	m_mouseMoveTime[ index ] = Min( newMouseTime, maximumTime );
}


void Keymap::VirtualTap( uint16_t const mappedCode, uint8_t const code ) {

	if ( ( mappedCode >= VIRTUAL_LAYER_SELECT ) && ( mappedCode < VIRTUAL_LAYER_SELECT + MAXIMUM_LAYERS ) ) {

		uint8_t const* const layer = m_layers[ mappedCode - VIRTUAL_LAYER_SELECT ];
		if ( layer != NULL ) {

			uint8_t const assignment = pgm_read_byte( layer + code );
			if ( assignment != 0 ) {

				uint16_t const layerMapped = ( ( assignment < KEYMAP_BASE_END ) ? pgm_read_word( g_baseKeymap + assignment ) : ( ( m_extendedKeymap != NULL ) ? pgm_read_word( m_extendedKeymap + assignment - KEYMAP_BASE_END ) : 0 ) );
				if ( ( layerMapped & KEYCODE_TYPE_MASK ) != KEYCODE_TYPE_VIRTUAL ) {

					uint8_t const index = ( assignment >> 4 );
					uint16_t const mask = ( 1u << ( assignment & 15 ) );
					m_pressed[ index ] |= mask;
				}
			}
		}
	}
	else {

		switch( mappedCode ) {

			case VIRTUAL_STICKY_LEFT_CONTROL:  { m_stickyModifiers ^= ( 1u << 0 ); m_unstickyModifiers &= ~( 1u << 0 ); break; }
			case VIRTUAL_STICKY_LEFT_SHIFT:    { m_stickyModifiers ^= ( 1u << 1 ); m_unstickyModifiers &= ~( 1u << 1 ); break; }
			case VIRTUAL_STICKY_LEFT_ALT:      { m_stickyModifiers ^= ( 1u << 2 ); m_unstickyModifiers &= ~( 1u << 2 ); break; }
			case VIRTUAL_STICKY_LEFT_GUI:      { m_stickyModifiers ^= ( 1u << 3 ); m_unstickyModifiers &= ~( 1u << 3 ); break; }
			case VIRTUAL_STICKY_RIGHT_CONTROL: { m_stickyModifiers ^= ( 1u << 4 ); m_unstickyModifiers &= ~( 1u << 4 ); break; }
			case VIRTUAL_STICKY_RIGHT_SHIFT:   { m_stickyModifiers ^= ( 1u << 5 ); m_unstickyModifiers &= ~( 1u << 5 ); break; }
			case VIRTUAL_STICKY_RIGHT_ALT:     { m_stickyModifiers ^= ( 1u << 6 ); m_unstickyModifiers &= ~( 1u << 6 ); break; }
			case VIRTUAL_STICKY_RIGHT_GUI:     { m_stickyModifiers ^= ( 1u << 7 ); m_unstickyModifiers &= ~( 1u << 7 ); break; }
		}
	}
}


void Keymap::VirtualPress( uint16_t const mappedCode ) {

	if ( ( mappedCode >= VIRTUAL_LAYER_SELECT ) && ( mappedCode < VIRTUAL_LAYER_SELECT + MAXIMUM_LAYERS ) )
		m_layerSelected = mappedCode - VIRTUAL_LAYER_SELECT;
	else if ( ( mappedCode >= VIRTUAL_LAYER_LOCK ) && ( mappedCode < VIRTUAL_LAYER_LOCK + MAXIMUM_LAYERS ) )
		m_layerLocked = mappedCode - VIRTUAL_LAYER_LOCK;
	else {

		switch( mappedCode ) {

			case VIRTUAL_LOCKING_CAPS_LOCK: {

				if ( ( m_pKeyboard != NULL ) && ( ! m_pKeyboard->GetLED( USB::HID::LED_CAPS_LOCK ) ) )
					m_pressed[ KEYMAP_KEYBOARD_CAPS_LOCK >> 4 ] |= ( 1u << ( KEYMAP_KEYBOARD_CAPS_LOCK & 15 ) );
				break;
			}

			case VIRTUAL_LOCKING_NUM_LOCK: {

				if ( ( m_pKeyboard != NULL ) && ( ! m_pKeyboard->GetLED( USB::HID::LED_NUM_LOCK ) ) )
					m_pressed[ KEYMAP_KEYBOARD_NUM_LOCK >> 4 ] |= ( 1u << ( KEYMAP_KEYBOARD_NUM_LOCK & 15 ) );
				break;
			}

			case VIRTUAL_LOCKING_SCROLL_LOCK: {

				if ( ( m_pKeyboard != NULL ) && ( ! m_pKeyboard->GetLED( USB::HID::LED_SCROLL_LOCK ) ) )
					m_pressed[ KEYMAP_KEYBOARD_SCROLL_LOCK >> 4 ] |= ( 1u << ( KEYMAP_KEYBOARD_SCROLL_LOCK & 15 ) );
				break;
			}

			case VIRTUAL_STICKY_LEFT_CONTROL:  { m_pressedStickyModifiers |= ( 1u << 0 ); break; }
			case VIRTUAL_STICKY_LEFT_SHIFT:    { m_pressedStickyModifiers |= ( 1u << 1 ); break; }
			case VIRTUAL_STICKY_LEFT_ALT:      { m_pressedStickyModifiers |= ( 1u << 2 ); break; }
			case VIRTUAL_STICKY_LEFT_GUI:      { m_pressedStickyModifiers |= ( 1u << 3 ); break; }
			case VIRTUAL_STICKY_RIGHT_CONTROL: { m_pressedStickyModifiers |= ( 1u << 4 ); break; }
			case VIRTUAL_STICKY_RIGHT_SHIFT:   { m_pressedStickyModifiers |= ( 1u << 5 ); break; }
			case VIRTUAL_STICKY_RIGHT_ALT:     { m_pressedStickyModifiers |= ( 1u << 6 ); break; }
			case VIRTUAL_STICKY_RIGHT_GUI:     { m_pressedStickyModifiers |= ( 1u << 7 ); break; }

			case VIRTUAL_MOUSE_N:          { VirtualMouseMoveStart( 0 ); m_pressedMouseDirections |= ( 1u << 0 ); break; }
			case VIRTUAL_MOUSE_NE:         { VirtualMouseMoveStart( 1 ); m_pressedMouseDirections |= ( 1u << 1 ); break; }
			case VIRTUAL_MOUSE_E:          { VirtualMouseMoveStart( 2 ); m_pressedMouseDirections |= ( 1u << 2 ); break; }
			case VIRTUAL_MOUSE_SE:         { VirtualMouseMoveStart( 3 ); m_pressedMouseDirections |= ( 1u << 3 ); break; }
			case VIRTUAL_MOUSE_S:          { VirtualMouseMoveStart( 4 ); m_pressedMouseDirections |= ( 1u << 4 ); break; }
			case VIRTUAL_MOUSE_SW:         { VirtualMouseMoveStart( 5 ); m_pressedMouseDirections |= ( 1u << 5 ); break; }
			case VIRTUAL_MOUSE_W:          { VirtualMouseMoveStart( 6 ); m_pressedMouseDirections |= ( 1u << 6 ); break; }
			case VIRTUAL_MOUSE_NW:         { VirtualMouseMoveStart( 7 ); m_pressedMouseDirections |= ( 1u << 7 ); break; }
			case VIRTUAL_MOUSE_WHEEL_UP:   { VirtualMouseMoveStart( 8 ); m_pressedMouseDirections |= ( 1u << 8 ); break; }
			case VIRTUAL_MOUSE_WHEEL_DOWN: { VirtualMouseMoveStart( 9 ); m_pressedMouseDirections |= ( 1u << 9 ); break; }

			case VIRTUAL_MOUSE_BUTTON_1_PRESS: { m_pressedMouseButtons |= ( 1u << 0 ); break; }
			case VIRTUAL_MOUSE_BUTTON_2_PRESS: { m_pressedMouseButtons |= ( 1u << 1 ); break; }
			case VIRTUAL_MOUSE_BUTTON_3_PRESS: { m_pressedMouseButtons |= ( 1u << 2 ); break; }

			case VIRTUAL_MOUSE_BUTTON_1_TOGGLE: { m_toggledMouseButtons ^= ( 1u << 0 ); break; }
			case VIRTUAL_MOUSE_BUTTON_2_TOGGLE: { m_toggledMouseButtons ^= ( 1u << 1 ); break; }
			case VIRTUAL_MOUSE_BUTTON_3_TOGGLE: { m_toggledMouseButtons ^= ( 1u << 2 ); break; }
		}
	}
}


void Keymap::VirtualRelease( uint16_t const mappedCode ) {

	if ( ( mappedCode >= VIRTUAL_LAYER_SELECT ) && ( mappedCode < VIRTUAL_LAYER_SELECT + MAXIMUM_LAYERS ) )
		m_layerSelected = 0xff;
	else {

		switch( mappedCode ) {

			case VIRTUAL_LOCKING_CAPS_LOCK: {

				if ( ( m_pKeyboard != NULL ) && m_pKeyboard->GetLED( USB::HID::LED_CAPS_LOCK ) )
					m_pressed[ KEYMAP_KEYBOARD_CAPS_LOCK >> 4 ] |= ( 1u << ( KEYMAP_KEYBOARD_CAPS_LOCK & 15 ) );
				break;
			}

			case VIRTUAL_LOCKING_NUM_LOCK: {

				if ( ( m_pKeyboard != NULL ) && m_pKeyboard->GetLED( USB::HID::LED_NUM_LOCK ) )
					m_pressed[ KEYMAP_KEYBOARD_NUM_LOCK >> 4 ] |= ( 1u << ( KEYMAP_KEYBOARD_NUM_LOCK & 15 ) );
				break;
			}

			case VIRTUAL_LOCKING_SCROLL_LOCK: {

				if ( ( m_pKeyboard != NULL ) && m_pKeyboard->GetLED( USB::HID::LED_SCROLL_LOCK ) )
					m_pressed[ KEYMAP_KEYBOARD_SCROLL_LOCK >> 4 ] |= ( 1u << ( KEYMAP_KEYBOARD_SCROLL_LOCK & 15 ) );
				break;
			}

			case VIRTUAL_STICKY_LEFT_CONTROL:  { m_pressedStickyModifiers &= ~( 1u << 0 ); break; }
			case VIRTUAL_STICKY_LEFT_SHIFT:    { m_pressedStickyModifiers &= ~( 1u << 1 ); break; }
			case VIRTUAL_STICKY_LEFT_ALT:      { m_pressedStickyModifiers &= ~( 1u << 2 ); break; }
			case VIRTUAL_STICKY_LEFT_GUI:      { m_pressedStickyModifiers &= ~( 1u << 3 ); break; }
			case VIRTUAL_STICKY_RIGHT_CONTROL: { m_pressedStickyModifiers &= ~( 1u << 4 ); break; }
			case VIRTUAL_STICKY_RIGHT_SHIFT:   { m_pressedStickyModifiers &= ~( 1u << 5 ); break; }
			case VIRTUAL_STICKY_RIGHT_ALT:     { m_pressedStickyModifiers &= ~( 1u << 6 ); break; }
			case VIRTUAL_STICKY_RIGHT_GUI:     { m_pressedStickyModifiers &= ~( 1u << 7 ); break; }

			case VIRTUAL_MOUSE_N:          { m_pressedMouseDirections &= ~( 1u << 0 ); break; }
			case VIRTUAL_MOUSE_NE:         { m_pressedMouseDirections &= ~( 1u << 1 ); break; }
			case VIRTUAL_MOUSE_E:          { m_pressedMouseDirections &= ~( 1u << 2 ); break; }
			case VIRTUAL_MOUSE_SE:         { m_pressedMouseDirections &= ~( 1u << 3 ); break; }
			case VIRTUAL_MOUSE_S:          { m_pressedMouseDirections &= ~( 1u << 4 ); break; }
			case VIRTUAL_MOUSE_SW:         { m_pressedMouseDirections &= ~( 1u << 5 ); break; }
			case VIRTUAL_MOUSE_W:          { m_pressedMouseDirections &= ~( 1u << 6 ); break; }
			case VIRTUAL_MOUSE_NW:         { m_pressedMouseDirections &= ~( 1u << 7 ); break; }
			case VIRTUAL_MOUSE_WHEEL_UP:   { m_pressedMouseDirections &= ~( 1u << 8 ); break; }
			case VIRTUAL_MOUSE_WHEEL_DOWN: { m_pressedMouseDirections &= ~( 1u << 9 ); break; }

			case VIRTUAL_MOUSE_BUTTON_1_PRESS: { m_pressedMouseButtons &= ~( 1u << 0 ); break; }
			case VIRTUAL_MOUSE_BUTTON_2_PRESS: { m_pressedMouseButtons &= ~( 1u << 1 ); break; }
			case VIRTUAL_MOUSE_BUTTON_3_PRESS: { m_pressedMouseButtons &= ~( 1u << 2 ); break; }
		}
	}
}

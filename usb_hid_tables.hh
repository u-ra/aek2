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
	\file usb_hid_tables.hh
	\brief USB::HID usage tables
*/




#ifndef __USB_HID_TABLES_HH__
#define __USB_HID_TABLES_HH__

#ifdef __cplusplus




namespace USB {


namespace HID {




//============================================================================
//    Generic enumeration
//============================================================================


/// \brief HID generic desktop page (0x01)
enum Generic {
	GENERIC_NONE = 0,
	GENERIC_POINTER,                      // CP
	GENERIC_MOUSE,                        // CA
	GENERIC_JOYSTICK = 4,                 // CA
	GENERIC_GAME_PAD,                     // CA
	GENERIC_KEYBOARD,                     // CA
	GENERIC_KEYPAD,                       // CA
	GENERIC_MULTI_AXIS_CONTROLLER,        // CA
	GENERIC_TABLET_PC_SYSTEM_CONTROLS,    // CA
	GENERIC_X = 48,                       // DV
	GENERIC_Y,                            // DV
	GENERIC_Z,                            // DV
	GENERIC_RX,                           // DV
	GENERIC_RY,                           // DV
	GENERIC_RZ,                           // DV
	GENERIC_SLIDER,                       // DV
	GENERIC_DIAL,                         // DV
	GENERIC_WHEEL,                        // DV
	GENERIC_HAT_SWITCH,                   // DV
	GENERIC_COUNTED_BUFFER,               // CL
	GENERIC_BYTE_COUNT,                   // DV
	GENERIC_MOTION_WAKEUP,                // OSC
	GENERIC_START,                        // OOC
	GENERIC_SELECT,                       // OOC
	GENERIC_VX = 64,                      // DV
	GENERIC_VY,                           // DV
	GENERIC_VZ,                           // DV
	GENERIC_VBRX,                         // DV
	GENERIC_VBRY,                         // DV
	GENERIC_VBRZ,                         // DV
	GENERIC_VNO,                          // DV
	GENERIC_FEATURE_NOTIFICATION,         // DV,DF
	GENERIC_RESOLUTION_MULTIPLIER,        // DV
	// system controls
	GENERIC_SYSTEM_CONTROL = 128,                     // CA
	GENERIC_SYSTEM_POWER_DOWN,                        // OSC
	GENERIC_SYSTEM_SLEEP,                             // OSC
	GENERIC_SYSTEM_WAKE_UP,                           // OSC
	GENERIC_SYSTEM_CONTEXT_MENU,                      // OSC
	GENERIC_SYSTEM_MAIN_MENU,                         // OSC
	GENERIC_SYSTEM_APP_MENU,                          // OSC
	GENERIC_SYSTEM_MENU_HELP,                         // OSC
	GENERIC_SYSTEM_MENU_EXIT,                         // OSC
	GENERIC_SYSTEM_MENU_SELECT,                       // OSC
	GENERIC_SYSTEM_MENU_RIGHT,                        // RTC
	GENERIC_SYSTEM_MENU_LEFT,                         // RTC
	GENERIC_SYSTEM_MENU_UP,                           // RTC
	GENERIC_SYSTEM_MENU_DOWN,                         // RTC
	GENERIC_SYSTEM_COLD_RESTART,                      // OSC
	GENERIC_SYSTEM_WARM_RESTART,                      // OSC
	GENERIC_D_PAD_UP,                                 // OOC
	GENERIC_D_PAD_DOWN,                               // OOC
	GENERIC_D_PAD_RIGHT,                              // OOC
	GENERIC_D_PAD_LEFT,                               // OOC
	GENERIC_SYSTEM_DOCK = 160,                        // OSC
	GENERIC_SYSTEM_UNDOCK,                            // OSC
	GENERIC_SYSTEM_SETUP,                             // OSC
	GENERIC_SYSTEM_BREAK,                             // OSC
	GENERIC_SYSTEM_DEBUGGER_BREAK,                    // OSC
	GENERIC_APPLICATION_BREAK,                        // OSC
	GENERIC_APPLICATION_DEBUGGER_BREAK,               // OSC
	GENERIC_SYSTEM_SPEAKER_MUTE,                      // OSC
	GENERIC_SYSTEM_HIBERNATE,                         // OSC
	GENERIC_SYSTEM_DISPLAY_INVERT = 176,              // OSC
	GENERIC_SYSTEM_DISPLAY_INTERNAL,                  // OSC
	GENERIC_SYSTEM_DISPLAY_EXTERNAL,                  // OSC
	GENERIC_SYSTEM_DISPLAY_BOTH,                      // OSC
	GENERIC_SYSTEM_DISPLAY_DUAL,                      // OSC
	GENERIC_SYSTEM_DISPLAY_TOGGLE_INT_EXT,            // OSC
	GENERIC_SYSTEM_DISPLAY_SWAP_PRIMARY_SECONDARY,    // OSC
	GENERIC_SYSTEM_DISPLAY_LCD_AUTOSCALE              // OSC
};




//============================================================================
//    Key enumeration
//============================================================================


/// \brief HID keyboard/keypad page (0x07)
enum Key {
	KEY_NONE = 0,
	KEY_ERROR_ROLLOVER,
	KEY_POST_FAIL,
	KEY_ERROR_UNDEFINED,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_0,
	KEY_ENTER,
	KEY_ESCAPE,
	KEY_BACKSPACE,
	KEY_TAB,
	KEY_SPACEBAR,
	KEY_DASH,
	KEY_EQUALS,
	KEY_LEFT_BRACKET,
	KEY_RIGHT_BRACKET,
	KEY_BACKSLASH,
	KEY_NON_US_HASH,
	KEY_SEMICOLON,
	KEY_QUOTE,
	KEY_BACKQUOTE,
	KEY_COMMA,
	KEY_PERIOD,
	KEY_SLASH,
	KEY_CAPS_LOCK,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_PRINT_SCREEN,
	KEY_SCROLL_LOCK,
	KEY_PAUSE,
	KEY_INSERT,
	KEY_HOME,
	KEY_PAGE_UP,
	KEY_DELETE,
	KEY_END,
	KEY_PAGE_DOWN,
	KEY_RIGHT_ARROW,
	KEY_LEFT_ARROW,
	KEY_DOWN_ARROW,
	KEY_UP_ARROW,
	KEY_NUM_LOCK,
	KEY_KEYPAD_SLASH,
	KEY_KEYPAD_STAR,
	KEY_KEYPAD_DASH,
	KEY_KEYPAD_PLUS,
	KEY_KEYPAD_ENTER,
	KEY_KEYPAD_1,
	KEY_KEYPAD_2,
	KEY_KEYPAD_3,
	KEY_KEYPAD_4,
	KEY_KEYPAD_5,
	KEY_KEYPAD_6,
	KEY_KEYPAD_7,
	KEY_KEYPAD_8,
	KEY_KEYPAD_9,
	KEY_KEYPAD_0,
	KEY_KEYPAD_PERIOD,
	KEY_NON_US_BACKSLASH,
	KEY_APPLICATION,
	KEY_POWER,
	KEY_KEYPAD_EQUALS,
	KEY_F13,
	KEY_F14,
	KEY_F15,
	KEY_F16,
	KEY_F17,
	KEY_F18,
	KEY_F19,
	KEY_F20,
	KEY_F21,
	KEY_F22,
	KEY_F23,
	KEY_F24,
	KEY_EXECUTE,
	KEY_HELP,
	KEY_MENU,
	KEY_SELECT,
	KEY_STOP,
	KEY_AGAIN,
	KEY_UNDO,
	KEY_CUT,
	KEY_COPY,
	KEY_PASTE,
	KEY_FIND,
	KEY_MUTE,
	KEY_VOLUME_UP,
	KEY_VOLUME_DOWN,
	KEY_LOCKING_CAPS_LOCK,
	KEY_LOCKING_NUM_LOCK,
	KEY_LOCKING_SCROLL_LOCK,
	KEY_KEYPAD_COMMA,
	KEY_KEYPAD_EQUAL_SIGN,    // different from KEY_KEYPAD_EQUALS, apparently
	KEY_INTERNATIONAL1,
	KEY_INTERNATIONAL2,
	KEY_INTERNATIONAL3,
	KEY_INTERNATIONAL4,
	KEY_INTERNATIONAL5,
	KEY_INTERNATIONAL6,
	KEY_INTERNATIONAL7,
	KEY_INTERNATIONAL8,
	KEY_INTERNATIONAL9,
	KEY_LANG1,
	KEY_LANG2,
	KEY_LANG3,
	KEY_LANG4,
	KEY_LANG5,
	KEY_LANG6,
	KEY_LANG7,
	KEY_LANG8,
	KEY_LANG9,
	KEY_ALTERNATE_ERASE,
	KEY_SYSREQ,
	KEY_CANCEL,
	KEY_CLEAR,
	KEY_PRIOR,
	KEY_RETURN,
	KEY_SEPARATOR,
	KEY_OUT,
	KEY_OPER,
	KEY_CLEAR_AGAIN,
	KEY_CRSEL_PROPS,
	KEY_EXSEL,
	// 165-175 reserved
	KEY_KEYPAD_00 = 176,
	KEY_KEYPAD_000,
	KEY_THOUSANDS_SEPARATOR,
	KEY_DECIMAL_SEPARATOR,
	KEY_CURRENCY_UNIT,
	KEY_CURRENCY_SUB_UNIT,
	KEY_KEYPAD_LEFT_PAREN,
	KEY_KEYPAD_RIGHT_PAREN,
	KEY_KEYPAD_LEFT_CURLY,
	KEY_KEYPAD_RIGHT_CURLY,
	KEY_KEYPAD_TAB,
	KEY_KEYPAD_BACKSPACE,
	KEY_KEYPAD_A,
	KEY_KEYPAD_B,
	KEY_KEYPAD_C,
	KEY_KEYPAD_D,
	KEY_KEYPAD_E,
	KEY_KEYPAD_F,
	KEY_KEYPAD_XOR,
	KEY_KEYPAD_CARET,
	KEY_KEYPAD_PERCENT,
	KEY_KEYPAD_LEFT_POINTY,
	KEY_KEYPAD_RIGHT_POINTY,
	KEY_KEYPAD_AMPERSAND,
	KEY_KEYPAD_DOUBLE_AMPERSAND,
	KEY_KEYPAD_PIPE,
	KEY_KEYPAD_DOUBLE_PIPE,
	KEY_KEYPAD_COLON,
	KEY_KEYPAD_HASH,
	KEY_KEYPAD_SPACE,
	KEY_KEYPAD_AT,
	KEY_KEYPAD_EXCLAMATION_POINT,
	KEY_KEYPAD_MEMORY_STORE,
	KEY_KEYPAD_MEMORY_RECALL,
	KEY_KEYPAD_MEMORY_CLEAR,
	KEY_KEYPAD_MEMORY_ADD,
	KEY_KEYPAD_MEMORY_SUBTRACT,
	KEY_KEYPAD_MEMORY_MULTIPLY,
	KEY_KEYPAD_MEMORY_DIVIDE,
	KEY_KEYPAD_PLUS_MINUS,
	KEY_KEYPAD_CLEAR,
	KEY_KEYPAD_CLEAR_ENTRY,
	KEY_KEYPAD_BINARY,
	KEY_KEYPAD_OCTAL,
	KEY_KEYPAD_DECIMAL,
	KEY_KEYPAD_HEXADECIMAL,
	// 222-223 reserved
	KEY_LEFT_CONTROL = 224,
	KEY_LEFT_SHIFT,
	KEY_LEFT_ALT,
	KEY_LEFT_GUI,
	KEY_RIGHT_CONTROL,
	KEY_RIGHT_SHIFT,
	KEY_RIGHT_ALT,
	KEY_RIGHT_GUI
	// 232- reserved
};




//============================================================================
//    LED enumeration
//============================================================================


/// \brief HID LED page (0x08)
enum LED {
	LED_NONE = 0,
	LED_NUM_LOCK,       // keyboard
	LED_CAPS_LOCK,      // keyboard
	LED_SCROLL_LOCK,    // keyboard
	LED_COMPOSE,        // keyboard
	LED_KANA,           // keyboard
	LED_POWER,          // general device
	LED_SHIFT,          // keyboard
	LED_DO_NOT_DISTURB,
	LED_MUTE,
	LED_TONE_ENABLE,
	LED_HIGH_CUT_FILTER,
	LED_LOW_CUT_FILTER,
	LED_EQUALIZER_ENABLE,
	LED_SOUND_FIELD_ON,
	LED_SURROUND_ON,
	LED_REPEAT,
	LED_STEREO,
	LED_SAMPLING_RATE_DETECT,
	LED_SPINNING,
	LED_CAV,
	LED_CLV,
	LED_RECORDING_FORMAT_DETECT,
	LED_OFF_HOOK,
	LED_RING,
	LED_MESSAGE_WAITING,
	LED_DATA_MODE,
	LED_BATTERY_OPERATION,    // general device
	LED_BATTERY_OK,           // general device
	LED_BATTERY_LOW,          // general device
	LED_SPEAKER,
	LED_HEAD_SET,
	LED_HOLD,
	LED_MICROPHONE,
	LED_COVERAGE,
	LED_NIGHT_MODE,
	LED_SEND_CALLS,
	LED_CALL_PICKUP,
	LED_CONFERENCE,
	LED_STAND_BY,    // general device
	LED_CAMERA_ON,
	LED_CAMERA_OFF,
	LED_ON_LINE,     // general device
	LED_OFF_LINE,    // general device
	LED_BUSY,        // general device
	LED_READY,       // general device
	LED_PAPER_OUT,
	LED_PAPER_JAM,
	LED_REMOTE,    // general device
	LED_FORWARD,
	LED_REVERSE,
	LED_STOP,
	LED_REWIND,
	LED_FAST_FORWARD,
	LED_PLAY,
	LED_PAUSE,
	LED_RECORD,
	LED_ERROR,    // general device
	// 58-74 are not on/off controls
	LED_GENERIC_INDICATOR = 75,     // general device
	LED_SYSTEM_SUSPEND,             // general device
	LED_EXTERNAL_POWER_CONNECTED    // general device
};




//============================================================================
//    Button enumeration
//============================================================================


/// \brief HID button page (0x09)
enum Button {
	BUTTON_NONE = 0,
	BUTTON_1,    // left
	BUTTON_2,    // right
	BUTTON_3,    // middle
	BUTTON_4,
	BUTTON_5,
	BUTTON_6,
	BUTTON_7,
	BUTTON_8
	// they keep going to BUTTON_65535
};




//============================================================================
//    Consumer enumeration
//============================================================================


/// \brief HID consumer control page (0x0c)
enum Consumer {
	CONSUMER_NONE = 0,
	CONSUMER_CONTROL = 1,                          // CA
	CONSUMER_NUMERIC_KEY_PAD,                      // NAry
	CONSUMER_PROGRAMMABLE_BUTTONS,                 // NAry
	CONSUMER_MICROPHONE,                           // CA
	CONSUMER_HEADPHONE,                            // CA
	CONSUMER_GRAPHIC_EQUALIZER,                    // CA
	CONSUMER_PLUS_10 = 32,                         // OSC
	CONSUMER_PLUS_100,                             // OSC
	CONSUMER_AM_PM,                                // OSC
	CONSUMER_POWER = 48,                           // OOC
	CONSUMER_RESET,                                // OSC
	CONSUMER_SLEEP,                                // OSC
	CONSUMER_SLEEP_AFTER,                          // OSC
	CONSUMER_SLEEP_MODE,                           // RTC
	CONSUMER_ILLUMINATION,                         // OOC
	CONSUMER_FUNCTION_BUTTONS,                     // NAry
	CONSUMER_MENU = 64,                            // OOC
	CONSUMER_MENU_PICK,                            // OSC
	CONSUMER_MENU_UP,                              // OSC
	CONSUMER_MENU_DOWN,                            // OSC
	CONSUMER_MENU_LEFT,                            // OSC
	CONSUMER_MENU_RIGHT,                           // OSC
	CONSUMER_MENU_ESCAPE,                          // OSC
	CONSUMER_MENU_VALUE_INCREASE,                  // OSC
	CONSUMER_MENU_VALUE_DECREASE,                  // OSC
	CONSUMER_DATA_ON_SCREEN = 96,                  // OOC
	CONSUMER_CLOSED_CAPTION,                       // OOC
	CONSUMER_CLOSED_CAPTION_SELECT,                // OSC
	CONSUMER_VCR_TV,                               // OOC
	CONSUMER_BROADCAST_MODE,                       // OSC
	CONSUMER_SNAPSHOT,                             // OSC
	CONSUMER_STILL,                                // OSC
	CONSUMER_SELECTION = 128,                      // NAry
	CONSUMER_ASSIGN_SELECTION,                     // OSC
	CONSUMER_MODE_STEP,                            // OSC
	CONSUMER_RECALL_LAST,                          // OSC
	CONSUMER_ENTER_CHANNEL,                        // OSC
	CONSUMER_ORDER_MOVIE,                          // OSC
	CONSUMER_CHANNEL,                              // LC
	CONSUMER_MEDIA_SELECTION,                      // NAry
	CONSUMER_MEDIA_SELECT_COMPUTER,                // Sel
	CONSUMER_MEDIA_SELECT_TV,                      // Sel
	CONSUMER_MEDIA_SELECT_WWW,                     // Sel
	CONSUMER_MEDIA_SELECT_DVD,                     // Sel
	CONSUMER_MEDIA_SELECT_TELEPHONE,               // Sel
	CONSUMER_MEDIA_SELECT_PROGRAM_GUIDE,           // Sel
	CONSUMER_MEDIA_SELECT_VIDEO_PHONE,             // Sel
	CONSUMER_MEDIA_SELECT_GAMES,                   // Sel
	CONSUMER_MEDIA_SELECT_MESSAGES,                // Sel
	CONSUMER_MEDIA_SELECT_CD,                      // Sel
	CONSUMER_MEDIA_SELECT_VCR,                     // Sel
	CONSUMER_MEDIA_SELECT_TUNER,                   // Sel
	CONSUMER_QUIT,                                 // OSC
	CONSUMER_HELP,                                 // OOC
	CONSUMER_MEDIA_SELECT_TAPE,                    // Sel
	CONSUMER_MEDIA_SELECT_CABLE,                   // Sel
	CONSUMER_MEDIA_SELECT_SATELLITE,               // Sel
	CONSUMER_MEDIA_SELECT_SECURITY,                // Sel
	CONSUMER_MEDIA_SELECT_HOME,                    // Sel
	CONSUMER_MEDIA_SELECT_CALL,                    // Sel
	CONSUMER_CHANNEL_INCREMENT,                    // OSC
	CONSUMER_CHANNEL_DECREMENT,                    // OSC
	CONSUMER_MEDIA_SELECT_SAP,                     // Sel
	CONSUMER_VCR_PLUS = 160,                       // OSC
	CONSUMER_ONCE,                                 // OSC
	CONSUMER_DAILY,                                // OSC
	CONSUMER_WEEKLY,                               // OSC
	CONSUMER_MONTHLY,                              // OSC
	CONSUMER_PLAY = 176,                           // OOC
	CONSUMER_PAUSE,                                // OOC
	CONSUMER_RECORD,                               // OOC
	CONSUMER_FAST_FORWARD,                         // OOC
	CONSUMER_REWIND,                               // OOC
	CONSUMER_SCAN_NEXT_TRACK,                      // OSC
	CONSUMER_SCAN_PREVIOUS_TRACK,                  // OSC
	CONSUMER_STOP,                                 // OSC
	CONSUMER_EJECT,                                // OSC
	CONSUMER_RANDOM_PLAY,                          // OOC
	CONSUMER_SELECT_DISC,                          // NAry
	CONSUMER_ENTER_DISC,                           // MC
	CONSUMER_REPEAT,                               // OSC
	CONSUMER_TRACKING,                             // LC
	CONSUMER_TRACK_NORMAL,                         // OSC
	CONSUMER_SLOW_TRACKING,                        // LC
	CONSUMER_FRAME_FORWARD,                        // RTC
	CONSUMER_FRAME_BACK,                           // RTC
	CONSUMER_MARK,                                 // OSC
	CONSUMER_CLEAR_MARK,                           // OSC
	CONSUMER_REPEAT_FROM_MARK,                     // OOC
	CONSUMER_RETURN_TO_MARK,                       // OSC
	CONSUMER_SEARCH_MARK_FORWARD,                  // OSC
	CONSUMER_SEARCH_MARK_BACKWARDS,                // OSC
	CONSUMER_COUNTER_RESET,                        // OSC
	CONSUMER_SHOW_COUNTER,                         // OSC
	CONSUMER_TRACKING_INCREMENT,                   // RTC
	CONSUMER_TRACKING_DECREMENT,                   // RTC
	CONSUMER_STOP_EJECT,                           // OSC
	CONSUMER_PLAY_PAUSE,                           // OSC
	CONSUMER_PLAY_SKIP,                            // OSC
	CONSUMER_VOLUME = 224,                         // LC
	CONSUMER_BALANCE,                              // LC
	CONSUMER_MUTE,                                 // OOC
	CONSUMER_BASS,                                 // LC
	CONSUMER_TREBLE,                               // LC
	CONSUMER_BASS_BOOST,                           // OOC
	CONSUMER_SURROUND_MODE,                        // OSC
	CONSUMER_LOUDNESS,                             // OOC
	CONSUMER_MPX,                                  // OOC
	CONSUMER_VOLUME_INCREMENT,                     // RTC
	CONSUMER_VOLUME_DECREMENT,                     // RTC
	CONSUMER_SPEED_SELECT = 240,                   // OSC
	CONSUMER_PLAYBACK_SPEED,                       // NAry
	CONSUMER_STANDARD_PLAY,                        // Sel
	CONSUMER_LONG_PLAY,                            // Sel
	CONSUMER_EXTENDED_PLAY,                        // Sel
	CONSUMER_SLOW,                                 // OSC
	CONSUMER_FAN_ENABLE = 256,                     // OOC
	CONSUMER_FAN_SPEED,                            // LC
	CONSUMER_LIGHT_ENABLE,                         // OOC
	CONSUMER_LIGHT_ILLUMINATION_LEVEL,             // LC
	CONSUMER_CLIMATE_CONTROL_ENABLE,               // OOC
	CONSUMER_ROOM_TEMPERATURE,                     // LC
	CONSUMER_SECURITY_ENABLE,                      // OOC
	CONSUMER_FIRE_ALARM,                           // OSC
	CONSUMER_POLICE_ALARM,                         // OSC
	CONSUMER_PROXIMITY,                            // LC
	CONSUMER_MOTION,                               // OSC
	CONSUMER_DURESS_ALARM,                         // OSC
	CONSUMER_HOLDUP_ALARM,                         // OSC
	CONSUMER_MEDICAL_ALARM,                        // OSC
	CONSUMER_BALANCE_RIGHT = 336,                  // RTC
	CONSUMER_BALANCE_LEFT,                         // RTC
	CONSUMER_BASS_INCREMENT,                       // RTC
	CONSUMER_BASS_DECREMENT,                       // RTC
	CONSUMER_TREBLE_INCREMENT,                     // RTC
	CONSUMER_TREBLE_DECREMENT,                     // RTC
	CONSUMER_SPEAKER_SYSTEM = 352,                 // CL
	CONSUMER_CHANNEL_LEFT,                         // CL
	CONSUMER_CHANNEL_RIGHT,                        // CL
	CONSUMER_CHANNEL_CENTER,                       // CL
	CONSUMER_CHANNEL_FRONT,                        // CL
	CONSUMER_CHANNEL_CENTER_FRONT,                 // CL
	CONSUMER_CHANNEL_SIDE,                         // CL
	CONSUMER_CHANNEL_SURROUND,                     // CL
	CONSUMER_CHANNEL_LOW_FREQUENCY_ENHANCEMENT,    // CL
	CONSUMER_CHANNEL_TOP,                          // CL
	CONSUMER_CHANNEL_UNKNOWN,                      // CL
	CONSUMER_SUBCHANNEL = 368,                     // LC
	CONSUMER_SUBCHANNEL_INCREMENT,                 // OSC
	CONSUMER_SUBCHANNEL_DECREMENT,                 // OSC
	CONSUMER_ALTERNATE_AUDIO_INCREMENT,            // OSC
	CONSUMER_ALTERNATE_AUDIO_DECREMENT,            // OSC
	// application launchers
	CONSUMER_APPLICATION_LAUNCH_BUTTONS = 384,               // NAry
	CONSUMER_AL_LAUNCH_BUTTON_CONFIGURATION_TOOL,            // Sel
	CONSUMER_AL_PROGRAMMABLE_BUTTON_CONFIGURATION,           // Sel
	CONSUMER_AL_CONSUMER_CONTROL_CONFIGURATION,              // Sel
	CONSUMER_AL_WORD_PROCESSOR,                              // Sel
	CONSUMER_AL_TEXT_EDITOR,                                 // Sel
	CONSUMER_AL_SPREADSHEET,                                 // Sel
	CONSUMER_AL_GRAPHICS_EDITOR,                             // Sel
	CONSUMER_AL_PRESENTATION_APP,                            // Sel
	CONSUMER_AL_DATABASE_APP,                                // Sel
	CONSUMER_AL_EMAIL_READER,                                // Sel
	CONSUMER_AL_NEWSREADER,                                  // Sel
	CONSUMER_AL_VOICEMAIL,                                   // Sel
	CONSUMER_AL_CONTACTS_OR_ADDRESS_BOOK,                    // Sel
	CONSUMER_AL_CALENDAR_OR_SCHEDULE,                        // Sel
	CONSUMER_AL_TASK_OR_PROJECT_MANAGER,                     // Sel
	CONSUMER_AL_LOG_OR_JOURNAL_OR_TIMECARD,                  // Sel
	CONSUMER_AL_CHECKBOOK_OR_FINANCE,                        // Sel
	CONSUMER_AL_CALCULATOR,                                  // Sel
	CONSUMER_AL_A_V_CAPTURE_OR_PLAYBACK,                     // Sel
	CONSUMER_AL_LOCAL_MACHINE_BROWSER,                       // Sel
	CONSUMER_AL_LAN_OR_WAN_BROWSER,                          // Sel
	CONSUMER_AL_INTERNET_BROWSER,                            // Sel
	CONSUMER_AL_REMOTE_NETWORKING_OR_ISP_CONNECT,            // Sel
	CONSUMER_AL_NETWORK_CONFERENCE,                          // Sel
	CONSUMER_AL_NETWORK_CHAT,                                // Sel
	CONSUMER_AL_TELEPHONY_OR_DIALER,                         // Sel
	CONSUMER_AL_LOGON,                                       // Sel
	CONSUMER_AL_LOGOFF,                                      // Sel
	CONSUMER_AL_LOGON_LOGOFF,                                // Sel
	CONSUMER_AL_TERMINAL_LOCK_OR_SCREENSAVER,                // Sel
	CONSUMER_AL_CONTROL_PANEL,                               // Sel
	CONSUMER_AL_COMMAND_LINE_PROCESSOR_OR_RUN,               // Sel
	CONSUMER_AL_PROCESS_OR_TASK_MANAGER,                     // Sel
	CONSUMER_AL_SELECT_TASK_OR_APPLICATION,                  // Sel
	CONSUMER_AL_NEXT_TASK_OR_APPLICATION,                    // Sel
	CONSUMER_AL_PREVIOUS_TASK_OR_APPLICATION,                // Sel
	CONSUMER_AL_PREEMPTIVE_HALT_TASK_OR_APPLICATION,         // Sel
	CONSUMER_AL_INTEGRATED_HELP_CENTER,                      // Sel
	CONSUMER_AL_DOCUMENTS,                                   // Sel
	CONSUMER_AL_THESAURUS,                                   // Sel
	CONSUMER_AL_DICTIONARY,                                  // Sel
	CONSUMER_AL_DESKTOP,                                     // Sel
	CONSUMER_AL_SPELL_CHECK,                                 // Sel
	CONSUMER_AL_GRAMMAR_CHECK,                               // Sel
	CONSUMER_AL_WIRELESS_STATUS,                             // Sel
	CONSUMER_AL_KEYBOARD_LAYOUT,                             // Sel
	CONSUMER_AL_VIRUS_PROTECTION,                            // Sel
	CONSUMER_AL_ENCRYPTION,                                  // Sel
	CONSUMER_AL_SCREEN_SAVER,                                // Sel
	CONSUMER_AL_ALARMS,                                      // Sel
	CONSUMER_AL_CLOCK,                                       // Sel
	CONSUMER_AL_FILE_BROWSER,                                // Sel
	CONSUMER_AL_POWER_STATUS,                                // Sel
	CONSUMER_AL_IMAGE_BROWSER,                               // Sel
	CONSUMER_AL_AUDIO_BROWSER,                               // Sel
	CONSUMER_AL_MOVIE_BROWSER,                               // Sel
	CONSUMER_AL_DIGITAL_RIGHTS_MANAGER,                      // Sel
	CONSUMER_AL_DIGITAL_WALLET,                              // Sel
	CONSUMER_AL_INSTANT_MESSAGING = 444,                     // Sel
	CONSUMER_AL_OEM_FEATURES_OR_TIPS_OR_TUTORIAL_BROWSER,    // Sel
	CONSUMER_AL_OEM_HELP,                                    // Sel
	CONSUMER_AL_ONLINE_COMMUNITY,                            // Sel
	CONSUMER_AL_ENTERTAINMENT_CONTENT_BROWSER,               // Sel
	CONSUMER_AL_ONLINE_SHOPPING_BROWSER,                     // Sel
	CONSUMER_AL_SMARTCARD_INFORMATION_OR_HELP,               // Sel
	CONSUMER_AL_MARKET_MONITOR_OR_FINANCE_BROWSER,           // Sel
	CONSUMER_AL_CUSTOMIZED_CORPORATE_NEWS_BROWSER,           // Sel
	CONSUMER_AL_ONLINE_ACTIVITY_BROWSER,                     // Sel
	CONSUMER_AL_RESEARCH_OR_SEARCH_BROWSER,                  // Sel
	CONSUMER_AL_AUDIO_PLAYER,                                // Sel
	// application controls
	CONSUMER_GENERIC_GUI_APPLICATION_CONTROLS = 512,    // NAry
	CONSUMER_AC_NEW,                                    // Sel
	CONSUMER_AC_OPEN,                                   // Sel
	CONSUMER_AC_CLOSE,                                  // Sel
	CONSUMER_AC_EXIT,                                   // Sel
	CONSUMER_AC_MAXIMIZE,                               // Sel
	CONSUMER_AC_MINIMIZE,                               // Sel
	CONSUMER_AC_SAVE,                                   // Sel
	CONSUMER_AC_PRINT,                                  // Sel
	CONSUMER_AC_PROPERTIES,                             // Sel
	CONSUMER_AC_UNDO = 538,                             // Sel
	CONSUMER_AC_COPY,                                   // Sel
	CONSUMER_AC_CUT,                                    // Sel
	CONSUMER_AC_PASTE,                                  // Sel
	CONSUMER_AC_SELECT_ALL,                             // Sel
	CONSUMER_AC_FIND,                                   // Sel
	CONSUMER_AC_FIND_AND_REPLACE,                       // Sel
	CONSUMER_AC_SEARCH,                                 // Sel
	CONSUMER_AC_GO_TO,                                  // Sel
	CONSUMER_AC_HOME,                                   // Sel
	CONSUMER_AC_BACK,                                   // Sel
	CONSUMER_AC_FORWARD,                                // Sel
	CONSUMER_AC_STOP,                                   // Sel
	CONSUMER_AC_REFRESH,                                // Sel
	CONSUMER_AC_PREVIOUS_LINK,                          // Sel
	CONSUMER_AC_NEXT_LINK,                              // Sel
	CONSUMER_AC_BOOKMARKS,                              // Sel
	CONSUMER_AC_HISTORY,                                // Sel
	CONSUMER_AC_SUBSCRIPTIONS,                          // Sel
	CONSUMER_AC_ZOOM_IN,                                // Sel
	CONSUMER_AC_ZOOM_OUT,                               // Sel
	CONSUMER_AC_ZOOM,                                   // LC
	CONSUMER_AC_FULL_SCREEN_VIEW,                       // Sel
	CONSUMER_AC_NORMAL_VIEW,                            // Sel
	CONSUMER_AC_VIEW_TOGGLE,                            // Sel
	CONSUMER_AC_SCROLL_UP,                              // Sel
	CONSUMER_AC_SCROLL_DOWN,                            // Sel
	CONSUMER_AC_SCROLL,                                 // LC
	CONSUMER_AC_PAN_LEFT,                               // Sel
	CONSUMER_AC_PAN_RIGHT,                              // Sel
	CONSUMER_AC_PAN,                                    // LC
	CONSUMER_AC_NEW_WINDOW,                             // Sel
	CONSUMER_AC_TILE_HORIZONTALLY,                      // Sel
	CONSUMER_AC_TILE_VERTICALLY,                        // Sel
	CONSUMER_AC_FORMAT,                                 // Sel
	CONSUMER_AC_EDIT,                                   // Sel
	CONSUMER_AC_BOLD,                                   // Sel
	CONSUMER_AC_ITALICS,                                // Sel
	CONSUMER_AC_UNDERLINE,                              // Sel
	CONSUMER_AC_STRIKETHROUGH,                          // Sel
	CONSUMER_AC_SUBSCRIPT,                              // Sel
	CONSUMER_AC_SUPERSCRIPT,                            // Sel
	CONSUMER_AC_ALL_CAPS,                               // Sel
	CONSUMER_AC_ROTATE,                                 // Sel
	CONSUMER_AC_RESIZE,                                 // Sel
	CONSUMER_AC_FLIP_HORIZONTAL,                        // Sel
	CONSUMER_AC_FLIP_VERTICAL,                          // Sel
	CONSUMER_AC_MIRROR_HORIZONTAL,                      // Sel
	CONSUMER_AC_MIRROR_VERTICAL,                        // Sel
	CONSUMER_AC_FONT_SELECT,                            // Sel
	CONSUMER_AC_FONT_COLOR,                             // Sel
	CONSUMER_AC_FONT_SIZE,                              // Sel
	CONSUMER_AC_JUSTIFY_LEFT,                           // Sel
	CONSUMER_AC_JUSTIFY_CENTER_H,                       // Sel
	CONSUMER_AC_JUSTIFY_RIGHT,                          // Sel
	CONSUMER_AC_JUSTIFY_BLOCK_H,                        // Sel
	CONSUMER_AC_JUSTIFY_TOP,                            // Sel
	CONSUMER_AC_JUSTIFY_CENTER_V,                       // Sel
	CONSUMER_AC_JUSTIFY_BOTTOM,                         // Sel
	CONSUMER_AC_JUSTIFY_BLOCK_V,                        // Sel
	CONSUMER_AC_INDENT_DECREASE,                        // Sel
	CONSUMER_AC_INDENT_INCREASE,                        // Sel
	CONSUMER_AC_NUMBERED_LIST,                          // Sel
	CONSUMER_AC_RESTART_NUMBERING,                      // Sel
	CONSUMER_AC_BULLETED_LIST,                          // Sel
	CONSUMER_AC_PROMOTE,                                // Sel
	CONSUMER_AC_DEMOTE,                                 // Sel
	CONSUMER_AC_YES,                                    // Sel
	CONSUMER_AC_NO,                                     // Sel
	CONSUMER_AC_CANCEL,                                 // Sel
	CONSUMER_AC_CATALOG,                                // Sel
	CONSUMER_AC_BUY_CHECKOUT,                           // Sel
	CONSUMER_AC_ADD_TO_CART,                            // Sel
	CONSUMER_AC_EXPAND,                                 // Sel
	CONSUMER_AC_EXPAND_ALL,                             // Sel
	CONSUMER_AC_COLLAPSE,                               // Sel
	CONSUMER_AC_COLLAPSE_ALL,                           // Sel
	CONSUMER_AC_PRINT_PREVIEW,                          // Sel
	CONSUMER_AC_PASTE_SPECIAL,                          // Sel
	CONSUMER_AC_INSERT_MODE,                            // Sel
	CONSUMER_AC_DELETE,                                 // Sel
	CONSUMER_AC_LOCK,                                   // Sel
	CONSUMER_AC_UNLOCK,                                 // Sel
	CONSUMER_AC_PROTECT,                                // Sel
	CONSUMER_AC_UNPROTECT,                              // Sel
	CONSUMER_AC_ATTACH_COMMENT,                         // Sel
	CONSUMER_AC_DELETE_COMMENT,                         // Sel
	CONSUMER_AC_VIEW_COMMENT,                           // Sel
	CONSUMER_AC_SELECT_WORD,                            // Sel
	CONSUMER_AC_SELECT_SENTENCE,                        // Sel
	CONSUMER_AC_SELECT_PARAGRAPH,                       // Sel
	CONSUMER_AC_SELECT_COLUMN,                          // Sel
	CONSUMER_AC_SELECT_ROW,                             // Sel
	CONSUMER_AC_SELECT_TABLE,                           // Sel
	CONSUMER_AC_SELECT_OBJECT,                          // Sel
	CONSUMER_AC_REDO_REPEAT,                            // Sel
	CONSUMER_AC_SORT,                                   // Sel
	CONSUMER_AC_SORT_ASCENDING,                         // Sel
	CONSUMER_AC_SORT_DESCENDING,                        // Sel
	CONSUMER_AC_FILTER,                                 // Sel
	CONSUMER_AC_SET_CLOCK,                              // Sel
	CONSUMER_AC_VIEW_CLOCK,                             // Sel
	CONSUMER_AC_SELECT_TIME_ZONE,                       // Sel
	CONSUMER_AC_EDIT_TIME_ZONES,                        // Sel
	CONSUMER_AC_SET_ALARM,                              // Sel
	CONSUMER_AC_CLEAR_ALARM,                            // Sel
	CONSUMER_AC_SNOOZE_ALARM,                           // Sel
	CONSUMER_AC_RESET_ALARM,                            // Sel
	CONSUMER_AC_SYNCHRONIZE,                            // Sel
	CONSUMER_AC_SEND_RECEIVE,                           // Sel
	CONSUMER_AC_SEND_TO,                                // Sel
	CONSUMER_AC_REPLY,                                  // Sel
	CONSUMER_AC_REPLY_ALL,                              // Sel
	CONSUMER_AC_FORWARD_MSG,                            // Sel
	CONSUMER_AC_SEND,                                   // Sel
	CONSUMER_AC_ATTACH_FILE,                            // Sel
	CONSUMER_AC_UPLOAD,                                 // Sel
	CONSUMER_AC_DOWNLOAD,                               // Sel
	CONSUMER_AC_SET_BORDERS,                            // Sel
	CONSUMER_AC_INSERT_ROW,                             // Sel
	CONSUMER_AC_INSERT_COLUMN,                          // Sel
	CONSUMER_AC_INSERT_FILE,                            // Sel
	CONSUMER_AC_INSERT_PICTURE,                         // Sel
	CONSUMER_AC_INSERT_OBJECT,                          // Sel
	CONSUMER_AC_INSERT_SYMBOL,                          // Sel
	CONSUMER_AC_SAVE_AND_CLOSE,                         // Sel
	CONSUMER_AC_RENAME,                                 // Sel
	CONSUMER_AC_MERGE,                                  // Sel
	CONSUMER_AC_SPLIT,                                  // Sel
	CONSUMER_AC_DISRIBUTE_HORIZONTALLY,                 // Sel
	CONSUMER_AC_DISTRIBUTE_VERTICALLY                   // Sel
};




}    // namespace HID


}    // namespace USB




#endif    /* __cplusplus */

#endif    /* __USB_HID_TABLES_HH__ */

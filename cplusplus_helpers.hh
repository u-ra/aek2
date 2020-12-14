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
	\file cplusplus_helpers.hh
	\brief C++ helper declarations
*/


/*
	Based on an AVR Freaks forum post by TFrancuz:
		http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&t=59453
*/




#include <stdlib.h>




//============================================================================
//    Operator new and delete
//============================================================================


void* operator new( size_t );
void* operator new[]( size_t );

void operator delete( void* );
void operator delete[]( void* );




#if 0    // I use "-fno-threadsafe-statics" instead


//============================================================================
//    Guards (for static initialization?)
//============================================================================


__extension__ typedef int __guard __attribute__(( mode( __DI__ ) ));

extern "C" int __cxa_guard_acquire( __guard* );
extern "C" void __cxa_guard_release( __guard* );
extern "C" void __cxa_guard_abort( __guard* );


#endif    // 0/1




//============================================================================
//    Enable pure virtual functions
//============================================================================


extern "C" void __cxa_pure_virtual( void );

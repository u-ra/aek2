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
	\file cplusplus_helpers.cc
	\brief C++ helper definitions
*/


/*
	Based on an AVR Freaks forum post by TFrancuz:
		http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&t=59453
*/




#include "cplusplus_helpers.hh"




//============================================================================
//    Operator new and delete
//============================================================================


void* operator new( size_t size ) {

	return malloc( size );
}


void* operator new[]( size_t size ) {

	return malloc( size );
}


void operator delete( void* pp ) {

	free( pp );
}


void operator delete[]( void* pp ) {

	free( pp );
}




#if 0    // I use "-fno-threadsafe-statics" instead


//============================================================================
//    Guards (for static initialization?)
//============================================================================


int __cxa_guard_acquire( __guard* gg ) {

	return( ! *( char* )( gg ) );
}


void __cxa_guard_release ( __guard* gg ) {

	*( char* )gg = 1;
}


void __cxa_guard_abort( __guard* ) {
}


#endif    // 0/1




//============================================================================
//    Enable pure virtual functions
//============================================================================


void __cxa_pure_virtual( void ) {
}

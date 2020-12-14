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
	\file helpers.h
	\brief Helper macros and functions
*/




#ifndef __HELPERS_H__
#define __HELPERS_H__




//============================================================================
//    LSB and MSB macros
//============================================================================


#define LSB( nn ) ( nn & 255 )
#define MSB( nn ) ( ( nn >> 8 ) & 255 )




//============================================================================
//    ARRAYLENGTH macro
//============================================================================


/**
	\brief Finds the number of elements in an array
	\param aa  array
	\result  length
*/
#define ARRAYLENGTH( aa ) ( sizeof( aa ) / sizeof( aa[ 0 ] ) )




#ifdef __cplusplus




//============================================================================
//    Square function
//============================================================================


/**
	\brief Squares a variable
	\param value  variable
*/
template< typename t_Type >
inline t_Type const Square( t_Type const& value ) {

	return( value * value );
}




//============================================================================
//    Swap function
//============================================================================


/**
	\brief Swaps two variables
	\param left   first variable
	\param right  second variable
*/
template< typename t_Type >
inline void Swap( t_Type& left, t_Type& right ) {

	t_Type tmp = left;
	left = right;
	right = tmp;
}




//============================================================================
//    Max functions
//============================================================================


/**
	\brief Returns the larger of two variables
	\param first   first variable
	\param second  second variable
*/
template< typename t_Type >
inline t_Type const& Max( t_Type const& first, t_Type const& second ) {

	if ( first < second )
		return second;
	return first;
}


/**
	\brief Returns the larger of three variables
	\param first   first variable
	\param second  second variable
	\param third   third variable
*/
template< typename t_Type >
inline t_Type const& Max( t_Type const& first, t_Type const& second, t_Type const& third ) {

	if ( third < second ) {

		if ( first < second )
			return second;
		return first;
	}
	if ( first < third )
		return third;
	return first;
}




//============================================================================
//    Min functions
//============================================================================


/**
	\brief Returns the smaller of two variables
	\param first   first variable
	\param second  second variable
*/
template< typename t_Type >
inline t_Type const& Min( t_Type const& first, t_Type const& second ) {

	if ( first > second )
		return second;
	return first;
}


/**
	\brief Returns the smaller of three variables
	\param first   first variable
	\param second  second variable
	\param third   third variable
*/
template< typename t_Type >
inline t_Type const& Min( t_Type const& first, t_Type const& second, t_Type const& third ) {

	if ( third > second ) {

		if ( first > second )
			return second;
		return first;
	}
	if ( first > third )
		return third;
	return first;
}




#endif    /* __cplusplus */




#endif    /* __HELPERS_H__ */

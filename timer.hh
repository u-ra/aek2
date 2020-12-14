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
	\file timer.hh
	\brief Timer implementation
*/




#ifndef __TIMER_HH__
#define __TIMER_HH__

#ifdef __cplusplus




#include "helpers.h"

#include <inttypes.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>




namespace _Private {




//============================================================================
//    Timer overflow interrupt
//============================================================================


inline void OverflowInterrupt();




}    // namespace _Private




//============================================================================
//    Timer class
//============================================================================


struct Timer {

	static inline Timer* const Instance();


	static inline uint16_t const MicrosecondsToTicks( uint16_t const microseconds );


	inline uint16_t const GetOverflows() const;
	inline uint16_t const GetTicks() const;


	inline void const DelayTicks( uint16_t const ticks ) const;


private:

	inline Timer();

	inline void OverflowInterrupt();


	uint16_t volatile m_overflows;


	friend void _Private::OverflowInterrupt();


	inline Timer( Timer const& other );
	inline Timer const& operator=( Timer const& other );
};




//============================================================================
//    Timer inline methods
//============================================================================


Timer* const Timer::Instance() {

	static Timer timer;
	return &timer;
}


uint16_t const Timer::MicrosecondsToTicks( uint16_t const microseconds ) {

	uint32_t const scale = F_CPU * ( 65536.0 / 1000000.0 );
	return( ( microseconds * scale ) >> 16 );
}


uint16_t const Timer::GetOverflows() const {

	uint16_t overflows = 0;

	// save and clear the interrupt flag
	uint8_t const sreg = SREG;
	cli();

	overflows = m_overflows;

	// restore the interrupt flag
	SREG = sreg;

	return overflows;
}


uint16_t const Timer::GetTicks() const {

	uint8_t const timeLow  = TCNT1L;
	uint8_t const timeHigh = TCNT1H;
	return( ( uint16_t )timeLow | ( ( uint16_t )timeHigh << 8 ) );
}


void const Timer::DelayTicks( uint16_t const ticks ) const {

	for ( uint16_t const startTime = GetTicks(); ( GetTicks() - startTime ) < ticks; );
}


Timer::Timer() {

	// save and clear the interrupt flag
	uint8_t const sreg = SREG;
	cli();

	// no prescaling on timer 1
	TCCR1B |= ( 1 << CS10 );
	TCNT1H = 0;
	TCNT1L = 0;

	// enable overflow interrupts
	TIMSK1 &= ~( 1 << TOIE1 );
	TIMSK1 |=  ( 1 << TOIE1 );
	m_overflows = 0;

	// restore the interrupt flag
	SREG = sreg;
}


void Timer::OverflowInterrupt() {

	++m_overflows;
}




namespace _Private {




//============================================================================
//    Timer overflow interrupt
//============================================================================


void OverflowInterrupt() {

	Timer::Instance()->OverflowInterrupt();
}




}    // namespace _Private




#endif    /* __cplusplus */

#endif    /* __TIMER_HH__ */

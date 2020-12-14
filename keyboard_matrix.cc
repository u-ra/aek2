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
	\file keyboard_matrix.cc
	\brief KeyboardMatrix implementation
*/




#include "keyboard_matrix.hh"

#include <string.h>
#include <util/delay.h>




//============================================================================
//    KeyboardMatrix methods
//============================================================================


KeyboardMatrix::KeyboardMatrix( char const* const rowString, char const* const columnString, bool const activeHigh ) :
	m_rows( 0 ),
	m_logColumns( 0 ),
	m_columns( 1 ),
	m_activeHigh( activeHigh ),
	m_antiGhosting( false ),
	m_debouncingIterations( 1 )
{
	for ( unsigned int ii = 0; ( rowString[ ii * 2 ] != '\0' ) && ( rowString[ ii * 2 + 1 ] != '\0' ) && ( m_rows < MAXIMUM_ROWS ); ++ii ) {

		char const name = rowString[ ii * 2     ];
		char const bit  = rowString[ ii * 2 + 1 ] - '0';

		uint8_t volatile* ddr  = NULL;
		uint8_t volatile* port = NULL;
		uint8_t volatile* pin  = NULL;
		if ( PinAllocate( &pin, &ddr, &port, name, bit ) ) {    /// \todo handle errors

			*ddr  &= ~( 1u << bit );    // direction = input
			*port |=  ( 1u << bit );    // value = high (pull-up resistor)

			m_rowPinNames[ m_rows ] = name;
			m_rowPinBits[  m_rows ] = bit;
			m_rowPins[     m_rows ] = pin;

			++m_rows;
		}
	}

	for ( unsigned int ii = 0; ( columnString[ ii * 2 ] != '\0' ) && ( columnString[ ii * 2 + 1 ] != '\0' ) && ( m_columns < MAXIMUM_COLUMNS ); ++ii ) {

		char const name = columnString[ ii * 2     ];
		char const bit  = columnString[ ii * 2 + 1 ] - '0';

		uint8_t volatile* ddr  = NULL;
		uint8_t volatile* port = NULL;
		uint8_t volatile* pin  = NULL;
		if ( PinAllocate( &pin, &ddr, &port, name, bit ) ) {    /// \todo handle errors

			*ddr  |=  ( 1u << bit );    // direction = output
			*port &= ~( 1u << bit );    // value = low

			m_columnPinNames[ m_logColumns ] = name;
			m_columnPinBits[  m_logColumns ] = bit;
			m_columnPorts[    m_logColumns ] = port;

			++m_logColumns;
			m_columns += m_columns;
		}
	}

	for ( uint8_t ii = 0; ii < m_rows; ++ii ) {

		m_switchMask[      ii ] = 0;
		m_rawPressedState[ ii ] = 0;
		m_pressedState[    ii ] = 0;
	}
}


KeyboardMatrix::~KeyboardMatrix() {

	for ( unsigned int ii = 0; ii < m_rows; ++ii )
		PinFree( m_rowPinNames[ ii ], m_rowPinBits[ ii ] );

	for ( unsigned int ii = 0; ii < m_logColumns; ++ii )
		PinFree( m_columnPinNames[ ii ], m_columnPinBits[ ii ] );
}


bool const KeyboardMatrix::Update() {

	bool changed = false;
	if ( m_antiGhosting ) {

		ColumnType workPressedState[ MAXIMUM_ROWS ];
		ReadKeyboardMatrix( workPressedState );

		// cluster all connected rows together
		Clustering clustering( workPressedState, m_rows );

		// it the user pressed/released a key during scanning, we might have a ghosted block in which some keys are't pressed. fix this by ORing together all rows in each cluster
		for ( uint8_t ii = 0; ii < m_rows; ++ii ) {

			if ( clustering.GetClusterSize( ii ) > 1 ) {

				ColumnType state = 0;
				for ( uint8_t jj = 0; jj < m_rows; ++jj )
					if ( clustering.GetCluster( jj ) == ii )
						state |= workPressedState[ jj ];
				for ( uint8_t jj = 0; jj < m_rows; ++jj )
					if ( clustering.GetCluster( jj ) == ii )
						workPressedState[ jj ] = state;
			}
		}

		// turn off all keys which don't exist
		for ( uint8_t ii = 0; ii < m_rows; ++ii ) {

			workPressedState[ ii ] &= m_switchMask[ ii ];
			changed |= ( workPressedState[ ii ] != m_rawPressedState[ ii ] );
			m_rawPressedState[ ii ] = workPressedState[ ii ];
		}

		if ( changed ) {

			// workPressedState will now contain the ghosted flags
			for ( uint8_t ii = 0; ii < m_rows; ++ii )
				workPressedState[ ii ] = 0;

			// find and mark all cycles
			DFS( workPressedState, clustering );

			// save and clear the interrupt flag
			uint8_t const sreg = SREG;
			cli();

			changed = false;
			for ( uint8_t ii = 0; ii < m_rows; ++ii ) {

				// a ghosted element is pressed iff it was already pressed
				ColumnType state = ( m_pressedState[ ii ] & m_rawPressedState[ ii ] & workPressedState[ ii ] ) | ( m_rawPressedState[ ii ] & ~workPressedState[ ii ] );
				changed |= ( state != m_pressedState[ ii ] );
				m_pressedState[ ii ] = state;
			}

			// restore the interrupt flag
			SREG = sreg;
		}
	}
	else {

		ReadKeyboardMatrix( m_rawPressedState );

		// save and clear the interrupt flag
		uint8_t const sreg = SREG;
		cli();

		for ( uint8_t ii = 0; ii < m_rows; ++ii ) {

			m_rawPressedState[ ii ] &= m_switchMask[ ii ];
			changed |= ( m_pressedState[ ii ] != m_rawPressedState[ ii ] );
			m_pressedState[ ii ] = m_rawPressedState[ ii ];
		}

		// restore the interrupt flag
		SREG = sreg;
	}

	return changed;
}


void KeyboardMatrix::ReadKeyboardMatrix( ColumnType workPressedState[] ) const {

	memcpy( workPressedState, m_switchMask, m_rows * sizeof( ColumnType ) );
	for ( unsigned int ii = 0; ii < m_debouncingIterations; ++ii ) {

		for ( uint8_t jj = 0; jj < m_columns; ++jj ) {

			for ( uint8_t kk = 0, mask = 1; kk < m_logColumns; ++kk, mask += mask ) {

				if ( jj & mask )
					*m_columnPorts[ kk ] |= ( 1u << m_columnPinBits[ kk ] );
				else
					*m_columnPorts[ kk ] &= ~( 1u << m_columnPinBits[ kk ] );
			}
			_delay_us( 10 );

			for ( uint8_t kk = 0; kk < m_rows; ++kk )
				if ( ( ( *m_rowPins[ kk ] & ( 1u << m_rowPinBits[ kk ] ) ) != 0 ) != m_activeHigh )    // is this row pin active?
					workPressedState[ kk ] &= ~( static_cast< ColumnType >( 1 ) << jj );
		}
	}
}


void KeyboardMatrix::DFS( ColumnType ghostedState[], Clustering const& clustering ) {

	// we follow the convention that nodes with indices less than m_rows are rows, and are otherwise columns

	// initialize depths and backpointers
	uint8_t parents[ MAXIMUM_ROWS + MAXIMUM_COLUMNS ];
	uint8_t depths[  MAXIMUM_ROWS + MAXIMUM_COLUMNS ];
	for ( uint8_t ii = 0; ii < m_rows + m_columns; ++ii ) {

		parents[ ii ] = 0xff;
		depths[ ii ] = 0xff;
	}

	// initialize to-be-visited edge markers
	ColumnType visitState[ MAXIMUM_ROWS ];
	for ( uint8_t ii = 0; ii < m_rows; ++ii )
		visitState[ ii ] = m_rawPressedState[ ii ];

	// each row cluster corresponds to one tree in the forest
	uint8_t nodeStack[ MAXIMUM_ROWS + MAXIMUM_COLUMNS ];
	uint8_t nodeStackSize = 0;
	for ( uint8_t ii = 0; ii < m_rows; ++ii ) {

		if ( clustering.GetClusterSize( ii ) > 1 ) {

			nodeStack[ nodeStackSize++ ] = ii;
			depths[ ii ] = 0;
		}
	}

	// perform DFS
	while ( nodeStackSize > 0 ) {

		uint8_t const node = nodeStack[ --nodeStackSize ];
		if ( node < m_rows ) {    // row

			for ( uint8_t ii = 0; ii < m_columns; ++ii ) {

				if ( ( visitState[ node ] & ( static_cast< ColumnType >( 1 ) << ii ) ) != 0 ) {

					if ( depths[ ii + m_rows ] != 0xff )    // if node already visited
						DFSMarkCycle( ghostedState, parents, depths, node, ( ii + m_rows ) );
					else {

						parents[ ii + m_rows ] = node;
						depths[ ii + m_rows ] = depths[ node ] + 1;
						nodeStack[ nodeStackSize++ ] = ( ii + m_rows );
					}

					visitState[ node ] &= ~( static_cast< ColumnType >( 1 ) << ii );
				}
			}
		}
		else {    // column

			for ( uint8_t ii = 0; ii < m_rows; ++ii ) {

				if ( ( visitState[ ii ] & ( static_cast< ColumnType >( 1 ) << ( node - m_rows ) ) ) != 0 ) {

					if ( depths[ ii ] != 0xff )    // if node already visited
						DFSMarkCycle( ghostedState, parents, depths, node, ii );
					else {

						parents[ ii ] = node;
						depths[ ii ] = depths[ node ] + 1;
						nodeStack[ nodeStackSize++ ] = ii;
					}

					visitState[ ii ] &= ~( static_cast< ColumnType >( 1 ) << ( node - m_rows ) );
				}
			}
		}
	}
}


void KeyboardMatrix::DFSMarkCycle( ColumnType ghostedState[], uint8_t parents[], uint8_t depths[], uint8_t node1, uint8_t node2 ) {

	// we follow the convention that nodes with indices less than m_rows are rows, and are otherwise columns

	uint8_t depth1;
	uint8_t depth2;
	if ( node1 < m_rows ) {    // row

		depth1 = depths[ node1 ];
		depth2 = depths[ node2 ];

		// the cycle-closing edge is ghosted
		ghostedState[ node1 ] |= ( static_cast< ColumnType >( 1 ) << ( node2 - m_rows ) );
	}
	else {

		depth1 = depths[ node1 ];
		depth2 = depths[ node2 ];

		// the cycle-closing edge is ghosted
		ghostedState[ node2 ] |= ( static_cast< ColumnType >( 1 ) << ( node1 - m_rows ) );
	}

	// if one path is longer than the other, walk up it until they're equal length
	if ( depth2 > depth1 ) {

		Swap( node1,  node2  );
		Swap( depth1, depth2 );
	}
	while ( depth1 > depth2 )
		DFSMoveUp( ghostedState, parents, depths, node1, depth1 );

	// walk up both paths until they meet
	while ( node1 != node2 ) {

		DFSMoveUp( ghostedState, parents, depths, node1, depth1 );
		DFSMoveUp( ghostedState, parents, depths, node2, depth2 );
	}
}


void KeyboardMatrix::DFSMoveUp( ColumnType ghostedState[], uint8_t parents[], uint8_t depths[], uint8_t& node, uint8_t& depth ) {

	// we follow the convention that nodes with indices less than m_rows are rows, and are otherwise columns

	if ( node < m_rows ) {    // row

		uint8_t const parent = parents[ node ];

		// the edge between this node and its parent is ghosted
		ghostedState[ node ] |= ( static_cast< ColumnType >( 1 ) << ( parent - m_rows ) );

		node = parent;
		depth = depths[ parent ];
	}
	else {

		uint8_t const parent = parents[ node ];

		// the edge between this node and its parent is ghosted
		ghostedState[ parent ] |= ( static_cast< ColumnType >( 1 ) << ( node - m_rows ) );

		node = parent;
		depth = depths[ parent ];
	}
}




//============================================================================
//    KeyboardMatrix::Clustering methods
//============================================================================


KeyboardMatrix::Clustering::Clustering( ColumnType const pressedState[], uint8_t const rows ) {

	// cluster two rows together if they are connected along a column (union-find algorithm)
	for ( uint8_t ii = 0; ii < rows; ++ii )
		m_clusters[ ii ] = ii;
	for ( uint8_t ii = 0; ii < rows; ++ii ) {

		if ( pressedState[ ii ] != 0 ) {

			for ( uint8_t jj = ii + 1; jj < rows; ++jj ) {

				if ( ( pressedState[ ii ] & pressedState[ jj ] ) != 0 ) {

					// combine the clusters containing ii and jj
					uint8_t root = FindRoot( ii );
					Reparent( ii, root );
					Reparent( jj, root );
				}
			}
		}
	}
	// flatten all clusters
	for ( uint8_t ii = 0; ii < rows; ++ii )
		if ( m_clusters[ ii ] != ii )
			Reparent( ii, FindRoot( ii ) );

	// find the sizes of all clusters
	for ( uint8_t ii = 0; ii < rows; ++ii )
		m_clusterSizes[ ii ] = 0;
	for ( uint8_t ii = 0; ii < rows; ++ii )
		++m_clusterSizes[ m_clusters[ ii ] ];
}


uint8_t const KeyboardMatrix::Clustering::FindRoot( uint8_t const node ) const {

	// find the root of the cluster containing node
	uint8_t root = 0xff;
	for ( root = node; root != m_clusters[ root ]; root = m_clusters[ root ] );

	return root;
}


void KeyboardMatrix::Clustering::Reparent( uint8_t node, uint8_t const root ) {

	// walk up node's tree, pointing all elements to the root
	for ( ; ; ) {

		uint8_t const next = m_clusters[ node ];
		if ( next == root )
			break;
		m_clusters[ node ] = root;
		node = next;
	}
}

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
	\file keyboard_matrix.hh
	\brief KeyboardMatrix implementation
*/




#ifndef __KEYBOARD_MATRIX_HH__
#define __KEYBOARD_MATRIX_HH__

#ifdef __cplusplus




#include "pins.h"
#include "helpers.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include <assert.h>
#include <stdlib.h>
#include <inttypes.h>




//============================================================================
//    KeyboardMatrix class
//============================================================================


/**
	\brief Keyboard matrix class

	Class which reads from a standard keyboard matrix, optionally corrects for
	ghosting, and stores the state of each key. The matrix is active-low, and
	current is taken to be flowing <em>from</em> the column pins <em>to</em>
	the row pins.

	A key property of this class is that it is runtime configurable--one may
	construct an empty keyboard matrix, and then specify the pins corresponding
	to the rows and columns, and the row/column intersections which are
	occupied by switches.

	This class is a bit more heavyweight than one might at first assume, mostly
	because the anti-ghosting algorithm is "correct", in the sense that a
	keypress will be marked as ghosted iff it is possible that the key is not
	actually being pressed. Doing this correctly requires knowing which
	row/column intersections in the keyboard matrix do not have the associated
	switches attached.

	All routines which modify the (visible) state of this class are protected
	by cli/sei calls, so it is safe to call all <em>const</em> methods from
	inside an interrupt. Non-const methods should only be called from "normal"
	code.
*/
struct KeyboardMatrix {

	typedef uint32_t ColumnType;    ///< unsigned integer type in which the column bitfields for each row are stored

	enum { MAXIMUM_ROWS = 16 };
	enum { MAXIMUM_COLUMNS = sizeof( ColumnType ) * 8 };


	/// \cond false
	static_assert( ( MAXIMUM_ROWS    < 128 ), "too many rows"    );
	static_assert( ( MAXIMUM_COLUMNS < 128 ), "too many columns" );
	/// \endcond


	/**
		\brief Constructor

		This function will also set the number of rows and columns in the
		keyboard matrix (accessed using GetRows() and GetColumns(),
		respectively), and clears all switch flags (see SetSwitch() and
		GetSwitch()).

		The two parameters are strings of the form "XDXDXD", where the number
		of "XD" blocks is the number of pins, "X" is a character in the range
		'a'-'f' or 'A'-'F', and "D" is in the range '0'-'9'.

		The matrix is active-low (see ReadKeyboardMatrix()), so we have pull-up
		resistors on the columns, and set the rows to low (when active).

		\todo create general classes wrapping raw pins, multiplexers,
		demultiplexers, encoders and decoders, both for active-high and
		active-low. then make this thunk to instances of these classes. this
		should probably be done by extending the functions in pins.h and pins.c
		to be way more general, and making string parsers to construct the
		various classes.

		\param rowString    configuration string for row pins
		\param columnString configuration string for column pins
		\param activeHigh   true if matrix is active-high, false otherwise
	*/
	KeyboardMatrix( char const* const rowString, char const* const columnString, bool activeHigh );

	/// \brief Destructor
	~KeyboardMatrix();


	/**
		\brief Checks if anti-ghosting is enabled
		\result  anti-ghosting flag
	*/
	inline bool const GetAntiGhosting() const;

	/**
		\brief Returns the number of debouncing iterations
		\result  number of debouncing iterations
	*/
	inline unsigned int const GetDebouncing() const;

	/**
		\brief Sets anti-ghosting flag
		\param antiGhosting  new anti-ghosting flag
	*/
	inline void SetAntiGhosting( bool const antiGhosting );

	/**
		\brief Sets the number of debouncing iterations
		\param debouncingIterations  new number of debouncing iteratations
	*/
	inline void SetDebouncing( unsigned int const debouncingIterations );


	/**
		\brief Gets the number of row pins currently in use
		\result  rows
	*/
	inline uint8_t const GetRows() const;

	/**
		\brief Gets the number of column pins currently in use
		\result  columns
	*/
	inline uint8_t const GetColumns() const;

	/**
		\brief Gets whether the matrix is active high
		\result  active-high flag
	*/
	inline bool const GetActiveHigh() const;


	/**
		\brief Checks if a switch exists

		Of course, this function doesn't check if the switch
		<em>physically</em> exists--it merely checks if it has been flagged
		using SetSwitch().

		\param row     switch row
		\param column  switch column
		\result  switch flag
	*/
	inline bool const GetSwitch( uint8_t const row, uint8_t const column ) const;

	/**
		\brief Sets a switch flag

		If a switch has not been enabled, then a keypress at the corresponding
		position will never be reported. The main use of these switch flags is
		in anti-ghosting, since we need to know which rows and columns could
		potentially be connected in order to perform anti-ghosting correctly.

		\param row     switch row
		\param column  switch column
		\param value   new switch flag
	*/
	inline void SetSwitch( uint8_t const row, uint8_t const column, bool const value );


	/**
		\brief Checks if a key is pressed

		The state of the keypress flags will be updated only during calls to
		Update(). If a switch has not been marked as existing using
		SetSwitch(), then it cannot be flagged as pressed.

		\param row     key row
		\param column  key column
		\result  keypress flag
	*/
	inline bool const GetPressed( uint8_t const row, uint8_t const column ) const;


	/**
		\brief Updates the keypress flags

		This function reads the keyboard matrix from the pins/switches
		specified by KeyboardMatrix() and SetSwitch(), performs anti-ghosting
		if it has been enabled with SetAntiGhosting(), and updates the keypress
		flags accessed by GetPressed(). If the state of the keyboard matrix has
		changed since the last call to Update(), then this function will return
		true.

		There are several phases to the anti-ghosting procedure:
		<ul>
			<li>first, we cluster the rows in such a way that if two rows
			contain a keypress in the same column, then these rows are in the
			same cluster (see Clustering)</li>
			<li>this clustering is then used to "fill in" any elements of the
			keyboard matrix which "should" be on, but were missed (due to a
			press/release in mid-scan)</li>
			<li>all keypresses registered at positions which do not contain a
			switch are removed</li>
			<li>if the matrix has changed, then any keypress which could be
			"explained" entirely by the other remaining keypresses is marked as
			ghosted (see DFS())</li>
			<li>finally, new ghosted keypresses are removed</li>
		</ul>

		\todo DFS is sort of slow (well, not really, but on this processor, it
		is)--is there any way that the DFS forest could be kept around, and
		only updated in response to changed keypresses?

		\result  changed flag
	*/
	bool const Update();


private:

	/**
		\brief Clustering helper class

		Contains a clustering of a set of keyboard matrix rows by
		column-connection. If two rows both contain a pressed switch in the
		same column, then they will be in the same cluster. This class uses the
		union-find algorithm to merge clusters.
	*/
	struct Clustering {

		/**
			\brief Clusters rows

			After completion, the m_clusters member will contain, for each row,
			the index of the cluster containing this row. Every cluster
			contains exactly one row with the same index as the cluster itself,
			for which the corresponding element of the m_clusterSizes member
			will be the size of the cluster. All other elements of
			m_clusterSizes are zero.

			\param pressedState  keypress states upon which to act
			\param rows  number of rows in keyboard matrix
		*/
		Clustering( ColumnType const pressedState[], uint8_t const rows );


		/**
			\brief Gets a row's cluster
			\param row  row
			\result  cluster index
		*/
		inline uint8_t const GetCluster( uint8_t const row ) const;

		/**
			\brief Gets a cluster's size
			\param cluster  cluster index
			\result  size
		*/
		inline uint8_t const GetClusterSize( uint8_t const cluster ) const;


	private:

		/**
			\brief Finds the root of a cluster

			Walks up the union-find tree starting at node, returning the
			root--this corresponds to the "owner" of the cluster containing the
			node.

			\param node  row
		*/
		uint8_t const FindRoot( uint8_t const node ) const;

		/**
			\brief Flattens a path in a union-find tree

			Walks up the union-find tree starting at node, setting all parent
			pointers to root. If the root is already the owner of this cluster,
			then this function "flattens" the path. Otherwise, it will merge
			the cluster containing the node with that owned by the given root
			node, along a flattened path.

			\param node  row
			\param root  root element of cluster
		*/
		void Reparent( uint8_t node, uint8_t const root );


		uint8_t m_clusters[     MAXIMUM_ROWS ];    ///< row clusters
		uint8_t m_clusterSizes[ MAXIMUM_ROWS ];    ///< row cluster sizes
	};


	/**
		\brief Helper function for Update()

		Reads the current (raw) state of the keyboard matrix into
		workPressedState. The matrix is active-low (see KeyboardMatrix()), so
		we have pull-up resistors on the columns, and set the rows to low (when
		active).

		\param workPressedState  destination
	*/
	void ReadKeyboardMatrix( ColumnType workPressedState[] ) const;


	/**
		\brief Finds all ghosted keypresses

		Consider a bipartite graph in which the row pins form one class,
		the column pins the other, and each edge connecting a row pin to a
		column pin corresponds to a pressed key. Then a keypress is
		ghosted, in the sense that there is some <em>other</em> set of
		keypresses which could explain its presence, iff its corresponding
		edge participates in a cycle.

		This function finds these cycles using depth first search, and
		flags the participating keypresses.

		\param ghostedState  filled in with flags indicating which keypresses are ghosted
		\param clustering    clustering of the rows of the keyboard matrix
	*/
	void DFS( ColumnType ghostedState[], Clustering const& clustering );

	/**
		\brief Helper function for DFS()

		This function takes two nodes in the DFS tree for which adding an
		edge connecting them would create a cycle, and marks all edges of
		this cycle as ghosted, using the DFSMoveUp() function.

		\param ghostedState  filled in with flags indicating which keypresses are ghosted
		\param parents       parent relation in the DFS tree
		\param depths        depth of DFS tree nodes
		\param node1         first node
		\param node2         second node
	*/
	void DFSMarkCycle( ColumnType ghostedState[], uint8_t parents[], uint8_t depths[], uint8_t node1, uint8_t node2 );

	/**
		\brief Helper function for DFSMarkCycle()

		This function takes a node in the DFS tree, and replaces it with
		its parent, while marking the edge connecting the two as ghosted.

		\param ghostedState  filled in with flags indicating which keypresses are ghosted
		\param parents       parent relation in the DFS tree
		\param depths        depth of DFS tree nodes
		\param node          row/column upon which to act
		\param depth         depth of this row/column in the DFS tree
	*/
	void DFSMoveUp( ColumnType ghostedState[], uint8_t parents[], uint8_t depths[], uint8_t& node, uint8_t& depth );


	uint8_t m_rows;    ///< rows in use \sa GetRows(), KeyboardMatrix()
	char m_rowPinNames[          MAXIMUM_ROWS ];    ///< row pin names \sa KeyboardMatrix()
	uint8_t m_rowPinBits[        MAXIMUM_ROWS ];    ///< row pin numbers \sa KeyboardMatrix()
	uint8_t volatile* m_rowPins[ MAXIMUM_ROWS ];    ///< row input registers \sa KeyboardMatrix()

	uint8_t m_logColumns;    ///< log of columns in use \sa GetColumns(), KeyboardMatrix()
	uint8_t m_columns;       ///< columns in use \sa GetColumns(), KeyboardMatrix()
	char m_columnPinNames[           MAXIMUM_COLUMNS ];    ///< column pin names \sa KeyboardMatrix()
	uint8_t m_columnPinBits[         MAXIMUM_COLUMNS ];    ///< column pin numbers \sa KeyboardMatrix()
	uint8_t volatile* m_columnPorts[ MAXIMUM_COLUMNS ];    ///< column output registers \sa KeyboardMatrix()

	bool m_activeHigh;    ///< active-high flag \sa GetActiveHigh(), KeyboardMatrix()

	bool m_antiGhosting;                    ///< anti-ghosting flag \sa GetAntiGhosting(), SetAntiGhosting()
	unsigned int m_debouncingIterations;    ///< number of debouncing iterations to perform \sa GetDebouncing(), SetDebouncing()

	ColumnType m_switchMask[ MAXIMUM_ROWS ];    ///< switch flags \sa GetSwitch(), SetSwitch()

	ColumnType m_rawPressedState[ MAXIMUM_ROWS ];    ///< raw keypress flags \sa Update()
	ColumnType m_pressedState[    MAXIMUM_ROWS ];    ///< anti-ghosted keypress flags \sa GetPressed(), Update()


	inline KeyboardMatrix( KeyboardMatrix const& );                     ///< \brief Private and unimplemented copy constructor
	inline KeyboardMatrix const& operator=( KeyboardMatrix const& );    ///< Private and unimplemented assignment operator
};




//============================================================================
//    KeyboardMatrix inline methods
//============================================================================


bool const KeyboardMatrix::GetAntiGhosting() const {

	return m_antiGhosting;
}


unsigned int const KeyboardMatrix::GetDebouncing() const {

	return m_debouncingIterations;
}


void KeyboardMatrix::SetAntiGhosting( bool const antiGhosting ) {

	// save and clear the interrupt flag
	uint8_t const sreg = SREG;
	cli();

	m_antiGhosting = antiGhosting;

	// restore the interrupt flag
	SREG = sreg;
}


void KeyboardMatrix::SetDebouncing( unsigned int const debouncingIterations ) {

	assert( debouncingIterations > 0 );

	// save and clear the interrupt flag
	uint8_t const sreg = SREG;
	cli();

	m_debouncingIterations = debouncingIterations;

	// restore the interrupt flag
	SREG = sreg;
}


uint8_t const KeyboardMatrix::GetRows() const {

	return m_rows;
}


uint8_t const KeyboardMatrix::GetColumns() const {

	return m_columns;
}


bool const KeyboardMatrix::GetActiveHigh() const {

	return m_activeHigh;
}


bool const KeyboardMatrix::GetSwitch( uint8_t const row, uint8_t const column ) const {

	return( ( m_switchMask[ row ] & ( static_cast< ColumnType >( 1 ) << column ) ) != 0 );
}


void KeyboardMatrix::SetSwitch( uint8_t const row, uint8_t const column, bool const value ) {

	// save and clear the interrupt flag
	uint8_t const sreg = SREG;
	cli();

	if ( value )
		m_switchMask[ row ] |= ( static_cast< ColumnType >( 1 ) << column );
	else
		m_switchMask[ row ] &= ~( static_cast< ColumnType >( 1 ) << column );

	// restore the interrupt flag
	SREG = sreg;
}


bool const KeyboardMatrix::GetPressed( uint8_t const row, uint8_t const column ) const {

	return( ( m_pressedState[ row ] & ( static_cast< ColumnType >( 1 ) << column ) ) != 0 );
}




//============================================================================
//    KeyboardMatrix::Clustering inline methods
//============================================================================


uint8_t const KeyboardMatrix::Clustering::GetCluster( uint8_t const row ) const {

	return m_clusters[ row ];
}


uint8_t const KeyboardMatrix::Clustering::GetClusterSize( uint8_t const cluster ) const {

	return m_clusterSizes[ cluster ];
}




#endif    /* __cplusplus */

#endif    /* __KEYBOARD_MATRIX_HH__ */

/**
  * @file    bitwise.c
  * @author  Jorge Guzman
  * @date    Ago 26, 2016
  * @version 0.1.0
  * @brief	 Codigo em C para manipular bits.
  * @details
  * @section LICENSE
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation; either version 2 of
  * the License, or (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful, but
  * WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  * General Public License for more details at
  * http://www.gnu.org/copyleft/gpl.html
*/

//==============================================================================
// INCLUDE FILES
//==============================================================================

#include "bitwise.h"

//==============================================================================
// PRIVATE DEFINITIONS
//==============================================================================

//==============================================================================
// PRIVATE TYPEDEFS
//==============================================================================

//==============================================================================
// PRIVATE VARIABLES
//==============================================================================

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

//==============================================================================
// SOURCE CODE
//==============================================================================

uint8_t decToBcd( uint8_t val )
{
    return ( ( val / 10 * 16 ) + ( val % 10 ) );
}

uint8_t bcdToDec( uint8_t val )
{
    return ( ( val / 16 * 10 ) + ( val % 16 ) );
}

uint8_t countUntilFirstByteOne( uint8_t valeu )
{
	uint8_t index;

	for( index = 0; index < sizeof(uint8_t) * 8; index++ )
	{
		if( tst_bit( valeu, index ) )
		{
			return index;
		}
	}

	return 0;
}

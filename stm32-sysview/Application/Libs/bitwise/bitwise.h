/**
 * @file    bitwise.h
 * @author  Jorge Guzman
 * @date    Dez 09, 2015
 * @version 0.1.0
 * @brief   Bibliteoca para manipular bits.
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

#ifndef _BITWISE_H
#define	_BITWISE_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
// INCLUDE FILES
//==============================================================================

#include <stdint.h>

//==============================================================================
// PUBLIC DEFINITIONS
//==============================================================================

#define MMIO8( addr )   		( * ( volatile uint8_t * ) ( addr ) )
#define MMIO16( addr )   		( * ( volatile uint16_t * ) ( addr ) )
#define MMIO32( addr )   		( * ( volatile uint32_t * ) ( addr ) )
#define MMIO64( addr )   		( * ( volatile uint64_t * ) ( addr ) )

/** @brief coloca em 1 o bit x da variavel. */
#define	set_bit( value, bit_x )			( value |= ( 1 << bit_x ) )

/** @brief coloca em 0 no bit_x da variavel. */
#define	clr_bit( value, bit_x )			( value &= ~( 1 << bit_x ) )

/** @brief troca o estado logico do bit x da variavel. */
#define toggle_bit( value, bit_x )    	( value ^= (1 << bit_x ) )

/** @brief retorna o estado do bit x na forma de mascara de bits. */
#define tst_bit( value, bit_x )         ( value & ( 1 << bit_x ) )

/** @brief retorna 0 ou 1 conforme leitura do bit x da variavel. */
#define tst_bit_bool( value, bit_x ) 	( ( value & ( 1 << bit_x ) ) >> bit_x )

/** @brief Escreve 0 ou 1 no bit x da variavel. */
#define wr_bit( value, bit_x, _bool ) 	( _bool == 1 ? set_bit( value, bit_x ) : clr_bit( value, bit_x ) )

//==============================================================================
// PUBLIC TYPEDEFS
//==============================================================================

//==============================================================================
// PUBLIC VARIABLES			
//==============================================================================

//==============================================================================
// PUBLIC FUNCTIONS
//==============================================================================

/**
 * A fucao retorna na forma de mascara de bits a posicao do primeiro bit 1 encontrado.
 * @param posicao na forma de mascarade bits.
 * @return
 */
uint8_t countUntilFirstByteOne( uint8_t valeu );

/**
 * Converte um valor decimal para o formato BCD
 * @param val Valor decimal a ser convertido para BCD
 * @return Valor no formato BCD
 */
uint8_t decToBcd( uint8_t val );

/**
 * Converte BDC em decimal
 * @param val Valor BCD a ser convertido para decimal
 * @return Valor no formato decimal
 */
uint8_t bcdToDec( uint8_t val );

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif	/* _BITWISE_H */

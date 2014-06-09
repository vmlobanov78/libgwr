/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-macro-bits.h                                                    *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Copyright (C) 2011-2013 Guillaume Wardavoir                             *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   This program is free software; you can redistribute it and/or modify    *
    *   it under the terms of the GNU General Public License as published by    *
    *   the Free Software Foundation; either version 2 of the License, or       *
    *   (at your option) any later version.                                     *
    *                                                                           *
    *   This program is distributed in the hope that it will be useful,         *
    *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
    *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
    *   GNU General Public License for more details.                            *
    *                                                                           *
    *   You should have received a copy of the GNU General Public License       *
    *   along with this program; if not, write to the Free Software             *
    *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301,   *
    *   USA.                                                                    *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Purpose : Some bits macros                                              *
    *                                                                           *
    *****************************************************************************
*/

#ifndef __LIBGWRC_MACRO_BITS_H__
#define __LIBGWRC_MACRO_BITS_H__

//  ***************************************************************************
//
//  Macros for inserting binary numbers in source code ( up to 32 bits )
//
//  endianness : b1 is lsb :
//
//	  MB32(b4,b3,b2,b1) = b1 + 2^8 * b2 + ... + 2^24 * b4
//
//  I know there are too many parenthesis, but gcc complains
//
//  ***************************************************************************

//
//  helper macros
//

//
//  GWR_HEX_(token)
//
//  Convert a 8-binary-token to a 32 bits integer
//
//  b8_max													=
//	  11111111												->
//	  0x11111111LU											->
//	  binary 0001 0001 0001 0001 0001 0001 0001 0001
//
//	So b8_max fits in 32 bits
//
#define GWR_HEX_(token)													    \
	(guint32)(0x##token##LU)
//
// 8-bit conversion macro, takes a the result of GCMD_HEX_() as input
//
#define GWR_B8_(x)															\
(																			\
	  ( (x & 0x00000001)   ? 0x01	: 0	)									\
	+ ( (x & 0x00000010)   ? 0x02	: 0	)									\
	+ ( (x & 0x00000100)   ? 0x04	: 0	)									\
	+ ( (x & 0x00001000)   ? 0x08	: 0	)									\
	+ ( (x & 0x00010000)   ? 0x10	: 0	)									\
	+ ( (x & 0x00100000)   ? 0x20	: 0	)									\
	+ ( (x & 0x01000000)   ? 0x40	: 0	)									\
	+ ( (x & 0x10000000)   ? 0x80	: 0	)									\
)
//
// The two above macros in one
//
#define GWR_B8(b)													        \
	( GWR_B8_( GWR_HEX_(b) ) )

//
//  16 bits
//
#define LIBGWR_B16(b2,b1)										            \
(																			\
	(guint16)																\
		(																	\
				( ((guint32)GWR_B8(b1))			    )						\
			+   ( ((guint32)GWR_B8(b2)) << 8		)						\
		)																	\
)

//
//  32 bits
//
#define GWR_B32(b4,b3,b2,b1)												\
(																			\
	(guint32)																\
		(																	\
				( ((guint32)GWR_B8(b1))			    )						\
			+   ( ((guint32)GWR_B8(b2)) << 8		)						\
			+   ( ((guint32)GWR_B8(b3)) << 16		)						\
			+   ( ((guint32)GWR_B8(b4)) << 24		)						\
		)																	\
)




#endif                                                                          //  __LIBGWRC_MACRO_BITS_H__

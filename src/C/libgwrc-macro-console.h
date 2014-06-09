/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-macro-console.h                                                 *
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
    *   Class   : None                                                          *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *****************************************************************************
*/


#ifndef __LIBGWRC_MACRO_CONSOLE_H__
#define __LIBGWRC_MACRO_CONSOLE_H__

//  ***************************************************************************
//
//  Macros for coloring console text
//
//  ***************************************************************************


/*
 *******************************************************************************

	Prompt color format : <ESC>[{attr};{fg};{bg}m

    {attr} needs to be one of the following :

	0 Reset All Attributes (return to normal mode)
	1 Bright (usually turns on BOLD)
	2 Dim
	3 Underline
	5 Blink
	7 Revdoesn't tempt me to use non-standard extensionerse
	8 Hidden

    {fg} needs to be one of the following :

      30 Black
      31 Red
      32 Green
      33 Yellow
      34 Blue
      35 Magenta
      36 Cyan
      37 White

    {bg} needs to be one of the following:

      40 Black
      41 Red
      42 Green
      43 Yellow
      44 Blue
      45 Magenta
      46 Cyan
      47 White

********************************************************************************
*/
/*
#define GWR_CON_MODE_RST    0
#define GWR_CON_MODE_BRI    1
#define GWR_CON_MODE_DIM    2
#define GWR_CON_MODE_UND    3
#define GWR_CON_MODE_BLI    5
#define GWR_CON_MODE_REV    7
#define GWR_CON_MODE_HID    8

#define GWR_CON_FG_BLA      30
#define GWR_CON_FG_RED      31
#define GWR_CON_FG_GRE      32
#define GWR_CON_FG_YEL      33
#define GWR_CON_FG_BLU      34
#define GWR_CON_FG_MAG      35
#define GWR_CON_FG_CYA      36
#define GWR_CON_FG_WHI      37

#define GWR_CON_BG_BLA      40
#define GWR_CON_BG_RED      41
#define GWR_CON_BG_GRE      42
#define GWR_CON_BG_YEL      43
#define GWR_CON_BG_BLU      44
#define GWR_CON_BG_MAG      45
#define GWR_CON_BG_CYA      46
#define GWR_CON_BG_WHI      47

namespace libgwr
{

enum eConsolePrompt
{
    CON_MODE_RST    =    0  ,
    CON_MODE_BRI    =    1  ,
    CON_MODE_DIM    =    2  ,
    CON_MODE_UND    =    3  ,
    CON_MODE_BLI    =    5  ,
    CON_MODE_REV    =    7  ,
    CON_MODE_HID    =    8  ,

    CON_FG_BLA      =   30  ,
    CON_FG_RED      =   31  ,
    CON_FG_GRE      =   32  ,
    CON_FG_YEL      =   33  ,
    CON_FG_BLU      =   34  ,
    CON_FG_MAG      =   35  ,
    CON_FG_CYA      =   36  ,
    CON_FG_WHI      =   37  ,

    CON_BG_BLA      =   40  ,
    CON_BG_RED      =   41  ,
    CON_BG_GRE      =   42  ,
    CON_BG_YEL      =   43  ,
    CON_BG_BLU      =   44  ,
    CON_BG_MAG      =   45  ,
    CON_BG_CYA      =   46  ,
    CON_BG_WHI      =   47
};



}
*/

#endif                                                                          //  __LIBGWRC_MACRO_CONSOLE_H__

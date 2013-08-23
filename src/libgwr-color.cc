/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-color.cc                                                         *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Part of libwgwr                                                         *
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
    *   Nspace  :                                                               *
    *                                                                           *
    *   Class   :                                                               *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *****************************************************************************
*/


//  ............................................................................
#include    "libgwr.h"
//  ............................................................................
namespace libgwr
{
namespace color
{

//! Color 'Std' returns Grn
const   gchar   *   g_console_colors_fg[LIBGWR_COLOR_DEFINED_CARD]     =
                    {
                        "32",
                        "30", "31", "32", "33", "34", "35", "36", "37",
                        "32", "32", "32", "32", "32", "32", "32", "32",
                        "32", "32", "32"
                    };
//! Color 'Std' returns Blk
const   gchar   *   g_console_colors_bg[LIBGWR_COLOR_DEFINED_CARD]     =
                    {
                        "40",
                        "40", "41", "42", "43", "44", "45", "46", "47",
                        "40", "40", "40", "40", "40", "40", "40", "40",
                        "40", "40", "40"
                    };

libgwr::Color   *   Colors[LIBGWR_COLOR_CARD_MAX];
guint32             Card   = LIBGWR_COLOR_DEFINED_CARD;

//==============================================================================
class ColorStatic
{
    public:
     ColorStatic();
    ~ColorStatic();
};
ColorStatic::ColorStatic()
{
    using namespace color;

    // no color
    Colors[Std]     =   GWR_NEW_CAST( Color, "Std"  ,   "#000000"   );          //!< This color wont be used in GwrTextTagHelper

    // console colors
    Colors[Blk]     =   GWR_NEW_CAST( Color, "Blk"  ,   "#000000"   );
    Colors[Red]     =   GWR_NEW_CAST( Color, "Red"  ,   "#FF0000"   );
    Colors[Grn]     =   GWR_NEW_CAST( Color, "Grn"  ,   "#007800"   );
    Colors[Yel]     =   GWR_NEW_CAST( Color, "Yel"  ,   "#FFFF00"   );
    Colors[Blu]     =   GWR_NEW_CAST( Color, "Blu"  ,   "#0000FF"   );
    Colors[Mag]     =   GWR_NEW_CAST( Color, "Mag"  ,   "#FF00FF"   );
    Colors[Cya]     =   GWR_NEW_CAST( Color, "Cya"  ,   "#00FFFF"   );
    Colors[Whi]     =   GWR_NEW_CAST( Color, "Whi"  ,   "#FFFFFF"   );

    // other colors
    Colors[Ora]     =   GWR_NEW_CAST( Color, "Ora"  ,   "#DC8E00"   );
    Colors[Grn1]    =   GWR_NEW_CAST( Color, "Grn1" ,   "#00FF00"   );
    Colors[Greyd]   =   GWR_NEW_CAST( Color, "Greyd",   "#dddddd"   );
    Colors[Grey9]   =   GWR_NEW_CAST( Color, "Grey9",   "#999999"   );
    Colors[Greyc]   =   GWR_NEW_CAST( Color, "Greyc",   "#cccccc"   );
    Colors[Blu1]    =   GWR_NEW_CAST( Color, "Blu1" ,   "#1e90ff"   );
    Colors[Turq1]   =   GWR_NEW_CAST( Color, "Turq1",   "#00ced1"   );
    Colors[Grey6]   =   GWR_NEW_CAST( Color, "Grey6",   "#666666"   );
    Colors[17]      =   GWR_NEW_CAST( Color, "17"  ,    "#000000"   );
    Colors[18]      =   GWR_NEW_CAST( Color, "18"  ,    "#000000"   );
    Colors[19]      =   GWR_NEW_CAST( Color, "19"  ,    "#000000"   );
};

ColorStatic::~ColorStatic()
{
    using namespace color;

    guint32 i = 0;

    for ( i = 0 ; i != color::Card ; i++ )
        delete Colors[i];
}



}   // namespace color

static  ColorStatic  Dummy;


}   // namespace libgwr


/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-color.cc                                                         *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Part of libwgwr                                                         *
    *                                                                           *
    *   Copyright (C) 2011-2014 Guillaume Wardavoir                             *
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
    *   Namespace : libgwr::color                                               *
    *                                                                           *
    *   Class     : Color                                                       *
    *                                                                           *
    *   Purpose   : RGB colors facilities                                       *
    *                                                                           *
    *****************************************************************************
*/


//  ............................................................................
#include    "libgwr.hh"
//  ............................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(color)

libgwr::color::Color    *   Colors[LIBGWR_COLOR_CARD_MAX];
//  ............................................................................
GWR_NAMESPACE_START(console)
                                                                                /*
	Console text format : <ESC>[{attr};{fg};{bg}m

    * {attr} needs to be one of the following :
        ...

    * {fg} needs to be one of the following :
        30 Black
        31 Red
        32 Green
        33 Yellow
        34 Blue
        35 Magenta
        36 Cyan
        37 White

    * {bg} needs to be one of the following:
        40 Black
        41 Red
        42 Green
        43 Yellow
        44 Blue
        45 Magenta
        46 Cyan
        47 White
                                                                                */
GWR_NAMESPACE_END(console)
//  ............................................................................
class StaticInit
{
    private:
    void    set_C_color(guint32 _index, guint32 _xrgb);

    public:
     StaticInit();
    ~StaticInit();
};
void    StaticInit::set_C_color(guint32 _index, guint32 _xrgb)
{
    libgwrC_Colors[ _index ].r = ( ( _xrgb & 0x00ff0000 ) >> 16 );
    libgwrC_Colors[ _index ].g = ( ( _xrgb & 0x0000ff00 ) >> 8  );
    libgwrC_Colors[ _index ].b = ( ( _xrgb & 0x000000ff )       );
}
StaticInit::StaticInit()
{
    //  ........................................................................
    //  C part
    set_C_color(Std     ,0xFFFFFF);

    // console colors
    set_C_color(Blk     ,0x000000);
    set_C_color(Red     ,0xFF0000);
    set_C_color(Grn     ,0x007800);
    set_C_color(Yel     ,0xFFFF00);
    set_C_color(Blu     ,0x0000FF);
    set_C_color(Mag     ,0xFF00FF);
    set_C_color(Cya     ,0x00FFFF);
    set_C_color(Whi     ,0xFFFFFF);

    // other colors
    set_C_color(Ora     ,0xDC8E00);
    set_C_color(Grn1    ,0x00FF00);
    set_C_color(Greyd   ,0xdddddd);
    set_C_color(Grey9   ,0x999999);
    set_C_color(Greyc   ,0xcccccc);
    set_C_color(Blu1    ,0x1e90ff);
    set_C_color(Turq1   ,0x00ced1);
    set_C_color(Grey6   ,0x666666);
    set_C_color(17      ,0x000000);
    set_C_color(18      ,0x000000);
    set_C_color(19      ,0x000000);
    //  ........................................................................
    //  C++ part
    // no color
    Colors[Std]     =   GWR_NEW_CAST( Color, "Std"  , "#FFFFFF", "32", "40" );

    // console colors
    Colors[Blk]     =   GWR_NEW_CAST( Color, "Blk"  , "#000000", "30", "40" );
    Colors[Red]     =   GWR_NEW_CAST( Color, "Red"  , "#FF0000", "31", "41" );
    Colors[Grn]     =   GWR_NEW_CAST( Color, "Grn"  , "#007800", "32", "42" );
    Colors[Yel]     =   GWR_NEW_CAST( Color, "Yel"  , "#FFFF00", "33", "43" );
    Colors[Blu]     =   GWR_NEW_CAST( Color, "Blu"  , "#0000FF", "34", "44" );
    Colors[Mag]     =   GWR_NEW_CAST( Color, "Mag"  , "#FF00FF", "35", "45" );
    Colors[Cya]     =   GWR_NEW_CAST( Color, "Cya"  , "#00FFFF", "36", "46" );
    Colors[Whi]     =   GWR_NEW_CAST( Color, "Whi"  , "#FFFFFF", "37", "47" );

    // other colors
    Colors[Ora]     =   GWR_NEW_CAST( Color, "Ora"  , "#DC8E00", "32", "40" );
    Colors[Grn1]    =   GWR_NEW_CAST( Color, "Grn1" , "#00FF00", "32", "40" );
    Colors[Greyd]   =   GWR_NEW_CAST( Color, "Greyd", "#dddddd", "32", "40" );
    Colors[Grey9]   =   GWR_NEW_CAST( Color, "Grey9", "#999999", "32", "40" );
    Colors[Greyc]   =   GWR_NEW_CAST( Color, "Greyc", "#cccccc", "32", "40" );
    Colors[Blu1]    =   GWR_NEW_CAST( Color, "Blu1" , "#1e90ff", "32", "40" );
    Colors[Turq1]   =   GWR_NEW_CAST( Color, "Turq1", "#00ced1", "32", "40" );
    Colors[Grey6]   =   GWR_NEW_CAST( Color, "Grey6", "#666666", "32", "40" );
    Colors[17]      =   GWR_NEW_CAST( Color, "17"  ,  "#000000", "32", "40" );
    Colors[18]      =   GWR_NEW_CAST( Color, "18"  ,  "#000000", "32", "40" );
    Colors[19]      =   GWR_NEW_CAST( Color, "19"  ,  "#000000", "32", "40" );
};

StaticInit::~StaticInit()
{
    using namespace color;

    guint32 i = 0;

    for ( i = 0 ; i != LIBGWR_COLOR_DEFINED_CARD ; i++ )
        delete Colors[i];
}

GWR_NAMESPACE_END(color)
GWR_NAMESPACE_END(libgwr)
//  ............................................................................
static  libgwr::color::StaticInit Dummy;
//  ............................................................................



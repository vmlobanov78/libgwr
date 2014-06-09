/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-treestore.cc                                                     *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Part of libwgwr                                                         *
    *                                                                           *
    *   Copyright (C) 2011-2013 Guillaume Wardavoir                             *
    *   Inspiration             Tim-Philipp Müller                              *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Tim-Philipp Müller wrote the excellent "GTK+ 2.0 Tree View Tutorial"    *
    *   whose section 11 'writing custom models' is the base of the             *
    *   Treestore code.                                                         *
    *
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
    *   Namespace   : libgwr::treestore                                         *
    *                                                                           *
    *   Class       : Store                                                     *
    *                                                                           *
    *   Purpose     : Some functions                                            *
    *                                                                           *
    *****************************************************************************
*/

//..............................................................................
#include	"libgwr-treestore.hh"
//..............................................................................

GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(treestore)

Store   *   Store::s_store          = NULL;
guint32_x2  Store::s_ret_guint32_x2;
gboolean    Store::s_ret_gboolean;

//  ****************************************************************************
//
//                          PATHs
//
//  ****************************************************************************
Path::Path(guint32 card)
{
    d_ascii_dump	= (gchar*)g_try_malloc0(64);
    a_card			= card;
    d_uid			= (guint32*) g_try_malloc0(a_card * sizeof(guint32) );
}
Path::~Path()
{
    g_free(d_ascii_dump);
    g_free(d_uid);
}

Path*
Path::dup()
{
    Path    *   dup = NULL;
    //.........................................................................
    dup = GWR_NEW_CAST( Path, card() );

    memcpy(
        dup->vpuid()                ,
        this->vpuid()               ,
        sizeof( guint32 ) * card() );

    return dup;
}

const gchar*
Path::dump()
{
    gchar   temp[128];
    guint32 i ;
    //.........................................................................
    if ( a_card == 0 )
    {
        sprintf(d_ascii_dump, "~ Empty ~");
        return (const gchar*)d_ascii_dump;
    }

    sprintf(d_ascii_dump, "%03i", uid_get(0));

    for ( i = 1 ; i < a_card ; i++ )
    {
        sprintf(temp, " %03i ", uid_get(i));
        strcat(d_ascii_dump, temp);
    }

    return (const gchar*)d_ascii_dump;
}
//  ****************************************************************************
//
//                          NodeRoot
//
//  ****************************************************************************
void
Store::node_root_init()
{
    g_return_if_fail( ! a_node_root_created );

    d_data_root =   GWR_NEW_CAST( IDataRoot );

    d_node_root =   GWR_NEW_CAST(
                        NodeRoot    ,
                        0, 0        ,
                        0, 0        ,
                        (Node*)NULL ,
                        d_data_root );

    a_node_root_created = TRUE;
}

GWR_NAMESPACE_END(treestore)
GWR_NAMESPACE_END(libgwr)



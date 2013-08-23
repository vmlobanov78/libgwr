/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-treestore.cc                                                     *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Part of libwgwr                                                         *
    *                                                                           *
    *   Copyright (C) 2011-2011 Guillaume Wardavoir                             *
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
    *   Purpose     : Some functrions                                           *
    *                                                                           *
    *****************************************************************************
*/

//..............................................................................
#include	"libgwr-treestore.h"
//..............................................................................

namespace libgwr
{
namespace treestore
{



//  ###########################################################################
//  ###																		###
//  ##																		 ##
//  #								PATH									  #
//  ##																		 ##
//  ###																		###
//  ###########################################################################
void*
Path::operator new(size_t t)
{
	Path * path = g_try_new0(Path, 1);

	if ( ! path )
    {
		//GCMD_ERR("Path::operator new:g_try_new0 failed");
    }
	return path;
}
void
Path::operator delete(void *p)
{
	g_free(p);
}

Path::Path(gint card)
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

gint
Path::card()
{
	return a_card;
}

guint32
Path::uid_get(gint _pos)
{
	return d_uid[_pos];
}

void
Path::uid_set(gint _pos, guint32 _uid)
{
	d_uid[_pos] = _uid;
}

Path*
Path::dup()
{
	Path	*   dup = NULL;
	gint		i   = 0;
	//.........................................................................

	dup = new Path(card());

	for ( i = 0 ; i != card() ; i++ )
		dup->uid_set(i, uid_get(i));

	return dup;
}

const gchar*
Path::dump()
{
	gchar   temp[128];
	gint	i ;
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




//  ###########################################################################
//  ###																		###
//  ##																		 ##
//  #							NODE ROOT									  #
//  ##																		 ##
//  ###																		###
//  ###########################################################################
void
Store::node_root_init()
{
	g_return_if_fail( ! a_node_root_created );

	d_node_root = new Node( 0, 0, 0, NULL, NULL);
	a_node_root_created = TRUE;
}

}       // namespace treestore
}       // namespace libgwr



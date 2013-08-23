/*
    GNOME Commander - A GNOME based file manager
    Copyright (C) 2001-2006 Marcus Bjurman
    Copyright (C) 2007-2010 Piotr Eljasiak
    Copyleft      2010-2010 Guillaume Wardavoir
							Tim-Philipp Müller

	***************************************************************************
	Tim-Philipp Müller wrote the excellent "GTK+ 2.0 Tree View Tutorial" whose
	section 11 'writing custom models' is the base of the
	Treestore code.
	***************************************************************************

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#ifndef _LIBGWR_TREESTORE_H_
#define _LIBGWR_TREESTORE_H_

#include	"libgwr.h"

//##############################################################################
//
//					Object definition
//
//##############################################################################

namespace libgwr
{
namespace treestore
{

//==============================================================================

    //!
    //!  \struct  Store
    //!
    //!   The purpose of this struct is :
    //!       - implement GtkTreeModelInterface
    //!       - implement useful other features
    //!
    //!   \defgroup _grp_treestore_ Store
    //!

//==============================================================================
struct Store
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//  GObject inheritance
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    public:
    GObject         parent;

	public:
	gint		refcount() { return ((GInitiallyUnowned*)this)->ref_count; }


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //! \defgroup   _grp_treestore_gtk_tree_model_interface_impl_ GtkTreeModelInterface implementation
    //! \ingroup    _grp_treestore_
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //! @{
	public:
	static  GtkTreeModelFlags   get_flags		(GtkTreeModel*);
	static  gint				get_n_columns   (GtkTreeModel*);
	static  GType				get_column_type (GtkTreeModel*, gint index);

	static  gboolean			get_iter		(GtkTreeModel*, GtkTreeIter *out, GtkTreePath  *path);
	static  GtkTreePath *		get_path		(GtkTreeModel*, GtkTreeIter *in);
	static  void				get_value		(GtkTreeModel*, GtkTreeIter *in,	gint column, GValue *value);
	static  gboolean			iter_next		(GtkTreeModel*, GtkTreeIter *inout);
	static  gboolean			iter_children   (GtkTreeModel*, GtkTreeIter *out, GtkTreeIter *parent);
	static  gboolean			iter_has_child  (GtkTreeModel*,	GtkTreeIter *in);
	static  gint				iter_n_children (GtkTreeModel*,	GtkTreeIter *in_parent);
	static  gboolean			iter_nth_child  (GtkTreeModel*,	GtkTreeIter *out_child,	GtkTreeIter *in_parent,	gint n);
	static  gboolean			iter_parent		(GtkTreeModel*,	GtkTreeIter *out_parent, GtkTreeIter  *in_child);
    //! @}

};


//##############################################################################
//
//					GLib stuff
//
//##############################################################################
GType         	libgwr_treestore_get_type   (void);
Store    *		libgwr_treestore_new		(void);

#define LIBGWR_TREESTORE_TYPE            (libgwr_treestore_get_type())
#define LIBGWR_TREESTORE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), LIBGWR_TREESTORE_TYPE, libgwr::treestore::Store))
#define IS_LIBGWR_TREESTORE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LIBGWR_TREESTORE_TYPE))

#define LIBGWR_TREESTORE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  LIBGWR_TREESTORE_TYPE, libgwr::treestore::StoreClass))
#define IS_LIBGWR_TREESTORE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  LIBGWR_TREESTORE_TYPE))
#define LIBGWR_TREESTORE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  LIBGWR_TREESTORE_TYPE, libgwr::treestore::StoreClass))


struct _StoreClass
{
  GObjectClass parent_class;
};

typedef struct _StoreClass  StoreClass;

}   // namespace treestore
}   // namespace libgwr


#endif

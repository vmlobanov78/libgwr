/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-treestore-gobject.h                                              *
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
    *   Purpose     : GObject stuff for Store class                             *
    *                                                                           *
    *****************************************************************************
*/

//..............................................................................
#include	"libgwr-treestore.hh"
//..............................................................................

GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(treestore)

static GObjectClass *   parent_class = NULL;

static void store_init(Store*);
static void store_class_init(StoreClass *klass);
static void store_tree_model_init (GtkTreeModelIface *iface);
static void store_finalize(GObject *object);

//=============================================================================

//!
//! store_init:
//!
//! This is called everytime a new custom list object instance is created
//!   (we do that in store_new).
//!

//=============================================================================
static void
store_init(
    Store *store)
{
    // default sort settings
    store->set_sort_type(eSortAscending);
    store->set_sort_type(eSortNone);
    store->set_sort_collate_key_to_use(0);

    store->d_node_root          = NULL;
    store->a_node_root_created  = FALSE;

    // others
    store->stamp_init();
    store->node_root_init();
    store->uid_init();
}

//=============================================================================

//!
//! store_class_init:
//!
//! More boilerplate GObject/GType stuff. Init callback for the type system,
//! called once when our new class is created.
//!

//=============================================================================
static void
store_class_init(StoreClass *klass)
{
    GObjectClass *object_class;

    parent_class = (GObjectClass*) g_type_class_peek_parent (klass);
    object_class = (GObjectClass*) klass;

    object_class->finalize = store_finalize;
}

//=============================================================================

//!
//! store_tree_model_init:
//!
//! Init callback for the interface registration in store_get_type. Here we
//! override the GtkTreeModel interface functions that we implement.
//!

//=============================================================================
static void
store_tree_model_init (GtkTreeModelIface *iface)
{
    iface->get_flags       = Store::get_flags;
    iface->get_n_columns   = Store::get_n_columns;
    iface->get_column_type = Store::get_column_type;
    iface->get_iter        = Store::get_iter;
    iface->get_path        = Store::get_path;
    iface->get_value       = Store::get_value;
    iface->iter_next       = Store::iter_next;
    iface->iter_children   = Store::iter_children;
    iface->iter_has_child  = Store::iter_has_child;
    iface->iter_n_children = Store::iter_n_children;
    iface->iter_nth_child  = Store::iter_nth_child;
    iface->iter_parent     = Store::iter_parent;
}


//=============================================================================

//!
//! store_finalize:
//!
//! This is called just before an instance is destroyed. Free dynamically
//! allocated memory here.
//!

//=============================================================================
static void
store_finalize(GObject *object)
{
    Store					*store  = NULL;
    //.........................................................................
    g_return_if_fail ( IS_LIBGWR_TREESTORE(object) );
    store = LIBGWR_TREESTORE(object);

    //GCMD_INF("Store::finalize()");

    // free all records and free all memory used by the list
    //store->ext_clear();

    // must chain up - finalize parent
    (* parent_class->finalize) (object);
}

//=============================================================================

//!
//! store_get_type:
//!
//! Here we register our new type and its interfaces with the type system.
//! If you want to implement additional interfaces like GtkTreeSortable, you
//! will need to do it here.
//!

//=============================================================================
GType
store_get_type (void)
{
    static GType store_type = 0;

    /* Some boilerplate type registration stuff */
    if (store_type == 0)
    {
        static const GTypeInfo store_info =
        {
            sizeof (StoreClass),
            NULL,                                         /* base_init */
            NULL,                                         /* base_finalize */
            (GClassInitFunc) store_class_init,
            NULL,                                         /* class finalize */
            NULL,                                         /* class_data */
            sizeof (Store),
            0,                                           /* n_preallocs */
            (GInstanceInitFunc) store_init
        };
        static const GInterfaceInfo tree_model_info =
        {
            (GInterfaceInitFunc) store_tree_model_init,
            NULL,
            NULL
        };

        /* First register the new derived type with the GObject type system */
        store_type = g_type_register_static (G_TYPE_OBJECT, "CustomList",
                                             &store_info, (GTypeFlags)0);

        /* Now register our GtkTreeModel interface with the type system */
        g_type_add_interface_static (store_type, GTK_TYPE_TREE_MODEL, &tree_model_info);
    }

    return store_type;
}

//=============================================================================

//!
//! store_new:
//!

//=============================================================================
Store*
store_new(void)
{
    Store *t;

    t = (Store*) g_object_new (LIBGWR_TREESTORE_TYPE, NULL);

    g_assert( t != NULL );

    return t;
}

GWR_NAMESPACE_END(treestore)
GWR_NAMESPACE_END(libgwr)




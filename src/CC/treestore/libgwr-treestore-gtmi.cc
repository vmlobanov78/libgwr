/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-treestore-gtmi.h                                                 *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Part of libwgwr                                                         *
    *                                                                           *
    *   Copyright (C) 2011-2014 Guillaume Wardavoir                             *
    *   Inspiration             Tim-Philipp Müller                              *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Tim-Philipp Müller wrote the excellent "GTK+ 2.0 Tree View Tutorial"    *
    *   whose section 11 'writing custom models' is the base of the             *
    *   Store code.                                                             *
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
    *   Namespace   : libgwr::treestore                                         *
    *                                                                           *
    *   Class       : Store                                                     *
    *                                                                           *
    *   Purpose     : GtkTreeModelInterface implementation                      *
    *                                                                           *
    *****************************************************************************
*/

//..............................................................................
#include	"libgwr-treestore.hh"
//..............................................................................

GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(treestore)


//  ###########################################################################
//  ###																		###
//  ##																		 ##
//  #				TREESTORE  - GtkTreeModelIface IMPLEMENTATION			  #
//  ##																		 ##
//  ###																		###
//  ###########################################################################

//=============================================================================

/**
*   Store::get_flags:
*
*   Tells the rest of the world whether our tree model has any special
*   characteristics. In our case, tree iters are non persistent
*
**/

//=============================================================================
GtkTreeModelFlags
Store::get_flags(GtkTreeModel *tree_model)
{
    g_return_val_if_fail (IS_LIBGWR_TREESTORE(tree_model), (GtkTreeModelFlags)0);

    return (GtkTreeModelFlags)0;
}

//=============================================================================

/**
*   Store::get_n_columns:
*
*   \brief This method always return 1. I have not implemented multi-columns
  *   since I can deduce all the columns and use appropriate GtkCellRenderers
  *   for the GtkTreeView via DataInterface. But it is possible to
  *   implement that.
*
**/

//=============================================================================
gint
Store::get_n_columns(GtkTreeModel *tree_model)
{
    //g_return_val_if_fail (IS_LIBGWR_TREESTORE(tree_model), 0);

    return 1;
}

//=============================================================================

/**
*   Store::get_column_type:
*
*   Our column is of type G_POINTER
*
**/

//=============================================================================
GType
Store::get_column_type(
    GtkTreeModel *treemodel,
    gint          index)
{
    g_return_val_if_fail (IS_LIBGWR_TREESTORE(treemodel), G_TYPE_INVALID);
    g_return_val_if_fail (index == 0, G_TYPE_INVALID);

    return G_TYPE_POINTER;
}

//  ----------------------------------------------------------------------------
//! \fn         Store::get_iter()
//!
//! \brief      Converts a tree path (physical position) into a tree iter structure.
//!
//! \internal   Returns evidently a _VISIBLE_ node.
//  ----------------------------------------------------------------------------
gboolean
Store::get_iter(
    GtkTreeModel *treemodel,
    GtkTreeIter  *iter,
    GtkTreePath  *path)
{
    Store           *   treestore   = NULL;
    Node            *   node        = NULL;
    NodeBlock       *   block       = NULL;
    gint            *   indices     = NULL  ,
                        pos         = 0     ,
                        depth       = 0     ;
    gint                i           = 0;
    //.........................................................................
    g_assert( IS_LIBGWR_TREESTORE(treemodel) );
    treestore = LIBGWR_TREESTORE(treemodel);

    //  convert this assertion to a FALSE return value
    //  because of the "no selected item" bug
    //  assertion : g_assert( path );
    if ( ! path )
    {
        //NODE_WNG("Store::get_iter():NULL GtkTreePath");
        return FALSE;
    }

    indices = gtk_tree_path_get_indices (path);
    depth   = gtk_tree_path_get_depth   (path);

    // get the position
    pos		= indices[i++];

    // ok, get first root node ( first child of the ~root~ node )
    node	=   treestore->node_root()->children()->node_get(pos);

    while ( node )
    {
        // path is done ?
        if ( i == depth )
        {
            ITER_FROM_NODE(treestore, iter, node);
            return TRUE;
        }

        // node = node->children()->get_pos( indices[i++] );
        block   = node->children();
        pos		= indices[i++];
        node	= block->node_get(pos);
    }

    //if ( treestore->node_root()->children()->card() != 0 )
    //{
    //GCMD_ERR("Store::get_iter::failed to get iter from path");
    //}
    return FALSE;
}
//  ----------------------------------------------------------------------------
//! \fn         Store::get_path()
//!
//! \brief      Return the GtkTreePath of an iter.
//!
//! \internal   Use this method only with _VISIBLE_ nodes.
//  ----------------------------------------------------------------------------
GtkTreePath *
Store::get_path(
    GtkTreeModel *treemodel,
    GtkTreeIter  *iter)
{
    Store           *   treestore   = NULL;
    GtkTreePath     *   path        = NULL;
    Node            *   node        = NULL;
    //.........................................................................
    g_return_val_if_fail ( IS_LIBGWR_TREESTORE(treemodel), NULL );
    treestore = LIBGWR_TREESTORE(treemodel);
    g_return_val_if_fail ( treestore->iter_is_valid(iter), NULL);

    path = gtk_tree_path_new();

    NODE_FROM_ITER(node, iter);
    g_return_val_if_fail( node              , NULL );
    g_return_val_if_fail( node->shown()     , NULL );                           //  node has to be shown

    do
    {
        gtk_tree_path_prepend_index(path, node->pos());
        node = node->parent();
    }
    while ( node != treestore->node_root() );

    //STORE_INF("get_path:%s", gtk_tree_path_to_string(path));

    return path;
}
//  ----------------------------------------------------------------------------
//! \fn         Store::get_value()
//!
//! \brief      Returns the GValue associated with an iter.
//!
//! \internal   This method consider _VISIBLE_ and _HIDDEN_ nodes indifferently.
//  ----------------------------------------------------------------------------
void
Store::get_value(
    GtkTreeModel *treemodel,
    GtkTreeIter  *iter,
    gint          column,
    GValue       *value)
{
    Store   *   treestore   = NULL;
    Node    *   node        = NULL;
    //.........................................................................
    if ( ! IS_LIBGWR_TREESTORE(treemodel) )		goto fail;
    treestore = LIBGWR_TREESTORE(treemodel);
    if ( ! treestore->iter_is_valid(iter) )					goto fail;
    //if ( column != 0 )									goto fail;
    //if ( !value )											goto fail;

    NODE_FROM_ITER(node, iter);
    if ( ! node )											goto fail;

    g_value_init(value, G_TYPE_POINTER);
    g_value_set_pointer(value, node->data());
    return;

fail:
    g_value_init(value, G_TYPE_POINTER);
    g_value_set_pointer(value, NULL);
}
//  ----------------------------------------------------------------------------
//!	\fn         Store::iter_next()
//!
//! \brief      Takes an iter structure and sets it to point to the next row.
//!
//! \internal   Returns the next _SHOWN_ node of a NodeBlock.
//  ----------------------------------------------------------------------------
gboolean
Store::iter_next(
    GtkTreeModel  *treemodel,
    GtkTreeIter   *iter)
{
    Store   *   treestore   = NULL;
    Node    *   node		= NULL;
    //.........................................................................
    g_return_val_if_fail ( IS_LIBGWR_TREESTORE(treemodel), FALSE );
    treestore = LIBGWR_TREESTORE(treemodel);

    g_return_val_if_fail ( treestore->iter_is_valid(iter), FALSE );

    NODE_FROM_ITER(node, iter);
    g_return_val_if_fail( node              , FALSE);
    g_return_val_if_fail( node->shown()     , FALSE);                           //  node has to be shown

    node = node->next();

    if ( ! node )
    {
        ITER_RESET(iter, treestore);
        return FALSE;
    }

    g_return_val_if_fail( node->shown()     , FALSE);                           //  node has to be shown

    ITER_FROM_NODE(treestore, iter, node);
    return TRUE;
}
//  ----------------------------------------------------------------------------
//! \fn         Store::iter_children()
//!
//! \brief      Returns TRUE or FALSE depending on whether the row specified by
//!     'parent' has any children. If it has children, then 'iter' is set to
//!     point to the first child. Special case: if 'parent' is NULL, then the
//!     first top-level row should be returned if it exists.
//!
//! \internal   This method consider only _VISIBLE_ nodes.
//  ----------------------------------------------------------------------------
gboolean
Store::iter_children(
    GtkTreeModel *treemodel,
    GtkTreeIter  *iter_child,
    GtkTreeIter  *iter_parent)
{
    Store   *   treestore   = NULL;
    Node    *   node        = NULL;
    //.........................................................................
    g_return_val_if_fail ( IS_LIBGWR_TREESTORE(treemodel), FALSE );
    treestore = LIBGWR_TREESTORE(treemodel);

    g_return_val_if_fail ( treestore->iter_is_valid_but_may_be_null(iter_parent), FALSE );

    if ( ! iter_parent )
    {
        node = treestore->node_root();
        g_return_val_if_fail( node, FALSE );

        if ( node->sterile() )
            return FALSE;

        ITER_FROM_NODE(treestore, iter_child, node->children()->node_get(0));
        return TRUE;
    }

    NODE_FROM_ITER(node, iter_parent);
    g_assert(node);

    if ( ! node->sterile() )
    {
        ITER_FROM_NODE(treestore, iter_child, node->children()->node_get(0));
        return TRUE;
    }

    return FALSE;
}
//  ----------------------------------------------------------------------------
//! \fn         Store::iter_has_child()
//!
//! \brief      Returns TRUE or FALSE depending on whether the row specified by
//!     'iter' has any children.
//!
//! \internal   This method consider only _VISIBLE_ nodes.
//  ----------------------------------------------------------------------------
gboolean
Store::iter_has_child(
    GtkTreeModel *treemodel,
    GtkTreeIter  *iter)
{
    Store					*treestore  = NULL;
    Node				*node		= NULL;
    //.........................................................................
    g_return_val_if_fail ( IS_LIBGWR_TREESTORE(treemodel), FALSE );
    treestore = LIBGWR_TREESTORE(treemodel);
    g_return_val_if_fail ( treestore->iter_is_valid(iter), FALSE );

    NODE_FROM_ITER(node, iter);
    g_assert(iter);

    return node->children()->card() != 0 ? TRUE : FALSE;
}
//  ----------------------------------------------------------------------------
//! \fn         Store::iter_n_children()
//!
//! \brief      Returns the number of children the row specified by 'iter' has.
//!     Special case : if 'iter' is NULL, return the number of top-level nodes.
//!
//! \internal   This method consider only _VISIBLE_ nodes.
//  ----------------------------------------------------------------------------
gint
Store::iter_n_children(
    GtkTreeModel *treemodel,
    GtkTreeIter  *iter)
{
    Store					*treestore  = NULL;
    Node				*node		= NULL;
    //.........................................................................
    g_return_val_if_fail ( IS_LIBGWR_TREESTORE(treemodel), 0 );
    treestore = LIBGWR_TREESTORE(treemodel);
    g_return_val_if_fail ( treestore->iter_is_valid_but_may_be_null(iter), 0 );

    if ( ! iter )
        node = treestore->node_root();
    else
        NODE_FROM_ITER(node, iter);

    g_assert(node);

    return node->children()->card();
}
//  ----------------------------------------------------------------------------
//! \fn         Store::iter_nth_child()
//!
//! \brief      If the row specified by 'parent' has any children, set 'iter' to
//!     the n-th child and return TRUE if it exists, otherwise FALSE.
//!     Special case : if 'parent' is NULL, we need to set 'iter' to the n-th
//!     toplevel node.
//!
//! \internal   This method consider only _VISIBLE_ nodes.
//  ----------------------------------------------------------------------------
gboolean
Store::iter_nth_child(
    GtkTreeModel *treemodel,
    GtkTreeIter  *iter_child,
    GtkTreeIter  *iter_parent,
    gint          n)
{
    Store			*   treestore   = NULL;
    Node			*   node        = NULL;
    NodeBlock		*   block       = NULL;
    //.........................................................................
    g_return_val_if_fail ( IS_LIBGWR_TREESTORE(treemodel), FALSE );
    treestore = LIBGWR_TREESTORE(treemodel);
    g_return_val_if_fail ( treestore->iter_is_valid_but_may_be_null(iter_parent), FALSE );

    if ( ! iter_parent )
        node = treestore->node_root();
    else
    {
        NODE_FROM_ITER(node, iter_parent);
        g_assert( node );
    }

    block = node->children();

    g_assert( n < block->card() );

    ITER_FROM_NODE(treestore, iter_child, block->node_get(n));
    return TRUE;
}
//  ----------------------------------------------------------------------------
//! \fn         Store::iter_parent()
//!
//! \brief      Point 'iter_parent' to the parent node of 'iter_child'.
//!
//! \internal   This method consider _VISIBLE_ and _HIDDEN_ nodes indifferently.
//  ----------------------------------------------------------------------------
gboolean
Store::iter_parent(
    GtkTreeModel *treemodel,
    GtkTreeIter  *iter_parent,
    GtkTreeIter  *iter_child)
{
    Store					*treestore  = NULL;
    Node				*node		= NULL;
    //.........................................................................
    g_return_val_if_fail ( IS_LIBGWR_TREESTORE(treemodel), FALSE );
    treestore = LIBGWR_TREESTORE(treemodel);
    g_return_val_if_fail ( treestore->iter_is_valid(iter_child), FALSE );

    NODE_FROM_ITER(node, iter_child);
    g_assert( node );
    g_assert( node->parent() );

    node = node->parent();
    g_assert( node );
    //g_assert( node != treestore->node_root() );
    ITER_FROM_NODE(treestore, iter_parent, node);

    return TRUE;
}

GWR_NAMESPACE_END(treestore)
GWR_NAMESPACE_END(libgwr)

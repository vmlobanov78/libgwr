/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-treestore-custom.h                                               *
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
    *   Purpose     : Custom methods                                            *
    *                                                                           *
    *****************************************************************************
*/

//..............................................................................
#include	"libgwr-treestore.hh"
//..............................................................................

GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(treestore)

//=============================================================================

  /**
	*   Store::Iter_is_valid:
	*
	*   @iter : the iter to check
	*
	**/

//=============================================================================
gboolean
Store::iter_is_valid(
	GtkTreeIter *iter)
{
	g_return_val_if_fail( iter->stamp == stamp(), FALSE );

	return TRUE;
}
//=============================================================================

  /**
	*   Store::Iter_is_valid_but_may_be_null:
	*
	*   @iter : the iter to check
	*
	**/

//=============================================================================
gboolean
Store::iter_is_valid_but_may_be_null(
	GtkTreeIter *iter)
{
	if ( !iter )
		return TRUE;

	return iter_is_valid(iter);
}
//=============================================================================

  /**
	*   Store::emit_row_inserted:
	*
	*   @iter : the iter inserted
	*
	**/

//=============================================================================
void
Store::emit_row_inserted(
Node* node)
{
	GtkTreePath					*path = NULL;
	GtkTreeIter					iter;
	//.........................................................................
	g_assert( node );

	ITER_FROM_NODE(this, &iter, node);

	path = get_path(GTK_TREE_MODEL(this), &iter);

	gtk_tree_model_row_inserted(
		GTK_TREE_MODEL(this),
		path,
		&iter);

	gtk_tree_path_free(path);
}

//=============================================================================

  /**
	*   Store::emit_row_changed:
	*
	*   @iter : the iter changed
	*
	**/

//=============================================================================
void
Store::emit_row_changed(
Node* node)
{
	GtkTreePath					*path = NULL;
	GtkTreeIter					iter;
	//.........................................................................
	g_assert( node );

	ITER_FROM_NODE(this, &iter, node);

	path = get_path(GTK_TREE_MODEL(this), &iter);

	gtk_tree_model_row_changed(
		GTK_TREE_MODEL(this),
		path,
		&iter);

	gtk_tree_path_free(path);
}

//=============================================================================

  /**
	*   Store::emit_row_deleted:
	*
	*   @iter : the iter deleted
	*
	**/

//=============================================================================
void
Store::emit_row_deleted(
Node* node)
{
	GtkTreePath					*path = NULL;
	GtkTreeIter					iter;
	//.........................................................................
	g_assert( node );

	ITER_FROM_NODE(this, &iter, node);

	path = get_path(GTK_TREE_MODEL(this), &iter);

	gtk_tree_model_row_deleted(
		GTK_TREE_MODEL(this),
		path);

	gtk_tree_path_free(path);
}

//=============================================================================

  /**
	*   Store::emit_row_has_child_toggled:
	*
	*   @iter : the iter that has
	*			- its first child
	*			  or
	*			- no more children
	*
	**/

//=============================================================================
void
Store::emit_row_has_child_toggled(
Node* node)
{
	GtkTreePath					*path = NULL;
	GtkTreeIter					iter;
	//.........................................................................
	g_assert( node );

	ITER_FROM_NODE(this, &iter, node);

	path = get_path(GTK_TREE_MODEL(this), &iter);

	gtk_tree_model_row_has_child_toggled(
		GTK_TREE_MODEL(this),
		path,
		&iter);

	gtk_tree_path_free(path);
}


//=============================================================================

  /**
	*   Store::iter_get_uid:
	*
	*   @iter : the iter to get the uid from
	*
	**/

//=============================================================================
guint32
Store::iter_get_uid(
	GtkTreeIter *_in_iter)
{
	Node				*node	= NULL;
	//.........................................................................
	g_return_val_if_fail( iter_is_valid(_in_iter), 0 );

	NODE_FROM_ITER(node, _in_iter);

	return node->uid();
}

//=============================================================================

  /**
	*   Store::ext_dump
	*
	*   Dump the treestore, for debugging purpose
	*
	**/

//=============================================================================
void
Store::ext_dump_tree(
    GtkTreeIter *   _iter)
{
    Node *   node    = NULL;
    //.........................................................................
    if ( ! _iter )
    {
        node = node_root();
    }
    else
    {
        NODE_FROM_ITER(node, _iter);
    }
    //NODE_TKI("  +-------------------------------------------+");
    //                                   [12345678]
    //NODE_TKI("  | Dumping Tree from node [%08x]         |", node);
    //NODE_TKI("  +-------------------------------------------+");

    node->dump_tree();

    //NODE_TKI("   ");
    //NODE_INF("  +-------------------------------------------+");
    //NODE_TKI("  Dumped !");
}

//=============================================================================

  /**
	*   Store::ext_add_child:
	*
	*	@in_parent  : the iter that will have a new child
	*	@out_child  : the child that will be created
	*	@data		: user's data for the child
	*
	*   Add a child, respect to sorting
	*
	*   Special case : if in_parent is NULL, set the toplevel node
	*
	**/

//=============================================================================
void
Store::ext_add_child(
	GtkTreeIter		*   _in_parent,
	GtkTreeIter		*   _out_child,
	IData           *   _data_child)
{
	Node				*n_parent	= NULL;
	Node				*n_child	= NULL;
	//.........................................................................
	g_return_if_fail( iter_is_valid_but_may_be_null( _in_parent ) );

	if ( ! _in_parent )                                                         // if no parent, take node_root
	{
		g_return_if_fail( node_root() );

		n_parent = node_root();
	}
	else
	{
		NODE_FROM_ITER(n_parent, _in_parent );
		g_return_if_fail( n_parent );
	}

	n_child = n_parent->children()->node_add(
        uid_new()                   ,
        a_sort_type                 ,
        a_sort_collate_key_to_use   ,
        _data_child);

	ITER_FROM_NODE(this, _out_child, n_child);

	_data_child->set_path_from_treestore( ext_path_from_iter(_out_child) );

    if ( n_child->shown() )                                                     //  only emit signal if node is shown
        emit_row_inserted( n_child );
}

//=============================================================================

  /**
	*   Store::ext_set_data:
	*
	*   @iter : set Data for an iter.
	*
	**/

//=============================================================================
void
Store::ext_set_data(
	GtkTreeIter		*   _iter_in,
	IData	        *   _data)
{
	Node				*   node		= NULL;
	IData		*	data_old	= NULL;
	IData		*   data_new	= NULL;
	//.........................................................................
	g_return_if_fail ( iter_is_valid(_iter_in) );

	NODE_FROM_ITER(node, _iter_in);
	g_assert(node);

	data_old = node->data();
	g_return_if_fail( data_old );

	data_new = _data;
	g_return_if_fail( data_new );

	// We set the Path for the new Data with a copy of the old Data's Path
	data_new->set_path_from_treestore( data_old->path()->dup() );

	// Then we delete the old Data, that will delete old Path too
	delete data_old;

	// Then we set the new Data as iter Data
	node->data() = data_new;

	emit_row_changed( node );
}

//=============================================================================

  /**
	*   Store::ext_data_changed:
	*
	*   @iter : set Data for an iter.
	*
	*   Tell the treestore that an iter's data has changed. So emit the
	*   row_changed signal.
	*
	**/

//=============================================================================
void
Store::ext_data_changed(
	GtkTreeIter		*   _iter_in)
{
	Node				*   node		= NULL;
	//.........................................................................
	g_return_if_fail ( iter_is_valid(_iter_in) );

	NODE_FROM_ITER(node, _iter_in);
	g_assert(node);

	emit_row_changed( node );
}

//=============================================================================

  /**
	*   Store::ext_get_data:
	*
	*   @iter : get Data for an iter.
	*
	**/

//=============================================================================
gboolean
Store::ext_get_data(
	GtkTreeIter		*		_iter_in,
	IData	**		_data_out)
{
	Node				*   node		= NULL;
	IData		*	data_old	= NULL;
	//.........................................................................
	g_return_val_if_fail ( _data_out != NULL, FALSE );
	*_data_out = NULL;

	g_return_val_if_fail ( iter_is_valid(_iter_in), FALSE );

	NODE_FROM_ITER(node, _iter_in);
	g_assert(node);

	data_old = node->data();
	g_return_val_if_fail( data_old, FALSE );

	*_data_out = data_old;

	return ( data_old != NULL );
}

//=============================================================================

  /**
	*   Store::ext_clear:
	*
	*   Clear * everything *
	*
	**/

//=============================================================================
gint
Store::ext_clear()
{
	gint count = 0;
	//.........................................................................
	g_return_val_if_fail( d_node_root, 0 );

	// We should code a quicker mehod                                           // _GWR_TODO_
	count = d_node_root->remove_children();

	// delete node_root
	delete d_node_root->children();
	delete d_node_root;

	// Set pointer to NULL
	d_node_root = NULL;
    d_data_root = NULL;

	//LEAKS_INF("Store::ext_clear()");
	//LEAKS_INF("  remaining nodes  : %04i %s", Node::Remaining(),		Node::Remaining() ?         "- not empty -" : "EMPTY !");
	//LEAKS_INF("  remaining blocks : %04i %s", NodeBlock::Remaining(),   NodeBlock::Remaining() ?    "- not empty -" : "EMPTY !");

	return count;
}


//=============================================================================

  /**
	*   Store::ext_get_root:
	*
	*   Retrives the root node of a given node
	*
	**/

//=============================================================================
/*
gboolean
Store::ext_get_root(
	GtkTreeIter *in,
	GtkTreeIter *out_root)
{
	Node				*node		= NULL;
	//.........................................................................
	g_return_val_if_fail ( iter_is_valid(in), FALSE );
	g_return_val_if_fail ( out_root, FALSE );

	NODE_FROM_ITER(node, in);
	g_return_val_if_fail( node, FALSE );

	// All this code for this simple loop :)
	while ( node->parent() != node_root() )
		node = node->parent();

	ITER_FROM_NODE(this, out_root, node);
	return TRUE;
}
*/
//=============================================================================

  /**
	*   Store::ext_is_root:
	*
	*   Tell wether the iter is a root iter
	*
	**/

//=============================================================================
gboolean
Store::ext_is_root(
	GtkTreeIter *in)
{
	Node				*node		= NULL;
	//.........................................................................
	g_return_val_if_fail ( iter_is_valid(in), FALSE );

	NODE_FROM_ITER(node, in);
	g_return_val_if_fail( node, FALSE );

	return node->flags_get_rtnode();
}

//=============================================================================

  /**
	*   Store::ext_iter_depth:
	*
	*   Return the depth of an iter
	*
	**/

//=============================================================================
gboolean
Store::ext_iter_depth(
	GtkTreeIter *in)
{
	Node				*node		= NULL;
	//.........................................................................
	g_return_val_if_fail ( iter_is_valid(in), FALSE );

	NODE_FROM_ITER(node, in);
	g_return_val_if_fail( node, FALSE );

	return node->depth();
}

//=============================================================================

  /**
	*   Store::ext_get_gtk_path_str_new:
	*
	*   @in : a GtkTreeIter
	*
	*   Return the string path for an iter ( GtkTreePath style )
	*
	**/

//=============================================================================
gchar*
Store::ext_get_gtk_path_str_new(
	GtkTreeIter *in)
{
	gint										i			= 0;
	GArray										*a			= NULL;
	gint										pos			= 0;
	gchar										 ascii_tmp1 [8];
	gchar										 ascii_tmp2 [1024];
	Node				*node		= NULL;
	//.........................................................................
	g_return_val_if_fail ( iter_is_valid(in), NULL );

	NODE_FROM_ITER(node, in);
	g_return_val_if_fail( node, NULL );

	a = g_array_sized_new(FALSE, TRUE, sizeof(gint), 10);

	do
	{
		pos = node->pos();
		g_array_append_vals(a, &pos, 1);

		node = node->parent();
		i++;
	}
	while ( node != node_root() );

	ascii_tmp2[0] = 0;

	//STORE_INF("get_path_str_new:depth %02i", i);

	if ( i > 1 )
		g_sprintf( ascii_tmp2, "%i:", g_array_index(a, gint, --i) );

	while ( i > 1 )
	{
		g_sprintf( ascii_tmp1, "%i:", g_array_index(a, gint, --i) );
		g_strlcat(ascii_tmp2, ascii_tmp1, 1024);
	}

	g_sprintf( ascii_tmp1, "%i", g_array_index(a, gint, 0) );
	g_strlcat(ascii_tmp2, ascii_tmp1, 1024);

	g_array_free(a, TRUE);

	////STORE_INF("get_path_str_new:%s", ascii_tmp2);

	return g_strdup(ascii_tmp2);
}

//=============================================================================

  /**
	*   Store::ext_iter_get_row:
	*
	*   @in : a GtkTreeIter
	*
	*   Get the row of an iter
	*
	**/

//=============================================================================
gint Store::ext_iter_get_row(
	GtkTreeIter *in)
{
	Node				*node		= NULL;
	//.........................................................................
	g_return_val_if_fail ( iter_is_valid(in), 0 );

	NODE_FROM_ITER(node, in);
	g_return_val_if_fail( node, 0 );

	return node->row();
}

//=============================================================================

  /**
	*   Store::ext_iter_remove_children_no_signal_row_deleted:
	*
	*   @in : a GtkTreeIter
	*
	*   Remove all children of iter, but dont send any row_deleted signal.
	*   Only send row_toggled if parent
	*   Call only this method if an iter is collapsed !
	*
	**/

//=============================================================================
gint
Store::ext_iter_remove_children_no_signal_row_deleted(
	GtkTreeIter *in)
{
    gint            count       = 0;
	Node		*   node		= NULL;
	//.........................................................................
    g_return_val_if_fail ( iter_is_valid(in), 0 );

    NODE_FROM_ITER(node, in);
    g_return_val_if_fail( node, 0 );

    // Create branch from the node
    //Branch branch(node);
    //g_return_val_if_fail( branch.is_valid(), -1 );

    //count = branch.cut_but_keep_root();

	// if a_root is now sterile, send child_toggled signal ( if parent != root )
	//if ( branch.root()->is_sterile() )
	//	if ( branch.root() != node_root() )
	//		emit_row_has_child_toggled(branch.root());

    //return count;

    //   Previous method, without the Branch concept

    	g_return_val_if_fail ( iter_is_valid(in), 0 );

    	NODE_FROM_ITER(node, in);
    	g_return_val_if_fail( node, 0 );

    	count = node->remove_children();

    	// if parent is now sterile, send child_toggled signal ( if parent != root )
    	if ( ! node->children()->card() )
    		if ( node->parent() != node_root() )
    			emit_row_has_child_toggled(node);

    	return count;
}

//=============================================================================

  /**
	*   Store::ext_iter_sterile_remove:
	*
	*   @in : a GtkTreeIter with no children
	*
	*   Remove the iter & send a row_deleted signal
	*
	**/

//=============================================================================
gint
Store::ext_iter_sterile_remove(
	GtkTreeIter *in)
{
    //gint            count       = 0;
    Node		*   node		= NULL;
    NodeBlock   *   block       = NULL;
	//.........................................................................
	g_return_val_if_fail ( iter_is_valid(in), FALSE );

	NODE_FROM_ITER(node, in);
	g_return_val_if_fail( node, FALSE );
	g_return_val_if_fail( node->sterile(), FALSE );

    // children block where node resides
    block = node->parent()->children();

    // *IMPORTANT* send signal before removing, according to GtkTreeModel doc
    emit_row_deleted(node);

    // remove the node
    block->remove_one_node( node->pos() );

    // if parent is now sterile, send child_toggled signal ( if parent != root )
    if ( ! block->card() )
        if ( block->parent() != node_root() )
            emit_row_has_child_toggled(block->parent());


    return -1;
}

//=============================================================================

  /**
	*   Store::ext_iter_remove:
	*
	*   @in : a GtkTreeIter
	*
	*   Remove the iter and emit all appropriate signals.
	*
	**/

//=============================================================================
/*
guint32
Store::ext_iter_remove(
	GtkTreeIter *   _in)
{
    //gint            count       = 0;
    Node		*   node		= NULL;
    NodeBlock   *   block       = NULL;
	//.........................................................................
	g_return_val_if_fail ( iter_is_valid(_in), FALSE );

	NODE_FROM_ITER(node, _in);
	g_return_val_if_fail( node, FALSE );

    block = node->parent()->children();                                         //  children

    // *IMPORTANT* send signal before removing, according to GtkTreeModel doc
    emit_row_deleted(node);

    // remove the node
    block->remove_one_node( node->pos() );

    // if parent is now sterile, send child_toggled signal ( if parent != root )
    if ( ! block->card() )
        if ( block->parent() != node_root() )
            emit_row_has_child_toggled(block->parent());

    return 0;
}
*/
//=============================================================================

  /**
	*   Store::ext_iter_from_path
	*
	*   @_path		: a Store::Path
	*   @_iter_out  : a GtkTreeIter
	*
	*   Retrieves an iter from a Path. _WARNING_ this is the only method that
    *   can access a hidden Node, dont forget to test if Node is visible
    *   before calling GtkTreeView on it !!!
	*
	**/

//=============================================================================
gboolean
Store::ext_iter_from_path(
	Path			*   _path,
	GtkTreeIter		*   _iter_out)
{
    Node    *   node    = NULL;
	//.........................................................................
    ITER_CLR( _iter_out );
	g_return_val_if_fail( _path,        FALSE );
    g_return_val_if_fail( node_root(),  FALSE );

    node = node_root()->children()->find_sub_node(_path);

    g_return_val_if_fail( node, FALSE );

	ITER_FROM_NODE(this, _iter_out, node);
    return TRUE;
}

//=============================================================================

  /**
	*   Store::ext_path_from_iter
	*
	*   @_path		: a Store::Path
	*   @_iter_out  : a GtkTreeIter
	*
	*   Return a mallocated Path from an iter
	*
	**/

//=============================================================================
Path*
Store::ext_path_from_iter(
	GtkTreeIter		*   _iter_in)
{
	Node			*   node		= NULL;
	gint										depth		= 0;
	gint										i			= 1;
	Path									*   path		= NULL;
	//.........................................................................
	g_return_val_if_fail( _iter_in,					FALSE );
	g_return_val_if_fail( iter_is_valid(_iter_in),  FALSE );

	NODE_FROM_ITER(node, _iter_in);
	g_return_val_if_fail( node,	FALSE );

	// build & fill a reverse path
	depth		= node->depth();
	path		= GWR_NEW_CAST( Path, depth );

	do
	{
		if ( node == node_root() )
		{
			//NODE_ERR("Store::ext_path_from_iter():node root reached !");
			delete path;
			return NULL;
		}

		path->uid_set( depth - i, node->uid() );

		node = node->parent();
	}
	while ( (i++) != depth );

	return path;
}


//*****************************************************************************
//*																			  *
//*							Match functions									  *
//*																			  *
//*****************************************************************************


//=============================================================================

  /**
	*   Store::ext_match_child_gint:
	*
	*   \param [in]  in_parent          a GtkTreeIter
	*   \param [out] out_child			a GtkTreeIter
	*   \param [in]  match_function     user-defined matching function
	*   \param       the_gint			the gint to match
	*
	*   For all children of <b>in_parent</b>, call match_function(child->data, the_gint).
	*
	*   - If <b>in_parent</b> is NULL, test all root nodes
	*
	*   - If <b>match_function</b> returns TRUE for a child iter, the function fills in
    *   <b>out_child</b> with that iter and return TRUE.
	*
	*   - If <b>match_function</b> return FALSE for all children of <b>in_parent</b>, return FALSE.
	*
	**/

//=============================================================================
gboolean
Store::ext_match_child_gint(
	GtkTreeIter								*   in_parent   ,
	GtkTreeIter								*   out_child   ,
	gboolean(*match_function)(IData*, gint)					,
	gint										the_gint)
{
	gint				i			= 0;
	Node			*   node		= NULL;
	NodeBlock		*   block		= NULL;
	//.........................................................................
	g_return_val_if_fail ( iter_is_valid_but_may_be_null(in_parent), FALSE );

	if ( !in_parent )
		node = node_root();
	else
		NODE_FROM_ITER(node, in_parent);

	g_return_val_if_fail( node, FALSE );

	block   = node->children();
	g_return_val_if_fail( block->card() != 0, FALSE );

	while ( i < block->card() )
	{
		node = block->node_get(i++);
		if ( match_function(node->data(), the_gint ) )
		{
			ITER_FROM_NODE(this, out_child, node);
			return TRUE;
		}
	}

	return FALSE;
}

//=============================================================================

  /**
	*   Store::ext_match_child_collate_key:
	*
	*   in_parent								: a GtkTreeIter
	*   out_child								: a GtkTreeIter
	*   collate_key_id							: a gint identifying
	*											  the collate key to use
	*											  for the childs
	*   utf8_collate_key						: a collate key
	*
	*   For all childs of in_parent, compare child->collatekey &
	*   collate_key_to_use.
	*
	*   If parent is NULL, test root nodes
	*
	*   If it collate for a child, the function fills in out_child with
	*   this child and return TRUE.
	*
	*   If no collate for all childs, return FALSE.
	*
	**/

//=============================================================================
gboolean
Store::ext_match_child_str(
	GtkTreeIter								*   in_parent   ,
	GtkTreeIter								*   out_child   ,
	const gchar								*   _str)
{
	gint										i			= 0;
	Node				*node		= NULL;
	NodeBlock		*block		= NULL;
	//.........................................................................
	g_return_val_if_fail ( iter_is_valid_but_may_be_null(in_parent), FALSE );

	if ( !in_parent )
		node = node_root();
	else
		NODE_FROM_ITER(node, in_parent);

	g_return_val_if_fail( node, FALSE );

	block   = node->children();
	g_return_val_if_fail( block->card() != 0, FALSE );

	while ( i < block->card() )
	{
		node = block->node_get(i++);
        //  Previous method, with direct access to collate keys
        //  from IData
        //
		//if (	strcmp
        //        (
		//			node->data()->utf8_collate_key(collate_key_id),
		//			utf8_collate_key
        //       ) == 0 )
        if ( node->data()->compare_str(_str) == 0 )
		{
			ITER_FROM_NODE(this, out_child, node);
			return TRUE;
		}
	}

	return FALSE;
}

//  ----------------------------------------------------------------------------
//  Store::ext_iter_is_shown()
//  ----------------------------------------------------------------------------
//! \fn Store::ext_iter_is_shown()
//!
//! \brief  Tell is an iter is presented to the GtkTreeModel
gboolean
Store::ext_iter_is_shown(GtkTreeIter    *   _in)
{
    Node    *   node = NULL;
    //  ........................................................................
    NODE_FROM_ITER(node, _in);
    g_return_val_if_fail( node, FALSE );

    return node->shown();
}
//  ----------------------------------------------------------------------------
//  Store::ext_iter_is_hidden()
//  ----------------------------------------------------------------------------
//! \fn Store::ext_iter_is_hidden()
//!
//! \brief  Tell is an iter is hidden to the GtkTreeModel
gboolean
Store::ext_iter_is_hidden(GtkTreeIter    *   _in)
{
    Node    *   node = NULL;
    //  ........................................................................
    NODE_FROM_ITER(node, _in);
    g_return_val_if_fail( node, FALSE );

    return node->hidden();
}


//  ############################################################################
//  ############################################################################

//  ----------------------------------------------------------------------------
//  Store::ext_iter_visibility_changed()
//  ----------------------------------------------------------------------------
//! \fn Store::ext_iter_visibility_changed()
//!
//! \brief  Refresh display of a Node and of its descendance when its visibility
//!     has changed.
//!
//!     __RECURSIVE__   ( Not by itself, but call recursive functions ).
//!
//! \param  _ret    [out] : guint32_x2 containing the numbers of Nodes whose
//!                         display changed :
//!                             _ret->u1 = Nodes H -> S
//!                             _ret->u1 = Nodes S -> H
//!
//! \return FALSE on error, especially when iter visibility did not change ; since
//!     this function uses recursive methods, please _ALWAYS_ verify that the iter
//!     visibility was not already changed by a previous recursive call :
//!
//!            -> call on 1H ->             -> call on 3S : error
//!     1H                          1S
//!     |                           |
//!     +-- 2H                      +-- 2S
//!     |                           |
//!     +-- 3H                      +-- >3S
gboolean
Store::ext_iter_visibility_changed(
    GtkTreeIter *   _in ,
    guint32_x2  *   _ret)
{
    Node        *   node    = NULL;
    //  ........................................................................
    NODE_FROM_ITER(node, _in);
    g_return_val_if_fail( node, FALSE );

    Set_static_store(this);                                                     //  init

    node->visibility_changed();                                                 //  call

    _ret->u1 = s_ret_guint32_x2.u1;                                             //  memorize # of modified Nodes
    _ret->u2 = s_ret_guint32_x2.u2;

    return s_ret_gboolean;                                                      //  return global result
}


GWR_NAMESPACE_END(treestore)
GWR_NAMESPACE_END(libgwr)

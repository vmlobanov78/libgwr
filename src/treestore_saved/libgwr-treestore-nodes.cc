/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-treestore-nodes.h                                                *
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
    *   Purpose     : Core structs Node NodeBlock                               *
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
//  #								NODES 							 	      #
//  ##																		 ##
//  ###																		###
//  ###########################################################################


//=============================================================================

  /**
	*   Node::new()
	*
	**/

//=============================================================================
void*	Node::operator new(
	size_t  size)
{
	Node	*n = g_try_new0(Node, 1);

	return n;
}

//=============================================================================

  /**
	*   Node::delete()
	*
	**/

//=============================================================================
void
Node::operator delete(
void *p)
{
	g_free(p);
}

//=============================================================================

  /**
	*   Node::node()
	*
	*   @_depth  : Depth of the node ( = same as depth in GtkTreePath )
	*   @_pos    : Pos ( starting from 0 ) of the node in the block
	*			   it belongs to
	*   @_parent : Parent node of this node
	*   @_data   : User's data
	*
	**/

//=============================================================================
Node::Node(
	guint32					_uid,
	gint					_depth, // only for the new node_block
	gint					_pos,
	Node				*   _parent,
	DataInterface		*   _data)
{
	a_bits			=   0;
	a_bits			+=  ( (guint32)( _uid                           << e_UID_SHIFT)      ) & e_UID_BITS;
	a_pos			=   _pos;
	a_parent	    =   _parent;
	a_next			=   NULL;

	a_children		=   new NodeBlock(_depth + 1, this);

	d_data			=   _data;

	//Count++;
	//NODE_INF("NOD+(%04i):[%s]",Count, log());
}

//=============================================================================

  /**
	*   Node::~Node()
	*
	*   Here we only delete the memory allowed for the struct itself.
    *   We do _NOT_ free the node_block containing the children.
	*   For recursive delete, use remove()
	*
	**/

//=============================================================================
Node::~Node()
{
	//NODE_INF("NOD-(%04i):[%s]", (Count - 1), log());

    delete data();

	//Count--;
}



//=============================================================================

  /**
	*   Node:remove_children()
	*
	*   __RECURSIVE__ remove the descendance of the node.
	*
	**/

//=============================================================================
gint
Node::remove_children()
{
    return children()->remove_nodes();
}

//=============================================================================

  /**
	*   Node:remove_child()
	*
	*   __RECURSIVE__ remove a child and all its descendance.
    *
	**/

//=============================================================================
gint
Node::remove_child(
    gint    _pos)
{
	gint									count   = 0;
	//.........................................................................

    // remove children nodes
	count += children()->remove_node(_pos);

	return count;
}
//=============================================================================

  /**
	*   Node::row
	*
	*   __RECURSIVE__
	*   Get a node's row index in the tree
	*
	**/

//=============================================================================
gint
Node::row()
{
	if ( depth() != 1 )
	{
		return ( pos() + 1 ) + parent()->row();
	}

	return pos();
}

//=============================================================================
//
//  node : Logging
//
//=============================================================================
const gchar* Node::log()
{
	static gchar Node_str_01[1024];

	//  format : "node : ref_count pos parent next children data"
	sprintf(Node_str_01, "this:%08x dep:%03i pos:%03i nxt:%s chd:%03i dat:%s",
        this,
		a_parent ? a_parent->children()->depth() : 1,
		a_pos,
		a_next ? "Y" : "N",
		a_children->card(),
		d_data ? "Y" : "N" );

	return Node_str_01;
}

void Node::dump_tree(
    gint    _level)
{
    NodeBlock * block   = NULL;
    Node *       child   = NULL;
    gint                                    card    = 0;
    gint                                    depth   = 0;

    gint                                i       = 0;
    gchar                               sp      [256];
    gchar                               s1      [256];
    gchar                               s2      [256];
    //.........................................................................
    //
    //  0123456789
    //  [xxxxxxxx]                      0   8
    //      |                           4   1
    //      +-----[xxxxxxxx]            4   10
    //                |
    //                +-----[xxxxxxxx]
    //

    //.........................................................................
    //
    //  some vars
    //
    block   = children();
    card    = block->card();
    depth   = block->depth();

    //.........................................................................
    //
    //  this
    //
    if ( _level  == 0 )
    {
        strcpy(s1, "     ");
        strcpy(s2, "     ");
    }
    if ( _level  == 1 )
    {
        strcpy(s1, "         |");
        strcpy(s2, "         +-----");
    }
    if ( _level >= 2 )
    {
        strcpy(sp, "     ");
        for ( i = 2 ; i <= _level ; i++ )
        {
            //          0123456789
            strcat(sp, "          ");
        }

        strcpy(s1, sp);
        strcpy(s2, sp);
        strcat(s1, "    |");
        strcat(s2, "    +-----");
    }

    //NODE_TKI("%s", s1);
    //NODE_TKI("%s[%08x]", s2, this);

    //.........................................................................
    //
    //  children
    //
    for ( i = 0 ; i != card ; i++ )
    {
        child = g_array_index(block->array(), Node*, i);

        child->dump_tree(1 + _level);
    }
}

//  ###########################################################################
//  ###																		###
//  ##																		 ##
//  #							 NODE BLOCKS								  #
//  ##																		 ##
//  ###																		###
//  ###########################################################################
gint NodeBlock::Count   = 0;

//=============================================================================

  /**
	*   NodeBlock::new:
	*
	**/

//=============================================================================
void*   NodeBlock::operator new(
	size_t								size)
{
	NodeBlock	*nb = g_try_new0(NodeBlock, 1);

	return nb;
}

//=============================================================================

  /**
	*   NodeBlock::delete:
	*
	**/

//=============================================================================
void	NodeBlock::operator delete (void *p)
{
	g_free(p);
}

//=============================================================================

  /**
	*   NodeBlock::NodeBlock()
	*
	*   @_depth  : The depth of the block ( = same as depth in GtkTreePath )
	*   @_parent : The parent node
	*
	**/

//=============================================================================
NodeBlock::NodeBlock(
	guint									_depth,
	Node		*   _parent)
{
	a_card		= 0;
	a_depth		= _depth;
	a_parent	= _parent;

	d_nodes		= g_array_sized_new(
		FALSE,					// zero_terminated element appended at the end
		TRUE,					// all bits set to zero
		sizeof(Node*),			// element_size,
		10);					//reserved_size);

	// Fuck, Fuck, Fuck !!!
	// Spended hours on this, g_array_sized_new( ...set bits to 0 )
	// doesnt fucking work !!!
	// printf("GArray 0x%08x d:%03i p:0x%08x [0]=0x%08x\n", nb->d_nodes, nb->a_depth, nb->a_parent, g_array_index(nb->d_nodes, Node*, 0));

	Count++;
	//BLOCK_INF("BLK+(%04i):d %03i", Count, a_depth);
}

//=============================================================================

  /**
	*   NodeBlock::~node_block()
	*
	*   Here we only delete the array of children ; it does _NOT_ affect the
	*   children objects.
	*   For recursive delete, use purge()
	*
	**/

//=============================================================================
NodeBlock::~NodeBlock()
{
	//BLOCK_INF("BLK-(%04i):d %03i c:%03i", Count - 1, a_depth, a_card);

	g_array_free( d_nodes, TRUE );

	Count--;
}



//=============================================================================

  /**
	*   NodeBlock::reset()
	*
	*   Set the number of children to zero, redim the GArray
	*
	**/

//=============================================================================
void
NodeBlock::reset()
{
    if ( ! a_card )
        return;

	g_array_remove_range(d_nodes, 0, a_card);
	a_card = 0;
}

//=============================================================================

  /**
	*   NodeBlock::node_get:
	*
	*   @pos : Position ( starting from zero ) of the node to get.
	*
	**/

//=============================================================================
Node*
NodeBlock::node_get(
	gint pos)
{
	Node			*node   = NULL;
	//.........................................................................
	if ( a_card == 0 )
	{
		// gtk+ calls us with bad indexes !!! ( when scrooling liftbars )
		// g_return_val_if_fail( index == 0, NULL );
		if ( pos != 0 )
			return NULL;

		//BLOCK_INF("BLK(%-20s) d:%03i p:%03i c:%03i [NULL]", "node_get", a_depth, index, a_card);
		// this is authorized, since gtk call us for 0th child !!!
		return NULL;
	}
	else
	{
		// gtk+ calls us with bad indexes !!! ( when scrooling liftbars )
		// g_return_val_if_fail( index < a_card, NULL );
		if ( pos >= a_card )
			return NULL;
	}

	node = g_array_index(d_nodes, Node*, pos);
	//BLOCK_INF("BLK(%-20s) d:%03i p:%03i c:%03i [%s]", "node_get", a_depth, pos, a_card, node->log());
	return node;
}

//=============================================================================

  /**
	*   NodeBlock::node_append:
	*
	*   @data : user's data
	*
	*   Insert a node in the block. This method respect the ordering settings
	*   of the store.
	*
    **/

//=============================================================================
Node*
NodeBlock::node_add(
	guint32				_uid,
	eSortType			_sort_type,
	gint				_collate_key_to_use,
	DataInterface	*   _data)
{
	Node *node   = NULL;
	Node *temp   = NULL;
	gint							i		= 0;
	//.........................................................................

	// create a new node with pos = 0 :
	// we cant set the position now, because we dont know at which position
	// we will be stored
	node = new Node(_uid, a_depth, 0, a_parent, _data);

    // eSortNone is 0x00
	if ( ! _sort_type  )
	{
		goto generic_append;
	}

	if ( _sort_type == eSortDescending )
	{
		temp = g_array_index(d_nodes, Node*, 0);
loop_acy:
		// Fuck, Fuck, Fuck !!!
		// Spended hours on this, g_array_sized_new( ...set bits to 0 )
		// doesnt fucking work ( in glib 2.16.6 ) !!!
		if ( i >= a_card )
			goto generic_append;

        //  Previous method, with direct access to collate keys
        //  from DataInterface
        //
        //		if ( strcmp (
        //				node->data()->utf8_collate_key(_collate_key_to_use),
        //				temp->data()->utf8_collate_key(_collate_key_to_use)
        //					) >= 0 )
        //
        if ( node->data()->compare(temp->data()) >= 0 )
            goto generic_insert;

		temp = temp->next();
		i++;
		goto loop_acy;
	}

	if ( _sort_type == eSortAscending )
	{
		temp = g_array_index(d_nodes, Node*, 0);
loop_dcy:
		// Fuck, Fuck, Fuck !!!
		// Spended hours on this, g_array_sized_new( ...set bits to 0 )
		// doesnt fucking work ( in glib 2.16.6 ) !!!
		if ( i >= a_card )
			goto generic_append;

        //  Previous method, with direct access to collate keys
        //  from DataInterface
        //
		//if ( strcmp (
		//		node->data()->utf8_collate_key(_collate_key_to_use),
		//		temp->data()->utf8_collate_key(_collate_key_to_use)
		//			) <= 0 )
        //
        if ( node->data()->compare(temp->data()) <= 0 )
			goto generic_insert;

		temp = temp->next(); i++;
		goto loop_dcy;
	}

	// Bad sort type
	g_assert(FALSE);

	//.........................................................................
	//
	// 'Append' back-end : We have to append 'node' at the end of the array
	//
	// append to the end of the array
generic_append:
	// Now we know the node's pos, it is a_card - 1 + 1 = a_card
	node->set_pos(a_card);

	// Note : I have never seen d_nodes change in append case
	// but I still reaffect the GArray, cf GLib documentation
	d_nodes = g_array_append_val(d_nodes, node);

	// modify the previous node so its ->next field points to the newly
	// created node ; do that only if we didnt create the first node.
	if ( a_card != 0 )
	{
		temp = g_array_index(d_nodes, Node*, a_card - 1);
		temp->set_next(node);
	}

	a_card++;
	return node;

	//.........................................................................
	//
	// 'Insert' back-end : We have to insert 'node' at pos i, instead of 'temp'
	//
generic_insert:
	// Now we know our pos, it is i
	node->set_pos(i);

	// I reaffect the GArray, cf GLib documentation, but I didnt dig this case
	d_nodes = g_array_insert_val(d_nodes, i, node);

	// modify the previous node so its ->next field points to 'node' ( at pos i )
	// do that only if we didnt create the first node.
	if ( i != 0 )
		g_array_index(d_nodes, Node*, i - 1)->set_next(node);

	// modify 'node' so its ->next field points to the node at pos i + 1
	// here we have collated, so we are sure that we have taken the place
	// of a node, that is now just after us
	node->set_next( g_array_index(d_nodes, Node*, i + 1) );

	// increase by 1 all ->pos fields of nodes following 'node'
	temp = node->next();
	g_assert(temp);
	while ( temp )
	{
		temp->inc_pos();
		temp = temp->next();
	}

	a_card++;
	return node;
}

//=============================================================================

  /**
	*   NodeBlock::remove_node():
	*
    *   @param node : the node to remove
	*
	*   __RECURSIVE__
	*   Remove the node and all its children. Update block info.
	*
	**/

//=============================================================================
gint
NodeBlock::remove_node(
	gint pos)
{
	Node*		node	= NULL;
	Node*		follow	= NULL;
	gint									i		= 0;
    gint                                    count   = 0;
	//.........................................................................
	node = node_get(pos);
	g_return_val_if_fail( node, 0 );

    //BLOCK_INF("remove_node():[%s]", node->log());

	// remove the node from the array
	g_array_remove_index(d_nodes, pos);
	a_card--;

	// modify eventually the previous node's "next" field
	// ( dont call node_get, we do inline-verifications
    //  - pos = 0 => no previous node to modify
    //  - pos != a_card <=> we remove the last node ; in this case next = NULL
	if ( pos != 0 )
			g_array_index(d_nodes, Node*, pos - 1)->set_next
            (
				( pos != a_card														?
					g_array_index(d_nodes, Node*, pos)   :
					NULL )
            );

	// update all nodes "pos" field, starting from node->pos() since it just
	// has been replaced by the g_array_index call
	// ( dont call node_get, we dont need verifications )
	for ( i = pos ; i < a_card ; i ++ )
	{
		follow = g_array_index(d_nodes, Node*, i);
		follow->dec_pos();
	}

    //
    // delete the node and all its children
    //

    // remove children nodes
	count += node->remove_children();

    // delete the NodeBlock struct
	delete node->children();

    // delete the Node struct
    delete node;

    // return count
	return ( 1 + count);
}

//=============================================================================

  /**
	*   NodeBlock::remove_nodes()
	*
	*   __RECURSIVE__
	*   Remove all the nodes and all their children. Update block info.
	*
	**/

//=============================================================================
gint
NodeBlock::remove_nodes()
{
	Node*		node	= NULL;
	Node*		next	= NULL;
    gint                                    count   = 0;
	//.........................................................................

    //BLOCK_INF("remove_nodes():[%03i]", a_card);

    node = node_get(0);

    while ( node )
    {
        next    =   node->next();
        count   +=  remove_node(node->pos());
        node    =   next;
    }
	//for ( i = 0 ; i < a_card ; i ++ )
	//{
	//	node = g_array_index(d_nodes, Node*, i);
	//	count += remove_node(i);
	//}

    reset();

    return count;
}



}       // namespace treestore
}       // namespace libgwr

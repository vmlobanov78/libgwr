/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-treestore-nodes.h                                                *
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
    *   Purpose     : Core structs Node NodeBlock                               *
    *                                                                           *
    *****************************************************************************
*/

//..............................................................................
#include	"libgwr-treestore.h"
//..............................................................................

GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(treestore)

//  ###########################################################################
//  ###																		###
//  ##																		 ##
//  #								NODES 							 	      #
//  ##																		 ##
//  ###																		###
//  ###########################################################################
//  ----------------------------------------------------------------------------
//  Node::new()
//  ----------------------------------------------------------------------------
//!
//! \brief  allocator new
//!
void*	Node::operator new(
	size_t  size)
{
	Node	*n = g_try_new0(Node, 1);

	return n;
}
//  ----------------------------------------------------------------------------
//  Node::delete
//  ----------------------------------------------------------------------------
//!
//! \brief  de-allocator delete
//!
void
Node::operator delete(
void *p)
{
	g_free(p);
}
//  ----------------------------------------------------------------------------
//  PNode::PNode()
//  ----------------------------------------------------------------------------
//!
//! \brief  PNode constructor
//!
//!	\param  _uid        uid of the node
//!	\param  _depth      Depth of the node ( = same as depth in GtkTreePath )
//!	\param  _apos       Absolute pos ( starting from 0 ) of the node in the block it belongs to
//!	\param  _vpos       Visible  pos ( starting from 0 ) of the node in the block it belongs to
//!	\param  _parent     Parent node of this node
//!	\param  _data       User's data
//!
PNode::PNode(
	guint32					_uid,
	gint					_depth, // only for the new node_block
	guint16					_spos,
	guint16					_hpos,
	Node				*   _parent,
	IData		        *   _data)
{
	a_bits			=   0;

    flags_set_uid(_uid);
    flags_set_visibility( _data->IGetVisibility() );

    a_spos          =   _spos;
    a_hpos          =   _hpos;

	a_parent	    =   _parent;

	a_aprev			=   NULL;
	a_anext			=   NULL;
	a_snext			=   NULL;

	d_children		=   new NodeBlock(_depth + 1, (Node*)this);

	d_data			=   _data;

	//Count++;
	//NODE_INF("NOD+(%04i):[%s]",Count, log());
}
//  ----------------------------------------------------------------------------
//  PNode::~PNode()
//  ----------------------------------------------------------------------------
//!
//! \brief PNode destructor
//!
//! Here we only delete the memory allowed for the struct itself.
//! We do _NOT_ free the NodeBlock containing the children.
//! For recursive delete, use remove()
//!
PNode::~PNode()
{
	//NODE_INF("NOD-(%04i):[%s]", (Count - 1), log());

    delete data();

	//Count--;
}
//  ----------------------------------------------------------------------------
//  PNode::check()
//  ----------------------------------------------------------------------------
//! \fn check()
//!
//! \brief  For debug.
//!
//! Called if LIBGWR_TREESTORE_CHECK_LEVEL != 0. Dont call on NodeRoot.
gboolean
PNode::check()
{
    Node    *   p = a_parent;
    //  ........................................................................
    g_return_val_if_fail( p         , FALSE );
    g_return_val_if_fail( data()    , FALSE );
    //  ........................................................................
    //  a shown node must :
    if ( flags_get_shown() )
    {
        g_return_val_if_fail( p->shown(), FALSE );                                          //  - have a shown parent

        g_return_val_if_fail( flags_get_visibility(), FALSE );                              //  - have visibility flag set

        g_return_val_if_fail( data()->IGetVisibility(), FALSE );                            //  - have a "Visibility = TRUE" IData

        g_return_val_if_fail( p->children()->get_shown_node(spos()) == this , FALSE );      //  - have a valid spos

        g_return_val_if_fail( hpos() == 0xFFFF, FALSE );                                    //  - have a unvalid hpos
    }
    //  ........................................................................
    //  a hidden node must :
    else
    {
        g_return_val_if_fail(flags_get_visibility() == data()->IGetVisibility(), FALSE);    //  - have visibility match

        g_return_val_if_fail( spos() == 0xFFFF, FALSE );                                    //  - have a unvalid spos

        g_return_val_if_fail( p->children()->get_hidden_node(hpos()) == this , FALSE );     //  - have a valid spos
    }

    //  ........................................................................
    {
        if ( snext() == this )
        {
            _GWR_BREAK_
        }
    }

    return TRUE;
}
//  ----------------------------------------------------------------------------
//  PNode::row()
//  ----------------------------------------------------------------------------
//!
//! \fn Node:row()
//!
//! brief\  __RECURSIVE__ Get a node's row index in the tree
//!
guint32
Node::row()
{
    guint32 r = 0;
    //  ........................................................................
    g_return_val_if_fail( shown(), 0 );                                         //  not shown : error

    NODE_CHECK(this);

    r = spos() + 1;                                                             //  our pos -> row

	if ( depth() != 1 )                                                         //  if not direct child of node_root, recurse
		r += parent()->row();

	return r;
}
//  ----------------------------------------------------------------------------
//  PNode::log()
//  ----------------------------------------------------------------------------
//!
//! \fn PNode:log()
//!
//! brief\  Get all infos about a Node
//!
const gchar*
PNode::log()
{
	static gchar Node_str_01[1024];

	//  format : "node : ref_count pos parent next children data"
	sprintf(Node_str_01, "%s this:%p uid:%05i dep:%03i spos:%03i hpos:%03i aprev:%p anext:%p snext:%p schd:%03i hchd:%03i dat:%s",
        flags_get_shown()                                   ?
            ( flags_get_visibility() ? " * * " : " *   " )  :
            ( flags_get_visibility() ? "   * " : "     " )  ,
        this                                        ,
        flags_get_uid()                             ,
		a_parent ? a_parent->children()->depth() : 1,
        spos()                                      ,
        hpos()                                      ,
		aprev()                        ,
		anext()                        ,
		snext()                        ,
		children()->scard()            ,
		children()->hcard()            ,
		data() ? "Y" : "N" );

	return Node_str_01;
}
//  ----------------------------------------------------------------------------
//  PNode::dump_tree()
//  ----------------------------------------------------------------------------
//!
//! \fn PNode::dump_tree()
//!
//! brief\  Get all infos about a Node
//!
void
PNode::dump_tree(guint32 _level)
{
    NodeBlock   *       block   = NULL;
    Node        *       child   = NULL;
    gint                card    = 0;
    gint                depth   = 0;

    guint32                              i       = 0;
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
    child = block->first_node(); while ( child )
    {
        child->dump_tree(1 + _level);
        child = child->anext();
    }
}
//  ###########################################################################
//  ###																		###
//  ##																		 ##
//  #							 NODE BLOCKS								  #
//  ##																		 ##
//  ###																		###
//  ###########################################################################
guint32 PNodeBlock::Count   = 0;

//  ----------------------------------------------------------------------------
//  NodeBlock::new
//  ----------------------------------------------------------------------------
//!
//! \fn NodeBlock::new()
//!
void*   NodeBlock::operator new(
	size_t								size)
{
	NodeBlock	*nb = g_try_new0(NodeBlock, 1);

	return nb;
}
//  ----------------------------------------------------------------------------
//  NodeBlock::delete
//  ----------------------------------------------------------------------------
//!
//! \fn NodeBlock::delete()
//!
void	NodeBlock::operator delete (void *p)
{
	g_free(p);
}
//  ----------------------------------------------------------------------------
//  PNodeBlock::PNodeBlock()
//  ----------------------------------------------------------------------------
//!
//! \fn PNodeBlock::PNodeBlock()
//!
PNodeBlock::PNodeBlock(
    guint32     _depth,
    Node    *   _parent)
{
	a_hcard		    = 0;
	a_scard		    = 0;

    a_first_node    = NULL;
    a_last_node     = NULL;

	a_depth		    = _depth;
	a_parent	    = _parent;

    //  array of all nodes
	d_hnodes = g_array_sized_new(
		FALSE,					// zero_terminated element appended at the end
		TRUE,					// all bits set to zero
		sizeof(Node*),			// element_size,
		10);					//reserved_size);

	d_snodes = g_array_sized_new(
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
//  ----------------------------------------------------------------------------
//  PNodeBlock::~PNodeBlock()
//  ----------------------------------------------------------------------------
//!
//! \fn PNodeBlock::~PNodeBlock()
//!
//! brief\  Here we only delete the array of children ; it does _NOT_ affect the
//! children nodes. For recursive deletion of children , call delete_all_nodes
//! first.
//!
PNodeBlock::~PNodeBlock()
{
    g_assert( ( hcard() == 0 ) && ( scard() == 0 ) );
	//BLOCK_INF("BLK-(%04i):d %03i c:%03i", Count - 1, a_depth, a_card);

	g_array_free( d_hnodes, TRUE );
	g_array_free( d_snodes, TRUE );

	Count--;
}
//  ----------------------------------------------------------------------------
//  PNodeBlock::check()
//  ----------------------------------------------------------------------------
//! \fn check()
//!
//! \brief  For debug.
//!
//! Called if LIBGWR_TREESTORE_CHECK_LEVEL != 0. Dont call on NodeRoot.
gboolean
PNodeBlock::check()
{
	Node        *   node    = NULL;
	Node        *   next    = NULL;
	guint32			i	    = 0;
    guint32         card    = 0;

	//..........................................................................
    card    = a_hcard + a_scard;

	//  ........................................................................
    //  verify chaining
    node    = a_first_node;

    do
    {
        //  no more nodes
        if ( ! node )
        {
            if ( i < card )
            {
                printf("*** (PNODEBLOCK) *** not enough Nodes\n");
                _GWR_BREAK_;
            }
            break;                                                              //  i >= card : normal exit
        }

        // from here we have a node

        //  too much nodes
        if ( i >= card )
        {
            printf("*** (PNODEBLOCK) *** too much Nodes\n");
            _GWR_BREAK_;
        }

        next = node->anext();                                                   //  avoid infinite loops
        if ( next == node )
        {
            printf("*** (PNODEBLOCK) *** node->anext() == node\n");
            _GWR_BREAK_;
        }

        i++;
        node = next;
    }
    while ( TRUE );

	//  ........................................................................
    //  verify indexes
    if ( (int)a_hcard != d_hnodes->len )
    {
        printf("*** (PNODEBLOCK) *** a_hcard != GArray->len\n");
        _GWR_BREAK_;
    }
    if ( (int)a_scard != d_snodes->len )
    {
        printf("*** (PNODEBLOCK) *** a_hcard != GArray->len\n");
        _GWR_BREAK_;
    }

	//  ........................................................................
    //  verify shown indexes
    for ( guint16 i = 0 ; i != a_scard ; i++ )
    {
        node = g_array_index(d_snodes, Node*, i);

        if ( PNODE(node)->spos() != i )
        {
            printf("*** (PNODEBLOCK) *** node->spos() != GArray position\n");
            _GWR_BREAK_;
        }
    }

	//  ........................................................................
    //  verify hidden indexes
    for ( guint16 i = 0 ; i != a_hcard ; i++ )
    {
        node = g_array_index(d_hnodes, Node*, i);

        if ( PNODE(node)->hpos() != i )
        {
            printf("*** (PNODEBLOCK) *** node->hpos() != GArray position\n");
            _GWR_BREAK_;
        }
    }

    return TRUE;
}
//  ----------------------------------------------------------------------------
//  PNodeBlock::children_clr_fields()
//  ----------------------------------------------------------------------------
//!
//! \fn PNodeBlock::children_clr_fields()
//!
//! \brief  Reset Garrays and some arrays-related vars.
//! Arrays must be empty.
//!
void
PNodeBlock::children_clr_fields()
{
    g_return_if_fail( ( a_hcard == 0 ) && ( a_scard == 0 ) );

	a_scard         = 0;
    a_hcard         = 0;
    a_first_node    = NULL;
    a_last_node     = NULL;
}
//  ----------------------------------------------------------------------------
//  PNodeBlock::get_shown_node()
//  ----------------------------------------------------------------------------
//!
//! \fn get_shown_node()
//!
//! \brief  Get a visible node from its index.
//!
//! \param  _pos    Position ( starting from zero ) of the node to get.
//!
//! \return NULL if bad error ; the Node else.
Node*
PNodeBlock::get_shown_node(
	guint16 _pos)
{
	Node			*node   = NULL;
	//.........................................................................
	if ( a_scard == 0 )
	{
		// gtk+ calls us with bad indexes !!! ( when scrooling liftbars ) :
		// cant do this : g_return_val_if_fail( index == 0, NULL );
		if ( _pos != 0 )
			return NULL;

		//BLOCK_INF("BLK(%-20s) d:%03i p:%03i c:%03i [NULL]", "node_get", a_depth, index, a_card);
		// this is authorized, since gtk call us for 0th child !!!
		return NULL;
	}
	else
	{
		// gtk+ calls us with bad indexes !!! ( when scrooling liftbars ) :
		// cant do this : g_return_val_if_fail( index < a_card, NULL );
		if ( _pos >= a_scard )
			return NULL;
	}

	node    =   g_array_index(d_snodes, Node*    , (gint)_pos );                //  get the node
	//BLOCK_INF("BLK(%-20s) d:%03i p:%03i c:%03i [%s]", "node_get", a_depth, pos, a_card, node->log());
	return node;
}
//  ----------------------------------------------------------------------------
//  PNodeBlock::get_hidden_node()
//  ----------------------------------------------------------------------------
//!
//! \fn get_hidden_node()
//!
//! \brief  Get a hidden node from its index. Returns NULL on index error.
//!
//! \param  _pos    Position ( starting from zero ) of the node to get.
//!
//! \return NULL if bad index ; the Node else.
Node*
PNodeBlock::get_hidden_node(
	guint16 _pos)
{
	Node			*node   = NULL;
	//.........................................................................
	if ( a_hcard == 0 )
	{
        g_assert( ! _pos );
		return NULL;
	}

    g_return_val_if_fail( _pos < a_hcard, NULL );

	node    =   g_array_index(d_hnodes, Node*    , (gint)_pos );                //  get the node
	//BLOCK_INF("BLK(%-20s) d:%03i p:%03i c:%03i [%s]", "node_get", a_depth, pos, a_card, node->log());
	return node;
}
//  ****************************************************************************
//
//  ADD
//
//  ****************************************************************************
//  ----------------------------------------------------------------------------
//  PNodeBlock::node_add()
//  ----------------------------------------------------------------------------
//!
//! \fn PNodeBlock::node_add()
//!
//! \brief  Create a Node, and add it to the block. This method respect the
//!     ordering setting given in parameter. This is the most important method
//!     in libgwr::Treestore stuff.
//!
//! \param  _uid                unique id of the node
//! \param  _sort_type          sort type
//! \param  _collate_key_to_use collate key index to use
//! \param  _data               user's data, implementing DataInterface
//! \param  _visible            wether the node will be visible or not
//!
Node*
PNodeBlock::node_add(
	guint32	            _uid,
	eSortType           _sort_type,
	gint                _collate_key_to_use,
	IData           *   _data)
{
	Node        *   node    = NULL;
	Node        *   lsn     = NULL;                                             //!< Last encountered Node while sorting
	Node        *   plsn    = NULL;                                             //!< Previous Node of lsn
	guint32			i	    = 0;
    guint32         card    = 0;

    Node        *   lvn     = NULL;
    Node        *   nvn     = NULL;

    Node        *   p       = NULL;
    gboolean        shown   = TRUE;
	//.........................................................................
	// find if we will add a shown or hidden node
    p = parent();

    shown   =   p->shown()                                      ?
                    ( _data->IGetVisibility()  ? TRUE : FALSE ) :               //  parent shown  : shown = visibility
                    ( FALSE )                                   ;               //  parent hidden : shown = FALSE

	// create a new node with pos = 0 :
	// we cant set the positions now, because we dont know at what position
	// it will be stored, and in what array ( visible / hidden )
	node = new Node(_uid, a_depth, 0, 0, a_parent, _data);

    card = a_hcard + a_scard;                                                   //  total number of nodes

    //  ........................................................................
    //  if no sort required, simply append either to shown or hidden array.
    //  we simulate the result of a sort by setting lsn to the good
    //  values, and we call lab_generic_append, which was originally coded
    //  for appending after a sort ( we dont set plsn, it is not used in
    //  that code )
	if ( ! _sort_type  )                                                        //  eSortNone is 0x00
    {
        plsn = a_last_node;
        //lsn    = NULL;                                                        _GWR_OPTIM_
		goto lab_sort_append;
    }
    //  ........................................................................
    //  sort descending ( basic 'insertion sort algorithm' )
lab_sort_descending:

	if ( _sort_type != eSortDescending )
        goto lab_sort_ascending;

    lsn = a_first_node;                                                         //  first node of d_hnodes and d_vnodes

lab_loop_sd:
    // Fuck, Fuck, Fuck !!!
    // Spended hours on this, g_array_sized_new( ...set bits to 0 )
    // doesnt fucking work ( in glib 2.16.6 ) !!!
    if ( i >= card )                                                            //  no node triggered : just append
        goto lab_sort_append;

    if ( node->data()->compare(lsn->data()) >= 0 )                              //  node has triggered : go to node insertion
        goto lab_generic_insert;

    plsn    = lsn;                                                              //  update previously tested node
    lsn     = lsn->anext();                                                     //  update next node to test
    i++;
    goto lab_loop_sd;

    //  ........................................................................
    //  sort ascending
    //  basic 'insertion sort algorithm'
lab_sort_ascending:

	if ( _sort_type != eSortAscending )
        goto lab_bad_sort_type_value;

    lsn = a_first_node;                                                         //  first node of d_hnodes and d_vnodes

lab_loop_sa:
    // Fuck, Fuck, Fuck !!!
    // Spended hours on this, g_array_sized_new( ...set bits to 0 )
    // doesnt fucking work ( in glib 2.16.6 ) !!!
    if ( i >= card )                                                            //  no Node triggered & no more Nodes: just append
        goto lab_sort_append;

    if ( node->data()->compare(lsn->data()) <= 0 )
        goto lab_generic_insert;                                                //  node has triggered : go to node insertion

    plsn    = lsn;                                                              //  update previously tested node
    lsn     = lsn->anext();                                                     //  update next node to test
    i++;
    goto lab_loop_sa;

    //  ........................................................................
    //  append after a sort
    //  here lsn is always NULL, and plsn stays for lsn cf L761, L736
lab_sort_append:

    PNODE(node)->set_anext(NULL);                                               //  _GWR_REDUNDANT_ There is no Node after us

    a_last_node = node;

    //  'node' is not the first chaininig Node
    if ( plsn )
    {
        PNODE(node)->set_aprev(plsn);
        PNODE(plsn)->set_anext(node);
    }
    //  'node' is the first chaininig Node
    else
    {
        PNODE(node)->set_aprev(NULL);                                           //  _GWR_REDUNDANT_
        a_first_node = node;
    }

    goto lab_generic_append;

    //  ........................................................................
    //  sort type error
lab_bad_sort_type_value:

	g_assert(FALSE);

    //  ........................................................................
    //  end of append case, common for sort and no-sort cases
lab_generic_append:

    //  append a shown node
    if ( shown )
    {
        //  append shown node & set its spos
        d_snodes = g_array_append_val(d_snodes, node);
        PNODE(node)->set_spos( a_scard );                                       //  a_spos = ( a_scard - 1 ) + 1 = a_scard
        PNODE(node)->set_hpos( 0xFFFF );                                        //  invalidate hpos

        //  snext stuff : no shown Node after node
        PNODE(node)->set_snext(NULL);                                           //  _GWR_REDUNDANT_

        //  snext stuff : if we didnt create the first _SHOWN_ node, modify
        //  the chaining of sprev node lvn
        if ( a_scard )
        {
            lvn = g_array_index(d_snodes, Node*, a_scard - 1);
            PNODE(lvn)->set_snext(node);
        }

        a_scard++;

        PNODE(node)->flags_set_shown(1);

        NODE_CHECK(node);
        BLOCK_CHECK(this);

        return node;
    }
    //  append a hidden node
    else
    {
        //  append hidden node
        d_hnodes = g_array_append_val(d_hnodes, node);
        PNODE(node)->set_hpos( a_hcard );                                       //  a_hpos = ( a_hcard - 1 ) + 1 = a_hcard
        PNODE(node)->set_spos( 0xFFFF );                                        //  invalidate spos

        //
        //  ...there is no hnext stuff...
        //

        a_hcard++;

        PNODE(node)->flags_set_shown(0);

        NODE_CHECK(node);
        BLOCK_CHECK(this);

        return node;
    }
	//.........................................................................
	//  'Insert' back-end : We have to insert 'node' at pos i, instead of 'lsn'
    //  lsn is not NULL, and plsn may be NULL.
lab_generic_insert:

    g_assert(lsn);

    //  ordered chaining
    PNODE(node)->set_anext(lsn);
    PNODE(lsn)->set_aprev(node);

        //  'node' is not the first chaining Node
        if ( plsn )
        {
            PNODE(node)->set_aprev(plsn);
            PNODE(plsn)->set_anext(node);
        }
        //  'node' is the first chaining Node
        else
        {
            PNODE(node)->set_aprev(NULL);                                       //  _GWR_REDUNDANT_
            a_first_node = node;
        }

    //  insert a shown node
    if ( shown )
    {
        PNODE(node)->flags_set_shown(1);

        //  find next shown Node of node
        nvn = PNODE(node)->find_anext_shown();                                  //  this is slow

        //  there is a Next Visible(shown) Node
        if ( nvn)
        {
            d_snodes = g_array_insert_val( d_snodes, PNODE(nvn)->spos(), node); //  insert in shown array
            PNODE(node)->set_snext(nvn);                                        //  set snext field of node
            PNODE(node)->set_spos(PNODE(nvn)->spos());                          //  set spos  field of node
            PNODE(node)->set_hpos( 0xFFFF );                                    //  invalidate hpos

            //  ................................................................
            //  snext stuff for other shown Nodes

            //  if we didnt create the first _SHOWN_ Node, modify the previous
            //  _SHOWN_ Node so its snext field points to the newly created Node
            if ( PNODE(nvn)->spos() )                                                               //  there is a Node before 'node'
                PNODE( parent()->children()->node_get(PNODE(nvn)->spos() - 1))->set_snext(node);    //  because nvn had pos > 0 // THIS ???

            //  ................................................................
            //  spos stuff
            //  increase by 1 all pos sfields of Nodes following 'node', beginning with
            //  node just after us, wich is nvn
            while ( nvn )
            {
                PNODE(nvn)->inc_spos();
                nvn = nvn->snext();
            }
        }
        //  there is no Next Visible Node : simply append to d_snodes
        else
        {
            d_snodes = g_array_append_val(d_snodes, node);
            PNODE(node)->set_snext(NULL);
            PNODE(node)->set_spos( a_scard );
            PNODE(node)->set_hpos( 0xFFFF );                                    //  invalidate hpos

            //  if we didnt create the first _SHOWN_ Node, modify the previous
            //  _SHOWN_ Node so its snext field points to the newly created Node
            if ( a_scard )
                PNODE(g_array_index(d_snodes, Node*, a_scard -1))->set_snext(node);
        }

        a_scard++;

        PNODE(node)->flags_set_shown(1);

        NODE_CHECK(node);
        BLOCK_CHECK(this);

        return node;
    }
    //  indert a hidden node
    else
    {
        PNODE(node)->set_snext(NULL);                                           //  set snext field of node

        /*
        //  find next hidden Node of 'node'
        nhn = PNODE(node)->find_anext_hidden();                                 //  this is slow

        //  there is a Next Hidden Node
        if ( nhn )
        {
            //  insert in hidden array : order is not important, since
            //  we dont need it...so we could just append it
            ////d_hnodes = g_array_insert_val( d_hnodes, PNODE(nhn)->spos(), node);
            d_hnodes = g_array_append_val( d_hnodes, node);

        }
        //  there is no Next Hidden Node : simply append to d_hnodes
        else
        {
            d_hnodes = g_array_append_val(d_hnodes, node);
        }
        */
        //  append hidden node
        d_hnodes = g_array_append_val(d_hnodes, node);
        PNODE(node)->set_hpos( a_hcard );                                       //  a_hpos = ( a_hcard - 1 ) + 1 = a_hcard
        PNODE(node)->set_spos( 0xFFFF );                                        //  invalidate spos

        //
        //  ...there is no hnext stuff...
        //

        a_hcard++;

        PNODE(node)->flags_set_shown(0);

        NODE_CHECK(node);
        BLOCK_CHECK(this);

        return node;
    }

    return NULL;
}
//  ****************************************************************************
//
//  FIND
//
//  ****************************************************************************
//  ----------------------------------------------------------------------------
//  PNode::find_anext_shown()
//  ----------------------------------------------------------------------------
//!
//! \fn Node:find_anext_shown()
//!
//! brief\  Get the anext visible Node of a Node. Useful when showing a hidden
//!     node.
//!
Node*
PNode::find_anext_shown()
{
    Node* n = a_anext;

    while ( n )
    {
        if ( n->shown() )
            return n;

        n = n->a_anext;
    }

    return NULL;
}
//  ----------------------------------------------------------------------------
//  PNode::find_anext_hidden()
//  ----------------------------------------------------------------------------
//!
//! \fn Node:find_anext_hidden()
//!
//! brief\  Get the anext visible Node of a Node. Useful when hiding a visible
//!     node.
//!
Node*
PNode::find_anext_hidden()
{
    Node* n = a_anext;

    while ( n )
    {
        if ( n->hidden() )
            return n;

        n = n->a_anext;
    }

    return NULL;
}
//  ----------------------------------------------------------------------------
//  PNodeBlock::find_sub_node()
//  ----------------------------------------------------------------------------
//!
//! \fn PNodeBlock::find_sub_node()
//!
//! \brief  __RECURSIVE__ Find a node among the descendance.
//!     __WARNING__ THIS METHOD CAN RETURN HIDDEN NODES         __WARNING__
//!     __WARNING__ THIS METHOD CAN ONLY BE CALLED ON ROOT NODE __WARNING__
//!
//! \param  _path   Path of Node to find.
//!
//! \return The Node found, NULL if error / not found.
Node*
PNodeBlock::find_sub_node(
	Path    *   _path)
{
    Node    *   node    = NULL;
    guint32     dep     = 0;
    //..........................................................................
    node = parent();                                                            //  node should be node root
    g_return_val_if_fail(   node                , NULL);
    g_return_val_if_fail( ! node->parent()      , NULL );                       //  only call on root node
    g_return_val_if_fail(   depth() == 1        , NULL );                       //  only call on root node

	dep = 1;                                                                    //  root's children have depth 1

loop:
	node = node->children()->a_first_node;                                      //  we scan all nodes, even hidden

	while ( node )
	{
		if ( node->uid() == _path->uid_get(dep-1) )                             //  uid correspondance
		{
			if ( ++dep > _path->card() )                                        //  this was the last uid : Node found !
				return node;
			else
				goto loop;                                                      //  more uid to scans : descend one depth
		}
		node = node->anext();                                                   //  we scan all nodes, even hidden
	}

	return NULL;
}
//  ****************************************************************************
//
//  REMOVE
//
//  ****************************************************************************
//  ----------------------------------------------------------------------------
//  PNode::remove_children()
//  ----------------------------------------------------------------------------
//!
//! \fn Node:remove_children()
//!
//! __RECURSIVE__ remove the descendance of the node.
//!
guint32
PNode::remove_children()
{
    return children()->remove_all_nodes();
}
//  ----------------------------------------------------------------------------
//  PNode::remove_child()
//  ----------------------------------------------------------------------------
//!
//! \fn Node:remove_child()
//!
//! brief\  __RECURSIVE__ remove a child and all its descendance.
//!
guint32
PNode::remove_child(
    guint16 _pos)
{
	guint32									count   = 0;
	//.........................................................................

    // remove children nodes
	count += children()->remove_one_node(_pos);

	return count;
}
//  ----------------------------------------------------------------------------
//  PNodeBlock::remove_visible_node()
//  ----------------------------------------------------------------------------
//!
//! \fn PNodeBlock::remove_visible_node()
//!
//! \brief  __RECURSIVE__ Remove a _VISIBLE_ Node and all its children
//!     ( including hidden children ! )
//!
//! \param  _pos    the position of visible Node to remove
//!
guint32
PNodeBlock::remove_shown_node(
	guint16 _pos)
{
    Node        *   node	= NULL;
	//.........................................................................
    node = get_shown_node(_pos);

    g_return_val_if_fail( node                          , 0         );
    g_return_val_if_fail( node->shown()                 , 0         );

    NODE_CHECK(node);
    BLOCK_CHECK(this);

    return remove_shown_node(node);
}
guint32
PNodeBlock::remove_shown_node(
	Node    *   node)
{
    Node        *   p       = NULL;
    Node        *   n       = NULL;
    Node        *   tmp     = NULL;
    guint32         count   = 0;
    guint16         pos     = 0;
	//.........................................................................
    g_return_val_if_fail( node          , 0  );
    g_return_val_if_fail( node->shown() , 0  );

    NODE_CHECK(node);
    BLOCK_CHECK(this);

    //  ordered chaining
    p   = node->aprev();
    n   = node->anext();
    PNODEBLOCK_ORDERED_CHAINING_CUT(p,n);

    // remove the node from the shown array
    pos = PNODE(node)->spos();
    g_array_remove_index(d_snodes, pos);

    tmp = g_array_index(d_snodes, Node*, pos);
    while ( tmp )
    {
        PNODE(tmp)->dec_spos();
        tmp = tmp->snext();
    }

    a_scard--;

    //  now remove all children
    //count = count + 1 + node->remove_all_children();

    //  delete objects
    delete node->children();
    delete node;

    return count;
}
//  ----------------------------------------------------------------------------
//  PNodeBlock::remove_hidden_node()
//  ----------------------------------------------------------------------------
//!
//! \fn PNodeBlock::remove_hidden_node()
//!
//! \brief  __RECURSIVE__ Remove a _HIDDEN_ Node and all its children
//!     ( including hidden children ! )
//!
//! \param  _pos    the position of hidden Node to remove
//!
guint32
PNodeBlock::remove_hidden_node(
	guint16 _pos)
{
    Node        *   node	= NULL;
    Node        *   p       = NULL;
    Node        *   n       = NULL;
    guint32         count   = 0;
	//.........................................................................
    node = get_hidden_node(_pos);

    g_return_val_if_fail( node               , 0  );
    g_return_val_if_fail( node->hidden()     , 0  );

    //  ordered chaining
    p   = node->aprev();
    n   = node->anext();
    PNODEBLOCK_ORDERED_CHAINING_CUT(p,n);

    // remove the node from the hidden array
    g_array_remove_index(d_hnodes, _pos);

    //
    //  ... no hpos ...
    //

    a_hcard--;

    //  now remove all children
    count = count + 1 + node->remove_all_children();

    //  delete objects
    delete node->children();
    delete node;

    return count;
}
//  ----------------------------------------------------------------------------
//  PNodeBlock::remove_all_nodes()
//  ----------------------------------------------------------------------------
//!
//! \fn PNodeBlock::remove_all_nodes()
//!
//! \brief  __RECURSIVE__ Remove all _VISIBLE_ Nodes and all their children
//!     ( including hidden children ! )
//!
guint32
PNodeBlock::remove_all_nodes()
{
	Node*		node	= NULL;
	Node*		next	= NULL;
    guint32     count   = 0;
    guint16     hc      = 0;
	//.........................................................................

    //BLOCK_INF("remove_nodes():[%03i]", a_card);

    node = a_first_node;

    while ( node )
    {
        next    =   PNODE(node)->anext();
        if ( node->shown() )
        {
            count   +=  remove_shown_node(PNODE(node)->spos());
        }
        else
        {
            count   +=  remove_hidden_node(hc);
            hc++;
        }

        node    =   next;
    }

    children_clr_fields();

    return count;
}
//  ****************************************************************************
//
//  SHOW / HIDE
//
//  ****************************************************************************
//  ----------------------------------------------------------------------------
//  PNode::visibility_changed()
//  ----------------------------------------------------------------------------
//! \fn visibility_changed()
//!
//! \brief  Refresh display of a Node and its descendance when its visibility
//!     has changed.
//!
//!     __RECURSIVE__   ( Not by itself, but call recursive functions ).
//!
//!     s_ret_guint32_x2.u1 : Nodes H -> S
//!     s_ret_guint32_x2.u2 : Nodes S -> H
//!     s_ret_gboolean      : Set to FALSE on error.
//!
//! \return s_ret_gboolean
gboolean
PNode::visibility_changed()
{
    Node        *   p       = NULL;
    NodeBlock   *   b       = NULL;

    gboolean        v_old   = FALSE;
    gboolean        v_new   = FALSE;
    //  ........................................................................
    //  init retvals
    Store::s_ret_guint32_x2.u1  = 0;
    Store::s_ret_guint32_x2.u2  = 0;
    Store::s_ret_gboolean       = TRUE;
    //  ........................................................................
    //  some checks
    {
    v_old = flags_get_visibility();
    v_new = d_data->IGetVisibility();

    if ( v_old == v_new )                                                       //  no visibility change : error
    {
        Store::s_ret_gboolean = FALSE;
        return FALSE;
    }
    }
    //  ........................................................................
    b       =   brothers();
    p       =   parent();

    this->flags_set_visibility( v_new );                                        //  update visibility

    if ( ! p->shown() )                                                         //  parent is hidden : do nothing
        return TRUE;

    if ( v_new )                                                                //  parent is shown : update display
    {
        if ( ! b->node_move_from_hidden_to_shown((Node*)this) )
        {
            Store::s_ret_gboolean = FALSE;
            return FALSE;
        }
        Store::s_ret_guint32_x2.u1++;
    }
    else
    {
        if ( ! b->node_move_from_shown_to_hidden((Node*)this) )
        {
            Store::s_ret_gboolean = FALSE;
            return FALSE;
        }
        Store::s_ret_guint32_x2.u2++;
    }
    //  ........................................................................
    //  Node's display was modified : refresh children's display
    if ( v_new )
        children()->r_parent_has_been_shown();
    else
        children()->r_parent_has_been_hidden();

    NODE_CHECK(this);

    return Store::s_ret_gboolean;
}
//  ----------------------------------------------------------------------------
//  PNodeBlock::r_parent_has_been_shown()
//  ----------------------------------------------------------------------------
//! \fn r_parent_has_been_shown()
//!
//! \brief  Refresh display of children parent has just been shown.
//!
//!     __RECURSIVE__
//!
//!     s_ret_guint32_x2.u1 : Add the number of Nodes that were effectively
//!         refreshed ( Nodes H -> S ).
//!     s_ret_gboolean      : Set to FALSE on error.
void
PNodeBlock::r_parent_has_been_shown()
{
    IData   *   d       = NULL;

	Node    *	n       = NULL;

	Node    *	na	    = NULL;
	//..........................................................................
    //  first some checks
    {

    if ( scard() )                                                              //  parent has been shown => it was hidden before => all Nodes must be hidden
    {
        Store::s_ret_gboolean  = FALSE;
        return;
    }

    if ( ! hcard() )                                                            //  no Node to show : end !
        return;
    }

    n   =   a_first_node;                                                       //  we know that n is hidden
    //  ........................................................................
lab_loop:

    na  =   n->anext();                                                         //  memorize n->anext() ( hidden too )

    d   =   n->data();                                                          //  get IData

    if ( ! d->IGetVisibility() )                                                //  visibility = FALSE : loop on anext
        goto lab_next;

    //
    //  from here Node n has to be shown
    //

    if ( ! node_move_from_hidden_to_shown(n) )                                  //  transfer Node from hidden to shown
    {
        Store::s_ret_gboolean = FALSE;
        return;
    }

    Store::s_ret_guint32_x2.u1++;                                               //  inc global H -> S result

    n->children()->r_parent_has_been_shown();                                   //  recursive call

lab_next:
        n = na;                                                                 //  roll

        if ( ! n )                                                              //  if no anext, we're done
            goto lab_end;

    goto lab_loop;                                                              //  loop

lab_end:

    BLOCK_CHECK(this);
}
//  ----------------------------------------------------------------------------
//  PNodeBlock::r_parent_has_been_hidden()
//  ----------------------------------------------------------------------------
//! \fn r_parent_has_been_hidden()
//!
//! \brief  Refresh display of children parent has just been hidden.
//!
//!     __RECURSIVE__
//!
//!     s_ret_guint32_x2.u2 : Add the number of Nodes that were effectively
//!         refreshed ( Nodes S -> H ).
//!     s_ret_gboolean      : Set to FALSE on error.
void
PNodeBlock::r_parent_has_been_hidden()
{
    IData   *   d       = NULL;

	Node    *	n       = NULL;

	Node    *	na	    = NULL;
	//..........................................................................
    //  first some checks
    {

    if ( ! scard() )                                                            //  no Node to hide : end !
        return;

    }

    n   =   a_first_node;
    //  ........................................................................
lab_loop:

    na  =   n->anext();                                                         //  memorize n->anext()

    if ( n->hidden() )                                                          //  _GWR_TODO_ loop on shown nodes only !
        goto lab_next;

    d   =   n->data();                                                          //  get IData

    if ( ! node_move_from_shown_to_hidden(n) )                                  //  transfer Node from shown to hidden
    {
        Store::s_ret_gboolean = FALSE;
        return;
    }

    Store::s_ret_guint32_x2.u2++;                                               //  inc global S -> H result

    n->children()->r_parent_has_been_hidden();                                  //  recursive call

lab_next:
        n = na;                                                                 //  roll

        if ( ! n )                                                              //  if no anext, we're done
            goto lab_end;

    goto lab_loop;                                                              //  loop

lab_end:

    BLOCK_CHECK(this);
}
//  ----------------------------------------------------------------------------
//  PNodeBlock::node_move_from_hidden_to_shown()
//  ----------------------------------------------------------------------------
//! \fn node_move_from_hidden_to_shown()
//!
//! \brief  "Show" one node i.e.
//!     - move it from hidden Nodes array to visible Nodes array
//!     - send "row-inserted" signal
//!
//! \remark Uses Store::Get_static_store()
//!
//! \return TRUE / FALSE
gboolean
PNodeBlock::node_move_from_hidden_to_shown(Node* _n)
{
	Node    *	h       = NULL;

	Node    *	ns	    = NULL;
	Node    *	ps	    = NULL;

    guint32     nsp     = 0;

    guint16     i       = 0;
    //  ........................................................................
    //  remove node from hidden Nodes array
    {

    d_hnodes = g_array_remove_index(d_hnodes, _n->hpos() );                     //  remove n from Hidden

    //
    //  ...there is no hidden chaining...
    //

    a_hcard--;                                                                  //  dec Hidden card here allow simple test 'i < a_hcard' in for...next loop

    for ( i = _n->hpos() ; i < a_hcard ; i++ )                                  //  from next hidden node in array to end of array, dec hpos
    {
        h = g_array_index( d_hnodes, Node*, i );
        PNODE(h)->dec_hpos();
    }

    PNODE(_n)->set_hpos( 0xFFFF );                                              //  invalidate hpos

    }
    //  ........................................................................
    //  insert node in shown array
    {

    ns = _n->find_anext_shown();                                                //  find anext shown

    //  insert before an existing visible Node
    if ( ns )
    {
        //  chaining
        nsp = PNODE(ns)->spos();
        d_snodes = g_array_insert_val(d_snodes, nsp, _n);                       //  insert

        PNODE(_n)->set_snext(ns);                                               //  _n -> snext

        if ( nsp )
        {
            ps = g_array_index(d_snodes, Node*, nsp - 1);
            ps->set_snext(_n);                                                  //  sprev (_n ) -> snext
        }

        //  indexes
        PNODE(_n)->set_spos(nsp);                                               //  _n -> spos

        while ( ns )                                                            //  set spos of all Nodes after n
        {
            PNODE(ns)->inc_spos();
            ns = ns->snext();
        }

    }
    //  simply append to visible node array
    else
    {
        //  chaining
        d_snodes = g_array_append_val(d_snodes, _n);                            //  append

        PNODE(_n)->set_snext(NULL);                                             //  _n -> snext

        if ( a_scard )
        {
            ps = g_array_index(d_snodes, Node*, a_scard - 1);
            ps->set_snext(_n);                                                  //  sprev ( _n ) -> snext
        }

        //  s-indexes
        PNODE(_n)->set_spos(a_scard);                                           //  _n -> spos
    }

    PNODE(_n)->flags_set_shown(1);

    a_scard++;                                                                  //  inc visible card

    }

    Store::Get_static_store()->emit_row_inserted(_n);

    return TRUE;
}
//  ----------------------------------------------------------------------------
//  PNodeBlock::node_move_from_shown_to_hidden()
//  ----------------------------------------------------------------------------
//! \fn node_move_from_shown_to_hidden()
//!
//! \brief  "Hide" one node i.e.
//!     - move it from shown Nodes array to hidden Nodes array
//!     - send "row-inserted" signal
//!
//! \remark Uses Store::Get_static_store()
//!
//! \return TRUE / FALSE
gboolean
PNodeBlock::node_move_from_shown_to_hidden(Node* _n)
{
    Node    *       ns      = NULL;
    Node    *       ps      = NULL;

    guint16         nsp     = 0;
	//.........................................................................
    Store::Get_static_store()->emit_row_deleted(_n);                            //  following gtk+ recommandations, signal is sent before modifying model

	//.........................................................................
    //  remove _n from shown Nodes array
    {

    //  chaining
    nsp         = _n->spos();
    d_snodes    = g_array_remove_index(d_snodes, nsp);                          //  remove _n from array

    ns          = _n->snext();                                                  //  next shown ( may be NULL )

    if ( nsp )                                                                  //  if there is previous shown...
    {
        ps  = g_array_index( d_snodes, Node*, nsp - 1 );
        PNODE(ps)->set_snext( ns );                                             //  ...sprev( _n ) -> snext
    }

    //  indexes
    while ( ns  )                                                               //  from ns to last shown, dec index
    {
        PNODE(ns)->dec_spos();
        ns = PNODE(ns)->snext();
    }

    _n->set_spos(0xFFFF);                                                       //  invalidate some fields
    _n->set_snext(NULL);

    a_scard--;

    }
    //  ........................................................................
    //  move _n in hidden array
    {

    //  just append all existing hidden Nodes ; the index of hidden Nodes
    //  is unuseful, since...they are hidden ! Hidden Nodes does not need
    //  to be sorted !
    //
    //                      ~ I am the best ~
    //
    //  ( But the hidden array not sorted may be not practical for debug ! )
    d_hnodes = g_array_append_val(d_hnodes, _n);                                //  append
    _n->set_hpos( a_hcard );

    a_hcard++;                                                                  //  dec hidden card

    }

    PNODE(_n)->flags_set_shown(0);

    return TRUE;
}

GWR_NAMESPACE_END(treestore)
GWR_NAMESPACE_END(libgwr)

/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-treestore-nodes.h                                                *
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
#include	"libgwr-treestore.hh"
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
//  Node::Node()
//  ----------------------------------------------------------------------------
//!
//! \brief  Node constructor
//!
//!	\param  _uid        uid of the node
//!	\param  _depth      Depth of the node ( = same as depth in GtkTreePath )
//!	\param  _apos       Absolute pos ( starting from 0 ) of the node in the block it belongs to
//!	\param  _vpos       Visible  pos ( starting from 0 ) of the node in the block it belongs to
//!	\param  _parent     Parent node of this node
//!	\param  _data       User's data
//!
Node::Node(
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

    d_children		=   GWR_NEW_CAST( NodeBlock, _depth + 1, (Node*)this );

    d_data			=   _data;

    //Count++;
    //NODE_INF("NOD+(%04i):[%s]",Count, log());
}
//  ----------------------------------------------------------------------------
//  Node::~Node()
//  ----------------------------------------------------------------------------
//!
//! \brief Node destructor
//!
//! Here we only delete the memory allowed for the struct itself.
//! We do _NOT_ free the NodeBlock containing the children.
//! For recursive delete, use remove()
//!
Node::~Node()
{
    //NODE_INF("NOD-(%04i):[%s]", (Count - 1), log());

    delete data();

    //Count--;
}
//  ----------------------------------------------------------------------------
//  Node::check()
//  ----------------------------------------------------------------------------
//! \fn check()
//!
//! \brief  For debug.
gboolean
Node::check()
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

        g_return_val_if_fail( hpos() == LIBGWR_TREESTORE_INVALID_POS, FALSE );              //  - have a unvalid hpos
    }
    //  ........................................................................
    //  a hidden node must :
    else
    {
        g_return_val_if_fail(flags_get_visibility() == data()->IGetVisibility(), FALSE);    //  - have visibility match

        g_return_val_if_fail( spos() == LIBGWR_TREESTORE_INVALID_POS, FALSE );              //  - have a unvalid spos

        g_return_val_if_fail( p->children()->get_hidden_node(hpos()) == this , FALSE );     //  - have a valid spos
    }

    return TRUE;
}
//  ----------------------------------------------------------------------------
//  Node::row()
//  ----------------------------------------------------------------------------
//!
//! \fn Node:row()
//!
//! \brief  __RECURSIVE__ Get a node's row index in the tree
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
//  Node::log()
//  ----------------------------------------------------------------------------
//!
//! \fn     log()
//!
//! \brief  Get all infos about a Node
//!
const gchar*
Node::log()
{
    static gchar Node_str_01[1024];

    //  format : "node : ref_count pos parent next children data"
    sprintf(Node_str_01, "%s this:%p uid:%05i dep:%03i spos:%03i hpos:%03i aprev:%p anext:%p schd:%03i hchd:%03i dat:%s",
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
            children()->scard()            ,
            children()->hcard()            ,
            data() ? "Y" : "N" );

    return Node_str_01;
}
//  ----------------------------------------------------------------------------
//  Node::dump_tree()
//  ----------------------------------------------------------------------------
//!
//! \fn Node::dump_tree()
//!
//! \brief  Get all infos about a Node
//!
void
Node::dump_tree(guint32 _level)
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
    child = block->first_node();
    while ( child )
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
guint32 NodeBlock::Count   = 0;
//  ----------------------------------------------------------------------------
//  NodeBlock::NodeBlock()
//  ----------------------------------------------------------------------------
//!
//! \fn NodeBlock::NodeBlock()
//!
NodeBlock::NodeBlock(
    guint32     _depth,
    Node    *   _parent)
{
    a_first_node    = NULL;
    a_last_node     = NULL;

    a_depth		    = _depth;
    a_parent	    = _parent;

    d_hnodes = GWR_NEW_CAST( libgwr_treestore_nodeblock_array, 65535, 8 );
    d_snodes = GWR_NEW_CAST( libgwr_treestore_nodeblock_array, 65535, 8 );

    // Fuck, Fuck, Fuck !!!
    // Spended hours on this, g_array_sized_new( ...set bits to 0 )
    // doesnt fucking work !!!
    // printf("GArray 0x%08x d:%03i p:0x%08x [0]=0x%08x\n", nb->d_nodes, nb->a_depth, nb->a_parent, g_array_index(nb->d_nodes, Node*, 0));

    Count++;
    //BLOCK_INF("BLK+(%04i):d %03i", Count, a_depth);
}
//  ----------------------------------------------------------------------------
//  NodeBlock::~PNodeBlock()
//  ----------------------------------------------------------------------------
//! \fn NodeBlock::~NodeBlock()
//!
//! \brief  Here we only delete the array of children ; it does _NOT_ affect the
//! children nodes. For recursive deletion of children , call delete_all_nodes
//! first.
//!
NodeBlock::~NodeBlock()
{
    g_assert( ( hcard() == 0 ) && ( scard() == 0 ) );
    //BLOCK_INF("BLK-(%04i):d %03i c:%03i", Count - 1, a_depth, a_card);

    delete d_hnodes;
    delete d_snodes;

    Count--;
}
//  ----------------------------------------------------------------------------
//  NodeBlock::check()
//  ----------------------------------------------------------------------------
//! \fn NodeBlock::check()
//!
//! \brief  For debug.
gboolean
NodeBlock::check()
{
    Node        *   node    = NULL;
    Node        *   next    = NULL;
    guint16			i	    = 0;
    guint32         card    = 0;

    //..........................................................................
    card    = hcard() + scard();

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
    /*
    if ( a_hcard != d_hnodes->card() )
    {
        printf("*** (PNODEBLOCK) *** a_hcard != GArray->len\n");
        _GWR_BREAK_;
    }
    if ( a_scard != d_snodes->card() )
    {
        printf("*** (PNODEBLOCK) *** a_hcard != GArray->len\n");
        _GWR_BREAK_;
    }
    */
    //  ........................................................................
    //  verify shown indexes
    /*
    for (  i = 0 ; i != a_scard ; i++ )
    {
        node = d_snodes->get( i );

        if ( node->spos() != i )
        {
            printf("*** (PNODEBLOCK) *** node->spos() != GArray position\n");
            _GWR_BREAK_;
        }
    }
    */
    //  ........................................................................
    //  verify hidden indexes
    /*
    for (  i = 0 ; i != a_hcard ; i++ )
    {
        node = d_hnodes->get( i );

        if ( node->hpos() != i )
        {
            printf("*** (PNODEBLOCK) *** node->hpos() != GArray position\n");
            _GWR_BREAK_;
        }
    }
    */
    return TRUE;
}
//  ----------------------------------------------------------------------------
//  NodeBlock::children_clr_fields()
//  ----------------------------------------------------------------------------
//! \fn NodeBlock::children_clr_fields()
//!
//! \brief  Reset Garrays and some arrays-related vars.
//! Arrays must be empty.
//!
void
NodeBlock::children_clr_fields()
{
    g_return_if_fail( ( hcard() == 0 ) && ( scard() == 0 ) );

    a_first_node    = NULL;
    a_last_node     = NULL;
}
//  ----------------------------------------------------------------------------
//  NodeBlock::get_shown_node()
//  ----------------------------------------------------------------------------
//! \fn NodeBlock::get_shown_node()
//!
//! \brief  Get a visible node from its index.
//!
//! \param  _pos    Position ( starting from zero ) of the node to get.
//!
//! \return NULL if bad error ; the Node else.
Node*
NodeBlock::get_shown_node(
    guint16 _pos)
{
    Node			*node   = NULL;
    //.........................................................................
    if ( scard() == 0 )
    {
        // gtk+ calls us with bad indexes !!! ( when scrooling liftbars ) :
        // we cant do this : g_return_val_if_fail( index == 0, NULL );
        if ( _pos != 0 )
            return NULL;

        //BLOCK_INF("BLK(%-20s) d:%03i p:%03i c:%03i [NULL]", "node_get", a_depth, index, a_card);
        // this is authorized, since gtk call us for 0th child !!!
        return NULL;
    }
    else
    {
        // gtk+ calls us with bad indexes !!! ( when scrooling liftbars ) :
        // we cant do this : g_return_val_if_fail( index < a_card, NULL );
        if ( _pos >= scard() )
            return NULL;
    }

    node        =   d_snodes->get( _pos );                                      //  get the node
    //BLOCK_INF("BLK(%-20s) d:%03i p:%03i c:%03i [%s]", "node_get", a_depth, pos, a_card, node->log());
    return node;
}
//  ----------------------------------------------------------------------------
//  NodeBlock::get_hidden_node()
//  ----------------------------------------------------------------------------
//! \fn NodeBlock::get_hidden_node()
//!
//! \brief  Get a hidden node from its index. Returns NULL on index error.
//!
//! \param  _pos    Position ( starting from zero ) of the node to get.
//!
//! \return NULL if bad index ; the Node else.
Node*
NodeBlock::get_hidden_node(
    guint16 _pos)
{
    Node			*node   = NULL;
    //.........................................................................
    if ( hcard() == 0 )
    {
        g_assert( ! _pos );
        return NULL;
    }

    g_return_val_if_fail( _pos < hcard(), NULL );

    node    =   d_hnodes->get( _pos );                                          //  get the node
    //BLOCK_INF("BLK(%-20s) d:%03i p:%03i c:%03i [%s]", "node_get", a_depth, pos, a_card, node->log());
    return node;
}
//  ****************************************************************************
//
//  ADD
//
//  ****************************************************************************
//  ----------------------------------------------------------------------------
//  NodeBlock::node_add()
//  ----------------------------------------------------------------------------
//! \fn         NodeBlock::node_add()
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
Node*
NodeBlock::node_add(
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

    Node        *   nvn     = NULL;

    Node        *   p       = NULL;
    gboolean        shown   = TRUE;
    //.........................................................................
    // find if we will add a shown or hidden node
    p = parent();

    shown   =   p->shown()                                      ?
                ( _data->IGetVisibility()  ? TRUE : FALSE ) :                   //  parent shown  : shown = visibility
                ( FALSE )                                   ;                   //  parent hidden : shown = FALSE

    // create a new node with pos = 0 :
    // we cant set the positions now, because we dont know at what position
    // it will be stored, and in what array ( visible / hidden )
    node = GWR_NEW_CAST( Node, _uid, a_depth, 0, 0, a_parent, _data);

    card = hcard() + scard();                                                   //  total number of nodes

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

    node->set_anext(NULL);                                                      //  _GWR_REDUNDANT_ There is no Node after us

    a_last_node = node;

    //  'node' is not the first chaininig Node
    if ( plsn )
    {
        node->set_aprev(plsn);
        plsn->set_anext(node);
    }
    //  'node' is the first chaininig Node
    else
    {
        node->set_aprev(NULL);                                                  //  _GWR_REDUNDANT_
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
        d_snodes->add( node );

        node->set_spos( scard() - 1 );                                              //  a_spos = ( a_scard - 1 ) + 1 = a_scard
        node->invalidate_hpos();                                                //  invalidate hpos

        node->flags_set_shown(1);

        NODE_CHECK(node);
        BLOCK_CHECK(this);

        return node;
    }
    //  append a hidden node
    else
    {
        //  append hidden node
        d_hnodes->add( node );
        node->set_hpos( hcard() - 1 );                                              //  a_hpos = ( a_hcard - 1 ) + 1 = a_hcard
        node->invalidate_spos();                                                //  invalidate spos

        //
        //  ...there is no hnext stuff...
        //

        node->flags_set_shown(0);

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
    node->set_anext(lsn);
    lsn->set_aprev(node);

    //  'node' is not the first chaining Node
    if ( plsn )
    {
        node->set_aprev(plsn);
        plsn->set_anext(node);
    }
    //  'node' is the first chaining Node
    else
    {
        node->set_aprev(NULL);                                                  //  _GWR_REDUNDANT_
        a_first_node = node;
    }

    //  insert a shown node
    if ( shown )
    {
        node->flags_set_shown(1);

        //  find next shown Node of node
        nvn = node->find_anext_shown();                                         //  this is slow

        //  there is a Next Visible(shown) Node
        if ( nvn )
        {
            d_snodes->ins( nvn->spos(), node );                                 //  insert in shown array
            node->set_spos(nvn->spos());                                        //  set spos  field of node
            node->invalidate_hpos();                                            //  invalidate hpos

            inc_spos_from_node( 1 + nvn->spos() );
        }
        //  there is no Next Visible Node : simply append to d_snodes
        else
        {
            d_snodes->add( node );
            node->set_spos( scard() - 1 );
            node->invalidate_hpos();                                            //  invalidate hpos
        }

        node->flags_set_shown(1);

        NODE_CHECK(node);
        BLOCK_CHECK(this);

        return node;
    }
    //  indert a hidden node
    else
    {
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
        d_hnodes->add( node );
        node->set_hpos( hcard() - 1 );                                              //  a_hpos = ( a_hcard - 1 ) + 1 = a_hcard
        node->invalidate_spos();                                                //  invalidate spos

        //
        //  ...there is no hnext stuff...
        //

        node->flags_set_shown(0);

        NODE_CHECK(node);
        BLOCK_CHECK(this);

        return node;
    }

    return NULL;
}
//  ****************************************************************************
//
//  INC / DEC spos
//
//  ****************************************************************************
//  ----------------------------------------------------------------------------
//  NodeBLock::dec_spos_from_node()
//  ----------------------------------------------------------------------------
//! \fn     NodeBlock:dec_spos_from_node()
//!
//! \brief  Decrease spos of all shown nodes after one node ( including it ).
void
NodeBlock::dec_spos_from_node(guint16 _spos)
{
    Node    *   n       = NULL;
    guint16     i       = 0;
    //..........................................................................
    for ( i = _spos ; i != scard() ; i++ )
    {
        n = get_shown_node( i );
#ifdef  LIBGWR_CHECK_TREESTORE_NODEBLOCK_DEC_SPOS_FROM_NODE
        g_assert( n != NULL );
#endif
        n->dec_spos();
    }
}
//  ----------------------------------------------------------------------------
//  NodeBlock::inc_spos_from_node()
//  ----------------------------------------------------------------------------
//! \fn     NodeBlock:inc_spos_from_node()
//!
//! \brief  Increase spos of all shown nodes after one node ( including it ).
void
NodeBlock::inc_spos_from_node(guint16 _spos)
{
    Node    *   n       = NULL;
    guint16     i       = 0;
    //..........................................................................
    for ( i = _spos ; i != scard() ; i++ )
    {
        n = get_shown_node( i );
#ifdef  LIBGWR_CHECK_TREESTORE_NODEBLOCK_INC_SPOS_FROM_NODE
        g_assert( n != NULL );
#endif
        n->inc_spos();
    }
}
//  ****************************************************************************
//
//  FIND
//
//  ****************************************************************************
//  ----------------------------------------------------------------------------
//  Node::find_anext_shown()
//  ----------------------------------------------------------------------------
//! \fn Node:find_anext_shown()
//!
//! \brief  Get the anext visible Node of a Node. Useful when showing a hidden
//!     node.
Node*
Node::find_anext_shown()
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
//  Node::find_anext_hidden()
//  ----------------------------------------------------------------------------
//! \fn Node:find_anext_hidden()
//!
//! \brief  Get the anext visible Node of a Node. Useful when hiding a visible
//!     node.
Node*
Node::find_anext_hidden()
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
//  NodeBlock::find_sub_node()
//  ----------------------------------------------------------------------------
//! \fn NodeBlock::find_sub_node()
//!
//! \brief  __RECURSIVE__ Find a node among the descendance.
//!     __WARNING__ THIS METHOD CAN RETURN HIDDEN NODES         __WARNING__
//!     __WARNING__ THIS METHOD CAN ONLY BE CALLED ON ROOT NODE __WARNING__
//!
//! \param  _path   Path of Node to find.
//!
//! \return The Node found, NULL if error / not found.
Node*
NodeBlock::find_sub_node(
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
//  Node::remove_children()
//  ----------------------------------------------------------------------------
//! \fn Node:remove_children()
//!
//! __RECURSIVE__ remove the descendance of the node.
guint32
Node::remove_children()
{
    return children()->remove_all_nodes();
}
//  ----------------------------------------------------------------------------
//  Node::remove_child()
//  ----------------------------------------------------------------------------
//! \fn Node:remove_child()
//!
//! \brief  __RECURSIVE__ remove a child and all its descendance.
guint32
Node::remove_child(
    guint16 _pos)
{
    guint32									count   = 0;
    //.........................................................................

    // remove children nodes
    count += children()->remove_one_node(_pos);

    return count;
}
//  ----------------------------------------------------------------------------
//  NodeBlock::remove_shown_node()
//  ----------------------------------------------------------------------------
//! \fn     NodeBlock::remove_shown_node()
//!
//! \brief  __RECURSIVE__ Remove a _VISIBLE_ Node and all its children
//!     ( including hidden children ! )
//!
//! \param  _pos    the position of visible Node to remove
guint32
NodeBlock::remove_shown_node(
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
NodeBlock::remove_shown_node(
    Node    *   node)
{
    Node        *   p       = NULL;
    Node        *   n       = NULL;
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
    NODEBLOCK_ORDERED_CHAINING_CUT(p,n);

    // remove the node from the shown array
    pos = node->spos();
    d_snodes->del( pos );

    dec_spos_from_node( pos );

    //  now remove all children
    //count = count + 1 + node->remove_all_children();

    //  delete objects
    delete node->children();
    delete node;

    return count;
}
//  ----------------------------------------------------------------------------
//  NodeBlock::remove_hidden_node()
//  ----------------------------------------------------------------------------
//! \fn     NodeBlock::remove_hidden_node()
//!
//! \brief  __RECURSIVE__ Remove a _HIDDEN_ Node and all its children
//!     ( including hidden children ! )
//!
//! \param  _pos    the position of hidden Node to remove
guint32
NodeBlock::remove_hidden_node(
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
    NODEBLOCK_ORDERED_CHAINING_CUT(p,n);

    // remove the node from the hidden array
    d_hnodes->del( _pos );

    //
    //  ... no hpos ...
    //

    //  now remove all children
    count = count + 1 + node->remove_children();

    //  delete objects
    delete node->children();
    delete node;

    return count;
}
//  ----------------------------------------------------------------------------
//  NodeBlock::remove_all_nodes()
//  ----------------------------------------------------------------------------
//! \fn NodeBlock::remove_all_nodes()
//!
//! \brief  __RECURSIVE__ Remove all _VISIBLE_ Nodes and all their children
//!     ( including hidden children ! )
guint32
NodeBlock::remove_all_nodes()
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
        next    =   node->anext();
        if ( node->shown() )
        {
            count   +=  remove_shown_node(node->spos());
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
//  Node::visibility_changed()
//  ----------------------------------------------------------------------------
//! \fn Node::visibility_changed()
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
Node::visibility_changed()
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
//  NodeBlock::r_parent_has_been_shown()
//  ----------------------------------------------------------------------------
//! \fn NodeBlock::r_parent_has_been_shown()
//!
//! \brief  Refresh display of children parent has just been shown.
//!
//!     __RECURSIVE__
//!
//!     s_ret_guint32_x2.u1 : Add the number of Nodes that were effectively
//!         refreshed ( Nodes H -> S ).
//!     s_ret_gboolean      : Set to FALSE on error.
void
NodeBlock::r_parent_has_been_shown()
{
    IData   *   d       = NULL;

    Node    *	n       = NULL;

    Node    *	na	    = NULL;
    //..........................................................................
    //  first some checks
    {

        if ( scard() )                                                          //  parent has been shown => it was hidden before => all Nodes must be hidden
        {
            Store::s_ret_gboolean  = FALSE;
            return;
        }

        if ( ! hcard() )                                                        //  no Node to show : end !
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
    n = na;                                                                     //  roll

    if ( ! n )                                                                  //  if no anext, we're done
        goto lab_end;

    goto lab_loop;                                                              //  loop

lab_end:

    BLOCK_CHECK(this);
}
//  ----------------------------------------------------------------------------
//  NodeBlock::r_parent_has_been_hidden()
//  ----------------------------------------------------------------------------
//! \fn NodeBlock::r_parent_has_been_hidden()
//!
//! \brief  Refresh display of children parent has just been hidden.
//!
//!     __RECURSIVE__
//!
//!     s_ret_guint32_x2.u2 : Add the number of Nodes that were effectively
//!         refreshed ( Nodes S -> H ).
//!     s_ret_gboolean      : Set to FALSE on error.
void
NodeBlock::r_parent_has_been_hidden()
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
    n = na;                                                                     //  roll

    if ( ! n )                                                                  //  if no anext, we're done
        goto lab_end;

    goto lab_loop;                                                              //  loop

lab_end:

    BLOCK_CHECK(this);
}
//  ----------------------------------------------------------------------------
//  NodeBlock::node_move_from_hidden_to_shown()
//  ----------------------------------------------------------------------------
//! \fn NodeBlock::node_move_from_hidden_to_shown()
//!
//! \brief  "Show" one node i.e.
//!     - move it from hidden Nodes array to visible Nodes array
//!     - send "row-inserted" signal
//!
//! \remark Uses Store::Get_static_store()
//!
//! \return TRUE / FALSE
gboolean
NodeBlock::node_move_from_hidden_to_shown(Node* _n)
{
    Node    *	h       = NULL;

    Node    *	ns	    = NULL;

    guint32     nsp     = 0;

    guint16     i       = 0;
    //  ........................................................................
    //  remove node from hidden Nodes array
    {

        d_hnodes->del( _n->hpos() );                                            //  remove n from Hidden

        //
        //  ...there is no hidden chaining...
        //

        for ( i = _n->hpos() ; i < hcard() ; i++ )                              //  from next hidden node in array to end of array, dec hpos
        {
            h = d_hnodes->get( i );
            h->dec_hpos();
        }

        _n->invalidate_hpos();                                                  //  invalidate hpos

    }
    //  ........................................................................
    //  insert node in shown array
    {

        ns = _n->find_anext_shown();                                            //  find anext shown

        //  insert before an existing visible Node
        if ( ns )
        {
            //  chaining
            nsp = ns->spos();
            d_snodes->ins( nsp, _n );                                           //  insert
            //  indexes
            _n->set_spos(nsp);                                                  //  _n -> spos

            inc_spos_from_node( 1 + nsp );
        }
        //  simply append to visible node array
        else
        {
            //  chaining
            d_snodes->add( _n );                                                //  append
            //  s-indexes
            _n->set_spos(scard() - 1);                                              //  _n -> spos
        }

        _n->flags_set_shown(1);


    }

    Store::Get_static_store()->emit_row_inserted(_n);

    BLOCK_CHECK(this);

    return TRUE;
}
//  ----------------------------------------------------------------------------
//  NodeBlock::node_move_from_shown_to_hidden()
//  ----------------------------------------------------------------------------
//! \fn NodeBlock::node_move_from_shown_to_hidden()
//!
//! \brief  "Hide" one node i.e.
//!     - move it from shown Nodes array to hidden Nodes array
//!     - send "row-inserted" signal
//!
//! \remark Uses Store::Get_static_store()
//!
//! \return TRUE / FALSE
gboolean
NodeBlock::node_move_from_shown_to_hidden(Node* _n)
{
    guint16         nsp     = 0;
    //.........................................................................
    Store::Get_static_store()->emit_row_deleted(_n);                            //  following gtk+ recommandations, signal is sent before modifying model

    //.........................................................................
    //  remove _n from shown Nodes array
    {

        //  chaining
        nsp         = _n->spos();
        d_snodes->del( nsp );                                                   //  remove _n from array

        dec_spos_from_node(nsp);                                                //  dec all Nodes spos after _n

        _n->invalidate_spos();                                                  //  invalidate _n spos

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
        d_hnodes->add( _n );                                                    //  append
        _n->set_hpos( hcard() - 1 );

    }

    _n->flags_set_shown(0);                                                     //  flag _n as hidden

    BLOCK_CHECK(this);

    return TRUE;
}

GWR_NAMESPACE_END(treestore)
GWR_NAMESPACE_END(libgwr)

/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-treestore-node-blocks.hi                                         *
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
    *   Purpose     : Header                                                    *
    *                                                                           *
    *****************************************************************************
*/

/// ****************************************************************************
//!
//! \namespace  libgwr::treestore
//!
//! \class      NodeBlock
//!
//! \brief      A NodeBlock struct contains the children Nodes of a Node.
//!
//! \detail     We dont store entire node structs for speeding when sorting :
//!     we dont move instances, but only pointers. The counterpart is that we
//!     are forced to malloc each node.
//!
//! <pre>
//!     Pointers on hidden and visible nodes are stored in separate arrays.
//!
//!                 0     1 2   3   <- shown    indexes
//! shown         : A     F-G   K   : The shown array is sorted ( through IData->compare() ).
//!                 \   /   \ /
//! hidden        :   U-D     B     : The hidden array is unsorted.
//!                   0 1     2     <- hidden   indexes
//!
//!     All elements keeps their sorting order, in this case
//!     A->anext = B , B->anext = C, etc...
//!
//! </pre>
//!
/// ****************************************************************************
class   NodeBlock
{
    friend class Node;
    friend class Store;
    //  ------------------------------------------------------------------------
    //  static count
    private:
    static  guint32     Count;
    public:
    static  guint32	    Remaining()	{ return Count; }
    //  ------------------------------------------------------------------------
    //  static count
    private:
    Node			*   a_parent;                                               //!< parent Node

    guint32				a_depth;                                                //!< depth

    libgwr::TMArrayP < Node, guint16 >      * d_hnodes;                         //!< Hidden nodes
    libgwr::TMArrayP < Node, guint16 >      * d_snodes;                         //!< Shown  nodes

    guint16				a_hcard;                                                //!< number of Hidden Nodes
    guint16				a_scard;                                                //!< number of Shown  Nodes

    //! First node ( relative to a_anext member of PNode ) ; this node pointer
    //! is too the first element of d_hnodes or d_vnodes.
    Node            *   a_first_node;
    //! Last node ( relative to a_anext member of PNode ) ; this node pointer
    //! is too the last element of d_hnodes or d_vnodes.
    Node            *   a_last_node;
    //  ------------------------------------------------------------------------
    //  get / set
    private:
                Node        *       get_hidden_node     (guint16 _pos);
                Node	    *       get_shown_node	    (guint16 _pos);

    inline	    Node	    *       snext               (guint16 _spos);

    private:
    inline		guint16				hcard()		                        { return a_hcard;	}
    inline		gint				hempty()	                        { return ! a_hcard; }

    inline		guint16				scard()		                        { return a_scard;	}
    inline		gint				sempty()	                        { return ! a_scard; }

    inline		guint				depth()		                        { return a_depth;   }
    inline      Node	    *       parent()                            { return a_parent;  }

                void                children_clr_fields ();

    public:
    inline		guint16				card()		                        { return scard();   }
    inline		gboolean    	    empty()	                            { return sempty();  }
    inline      Node	*           node_get(guint16 _pos)              { return get_shown_node(_pos);      }
    //  ------------------------------------------------------------------------
    //  inc / dec spos
    private:
                void                dec_spos_from_node(guint16 _spos);
                void                inc_spos_from_node(guint16 _spos);
    //  ------------------------------------------------------------------------
    //  add
    private:
                Node	    *	    node_add			(guint32 _uid, eSortType, gint _collate_key_to_use, IData*);
    //  ------------------------------------------------------------------------
    //  find
    protected:
                Node        *       find_sub_node(Path*);

    public:
    inline      Node	    *       first_node()                        { return a_first_node;          }
    //  ------------------------------------------------------------------------
    //  remove
    private:
                guint32		        remove_hidden_node(guint16 _pos);
                guint32		        remove_shown_node   (guint16 _pos);
                guint32		        remove_shown_node   (Node*);
                guint32		        remove_hidden_node  (Node*);
                guint32		        remove_all_nodes();

    public:
    inline      guint32             remove_one_node(guint16 _pos)   { return remove_shown_node(_pos);   }
    //  ------------------------------------------------------------------------
    //  show / hide
    private:
                void                r_parent_has_been_shown             ();
                void                r_parent_has_been_hidden            ();
                gboolean            node_move_from_hidden_to_shown      (Node*);
                gboolean            node_move_from_shown_to_hidden      (Node*);
    //  ------------------------------------------------------------------------
    //  debug
    public:
    gboolean    check();
    //  ------------------------------------------------------------------------
    //  ()~()
    //  ------------------------------------------------------------------------
    public:
             NodeBlock(guint32 _depth, Node *_parent);
    virtual ~NodeBlock();
};

//  ----------------------------------------------------------------------------
//  NodeBlock::snext()
//  ----------------------------------------------------------------------------
//! \fn     snext()
//!
//! \brief  Return the next shown Node of a shown Node.
//!
//! \return A Node* , eventually NULL.
inline  Node*
NodeBlock::snext(guint16 _spos)
{
    #if ( LIBGWR_CHECK_TREESTORE_SNEXT != 0 )
        g_return_val_if_fail( _spos < scard(), NULL );
    #endif

    if ( ( 1 + _spos ) != scard() )
        return get_shown_node(1 + _spos);
    else
        return NULL;
}

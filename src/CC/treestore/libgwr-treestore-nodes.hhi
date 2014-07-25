/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-treestore-nodes.hhi                                              *
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
    *   Purpose     : Header                                                    *
    *                                                                           *
    *****************************************************************************
*/


/// ****************************************************************************
//!
//! \namespace  libgwr::treestore
//!
//! \class      Node
//!
//! \brief      The Node struct contains :
//!     - a row's data.
//!     - a NodeBlock struct, containing its children nodes
//!
/// ****************************************************************************
class   Node
{
    friend class    NodeBlock;
    friend class    NodeRoot;
    friend class    Store;

    private:
    //  ------------------------------------------------------------------------
    enum
    {
        e_UID_SHIFT			= 0												,
        e_UID_BITS			= GWR_B32(00000000,00001111,11111111,11111111)  ,
        e_UID_MASK			= ~e_UID_BITS									,

        e_SHOWN_SHIFT       = 20                                            ,
        e_SHOWN_BITS        = GWR_B32(00000000,00110000,00000000,00000000)  ,
        e_SHOWN_MASK        = ~e_SHOWN_BITS                                 ,

        e_VISIBILITY_SHIFT  = 22											,
        e_VISIBILITY_BITS   = GWR_B32(00000000,01000000,00000000,00000000)  ,
        e_VISIBILITY_MASK   = ~e_VISIBILITY_BITS                            ,

        e_RTNODE_SHIFT      = 31                                            ,
        e_RTNODE_BITS       = GWR_B32(10000000,00000000,00000000,00000000)  ,
        e_RTNODE_MASK       = ~e_RTNODE_BITS
    };
    //  ------------------------------------------------------------------------
    //  static count
    protected:
    static  gint Count;

    public:
    static  gint Remaining()	{ return Count; }
    //  ------------------------------------------------------------------------
    //  bit flags
    protected:
    guint32					a_bits;                                             //!< contain the flag bits

    protected:
    inline  void            flags_set_uid       (guint32 _uid)  { a_bits &= e_UID_MASK;                 a_bits |= ( ((guint32)(_uid  << e_UID_SHIFT   )) & e_UID_BITS   );  }
    inline  void            flags_set_shown     (guint32 _i)    { a_bits &= e_SHOWN_MASK;               a_bits |= ( ((guint32)(_i    << e_SHOWN_SHIFT )) & e_SHOWN_BITS );  }
    inline  void            flags_set_visibility(gboolean _v)   {                               _v ?    a_bits |= e_VISIBILITY_BITS  : a_bits &= e_VISIBILITY_MASK;         }
    inline  void            flags_set_rtnode    ()              {                                       a_bits |= e_RTNODE_BITS;                                            }

    inline  guint32         flags_get_uid       ()              { return ( a_bits & e_UID_BITS		    ) >> e_UID_SHIFT;					}
    inline  guint32         flags_get_shown     ()              { return ( a_bits & e_SHOWN_BITS        ) >> e_SHOWN_SHIFT;                 }
    inline  gboolean        flags_get_visibility()              { return ( a_bits & e_VISIBILITY_BITS   ) != (guint32)0 ? TRUE  : FALSE;	}
    inline  gboolean        flags_get_rtnode    ()              { return ( a_bits & e_RTNODE_BITS       ) != (guint32)0 ? TRUE  : FALSE;	}

    public:
    inline  guint32			uid                 ()		        { return flags_get_uid();                   }
    inline  gboolean		shown               ()	            { return ( flags_get_shown() != 0 );        }
    inline  gboolean		hidden              ()	            { return ( flags_get_shown() == 0 );        }
    inline  gboolean		visibility          ()	            { return flags_get_visibility();            }

    //  ------------------------------------------------------------------------
    //  parent / children / data
    protected:
    Node				*   a_parent;                                           //!< parent Node
    NodeBlock			*   d_children;                                         //!< children
    IData               *   d_data;                                             //!< user data

    public:
    inline  Node		*   parent              ()	            { return a_parent;                                          }
    inline	NodeBlock	*   brothers            ()	            { return a_parent ? a_parent->d_children : NULL;            }
    inline	NodeBlock	*   children            ()	            { return d_children;	                                    }
    inline	IData       *&	data                ()              { return d_data;                                            }
    //  ------------------------------------------------------------------------
    //  hpos / spos
    protected:
    guint16                             a_spos;                                 //!< Shown  index in the parent's NodeBlock
    guint16                             a_hpos;                                 //!< Hidden index in the parent's NodeBlock

    protected:
    inline  void		    inc_spos            ()	            { a_spos++;		        }
    inline  void		    dec_spos            ()	            { a_spos--;             }
    inline  void		    set_spos            (guint16 _p)    { a_spos = _p;          }
    inline  void		    invalidate_spos     ()              { a_spos = LIBGWR_TREESTORE_INVALID_POS;    }

    inline  void		    inc_hpos            ()	            { a_hpos++;		        }
    inline  void		    dec_hpos            ()	            { a_hpos--;             }
    inline  void		    set_hpos            (guint16 _p)    { a_hpos = _p;          }
    inline  void		    invalidate_hpos     ()              { a_hpos = LIBGWR_TREESTORE_INVALID_POS;    }

    protected:
    inline  guint16         spos                ()              { return a_spos;        }
    inline  guint16         hpos                ()              { return a_hpos;        }


    public:
    inline  guint16			pos                 ()	            { return spos();                                    }
    //  ------------------------------------------------------------------------
    //  next, prev, ...
    protected:
    Node				*   a_aprev;                                            //!< prev Absolute node
    Node				*   a_anext;                                            //!< next Absolute node

    protected:
    inline  void	        set_aprev           (Node *_n)	    { a_aprev = _n;         }
    inline  void	        set_anext           (Node *_n)	    { a_anext = _n;         }

    inline  Node        *   aprev               ()              { return a_aprev;       }
    inline  Node        *   anext               ()              { return a_anext;       }
    inline  Node        *   snext               ();

    public:
    inline	Node		*   next                ()	            { return snext();       }
    //  ------------------------------------------------------------------------
    //  depth
    inline  gint			depth               ()              { return a_parent ? a_parent->children()->depth() : 0;  }
    //  ------------------------------------------------------------------------
    //  find
    protected:
            Node        *   find_anext_shown    ();
            Node        *   find_anext_hidden   ();
    //  ------------------------------------------------------------------------
    //  remove
    protected:
            guint32		    remove_child        (guint16 _pos);
            guint32		    remove_children     ();
    //  ------------------------------------------------------------------------
    //  show / hide
    protected:
            gboolean        visibility_changed  ();
    //  ------------------------------------------------------------------------
    //  utils
    inline	gboolean		sterile()	                    { return children()->empty();                       }
            guint32			row();
    //  ------------------------------------------------------------------------
    //  debug
    public:
    const   gchar     *	    log();
            void            dump_tree(guint32 _level = 0);
            gboolean        check();
    //  ------------------------------------------------------------------------
    //  ()~()
    public:
            Node(
                guint32         _uid    ,
                gint            _depth  ,
                guint16         _spos   ,
                guint16         _hpos   ,
                Node        *   _parent,
                IData       *   _data);
    virtual ~Node();
};
//  ----------------------------------------------------------------------------
//  Node::snext()
//  ----------------------------------------------------------------------------
//! \fn     snext()
//!
//! \brief  Return the next shown Node of a shown Node.
//!
//! \return A Node* , eventually NULL.
inline  Node*
Node::snext()
{
    #if ( LIBGWR_CHECK_TREESTORE_SNEXT != 0 )
        g_return_val_if_fail( flags_get_visibility(), NULL );
        g_return_val_if_fail( spos() != 0xffff, NULL );
    #endif

    return visibility() ? brothers()->snext(spos()) : NULL;
}
/// ****************************************************************************
//!
//! \class    NodeRoot
//!
/// ****************************************************************************
class   NodeRoot : public Node
{
    public:
    NodeRoot(guint32 _uid, gint _depth, guint16 _spos, guint16 _hpos, Node *_parent, IData* _data) :
        Node(_uid, _depth, _spos, _hpos, _parent, _data)
    {
        flags_set_rtnode();
        flags_set_shown(2);
        flags_set_visibility(TRUE);
    }

    virtual ~NodeRoot() {}
};

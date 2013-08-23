/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-treestore.h                                                      *
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
#ifndef _LIBGWR_TREESTORE_H_
#define _LIBGWR_TREESTORE_H_

//..............................................................................
#include	<gtk/gtk.h>
//..............................................................................
#include	"libgwr.h"
//##############################################################################
//
//					Object definition
//
//##############################################################################

GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(treestore)

//==============================================================================
//  All structs, for crossed refs in this header
//==============================================================================
struct  Node;
struct  PNode;

struct  NodeRoot;

struct  NodeBlock;
struct  PNodeBlock;

struct  Store;

struct  Path;
struct  IData;
struct  IDataRoot;
//==============================================================================
//  Macros
//==============================================================================
#define ITER_CLR(_iter)													    \
	(_iter)->stamp		= 0;												\
	(_iter)->user_data	= NULL;												\
	(_iter)->user_data2	= NULL;												\
	(_iter)->user_data3	= NULL;

#define ITER_RESET(_iter, T) 										        \
	(_iter)->stamp		= T->stamp();   									\
	(_iter)->user_data	= NULL;												\
	(_iter)->user_data2	= NULL;												\
	(_iter)->user_data3	= NULL;

#define NODE(x)     ( (Node* )x )
#define PNODE(x)    ( (PNode*)x )

#define NODE_FROM_ITER(_node, _iter)										\
	_node = (libgwr::treestore::Node*)(_iter->user_data)

#define ITER_FROM_NODE(_treestore, _iter, _node)							\
	(_iter)->stamp		= _treestore->stamp();								\
	(_iter)->user_data	= _node;											\
	(_iter)->user_data2	= NULL;												\
	(_iter)->user_data3	= _node->data();

#define PNODEBLOCK_ORDERED_CHAINING_CUT(p,n)                                    \
    if ( p )                                                                    \
    {                                                                           \
        if ( n )                                                                \
        {                                                                       \
            PNODE(p)->set_anext(n);                                             \
            PNODE(n)->set_aprev(p);                                             \
        }                                                                       \
        else                                                                    \
        {                                                                       \
            PNODE(p)->set_anext(NULL);                                          \
        }                                                                       \
    }                                                                           \
    else                                                                        \
    {                                                                           \
        if ( n )                                                                \
        {                                                                       \
            PNODE(n)->set_aprev(NULL);                                          \
            a_first_node = n;                                                   \
        }                                                                       \
        else                                                                    \
        {                                                                       \
            a_first_node = NULL;                                                \
        }                                                                       \
    }
//..............................................................................
#define     LIBGWR_TREESTORE_CHECK_LEVEL    1

#if ( LIBGWR_TREESTORE_CHECK_LEVEL != 0 )
    #define NODE_CHECK(node)                                                    \
        node->check();
#else
    #define NODE_CHECK(node)
#endif

#if ( LIBGWR_TREESTORE_CHECK_LEVEL != 0 )
    #define BLOCK_CHECK(block)                                                  \
        block->check();
#else
    #define BLOCK_CHECK(block)
#endif


//==============================================================================
//  Some common things
//==============================================================================
enum eSortType
{
    eSortNone			= 0x00,
    eSortAscending		= 0x01,
    eSortDescending		= 0x02,
};

//==============================================================================

    //!
    //! \struct Path
    //!
    //! \brief  This struct contain a suite of unique ids
    //!
    //! It is made for retrieving iters in the tree, because using GtkTreeIters :
    //! - we should compare users data, which could be longer than comparing uids,
    //! that are integers
    //! - we should browse the tree by calling GnomeCmdFoldviewTrestore::iter_children()
    //! and GnomeCmdFoldviewTrestore::iter_next() many & many times. Instead of this,
    //! one can use a single call to GnomeCmdFoldviewTrestore::ext_iter_from_path()
    //! and GnomeCmdFoldviewTrestore::ext_path_from_iter()
    //!

//==============================================================================
struct Path
{
    private:
    gchar   *   d_ascii_dump;   //!< uids dumped in a string ( for debugging purpose )
    guint32		a_card;         //!< number of uids
    guint32	*   d_uid;          //!< array of uids

    public:
    void	*   operator new	(size_t size);
    void		operator delete (void *p);
                Path			(guint32 card);
                ~Path			();
    public:
    guint32			    card			();
    guint32				uid_get			(gint _pos);
    void				uid_set			(gint _pos, guint32 _uid);
    Path*				dup				();
    const gchar		*	dump			();
};

//==============================================================================

    //!
    //! \struct IData
    //!
    //! Instead of storing boring G_TYPES vars in the store, we can store
    //! any struct that inherits from Treestore::IData
    //! when using Treestore :)
    //!
    //! It allows :
    //!
    //! - calling delete on that struct will directly call g_free on the
    //!   users's data, that inherits from this struct
    //!
    //! - compare iters ; follows the comparizon convention :
    //!   - IData_A.compare_xxx(IData_B)
    //!   - = 0 if IData_A & IData_B cant be ordered
    //!   - < 0 if A is before B
    //!   -  > 0 if A is after B
    //!

//==============================================================================
struct IData
{
    public:
    inline virtual          ~IData()  {}

    public:
    virtual gint            compare                     (IData*)                = 0;
    virtual gint            compare_str                 (const gchar*)          = 0;

    public:
    virtual	void			set_path_from_treestore		(Path*)					= 0;
    virtual Path		*	path						()						= 0;

    //  ........................................................................
    //  visibility
    public:
    virtual gboolean        IGetVisibility              ()                      = 0;
};
template < class T > class IDataAggregate : public IData
{
    private:
    T                           *   a_aggregator;
    Path                        *   a_path;
    //  ........................................................................
    public:
    inline          T           *   aggregator                  ()                  { return a_aggregator;                                      }
    //  ........................................................................
    inline  virtual gint            compare                     (IData* _data)      { return a_aggregator->aggregator_IData_compare(_data);     }
    inline  virtual gint            compare_str                 (const gchar* _str) { return a_aggregator->aggregator_IData_compare_str(_str);  }
    //  ........................................................................
    inline  virtual	void			set_path_from_treestore		(Path* _p)          { a_path = _p;                                              }
    inline  virtual Path		*	path						()                  { return a_path;                                            }
    //  ........................................................................
    inline  virtual gboolean        IGetVisibility              ()                  { return a_aggregator->aggregator_IData_IGetVisibility();    }
    //  ........................................................................
    public:
            IDataAggregate(T* _t)   : a_aggregator(_t)  , a_path(NULL)  {}
            IDataAggregate()        : a_aggregator(NULL), a_path(NULL)  {}
    virtual ~IDataAggregate()
    {
        aggregator()->aggregator_IData_aggregate_destructor_callback();
        if ( a_path ) delete a_path;
    }
};
/**
  *
  ******************************************************************************

  \struct    PNodeBlock

  ******************************************************************************
  *
  * \brief  A NodeBlock struct contains the children Nodes of a Node
  *
  * Internals
  *
  */
struct PNodeBlock
{
    friend struct PNode;
    //  ------------------------------------------------------------------------
    protected:
    static  guint32     Count;
    static  guint32	    Remaining()	{ return Count; }

    protected:
    Node			*   a_parent;                                               //!< parent Node
    guint32				a_depth;                                                //!< depth

    //! First node ( relative to a_anext member of PNode ) ; this node pointer
    //! is too the first element of d_hnodes or d_vnodes.
    Node            *   a_first_node;
    //! Last node ( relative to a_anext member of PNode ) ; this node pointer
    //! is too the last element of d_hnodes or d_vnodes.
    Node            *   a_last_node;

    guint16				a_hcard;                                                //!< number of Hidden Nodes
    guint16				a_scard;                                                //!< number of Shown  Nodes

    GArray			*   d_hnodes;                                               //!< Hidden nodes
    GArray			*   d_snodes;                                               //!< Shown  nodes
    //  ------------------------------------------------------------------------
    //  get / set
    private:
                Node        *       get_hidden_node     (guint16 _pos);
    protected:
    inline		guint16				hcard()		                        { return a_hcard;	}
    inline		gint				hempty()	                        { return ! a_hcard; }

    inline		guint16				scard()		                        { return a_scard;	}
    inline		gint				sempty()	                        { return ! a_scard; }

    inline		guint				depth()		                        { return a_depth;   }
    inline      Node	    *       parent()                            { return a_parent; }

                void                children_clr_fields ();

                Node	    *       get_shown_node	    (guint16 _pos);
    //  ------------------------------------------------------------------------
    //  add
    protected:
                Node	    *	    node_add			(guint32 _uid, eSortType, gint _collate_key_to_use, IData*);
    //  ------------------------------------------------------------------------
    //  find
    protected:
                Node    *   find_sub_node(Path*);
    //  ------------------------------------------------------------------------
    //  remove
    private:
                guint32		        remove_hidden_node  (guint16 _pos);
    protected:
                guint32		        remove_shown_node   (guint16 _pos);
                guint32		        remove_shown_node   (Node*);
                guint32		        remove_hidden_node  (Node*);
                guint32		        remove_all_nodes();
    //  ------------------------------------------------------------------------
    //  show / hide
    private:
                void                r_parent_has_been_shown             ();
                void                r_parent_has_been_hidden            ();
                gboolean            node_move_from_hidden_to_shown      (Node*);
                gboolean            node_move_from_shown_to_hidden      (Node*);
    //  ------------------------------------------------------------------------
    //  debug
    gboolean    check();
    //  ------------------------------------------------------------------------
    protected:
             PNodeBlock(guint32 _depth, Node *_parent);
    virtual ~PNodeBlock();
};
/**
  *
  ******************************************************************************

  \struct    NodeBlock

  ******************************************************************************
  *
  * \brief  A NodeBlock struct contains the children Nodes of a Node
  *
  * We dont store entire node structs for speeding when sorting : we dont move
  * struct, but only pointers. The counterpart is that we are forced to malloc
  * each node.
  *
  * Pointers on hidden and visible nodes are stored in separate GArrays.
  *
  *                 0     1 2   3   <- shown    indexes
  * shown         : A     F-G   K   : The shown array is sorted ( through IData->compare() ).
  *                  \   /   \ /
  * hidden        :   U-D     B     : The hidden array is unsorted.
  *                   0 1     2     <- hidden   indexes
  *
  */
struct  NodeBlock : public PNodeBlock
{
    friend struct Store;
    friend struct Node;
    friend struct PNode;
    friend struct PNodeBlock;
    //  ------------------------------------------------------------------------
    //  get / set
    private:
    inline		guint16				card()		            { return PNodeBlock::scard();   }
    inline		gboolean    		empty()		            { return PNodeBlock::sempty();  }
    inline		guint32				depth()		            { return a_depth;               }
    inline      Node	    *       parent()                { return a_parent;              }
    inline      Node	    *       first_node()            { return a_first_node;          }

    inline      void                children_clr_fields()   { PNodeBlock::children_clr_fields();            }
    inline      Node	*           node_get(guint16 _pos)  { return PNodeBlock::get_shown_node(_pos);      }
    //  ------------------------------------------------------------------------
    //  add
    inline      Node	*	        node_add(   guint32     _uid,
                                                eSortType   _type,
                                                gint        _collate_key_to_use,
                                                            IData* _data)
                                                            { return PNodeBlock::node_add(_uid, _type, _collate_key_to_use, _data); }
    //  ------------------------------------------------------------------------
    //  find
    private:
    inline      Node    *   find_sub_node(Path* _path)      { return PNodeBlock::find_sub_node(_path);      }
    //  ------------------------------------------------------------------------
    //  remove
    inline      guint32     remove_one_node(guint16 _pos)   { return PNodeBlock::remove_shown_node(_pos);   }
    inline      guint32     remove_all_nodes()              { return PNodeBlock::remove_all_nodes();        }
    //  ------------------------------------------------------------------------
    private:
    void*		operator new	(size_t size);
    void		operator delete (void *p);

             NodeBlock(guint32 _depth, Node *_parent) : PNodeBlock(_depth, _parent) {}
    virtual ~NodeBlock() {}
};
/**
  *
  ******************************************************************************

  \struct    PNode

  ******************************************************************************
  *
  * \struct   PNode
  *
  * The Node struct contains :
  *     - a row's data.
  *     - a NodeBlock struct, containing its children nodes
  *
  */
struct PNode
{
    friend struct   PNodeBlock;
    friend struct   NodeRoot;

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
    private:
    guint32								a_bits;     //!< contain the flag bits

    protected:
    Node							*   a_parent;                               //!< parent Node
    NodeBlock						*   d_children;                             //!< children
    IData                           *	d_data;                                 //!< user data
    guint16                             a_spos;                                 //!< Shown  index in the parent's NodeBlock
    guint16                             a_hpos;                                 //!< Hidden index in the parent's NodeBlock
    Node							*   a_aprev;                                //!< prev Absolute node
    Node							*   a_anext;                                //!< next Absolute node
    Node							*   a_snext;                                //!< next Shown node
    //  ------------------------------------------------------------------------
    //  get / set
    private:
    inline  void            set_uid             (guint32 _uid)  { flags_set_uid(_uid);  }

    inline  void		    inc_spos            ()	            { a_spos++;		        }
    inline  void		    dec_spos            ()	            { a_spos--;             }
    inline  void		    set_spos            (guint16 _p)    { a_spos = _p;          }

    inline  void		    inc_hpos            ()	            { a_hpos++;		        }
    inline  void		    dec_hpos            ()	            { a_hpos--;             }
    inline  void		    set_hpos            (guint16 _p)    { a_hpos = _p;          }

    inline  void	        set_aprev           (Node *_n)	    { a_aprev = _n;         }
    inline  void	        set_anext           (Node *_n)	    { a_anext = _n;         }
    inline  void	        set_snext           (Node *_n)	    { a_snext = _n;         }

    inline  void            flags_set_uid       (guint32 _uid)  { a_bits &= e_UID_MASK;                 a_bits |= ( ((guint32)(_uid  << e_UID_SHIFT   )) & e_UID_BITS   );  }
    inline  void            flags_set_shown     (guint32 _i)    { a_bits &= e_SHOWN_MASK;               a_bits |= ( ((guint32)(_i    << e_SHOWN_SHIFT )) & e_SHOWN_BITS );  }
    inline  void            flags_set_visibility(gboolean _v)   {                               _v ?    a_bits |= e_VISIBILITY_BITS  : a_bits &= e_VISIBILITY_MASK;         }
    inline  void            flags_set_rtnode    ()              {                                       a_bits |= e_RTNODE_BITS;                                            }

    protected:
    inline  guint32         flags_get_uid       ()              { return ( a_bits & e_UID_BITS		    ) >> e_UID_SHIFT;					}
    inline  guint32         flags_get_shown     ()              { return ( a_bits & e_SHOWN_BITS        ) >> e_SHOWN_SHIFT;                 }
    inline  gboolean        flags_get_visibility()              { return ( a_bits & e_VISIBILITY_BITS   ) != (guint32)0 ? TRUE  : FALSE;	}
    inline  gboolean        flags_get_rtnode    ()              { return ( a_bits & e_RTNODE_BITS       ) != (guint32)0 ? TRUE  : FALSE;	}

    inline  Node        *   aprev               ()              { return a_aprev;       }
    inline  Node        *   anext               ()              { return a_anext;       }
    inline  Node        *   snext               ()              { return a_snext;       }
    inline  guint16         spos                ()              { return a_spos;        }
    inline  guint16         hpos                ()              { return a_hpos;        }

    inline  Node		*   parent              ()	            { return a_parent;                                          }
    inline	NodeBlock	*   brothers            ()	            { return a_parent ? PNODE(a_parent)->d_children : NULL;     }
    inline	NodeBlock	*   children            ()	            { return d_children;	                                    }
    inline	IData       *&	data                ()              { return d_data;                                            }

    //  ------------------------------------------------------------------------
    //  find
    private:
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
    //  debug
    public:
            const gchar     *	log();
            void                dump_tree(guint32 _level = 0);
            gboolean            check();
    //  ------------------------------------------------------------------------
    protected:
            PNode(
                guint32         _uid    ,
                gint            _depth  ,
                guint16         _spos   ,
                guint16         _hpos   ,
                Node        *   _parent,
                IData       *   _data);
    virtual ~PNode();
};
/**
  *
  ******************************************************************************

  \struct    Node

  ******************************************************************************
  *
  * \struct   Node
  *
  * \brief  This class is mainly a wrapper for PNode, which it ineherits from.
  *     It wraps on :
  *         - PNode    methods, which modify internals
  *         - parent() methods, when necessary
  *
  */
struct Node : public PNode
{
    friend struct PNode;
    friend struct Store;
    friend struct NodeBlock;
    friend struct PNodeBlock;
    private:
    //  ------------------------------------------------------------------------
    static  gint Count;
    static  gint Remaining()	{ return Count; }
    //  ------------------------------------------------------------------------
    //  get / set
    inline  guint32				uid             ()		        { return    PNode::flags_get_uid();                 }

    inline  gboolean			shown           ()	            { return  ( PNode::flags_get_shown() != 0 );        }
    inline  gboolean			hidden          ()	            { return  ( PNode::flags_get_shown() == 0 );        }
    inline  gboolean			visibility      ()	            { return  PNode::flags_get_visibility();            }

    inline  guint16			    pos             ()	            { return a_spos;                                    }
    inline  gint				depth           ()              { return a_parent ? a_parent->children()->depth() : 0;  }

    inline  Node			*   parent          ()	            { return a_parent;                                  }
    inline	Node			*   next            ()	            { return a_snext;                                   }
    inline	NodeBlock		*   brothers        ()	            { return PNode::brothers();                         }
    inline	NodeBlock		*   children        ()	            { return d_children;	                            }

    inline	gboolean		    sterile()	                    { return children()->empty();                       }

    inline	IData           *&	data()		                    { return d_data;		                            }

            guint32				row();
    //  ------------------------------------------------------------------------
    //  remove
    private:
    inline  guint16				remove_one_child(guint16 _pos)  { return PNode::remove_child(_pos);                 }
    inline  guint16				remove_all_children()           { return PNode::remove_children();                  }
    //  ------------------------------------------------------------------------
    //  debug
    public:
    inline  const gchar     *	log()                           { return PNode::log();          }
    inline  void                dump_tree(guint32 _level = 0)   { PNode::dump_tree(_level);     }
    //  ------------------------------------------------------------------------
    protected:
    void*		operator new	(size_t size);
    void		operator delete (void *p);

    Node(guint32 _uid, gint _depth, guint16 _spos, guint16 _hpos, Node *_parent, IData* _data) :
        PNode(_uid, _depth, _spos, _hpos, _parent, _data)       {}
    virtual ~Node() {}
};
/**
  *
  ******************************************************************************

  \struct    NodeRoot

  ******************************************************************************
  *
  * \struct   NodeRoot
  *
  */
struct NodeRoot : public Node
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
/**
  *
  ******************************************************************************

  \struct    IDataRoot

  ******************************************************************************
  *
  * \struct   IDataRoot
  *
  */
struct  IDataRoot : public IData
{
    public:
    IDataRoot()
    {
    }
    inline virtual          ~IDataRoot()
    {
    }

    public:
    virtual gint            compare                     (IData*)        { g_assert(FALSE); return 0;        }
    virtual gint            compare_str                 (const gchar*)  { g_assert(FALSE); return 0;        }

    public:
    virtual	void			set_path_from_treestore		(Path*)         { g_assert(FALSE);                  }
    virtual Path		*	path						()              { g_assert(FALSE) ; return NULL;    }

    //  ........................................................................
    //  visibility
    public:
    virtual gboolean        IGetVisibility              ()              { return TRUE;                          }
};

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
    friend struct PNodeBlock;
    friend struct PNode;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//  GObject inheritance
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    public:
    GObject             parent;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//  static vars for Node & NodeBlock access to the current Store without
    //  polluting the x86 stack
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    private:
    static  Store           *   s_store;
    static  gboolean            s_ret_gboolean;
    static  guint32_x2          s_ret_guint32_x2;

    private:
    static  void        Set_static_store(Store* _s) { s_store = _s;     }
    static  Store   *   Get_static_store()          { return s_store;   }
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//  This one is THE root node, hidden, and father
	//  of all nodes
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	public:
	gboolean			a_node_root_created;
	NodeRoot        *   d_node_root;
    IDataRoot       *   d_data_root;

	public:
	inline  Node    *	node_root()			{ return d_node_root; }
			void		node_root_init();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//  uids
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	private:
	guint32				a_uid;

	public:
	void				uid_init()	{ a_uid = 0; }
	guint32				uid_new()   { return ++a_uid; }
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//  stamp
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	private:
	gint        		a_stamp;

	public:
	inline  gint		stamp()			{ return a_stamp;			}
	void				stamp_init()	{ a_stamp = 0x00;			}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//  sorting
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	eSortType	a_sort_type;
	gint		a_sort_collate_key_to_use;

	public:
	gboolean	must_sort_ascending()		{ return a_sort_type == eSortAscending;		}
	gboolean	must_sort_descending()		{ return a_sort_type == eSortDescending;	}

	public:
	void		set_sort_type(eSortType sort_type)
                {
                    a_sort_type	= sort_type;
                }
	void		set_sort_collate_key_to_use(gint sort_collate_key_to_use)
				{
					a_sort_collate_key_to_use	= sort_collate_key_to_use;
				}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// some helpers
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//
	// iter validation
	//
	private:
	gboolean    iter_is_valid(GtkTreeIter*);
	gboolean    iter_is_valid_but_may_be_null(GtkTreeIter*);

	//
	// helpers for sending signals
	//
	private:
	void		emit_row_inserted			(Node*);
	void		emit_row_changed			(Node*);
	void		emit_row_deleted			(Node*);
	void		emit_row_has_child_toggled	(Node*);

	//
	// private
	//
	private:
	guint32		iter_get_uid						(GtkTreeIter*);

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

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //! \defgroup   _grp_treestore_extensions_nodes_ GtkTreeModelInterface extensions : Nodes related
    //! \ingroup    _grp_treestore_
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //! @{
	public:
    void        ext_dump_tree                       (GtkTreeIter *);
	void		ext_add_child						(GtkTreeIter *_in_parent,	GtkTreeIter *_out_child, IData*);
	void		ext_set_data						(GtkTreeIter *in,			IData*);
	void		ext_data_changed					(GtkTreeIter *in);
	gboolean	ext_get_data						(GtkTreeIter *in,			IData**);
	gint		ext_clear();
	//gboolean	ext_get_root						(GtkTreeIter *in,			GtkTreeIter *out_root);
	gboolean	ext_is_root							(GtkTreeIter *in);
	gint		ext_iter_depth						(GtkTreeIter *in);
	gchar*		ext_get_gtk_path_str_new			(GtkTreeIter *in);
	gint		ext_iter_get_row					(GtkTreeIter *in);
	gint		ext_iter_remove_children_no_signal_row_deleted  (GtkTreeIter *in);
	gint	    ext_iter_sterile_remove				(GtkTreeIter *in);
	//guint32	    ext_iter_remove				        (GtkTreeIter *in);      _GWR_TODO_
    gboolean    ext_iter_is_shown                   (GtkTreeIter *);
    gboolean    ext_iter_is_hidden                  (GtkTreeIter *);
	gboolean    ext_iter_visibility_changed			(GtkTreeIter*, guint32_x2*);
    //! @}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //! \defgroup   _grp_treestore_extensions_paths_ GtkTreeModelInterface extensions : paths of uids
    //! \ingroup    _grp_treestore_
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //! @{
	public:
	gboolean	ext_iter_from_path  (Path*path, GtkTreeIter *_iter_out);
	Path*	    ext_path_from_iter  (GtkTreeIter* _iter_in);
    //! @}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //! \defgroup   _grp_treestore_extensions_match_ GtkTreeModelInterface extensions : Nodes matching
    //! \ingroup    _grp_treestore_
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //! @{
	public:
	gboolean	ext_match_child_gint		(GtkTreeIter *in_parent, GtkTreeIter *out_child, gboolean(*)(IData*, gint),  gint the_gint);
	gboolean	ext_match_child_str         (GtkTreeIter *in_parent, GtkTreeIter *out_child,							 const gchar * the_str);
    //! @}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Divers
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void	    resort();

	public:
	gint		refcount() { return ((GInitiallyUnowned*)this)->ref_count; }

};

//##############################################################################
//
//					GLib stuff
//
//##############################################################################
GType         	store_get_type  (void);
Store    *		store_new		(void);

#define LIBGWR_TREESTORE_TYPE            (libgwr::treestore::store_get_type())
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

GWR_NAMESPACE_END(treestore)
GWR_NAMESPACE_END(libgwr)



#endif  // __LIBGWR_TREESTORE_H__

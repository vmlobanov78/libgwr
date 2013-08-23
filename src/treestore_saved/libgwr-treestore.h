/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-treestore.h                                                      *
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
//..............................................................................

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
//  All structs, for crossed refs in this header
//==============================================================================
struct  Node;
struct  NodeBlock;

struct  Path;
struct  DataInterface;

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
    gint		a_card;         //!< number of uids
    guint32	*   d_uid;          //!< array of uids

    public:
    void	*   operator new	(size_t size);
    void		operator delete (void *p);
                Path			(gint card);
                ~Path			();
    public:
    gint				card			();
    guint32				uid_get			(gint _pos);
    void				uid_set			(gint _pos, guint32 _uid);
    Path*				dup				();
    const gchar		*	dump			();
};

//==============================================================================

    //!
    //! \struct DataInterface
    //!
    //! Instead of storing boring G_TYPES vars in the store, we can store
    //! any struct that inherits from Treestore::DataInterface
    //! when using Treestore :)
    //!
    //! It allows :
    //!
    //! - calling delete on that struct will directly call g_free on the
    //!   users's data, that inherits from this struct
    //!
    //! - compare iters ; follows the comparizon convention :
    //!   - DataInterface_A.compare_xxx(DataInterface_B)
    //!   - = 0 if DataInterface_A & DataInterface_B cant be ordered
    //!   - < 0 if A is before B
    //!   -  > 0 if A is after B
    //!

//==============================================================================
struct DataInterface
{
    public:
    inline virtual ~DataInterface() {}

    public:
    virtual gint            compare                     (DataInterface*)            = 0;
    virtual gint            compare_str                 (const gchar*)              = 0;

    public:
    virtual	void			set_path_from_treestore		(Path*)						= 0;
    virtual Path		*	path						()							= 0;
};

//==============================================================================

    //!
    //! \struct NodeBlock
    //!
    //! \brief  A NodeBlock struct contains the children Nodes of a Node
    //!

//==============================================================================
struct  NodeBlock
{
    friend struct Store;
    friend struct Node;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    private:
    static  gint Count;
    public:
    static  gint	Remaining()	{ return Count; }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    private:
    gint				a_card;     //!< number of Nodes
    guint				a_depth;    //!< depth
    Node			*   a_parent;   //!< parent Node

    //! d_nodes is an GArray of node* ; we dont store entire node structs
    //! for speeding when sorting : we dont move struct, but only pointers. The
    //! counterpart is that we are forced to malloc each node
    //! \todo We should use something lighter than a GArray
    GArray			*   d_nodes;    //!< Nodes contained
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    public:
    void*		operator new	(size_t size);
    void		operator delete (void *p);

     NodeBlock  (guint _depth, Node *_parent);
    ~NodeBlock  ();
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    private:
    gint		remove_node			(gint	pos);
    gint		remove_nodes		();
    //.....................................................................
    public:
    inline		gint				card()		{ return a_card;	}
    inline		gint				empty()		{ return ! a_card; 	}
    inline		guint				depth()		{ return a_depth;   }
    inline		GArray*				array()		{ return d_nodes;   }

    void        reset();
    Node	*   parent()			{ return a_parent; }
    Node	*   node_get			(gint	pos);
    Node	*	node_add			(guint32 _uid, eSortType, gint _collate_key_to_use, DataInterface*);
};

//==============================================================================

    //!
    //! \struct   Node
    //!
    //! The Node struct contains :
    //!     - a row's data.
    //!     - a NodeBlock struct, containing its children nodes
    //!

//==============================================================================
#define NODE_FROM_ITER(_node, _iter)										\
	_node = (Node*)_iter->user_data

#define ITER_FROM_NODE(_treestore, _iter, _node)							\
	(_iter)->stamp		= _treestore->stamp();								\
	(_iter)->user_data	= _node;											\
	(_iter)->user_data2	= NULL;												\
	(_iter)->user_data3	= _node->data();

#define ITER_RESET(_iter)													\
	(_iter)->stamp		= 0;												\
	(_iter)->user_data	= NULL;												\
	(_iter)->user_data2	= NULL;												\
	(_iter)->user_data3	= NULL;

struct Node
{
    friend struct Store;
    friend struct NodeBlock;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    private:
    enum
    {
        e_UID_SHIFT			= 0												,
        e_UID_BITS			= GWR_B32(00000000,00001111,11111111,11111111)  ,
        e_UID_MASK			= ~e_UID_BITS									,

        e_VISIBLE_SHIFT		= 20											,
        e_VISIBLE_BITS		= GWR_B32(00000000,00010000,00000000,00000000)  ,
        e_VISIBLE_MASK		= ~e_VISIBLE_BITS
    };
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    private:
    static  gint Count;
    public:
    static  gint Remaining()	{ return Count; }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    private:
    guint32								a_bits;     //!< contain the flag bits
    gint								a_pos;      //!< index in the parent's NodeBlock
    Node							*   a_parent;   //!< parent Node
    Node							*   a_next;     //!< next node
    NodeBlock						*   a_children; //!< children
    DataInterface					*	d_data;     //!< user data
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    public:
    void*		operator new	(size_t size);
    void		operator delete (void *p);

     Node(guint32 _uid, gint _depth, gint _pos, Node *_parent, DataInterface* _data);
    ~Node();
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    private:
            gint				remove_child(gint _pos);
            gint				remove_children();
    //.....................................................................
    public:
    const gchar*				log         ();
    void                        dump_tree   (gint _level = 0);

    inline  guint32				uid()		        { return ( a_bits & e_UID_BITS		) >> e_UID_SHIFT;					}
    inline  gboolean			visible()	        { return ( a_bits & e_VISIBLE_BITS  ) != (guint32)0 ? TRUE  : FALSE;	}
    inline  gboolean			hidden()	        { return ( a_bits & e_VISIBLE_BITS  ) == (guint32)0 ? FALSE : TRUE;		}
    inline  gint				pos()		        { return a_pos;			                                                }
    inline  void				inc_pos()		    { a_pos += 1;    		                                                }
    inline  void				dec_pos()		    { a_pos -= 1;    		                                                }
    inline  void				set_pos(gint _p)    { a_pos = _p;			                                                }
    inline  gint				depth()             { return parent() ? parent()->children()->depth() : 0;                  }
    inline  Node			*   parent()	        { return a_parent;		                                                }
    inline	Node			*   next()		        { return a_next;		                                                }
    inline	void		        set_next(Node *_n)	{ a_next = _n;  		                                                }
    inline	NodeBlock		*   children()	        { return a_children;	                                                }
    inline	gboolean		    is_sterile()	    { return a_children->empty();	                                        }
    inline	DataInterface   *&	data()		        { return d_data;		                                                }
            gint				row();
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
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//  GObject inheritance
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    public:
    GObject         parent;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//  This one is THE root node, hidden, and father
	//  of all nodes
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	public:                                                                     // TODO
	gboolean			a_node_root_created;
	Node			*   d_node_root;

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
	// private helpers for sending signals
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
	void		ext_add_child						(GtkTreeIter *_in_parent,	GtkTreeIter *_out_child, DataInterface*);
	void		ext_set_data						(GtkTreeIter *in,			DataInterface*);
	void		ext_data_changed					(GtkTreeIter *in);
	gboolean	ext_get_data						(GtkTreeIter *in,			DataInterface**);
	gint		ext_clear();
	gboolean	ext_get_root						(GtkTreeIter *in,			GtkTreeIter *out_root);
	gboolean	ext_is_root							(GtkTreeIter *in);
	gint		ext_iter_depth						(GtkTreeIter *in);
	gchar*		ext_get_gtk_path_str_new			(GtkTreeIter *in);
	gint		ext_iter_get_row					(GtkTreeIter *in);
	gint		ext_iter_remove_children_no_signal_row_deleted  (GtkTreeIter *in);
	gint	    ext_iter_sterile_remove				(GtkTreeIter *in);
    //! @}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //! \defgroup   _grp_treestore_extensions_paths_ GtkTreeModelInterface extensions : paths of uids
    //! \ingroup    _grp_treestore_
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //! @{
	public:
	gboolean	ext_iter_from_path(			Path *path, GtkTreeIter *_iter_out);
	gboolean	ext_iter_from_path(const	Path *path, GtkTreeIter *_iter_out);
	Path*	    ext_path_from_iter(GtkTreeIter *_iter_in);
    //! @}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //! \defgroup   _grp_treestore_extensions_match_ GtkTreeModelInterface extensions : Nodes matching
    //! \ingroup    _grp_treestore_
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //! @{
	public:
	gboolean	ext_match_child_gint		(GtkTreeIter *in_parent, GtkTreeIter *out_child, gboolean(*)(DataInterface*, gint),  gint the_gint);
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

}       // namespace treestore
}       // namespace libgwr

#endif  // __LIBGWR_TREESTORE_H__

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

//  ...........................................................................
#include	<gtk/gtk.h>
#include	<glib/gprintf.h>
//  ...........................................................................
#include    <sys/stat.h>
#include    <unistd.h>
#include    <stdlib.h>
#include    <errno.h>
#include    <string.h>
//  ...........................................................................
#include    "libgwr-define-optim.h"
#include    "libgwr-common.h"
#include	"libgwr-macro.h"
#include	"libgwr-object.h"
#include	"libgwr-t-array.h"
#include	"libgwr-t-m-array-p.h"
#include	"libgwr-stack.h"

GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(treestore)

//==============================================================================
//  All structs, for crossed refs in this header
//==============================================================================
class   Node;
class   NodeRoot;
struct  NodeBlock;
struct  Store;

struct  Path;
struct  IData;
struct  IDataRoot;

typedef libgwr::TMArrayP < libgwr::treestore::Node , guint16 >  libgwr_treestore_nodeblock_array;

//==============================================================================
//  Macros
//==============================================================================
#define LIBGWR_TREESTORE_INVALID_POS    0xffff

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

#define NODE_FROM_ITER(_node, _iter)										\
	_node = (libgwr::treestore::Node*)(_iter->user_data)

#define ITER_FROM_NODE(_treestore, _iter, _node)							\
	(_iter)->stamp		= _treestore->stamp();								\
	(_iter)->user_data	= _node;											\
	(_iter)->user_data2	= NULL;												\
	(_iter)->user_data3	= _node->data();

#define NODEBLOCK_ORDERED_CHAINING_CUT(p,n)                                     \
    if ( p )                                                                    \
    {                                                                           \
        if ( n )                                                                \
        {                                                                       \
            p->set_anext(n);                                                    \
            n->set_aprev(p);                                                    \
        }                                                                       \
        else                                                                    \
        {                                                                       \
            p->set_anext(NULL);                                                 \
        }                                                                       \
    }                                                                           \
    else                                                                        \
    {                                                                           \
        if ( n )                                                                \
        {                                                                       \
            n->set_aprev(NULL);                                                 \
            a_first_node = n;                                                   \
        }                                                                       \
        else                                                                    \
        {                                                                       \
            a_first_node = NULL;                                                \
        }                                                                       \
    }



//  ****************************************************************************
//
//  Some common things
//
//  ****************************************************************************
enum eSortType
{
    eSortNone			= 0x00,
    eSortAscending		= 0x01,
    eSortDescending		= 0x02,
};
/// ****************************************************************************
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
/// ****************************************************************************
struct Path
{
    private:
    gchar   *   d_ascii_dump;   //!< uids dumped in a string ( for debugging purpose )
    guint32		a_card;         //!< number of uids
    guint32	*   d_uid;          //!< array of uids

    public:
                Path			(guint32 card);
    virtual     ~Path			();

    private:
    inline      guint32     *       puid()  { return d_uid;         }
    inline      void        *       vpuid() { return (void*)d_uid;  }

    public:
    inline      guint32			    card()                              { return a_card;        }
    inline      guint32             uid_get(gint _pos)                  { return d_uid[_pos];   }
    inline      void                uid_set(gint _pos, guint32 _uid)    { d_uid[_pos] = _uid;   }

    Path*				dup				();
    const gchar		*	dump			();
};
/// ****************************************************************************
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
/// ****************************************************************************
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
    inline  virtual gboolean        IGetVisibility              ()                  { return a_aggregator->aggregator_IData_IGetVisibility();   }
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
//  ****************************************************************************
//
//					            NODE BLOCKS
//
//  ****************************************************************************
#include    "libgwr-treestore-node-blocks.hi"
//  ****************************************************************************
//
//					            NODES
//
//  ****************************************************************************
#include    "libgwr-treestore-nodes.hi"

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

//  ****************************************************************************
//
//					            TreeModel
//
//  ****************************************************************************
//!
//!  \struct  Store
//!
//!   The purpose of this struct is :
//!       - implement GtkTreeModelInterface
//!       - implement useful other features
//!
//!   \defgroup _grp_treestore_ Store
//!
struct Store
{
    friend struct NodeBlock;
    friend struct Node;

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

//  ****************************************************************************
//
//					            Glib stuff
//
//  ****************************************************************************
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

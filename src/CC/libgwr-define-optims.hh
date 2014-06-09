/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-define-optims.hh                                                 *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Copyright (C) 2011-2013 Guillaume Wardavoir                             *
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
    *   Purpose : libgwr optimizations                                          *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_DEFINE_OPTIMS_HH__
#define     __LIBGWR_DEFINE_OPTIMS_HH__

//  ****************************************************************************
//  libgwr::treestore
//  ****************************************************************************
#define     LIBGWR_TREESTORE_CHECK_NODES                                        1
#define     LIBGWR_TREESTORE_CHECK_NODEBLOCKS                                   1

#define     LIBGWR_CHECK_TREESTORE_PATH                                         1

#define     LIBGWR_CHECK_TREESTORE_NODEBLOCK_DEC_SPOS_FROM_NODE                 1
#define     LIBGWR_CHECK_TREESTORE_NODEBLOCK_INC_SPOS_FROM_NODE                 1

#define     LIBGWR_CHECK_TREESTORE_SNEXT                                        1

#define     LIBGWR_LOGTUNE_CHECK_INTEGRITY                                      1
//  ----------------------------------------------------------------------------
#if ( LIBGWR_TREESTORE_CHECK_NODES != 0 )
#define NODE_CHECK(node)                                                        \
        node->check();
#else
#define NODE_CHECK(node)
#endif

#if ( LIBGWR_TREESTORE_CHECK_NODEBLOCKS != 0 )
#define BLOCK_CHECK(block)                                                      \
        block->check();
#else
#define BLOCK_CHECK(block)
#endif
//  ****************************************************************************
//
//  ****************************************************************************







#endif                                                                          //  __LIBGWR_DEFINE_OPTIMS_HH__

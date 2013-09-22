/*
    *****************************************************************************
    *                                                                           *
    *   libgwr.h                                                                *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Part of libwgwr                                                         *
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
    *   Class   : Object                                                        *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_H__
#define     __LIBGWR_H__
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
#include    "libgwr-common.h"
#include	"libgwr-macro.h"
#include	"libgwr-counter.h"
#include	"libgwr-collection.h"
#include	"libgwr-str.h"
#include	"libgwr-env.h"
#include	"libgwr-stack.h"
#include	"libgwr-spacer.h"
#include	"libgwr-object.h"
#include	"libgwr-pipe.h"
#include	"libgwr-color.h"
#include	"libgwr-text-style.h"
#include	"libgwr-text-attributes.h"
#include	"libgwr-chrono.h"
//  ...........................................................................
#include    "libgwr-svipc.h"
//  ...........................................................................
#include	"widget/libgwr-widget-gtk-utils.h"
#include	"widget/libgwr-widget-textview.h"
#include	"widget/libgwr-widget-menu.h"
#include	"libgwr-logger.h"
//  ...........................................................................
#include    "libgwr-treestore.h"
//  ...........................................................................
GWR_NAMESPACE_START(libgwr)

extern  Logger      *   GD_libgwr_logger;
extern  guint32         GA_libgwr_logger_channel;

inline  void            P_set_logger(Logger* _logger, guint32 _channel)
{
    GD_libgwr_logger            = _logger;
    GA_libgwr_logger_channel    = _channel;
}

//  ............................................................................
//  GLib extensions
extern  guint32     g_ptr_array_find_first_free_index   (GPtrArray*, gboolean   _allocate = TRUE            );
extern  gpointer    g_ptr_array_get                     (GPtrArray*, guint32    _index                      );
extern  gboolean    g_ptr_array_set                     (GPtrArray*, guint32    _index          , gpointer  );
//  ............................................................................
GWR_NAMESPACE_END(libgwr)

#endif

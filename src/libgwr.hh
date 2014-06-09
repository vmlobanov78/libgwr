/*
    *****************************************************************************
    *                                                                           *
    *   libgwr.hh                                                               *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Part of libgwr                                                          *
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
    *   Purpose : Main include for compiling against libgwr.a                   *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_H__
#define     __LIBGWR_H__
//  ...........................................................................
#include	<gtk/gtk.h>
#include	<glib/gprintf.h>
//  ...........................................................................
#include    "libgwrc.h"
//  ...........................................................................
#include    <sys/stat.h>
#include    <unistd.h>
#include    <stdlib.h>
#include    <errno.h>
#include    <string.h>
//  ...........................................................................
#include	"CC/libgwr-define-settings.hh"
#include    "CC/libgwr-define-optims.hh"

#include	"CC/libgwr-macro.hh"
#include	"CC/libgwr-counter.hh"
#include	"CC/libgwr-types.hh"
#include	"CC/libgwr-chrono.hh"
#include	"CC/libgwr-collection.hh"

#include    "C/libgwrc-array-equal.h"
#include    "C/libgwrc-array-data-multi.h"

#include	"CC/libgwr-t-array.hh"
#include	"CC/libgwr-t-array-p.hh"
#include	"CC/libgwr-t-m-array-p.hh"
#include	"CC/libgwr-kv-array.hh"
#include	"CC/libgwr-object.hh"

#include    "CC/libgwr-str.hh"

#include	"CC/libgwr-file.hh"
#include	"CC/libgwr-stack.hh"
#include	"CC/libgwr-spacer.hh"
#include	"CC/libgwr-pipe.hh"
#include	"CC/libgwr-color.hh"
#include	"CC/libgwr-text-style.hh"
#include	"CC/libgwr-text-attributes.hh"
//  ...........................................................................
#include    "CC/libgwr-svipc.hh"
//  ...........................................................................
#include	"CC/widget/libgwr-widget-gtk-matrix.hh"
#include	"CC/widget/libgwr-widget-textview.hh"
#include	"CC/widget/libgwr-widget-menu.hh"
#include	"CC/widget/libgwr-widget-extensible-header.hh"
#include	"CC/widget/libgwr-widget-gtk-fasttextview.hh"
#include	"CC/logger/libgwr-logger.hh"
#include	"CC/gen-log/libgwr-gen-log.hh"
#include	"CC/widget/libgwr-widget-gen-log-tune.hh"
//  ...........................................................................
#include    "CC/treestore/libgwr-treestore.hh"
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

#endif                                                                          //  __LIBGWR_H__

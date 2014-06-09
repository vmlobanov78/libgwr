/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc.h                                                               *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Part of libgwrc                                                         *
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
    *   Purpose : Main include for compiling against libgwrc.a                  *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWRC_H__
#define     __LIBGWRC_H__
//  ...........................................................................
#include    <string.h>
//  ...........................................................................
#include    <glib.h>
//  ...........................................................................
#include	"C/libgwrc-define-settings.h"
#include	"C/libgwrc-define-optims.h"
#include	"C/libgwrc-macro.h"
#include	"C/libgwrc-define-logs.h"

#include	"C/libgwrc-types.h"
#include	"C/libgwrc-color.h"
#include	"C/libgwrc-hash.h"

#include	"C/libgwrc-array-equal.h"
#include	"C/libgwrc-array-data-multi.h"

#include	"C/widget/libgwrc-gwrgtk-utils.h"
#include	"C/widget/libgwrc-gwrgtk-textview.h"
#include	"C/widget/libgwrc-gwrgtk-textbuffer.h"
//  ...........................................................................
#endif                                                                          //  __LIBGWRC_H__

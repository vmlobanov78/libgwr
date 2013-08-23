/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-pipe.h                                                           *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Gnome Universal Logger Library                                          *
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
    *   Nspace  :                                                               *
    *                                                                           *
    *   Class   :                                                               *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_PIPE_H__
#define     __LIBGWR_PIPE_H__
//  ...........................................................................
//  ...........................................................................
namespace libgwr
{


/**
  *
  ******************************************************************************

  \class  PipeHelper

  ******************************************************************************
  *
  * \brief  Encapsulation of an unix pipe
  *
  */
class PipeHelper
{
    public:
    enum eDirection
    {
        eRead   = 0 ,
        eWrite  = 1
    };
    private:
    gboolean    a_pipe_created;
    int         a_file_descriptor[2];

    public:
    inline  int *   desc()              { return a_file_descriptor;             }
    inline  int     desc(eDirection _d) { return a_file_descriptor[(int)_d];    }

    public:
     PipeHelper();
    ~PipeHelper();

    gboolean        pipe_create();                                              //!< create the unix pipe
    gboolean        pipe_delete();                                              //!< delete the unix pipe

    gboolean        redirect_stdin();                                           //!< set stdin  to read  side of pipe
    gboolean        redirect_stdout();                                          //!< set stdout to write side of pipe
    gboolean        redirect_stderr();                                          //!< set stderr to write side of pipe
    gboolean        redirect_reset();

    gboolean        unblock_read();                                             //!< set read call non blocant
};




}   // libgwr

#endif  // __LIBGWR_PIPE_H__

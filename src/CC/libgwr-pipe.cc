/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-pipe.cc                                                          *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Gnome Universal Logger Library                                          *
    *                                                                           *
    *   Copyright (C) 2011-2014 Guillaume Wardavoir                             *
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

//  ............................................................................
#include    "libgwr.hh"
//  ............................................................................
#include    <unistd.h>
#include    <fcntl.h>
//  ............................................................................
namespace libgwr
{

PipeHelper::PipeHelper()
{
    a_pipe_created  = FALSE;
}
PipeHelper::~PipeHelper()
{
    redirect_reset();
    pipe_delete();
}


gboolean
PipeHelper::pipe_create()
{
    //..........................................................................
    //
    //      #include <unistd.h>
    //      int pipe(int filedes[2]);
    //
    //      pipe() creates a pair of file descriptors, pointing to a pipe inode,
    //      and places them in the array pointed to by filedes. filedes[0] is
    //      for reading, filedes[1] is for writing. "
    //
    //      Errors :
    //      EFAULT  : filedes is not valid.
    //      EMFILE  : Too many file descriptors are in use by the process.
    //      ENFILE  : The system limit on the total number of open files has been reached.
    //
    //..........................................................................

    // couldnt create pipe
    if ( pipe(desc()) == -1 )
        return FALSE;

    a_pipe_created = TRUE;

    return TRUE;
 }
gboolean
PipeHelper::pipe_delete()
{
    return FALSE;
}


gboolean
PipeHelper::redirect_stdin()
{
    if ( ! a_pipe_created )
        return FALSE;

    // close unused "write" side of pipe ( unuseful on linux )
    close( desc(PipeHelper::eWrite) );

    // redirect stdin
    if ( dup2(desc(PipeHelper::eRead), STDIN_FILENO) == -1 )
        return FALSE;

    return TRUE;
}


gboolean
PipeHelper::redirect_stdout()
{
    if ( ! a_pipe_created )
        return FALSE;

    // close unused "read" side of pipe ( unuseful on linux )
    close( desc(PipeHelper::eRead) );

    // redirect stdout
    if ( dup2(desc(PipeHelper::eWrite), STDOUT_FILENO) == -1 )
        return FALSE;

    return TRUE;
}
gboolean
PipeHelper::redirect_stderr()
{
    if ( ! a_pipe_created )
        return FALSE;

    // close unused "read" side of pipe ( unuseful on linux )
    close( desc(PipeHelper::eRead) );

    // redirect stderr
    if ( dup2(desc(PipeHelper::eWrite), STDERR_FILENO) == -1 )
        return FALSE;

    return TRUE;
}
gboolean
PipeHelper::redirect_reset()
{
    return FALSE;
}

gboolean
PipeHelper::unblock_read()
{
    // make read non blocant
    if ( fcntl( desc(PipeHelper::eRead), F_SETFL, O_NONBLOCK) < 0 )
        return FALSE;

    return TRUE;
}



}   // namespace libgwr

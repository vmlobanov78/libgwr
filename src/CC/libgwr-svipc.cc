/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-sysvipc.cc                                                         *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Part of libwgwr                                                         *
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
    *   Parent  : None                                                          *
    *                                                                           *
    *****************************************************************************
*/


#include <sys/stat.h>
#include <sys/fcntl.h>
//  ............................................................................
#include    "libgwr.hh"
//  ............................................................................
namespace libgwr
{
namespace svipc
{
//  ############################################################################
//
//  MsgQueue
//
//  ############################################################################
MsgQueue::MsgQueue(
    key_t   _key,
    int     _qid) :
    a_key(_key), a_qid(_qid)
{
}
MsgQueue::~MsgQueue()
{
}

MsgQueue*
MsgQueue::Get(
    const   char*   _filename,
    int             _proj_id,
    int             _msgflag)
{
    key_t           key;
    MsgQueue    *   mq      =   NULL;
    int             id      = 0;
    //..........................................................................

    // key
    key = ftok(_filename, _proj_id);

    // Create / get the kernel queue
    id = msgget(key, ( S_IRUSR | S_IWUSR | S_IWOTH ) | IPC_CREAT);

    if ( id < 0 )
        return NULL;

    // create the MsgQueue
    mq = GWR_NEW_CAST( MsgQueue, key, id );

    return mq;
}

void
MsgQueue::Delete(MsgQueue* _mq)
{
    msgctl(_mq->qid(), IPC_RMID, NULL);
}




}   // svipc
}   // namespace libgwr


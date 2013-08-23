/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-svipc.h                                                          *
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

#ifndef     __LIBGWR_SVIPC_H__
#define     __LIBGWR_SVIPC_H__
//  ...........................................................................
// SYSTEMV IPC message queues
#include    <sys/msg.h>
#include    <stddef.h>
//  ...........................................................................
namespace libgwr
{

namespace svipc
{

/**
  *
  ******************************************************************************

  \class  MQData

  ******************************************************************************
  *
  * \brief  Helper for sending / receiving to / from a MsgQueue
  *
  */
template<typename S> class MQData
{
    private:

    struct
    {
        long    a_t;
        S       a_d;
    } a_msg;

    public:
    MQData()
    {
    }
    virtual ~MQData()
    {
    }

    public:
    inline          void        set_type(long _type)    { a_msg.a_t = _type;        }

    inline          S       *   data()                  { return &(a_msg.a_d);      }
};

/**
  *
  ******************************************************************************

  \class  MsgQueue

  ******************************************************************************
  *
  * \brief  Encapsulation of an unix SystemV IPC message queue
  *
  */
class MsgQueue
{
    friend class libgwr::Object<MsgQueue>;

    private:
    key_t   a_key;
    int     a_qid;

    private:
             MsgQueue(key_t _key, int _qid);
    virtual ~MsgQueue();
    public:
    static  MsgQueue    *   Get(const char*, int _proj_id, int _msgflag);
    static  void            Delete(MsgQueue*);

    public:
    inline  int     qid()   { return a_qid;  }
    inline  key_t   key()   { return a_key; }

    public:
    template<class S>   int     send(long _type, MQData<S>* _data);
    template<class S>   int     recv(long _type, MQData<S>* _data);
};

template <class S>
int
MsgQueue::send(
    long            _type,
    MQData<S>   *   _mqd)
{
    _mqd->set_type(_type);
    return msgsnd(qid(), _mqd->data(), 512, 0);
}

template <class S>
int
MsgQueue::recv(
    long            _type,
    MQData<S>   *   _mqd)
{
    return msgrcv(qid(), _mqd->data(), 512, _type, IPC_NOWAIT);
}



}   // svipc
}   // libgwr

#endif  // __LIBGWR_SVIPC_H__

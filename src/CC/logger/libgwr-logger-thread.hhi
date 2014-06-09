/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-logger-thread.hhi                                                *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Helper for logging                                                      *
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
    *   Class   : Logger                                                        *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *   Purpose : Snippet of libgwr-logger.h                                    *
    *                                                                           *
    *****************************************************************************
*/

    /// ************************************************************************
    //!
    //! \class  Thread
    //!
    //! \brief  Channel
    //!         |
    //!         +----- SubChannel #1
    //!         |
    //!         +----- SubChannel #2
    //!         |
    //!         +----- etc...
    //!
    /// ************************************************************************
    public:
    class Thread
    {
        friend class Logger;

        public:
        class   Fifo
        {
            private:
            gchar           *   d_fifo_location;

            //  ................................................................
            public:
                     Fifo(const gchar* _location);
            virtual ~Fifo();
        };
    };

    public:
    struct  _tFifoMsgHeader
    {
        static  const   guint32 Size    =   10;

        guint16             magic;                                              //  0xfed4;
        guint16             sub;
        guint32             fmt;
        guint16             size;
    };
    typedef struct _tFifoMsgHeader tFifoMsgHeader;






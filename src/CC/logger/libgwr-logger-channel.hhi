/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-logger-channel.hhi                                               *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Helper for logging                                                      *
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
    //! \class  Channel
    //!
    //! \brief  Logger
    //!         |
    //!         +----- Channel #1
    //!         |
    //!         +----- Channel #2
    //!         |
    //!         +----- etc...
    //!
    /// ************************************************************************
    public:
    class Channel
    {
        friend class Logger;

        private:
        gchar                   *   d_header;
        guint32                     a_index;

        private:
        TArrayP< SubChannel >   *   d_subchannels;

        gboolean                    a_fifo_buffered;
        int                         a_fd_fifo_r;
        int                         a_fd_fifo_w;
        FILE                    *   a_fl_fifo_r;
        FILE                    *   a_fl_fifo_w;
        gchar                   *   d_fifo_location;

        GwrTextView             *   a_gwr_text_view;

        int                         a_fd;
        int                         a_fd_bin;

        GwrFastTextBuffer        *   a_gwrgtk_text_buffer;
        //----------------------------------------------------------------------
        public:
        guint32             index();

        void                set_header              (const gchar* _header);
        const gchar*        get_header              ();

        void                set_output_gwr_text_view(GwrTextView* _gtv);
        GwrTextView*        get_output_gwr_text_view();

        void                set_output_gwrgtk_text_buffer(GwrFastTextBuffer* _b);
        GwrFastTextBuffer*   get_output_gwrgtk_text_buffer();

        void                set_output_fd           (int);
        int                 get_output_fd           ();

        void                set_output_fd_bin       (int);
        int                 get_output_fd_bin       ();

        void                set_server              ();

        gboolean            fifo_buffered           ();
        const   gchar   *   fifo_location           ();
        FILE            *   fifo_fl_r               ();
        FILE            *   fifo_fl_w               ();
        //----------------------------------------------------------------------
        private:
        void                enable                  (Logger::eOutput, gboolean);
        gboolean            create_fifo             ();
        //----------------------------------------------------------------------
        public:
        void                    create_default_subchannels(gboolean _output_console, gboolean _output_textview);
        gboolean                create_subchannel(
                                            guint32             _ix             ,
                                    const   gchar           *   _header         ,
                                            TextAttributes      _att            ,
                                            gboolean            _output_console ,
                                            gboolean            _output_textview);

        public:
        inline  SubChannel  *   subchannel(guint32 _ix)
                                {
                                    SubChannel * sub = NULL;
                                    if ( d_subchannels->get( _ix, sub ) )
                                        return sub;

                                    return NULL;
                                }
        //----------------------------------------------------------------------
        //  ()~()
        //----------------------------------------------------------------------
        public:
                 Channel(
                            guint32                         _index                          ,
                    const   gchar                       *   _header                         ,
                            guint32                         _subchannels_card_max           ,
                            guint32                         _subchannels_realloc            ,
                            gboolean                        _fifo_buffered      = FALSE     );

        virtual ~Channel();
    };

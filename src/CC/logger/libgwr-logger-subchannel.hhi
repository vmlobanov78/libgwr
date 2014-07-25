/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-logger-subchannel.hhi                                            *
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
    //! \class  SubChannel
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
    class SubChannel
    {
        friend class Logger;

        private:
        gchar               *   d_header;

        gboolean                a_enabled_console;
        gboolean                a_enabled_textview;

        TextAttributes          a_att;
        gchar                   a_att_str[16];
        //  ....................................................................
        public:
        inline  const   gchar       *       get_header()    {   return d_header;        }
        inline          guint32             flags()         {   return a_att.flags();   }
        inline          guint32             fgcol()         {   return a_att.fgcol();   }
        inline          guint32             bgcol()         {   return a_att.bgcol();   }
        inline          guint32             style()         {   return a_att.style();   }
        inline          const gchar *       get_console_attributes()
                                            {
                                                return a_att_str;
                                            }
        //  ....................................................................
        private:
                void        enable                  (Logger::eOutput, gboolean);
        public:
        inline  gboolean    enabled_console     ()  { return a_enabled_console;     }
        inline  gboolean    enabled_textview    ()  { return a_enabled_textview;    }
        //  ....................................................................
        public:
                SubChannel(
                    const   gchar           *   _header                     ,
                            TextAttributes      _att                        ,
                            gboolean            _enable_console             ,
                            gboolean            _enable_textview            )
                    :   a_enabled_console   (_enable_console)   ,
                        a_enabled_textview  (_enable_textview)  ,
                        a_att               (_att)
                {
                    d_header    = _header ? g_strdup(_header) : NULL;

                    bzero( (void*)a_att_str, 16 );
                    libgwr::text::G_console_add_attributes(a_att_str, _att);
                }
        virtual ~SubChannel()
                {
                    g_free_safe(d_header);
                }
    };


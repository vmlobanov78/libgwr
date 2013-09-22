/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-widget-textview.h                                                *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   part of libgwr                                                          *
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
    *   Class   : GwrTextView                                                   *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_WIDGET_TEXTVIEW_H__
#define     __LIBGWR_WIDGET_TEXTVIEW_H__

//  ............................................................................
#include    "../libgwr.h"
//  ............................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(widget)


/**
  *
  ******************************************************************************

  \class  GwrTextView

  ******************************************************************************
  *
  * \brief The purpose of this class is to wrap boring GtkWidgets into a
  * convenience class. GwrTextView define 3 inner classes : model, view,
  * controller.
  *
  */
class GwrTextView
{
    //  ========================================================================
    //  fore declarations
    //  ========================================================================
    private:
    class   Model;
    class   View;
    class   Control;
    public:
    class   TextTagHelper;

    //  ========================================================================
    //  enum, typedef, ...
    //  ========================================================================

    //  ========================================================================
    //  inner classes
    //  ========================================================================

    /**
      *
      **************************************************************************

      \class  TextTagHelper

      **************************************************************************
      *
      * Handle basic text formatting in a simply way, by creating appropriate
      * GTkTextTag objects.
      *
      * usage: eTextTag::Nwl + eTextTag::Col + eTextTag::Blk + eTextTag::Bold
      *
      */
    public:
    class TextTagHelper
    {
        //  ====================================================================
        //  enum, typedef, ...
        //  ====================================================================

        //  ====================================================================
        //  members
        //  ====================================================================
        private:
        GArray      *   d_colors;                                               //!< Colors definitions array
        GArray      *   d_tags_color_fg;                                        //!< Foreground colors tags
        GArray      *   d_tags_color_bg;                                        //!< Background colors tags

        GtkTextBuffer   *   a_text_buffer;
        GtkTextTag      *   d_tag_und;
        GtkTextTag      *   d_tag_bld;
        GtkTextTag      *   d_tag_ita;
        GtkTextTag      *   d_tag_stk;

        //  ====================================================================
        //  methods
        //  ====================================================================
        private:
        inline GtkTextTag   *   get_tag_color_fg(guint32 _index)
                                {
                                    return g_array_index(d_tags_color_fg, GtkTextTag*, _index);
                                }
        inline GtkTextTag   *   get_tag_color_bg(guint32 _index)
                                {
                                    return g_array_index(d_tags_color_bg, GtkTextTag*, _index);
                                }
        inline GtkTextTag   *   get_tag_und()   {   return d_tag_und;   }
        inline GtkTextTag   *   get_tag_bld()   {   return d_tag_bld;   }
        inline GtkTextTag   *   get_tag_ita()   {   return d_tag_ita;   }
        inline GtkTextTag   *   get_tag_stk()   {   return d_tag_stk;   }

        public:
        TextTagHelper(GtkTextBuffer*);
       ~TextTagHelper();

        //! \brief  Given guint32 _tag, fill _slist with GtkTextTag* to apply
        void    build_tag_list(GSList** _slist, guint32 _flags);

        //void    tag_new(guint32 _style, guint32 _fg_color, guint32 _bg_color);

    };
    /**
      *
      **************************************************************************

      \class  Model

      **************************************************************************
      *
      * \brief
      *
      */
    private:
    class Model
    {
        friend class GwrTextView;
        friend class Control;
        //  ====================================================================
        //  members
        //  ====================================================================
        private:
        GtkTextTagTable     *   d_gtk_text_tag_table;
        GtkTextBuffer       *   d_gtk_text_buffer;
        TextTagHelper       *   d_text_tag_helper;
        guint32                 a_count;
        //  ====================================================================
        //  methods
        //  ====================================================================
        public:
        inline  GtkTextBuffer   *   buffer()
                                    {
                                        return d_gtk_text_buffer;
                                    }
        //  --------------------------------------------------------------------
        public:
         Model();
        ~Model();

        private:
        void    clear();
        void    append      (const gchar* , guint32);
        void    appendln    (const gchar* , guint32);
    };
    /**
      *
      **************************************************************************

      \class  View

      **************************************************************************
      *
      * \brief A GtkTextView parented by a GtkScrolledWindow
      *
      */
    class View
    {
        //  ====================================================================
        //  enum, typedef, ...
        //  ====================================================================
        private:
        GtkTextBuffer   *   a_text_buffer;

        GtkWidget       *   d_gtk_vbox;
        GtkWidget       *       d_gtk_scrolled_window;
        GtkWidget       *           d_gtk_text_view;
        GtkWidget       *       d_gtk_hbox;
        GtkWidget       *       d_gtk_lab1;
        GtkWidget       *           d_gtk_entry_find;

        //  ====================================================================
        //  members
        //  ====================================================================
        private:
        Control         *   a_control;

        private:
        gboolean            a_is_shown;
        //  ====================================================================
        //  methods
        //  ====================================================================
        private:
        inline  Control     *       control()     { return a_control;   }

        private:
        inline  GtkTextBuffer   *   buffer()
                                    {
                                        return a_text_buffer;
                                    }

        public:
        inline  GtkWidget       *   get_gtk_widget()
                                    {
                                        return GTK_WIDGET(d_gtk_vbox);
                                    }
        inline  GtkWidget       *   get_gtk_text_view()
                                    {
                                        return d_gtk_text_view;
                                    }

        public:
        void    scroll(GtkTextIter*);
        void    scroll_to_end();
        void    set_font_monospace(guint32 _size);
        void    set_color_bg(gchar* _color);
        //  --------------------------------------------------------------------
        //  find
        //  --------------------------------------------------------------------
        private:
        gchar           *   d_find_text;
        gchar           *   d_find_text_ck;
        GtkTextMark     *   d_mark_find_start;

        void                _find_text_set_mark_find_start_at_buffer_top();
        gboolean            _find_text_and_select();

        public:
        void                find_text(const gchar * _st = NULL);

        void                find_entry_show(gboolean);
        void                find_entry_show_toggle();

        void                clear();
        //  ====================================================================
        //  signals
        //  ====================================================================
        static  gboolean    Sgn_key_release_event             (GtkWidget*, GdkEvent*, gpointer);
        static  gboolean    Sgn_entry_find_key_release_event  (GtkWidget*, GdkEvent*, gpointer);

        public:
        View(Control*, gboolean _editable);
       ~View();

    };
    /**
      *
      **************************************************************************

      \class  Control

      **************************************************************************
      *
      * \brief Currently it is unuseful, since no GtkSignals are connected...
      *
      */
    class Control
    {
        friend class GwrTextView;
        //  ====================================================================
        //  enum, typedef, ...
        //  ====================================================================

        //  ====================================================================
        //  members
        //  ====================================================================
        private:
        Model       *   d_model;
        View        *   d_view;
        //  ====================================================================
        //  methods
        //  ====================================================================
        public:
         Control(gboolean _editable);
        ~Control();

        private:
        inline  Model       *   model()     { return d_model;   }
        inline  View        *   view()      { return d_view;    }

        inline  void            clear()
                                {
                                    model()->clear();
                                    view()->clear();
                                }
        inline  GtkTextBuffer * get_gtk_text_buffer()
                                {
                                    return model()->buffer();
                                }
        inline  GtkWidget   *   get_gtk_widget()
                                {
                                    return view()->get_gtk_widget();
                                }
        inline  GtkWidget   *   get_gtk_text_view()
                                {
                                    return view()->get_gtk_text_view();
                                }
        inline  void            append      (const gchar* _text, guint32 _tag)
                                {
                                    model()->append(_text, _tag);
                                    view()->scroll_to_end();
                                }
        inline  void            set_font_monospace(guint32 _size)
                                {
                                    view()->set_font_monospace(_size);
                                }
        inline  void            set_color_bg(gchar* _color)
                                {
                                    view()->set_color_bg(_color);
                                }

        inline  void            search_entry_show(gboolean _bsh)
                                {
                                    view()->find_entry_show(_bsh);
                                }


    };

    //  ========================================================================
    //  members
    //  ========================================================================
    private:
    Control     *   d_control;
    //  ========================================================================
    //  metohds
    //  ========================================================================
    public:
             GwrTextView(gboolean _editable = FALSE);
    virtual ~GwrTextView();
    //..........................................................................
    public:
    inline  Control *   control()   { return d_control; }
    //..........................................................................
    public:
    inline  void            clear()
                            {
                                control()->clear();
                            }
    inline  GtkWidget   *   widget()
                            {
                                return control()->get_gtk_widget();
                            }
    inline  GtkWidget   *   get_gtk_text_view()
                            {
                                return control()->get_gtk_text_view();
                            }
    inline  GtkTextBuffer * get_gtk_text_buffer()
                            {
                                return control()->get_gtk_text_buffer();
                            }
    inline  void            append      (const gchar* _text, guint32 _flags)
                            {
                                control()->append(_text, _flags);
                            }
    inline  void            set_font_monospace(guint32 _size)
                            {
                                control()->set_font_monospace(_size);
                            }
    inline  void            set_color_bg(gchar* _color)
                            {
                                control()->set_color_bg(_color);
                            }


    inline  void            search_entry_show(gboolean _bsh)
                            {
                                control()->search_entry_show(_bsh);
                            }

};






GWR_NAMESPACE_END(widget)
GWR_NAMESPACE_END(libgwr)

#endif  // #define __LIBGWR_WIDGET_TEXTVIEW_H__

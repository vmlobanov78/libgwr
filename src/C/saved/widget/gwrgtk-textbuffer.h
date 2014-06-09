#ifndef __GWRGTK_TEXT_BUFFER_H__
#define __GWRGTK_TEXT_BUFFER_H__

#include <glib.h>
//  ----------------------------------------------------------------------------
typedef struct  _GwrGtkTextBufferLine       GwrGtkTextBufferLine;

struct  _GwrGtkTextBufferLine
{
            guint8          a_fg;                                               //!< Foreground color
            guint8          a_bg;                                               //!< Background color
            guint16         a_style;                                            //!< Style infos
            guint16         a_len;                                              //!< String len
            gchar       *   a_str;                                              //!< String
}   __attribute__ ((packed));

extern      guint32     GwrGtkTextBufferLine_S;
//  ----------------------------------------------------------------------------
typedef struct  _GwrGtkTextBuffer           GwrGtkTextBuffer;

struct  _GwrGtkTextBuffer
{
    gpointer    priv;                                                           //!< Private data

    guint32     lines_card;
};
//  ============================================================================
#if ( __cplusplus )
extern "C" {
#endif

extern  inline  guint16                 gwrgtk_text_buffer_line_get_length      (
            GwrGtkTextBufferLine    *       _line           );
extern  inline  void                    gwrgtk_text_buffer_line_set_length      (
            GwrGtkTextBufferLine    *       _line           ,
            guint16                         _len            );
extern  inline  void                    gwrgtk_text_buffer_line_add_length      (
            GwrGtkTextBufferLine    *       _line           ,
            guint16                         _add            );

extern  inline  guint16                 gwrgtk_text_buffer_line_get_style       (
            GwrGtkTextBufferLine    *       _line           );

extern  inline  void                    gwrgtk_text_buffer_line_set_attributes  (
            GwrGtkTextBufferLine    *       _line           ,
            guint32                         _attributes     );

#if ( __cplusplus )
}
#endif
//  ============================================================================
#if ( __cplusplus )
extern "C" {
#endif

extern          GwrGtkTextBuffer    *   gwrgtk_text_buffer_new                  (
            guint32                         _block_size     );

extern          void                    gwrgtk_text_buffer_reset                (
            GwrGtkTextBuffer        *       _buffer         );

extern          gboolean                gwrgtk_text_buffer_get_line             (
            GwrGtkTextBuffer        *       _buffer         ,
            guint32                         _line_index     ,
            GwrGtkTextBufferLine    *       _line           );

extern          void                    gwrgtk_text_buffer_add_line             (
            GwrGtkTextBuffer        *       _buffer         ,
            guint32                         _attributes     ,
    const   gchar*                          _text           );

#if ( __cplusplus )
}
#endif




#endif                                                                          // __GWRGTK_TEXT_BUFFER_H__

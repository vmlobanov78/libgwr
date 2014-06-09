#include    <string.h>
#include    "gwrgtk-textbuffer.h"
#include    "gwrgtk-textbuffer-private.h"

guint32     GwrGtkTextBufferLine_S      =   sizeof(GwrGtkTextBufferLine);
guint32     GwrGtkTextBufferLineDesc_S  =   sizeof(GwrGtkTextBufferLineDesc_S);
//  ============================================================================
guint16             gwrgtk_text_buffer_line_get_style       (GwrGtkTextBufferLine* _line)
{
    return _line->a_style;
}
void                gwrgtk_text_buffer_line_set_attributes  (GwrGtkTextBufferLine* _line, guint32 _attributes)
{
    _line->a_fg     =   ( _attributes & 0xff000000 ) >> 24;
    _line->a_bg     =   ( _attributes & 0x00ff0000 ) >> 16;
    _line->a_style  =   ( _attributes & 0x0000ffff );
}
//  ============================================================================
//  ----------------------------------------------------------------------------
GwrGtkTextBuffer*   gwrgtk_text_buffer_new              (guint32 _block_size)
{
    GwrGtkTextBuffer        *   b   =   NULL;
    GwrGtkTextBufferPrivate *   p   =   NULL;
    //  ........................................................................
    b                   =   (GwrGtkTextBuffer*)         g_new0( GwrGtkTextBuffer        , 1 );
    p                   =   (GwrGtkTextBufferPrivate*)  g_new0( GwrGtkTextBufferPrivate , 1 );

    p->d_lines_desc     =   gwr_array_data_multi_new( _block_size );
    p->d_lines_text     =   gwr_array_data_multi_new( _block_size );
    p->lines_max_len    =   0;

    b->priv             =   (gpointer)p;

    b->lines_card       =   p->d_lines_desc->d_infos->a_blocks_used;
    return b;
}
void                gwrgtk_text_buffer_reset            (GwrGtkTextBuffer* _buffer)
{
    GwrGtkTextBufferPrivate *   p   =   NULL;
    //  ........................................................................
    p                   =   (GwrGtkTextBufferPrivate*)(_buffer->priv);

    //p->buffer_pos       =   p->buffer_mem;

    //p->lines_card       =   0;
    //p->lines_max_len    =   0;

    //_buffer->lines_card =   p->lines_card;
}
//  ----------------------------------------------------------------------------
gboolean            gwrgtk_text_buffer_get_line         (GwrGtkTextBuffer* _buffer, guint32 _line_index, GwrGtkTextBufferLine* _line)
{
    GwrGtkTextBufferPrivate *   bp  =   NULL;
    GwrCADMData                 d_desc;
    GwrCADMData                 d_text;
    GwrGtkTextBufferLineDesc    line_desc;
    //  ........................................................................
    if ( _line_index >= _buffer->lines_card )
        return FALSE;

    bp  =   (GwrGtkTextBufferPrivate*)_buffer->priv;

    g_return_val_if_fail(
        gwr_array_data_multi_get_data( bp->d_lines_desc, _line_index, &d_desc ),
        FALSE           );

    g_return_val_if_fail(
        gwr_array_data_multi_get_data( bp->d_lines_text, _line_index, &d_text ),
        FALSE           );

    memcpy( &line_desc, d_desc.a_mem, d_desc.a_size );

    gwrgtk_text_buffer_line_set_attributes(_line, line_desc.a_attributes );

    _line->a_len    =   d_text.a_size;
    _line->a_str    =   d_text.a_mem;

    return TRUE;
}
void                gwrgtk_text_buffer_add_line         (GwrGtkTextBuffer* _buffer, guint32 _attributes, const gchar* _text)
{
    GwrGtkTextBufferPrivate *   bp                      =   NULL;
    guint32                     len                     =   0;
    GwrGtkTextBufferLineDesc    line_desc;
    //  ........................................................................
    //  init some vars
    bp   =   _buffer->priv;

    len  =   strlen(_text);
    g_return_if_fail( len < 32768 );

    line_desc.a_attributes  =   _attributes;
    gwr_array_data_multi_add_data( bp->d_lines_desc, &line_desc, GwrGtkTextBufferLineDesc_S );
    gwr_array_data_multi_add_data( bp->d_lines_text, (gpointer)_text, (guint16)( (1+len) & 0x0000ffff) );
    //  ........................................................................
    //  update buffer infos
    if ( len > bp->lines_max_len )
        bp->lines_max_len = len;

    bp->lines_card          =   bp->d_lines_desc->d_infos->a_blocks_used;
    _buffer->lines_card     =   bp->lines_card ;
}


#ifndef __GWRGTK_TEXT_BUFFER_PRIVATE_H__
#define __GWRGTK_TEXT_BUFFER_PRIVATE_H__

#include    "libgwr/C/libgwr-array-data-multi.h"

typedef struct  _GwrGtkTextBufferLineDesc   GwrGtkTextBufferLineDesc;

struct  _GwrGtkTextBufferLineDesc
{
            guint32     a_attributes;                                           //!< see libgwr-text-attributes.h
}   __attribute__ ((packed));

typedef struct  _GwrGtkTextBufferPrivate    GwrGtkTextBufferPrivate;

struct  _GwrGtkTextBufferPrivate
{
    GwrCArrayDataMulti  *   d_lines_text;                                       //!< Contain all lines text
    GwrCArrayDataMulti  *   d_lines_desc;                                       //!< Contain all lines metadata

    guint32                 lines_card;
    guint32                 lines_max_len;                                      //!< Longest line length ( in char ) in buffer
};

#endif

/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-file.hh                                                          *
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
    *   Class   :                                                               *
    *                                                                           *
    *   Purpose : C FILE, DIR encapsulation                                     *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_FILE_HH__
#define     __LIBGWR_FILE_HH__
//  ............................................................................
#include    <dirent.h>
//  ............................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(file)

typedef enum
{
    TYPE_REG    =   0x00000001  ,
    TYPE_DIR    =   0x00000002  ,
    TYPE_CHR    =   0x00000004  ,
    TYPE_BLK    =   0x00000008  ,
    TYPE_FIFO   =   0x00000010  ,
    TYPE_LNK    =   0x00000020  ,
    TYPE_SOCK   =   0x00000040  ,
    TYPE_ALL    =   0x000000ff
}   tType;

class   DirectoryName
{
    LIBGWR_MEMBER_STRING( dn );
    //  ------------------------------------------------------------------------

    public:
            void            set(const gchar* _str);
    //  ------------------------------------------------------------------------
    //  ()~()
    //  ------------------------------------------------------------------------
    public:
             DirectoryName();
             DirectoryName(const gchar* _str);
    virtual ~DirectoryName()    {}
};

class   FileName : public DirectoryName
{
    LIBGWR_MEMBER_STRING( fn );
    LIBGWR_MEMBER_STRING( str );
    //  ------------------------------------------------------------------------
    private:
            void            split_str();

    public:
            void            set(const gchar* _str);
            void            set(const gchar* _dname, const gchar* _fname);
    //  ------------------------------------------------------------------------
    //  ()~()
    //  ------------------------------------------------------------------------
    public:
             FileName();
             FileName(const gchar* _dname, const gchar* _fname);
             FileName(const gchar* _str);
    virtual ~FileName() {}
};

class   File : public FileName
{
    LIBGWR_MEMBER_AUTO_TYPE( FILE*, fd );
    //  ------------------------------------------------------------------------
    //  ()~()
    //  ------------------------------------------------------------------------
    public:
             File();
             File(const gchar* _dname, const gchar* _fname);
             File(const gchar* _str);
    virtual ~File() {}

    gboolean    open(const gchar* _mode);
    gboolean    close();
};

class   Directory : public DirectoryName
{
    private:
    DIR                             *   a_dir;
    libgwr::TArrayP < File >        *   d_array_files;

    private:
    void        p0_array_files_reset();

    private:
    gboolean    p0_open();
    gboolean    p0_close();
    gboolean    p0_is_open()    { return ( a_dir != NULL ); }

    private:
    gboolean    p0_dirent_has_type  (struct dirent*, libgwr::file::tType);

    public:
    gboolean                        scan(libgwr::file::tType);
    libgwr::TArrayP < File >    *   files() { return d_array_files; }
    //  ------------------------------------------------------------------------
    //  ()~()
    //  ------------------------------------------------------------------------
             Directory();
             Directory(const gchar* _str);
    virtual ~Directory();
};

extern  void                Get_current_user_home_directory(libgwr::String*);
extern  Directory       *   Get_current_user_home_directory();
extern  void                Get_current_user_home_directory(libgwr::file::Directory*);

GWR_NAMESPACE_END(file)
GWR_NAMESPACE_END(libgwr)
//  ............................................................................

#endif                                                                          // __LIBGWR_FILE_HH__

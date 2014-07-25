/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-file.cc                                                          *
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


//  ............................................................................
#include    "libgwr.hh"
//  ............................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(file)

//  ############################################################################
//
//  libgwr::file::DirectoryName
//
//  ############################################################################
DirectoryName::DirectoryName()
{
}
DirectoryName::DirectoryName(
    const gchar* _str)
{
    g_return_if_fail( _str );

    a_dn.set( _str );
}
//  ============================================================================
void
DirectoryName::set(const gchar* _str)
{
    g_return_if_fail( _str );

    a_dn.set( _str );
}
//  ############################################################################
//
//  libgwr::file::FileName
//
//  ############################################################################
FileName::FileName()
{
}
FileName::FileName(
    const   gchar   *   _dname  ,
    const   gchar   *   _fname  )
{
    g_return_if_fail( _dname );
    g_return_if_fail( _fname );

    a_dn.set( _dname );
    a_fn.set( _fname );
}
FileName::FileName(const gchar* _str)
{
    g_return_if_fail( _str );

    a_str.set( _str );

    split_str();
}
//  ============================================================================
void
FileName::set(const gchar* _str)
{
    g_return_if_fail( _str );

    a_str.set( _str );

    split_str();
}
void
FileName::set(
    const   gchar   *   _dname  ,
    const   gchar   *   _fname  )
{
    g_return_if_fail( _dname );
    g_return_if_fail( _fname );

    a_dn.set( _dname );
    a_fn.set( _fname );

    a_str.rep( g_strconcat( _dname, "/", _fname, NULL ) );
}
//  ============================================================================
void
FileName::split_str()
{
    gchar                       *   p   =   (gchar*)str();                      //  string start
    gchar                       *   q1  =   NULL;                               //  first slash found
    gchar                       *   q2  =   NULL;                               //  last slash found
    gchar                       *   r   =   NULL;                               //  iterator
    gchar                           c   =   0;
    int                             l   =   strlen( p );
    //  ........................................................................
    g_return_if_fail( p );
    g_return_if_fail( l );
    //  ........................................................................
    //  loop for finding slashes
    {

    r   =   p - 1;

    //  ........................................................................
lab_loop:

    c = ( *(++r) );

    if ( ! c )                                                                  //  end of string
        goto lab_split;

    if ( c == '/' )                                                             //  slash found
    {
        if ( ! q1 )                                                             //  if no first slash, memorize it
            q1 = r;

        q2 = r;                                                                 //  memorize last slash
    }

    goto lab_loop;

    }
    //  ........................................................................
lab_split:

    //  if no first slash, no last slash too => no slash at all
    if ( ! q1 )
    {
        a_dn.rst();
        a_fn.set(p);
        return;
    }

    //  [[ from here we have a first slash and a second slash ]]

    //  find dn
    {

    if ( ( q1 == p ) && ( q1 == q2 ) )                                                              //  ? slash is first char
        a_dn.set( "/" );                                                        //  Y dn ="/" ...
    else
        a_dn.rep( g_strndup( p, q2 - p) );                                      //  N generic substring extraction

    }

    //  find fn
    {

    a_fn.rep( g_strndup( q2 + 1 , l - ( q2 - p + 1) ) );                        //  generic substring extraction

    }
}
//  ############################################################################
//
//  libgwr::file::Directory
//
//  ############################################################################
Directory::Directory()
    :   DirectoryName()
{
    d_array_files   =   GWR_NEW_CAST( libgwr::TArrayP < libgwr::file::File >, 150, 10 );
}
Directory::Directory(const gchar* _str)
    :   DirectoryName( _str )
{
    d_array_files   =   GWR_NEW_CAST( libgwr::TArrayP < libgwr::file::File >, 150, 10 );
}
Directory::~Directory()
{
    delete d_array_files;
}
//  ============================================================================
gboolean    Directory::p0_open()
{
    g_return_val_if_fail( a_dn.get(), FALSE );

    a_dir = opendir( a_dn.get() );

    GWR_RETURN_VAL_IF_FAIL( a_dir, FALSE );

    return TRUE;
}
gboolean    Directory::p0_close()
{
    g_return_val_if_fail( a_dir, FALSE );

    if ( closedir( a_dir ) == 0 )
        return TRUE;

    return FALSE;
}

void        Directory::p0_array_files_reset()
{
    for ( guint32 i = 0 ; i != d_array_files->card() ; i++ )
    {
        libgwr::file::File  *   f   =   d_array_files->get(i);
        delete f;
    }

    d_array_files->reset();
}
gboolean    Directory::p0_dirent_has_type(
    struct dirent           *   _dt     ,
    libgwr::file::tType         _type   )
{
    if ( _type == libgwr::file::TYPE_ALL )
        return TRUE;

    if ( ( _type & libgwr::file::TYPE_REG   ) && ( _dt->d_type == DT_REG    ) )
        return TRUE;

    if ( ( _type & libgwr::file::TYPE_DIR   ) && ( _dt->d_type == DT_DIR    ) )
        return TRUE;

    if ( ( _type & libgwr::file::TYPE_BLK   ) && ( _dt->d_type == DT_BLK    ) )
        return TRUE;

    if ( ( _type & libgwr::file::TYPE_FIFO  ) && ( _dt->d_type == DT_FIFO   ) )
        return TRUE;

    if ( ( _type & libgwr::file::TYPE_LNK   ) && ( _dt->d_type == DT_LNK    ) )
        return TRUE;

    if ( ( _type & libgwr::file::TYPE_SOCK  ) && ( _dt->d_type == DT_SOCK   ) )
        return TRUE;

    return FALSE;
}
gboolean    Directory::scan(libgwr::file::tType _type)
{
    struct dirent   *   dt  = NULL;
    //  ........................................................................
    GWR_RETURN_VAL_IF_FAIL( p0_open(), FALSE );

    p0_array_files_reset();

    do
    {
        dt = readdir( a_dir );

        if ( ! dt )
            break;

        if ( p0_dirent_has_type( dt, _type ) )
        {
            d_array_files->add( GWR_NEW_CAST( libgwr::file::File, dt->d_name ) );
        }

    } while ( TRUE );

    GWR_RETURN_VAL_IF_FAIL( p0_close(), FALSE );


    return TRUE;
}
//  ############################################################################
//
//  libgwr::file::File
//
//  ############################################################################
File::File()
{
}
File::File(const gchar* _dname, const gchar* _fname)
    :   FileName(_dname, _fname)
{

}
File::File(const gchar* _str)
    :   FileName( _str )
{
}

gboolean
File::open(const gchar* _mode)
{
    set_fd( fopen( str(), _mode ) );

    GWR_RETURN_VAL_IF_FAIL( fd(), FALSE );

    return TRUE;
}

gboolean
File::close()
{
    GWR_RETURN_VAL_IF_FAIL( ! fclose(fd()), FALSE );
    set_fd(0);
    return TRUE;
}
//  ############################################################################
//
//  Divers
//
//  ############################################################################
void            Get_current_user_home_directory(libgwr::String* _s)
{
    gchar           *   username    =   NULL;
    //  ........................................................................
    username = getlogin();

    if ( ! username )
        _s->set( "/home");
    else
        _s->vset( "/home/", username, NULL );
}
Directory   *   Get_current_user_home_directory()
{
    gchar           *   username    =   NULL;
    libgwr::String      hd;
    //  ........................................................................
    username = getlogin();

    if ( ! username )
        hd.set( "/home");
    else
        hd.vset( "/home/", username, NULL );

    return GWR_NEW_CAST( libgwr::file::Directory, hd.get() );
}
void             Get_current_user_home_directory(libgwr::file::Directory* _dir)
{
    gchar           *   username    =   NULL;
    libgwr::String      hd;
    //  ........................................................................
    username = getlogin();

    if ( ! username )
        hd.set( "/home");
    else
        hd.vset( "/home/", username, NULL );

    _dir->set( hd.get() );
}

GWR_NAMESPACE_END(file)
GWR_NAMESPACE_END(libgwr)


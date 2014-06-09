/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-widget-gen-log-tune.cc                                           *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   part of libkconfig                                                      *
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
    *   Class   :                                                               *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *   Purpose : Widgets for fine-controlling logs generated with              *
    *             libgwr-gen-log executable.                                    *
    *                                                                           *
    *****************************************************************************
*/
//  ............................................................................
#include    "libgwr-widget-gen-log-tune.hh"
//  ............................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(widget)
//  ############################################################################
//
//  Logs
//
//  ############################################################################
#if  ( LIBGWR_LOG_WIDGET_LOG_TUNE > 0 )
    #define     WLT_INF(FORMAT, ...)     LIBGWR_LOG( "\x1b[32m" "INF:"            "WLT:" FORMAT "\n", __VA_ARGS__ );
#else
    #define     WLT_INF(FORMAT, ...)
#endif

#if  ( LIBGWR_LOG_WIDGET_LOG_TUNE > 1 )
    #define     WLT_WNG(FORMAT, ...)     LIBGWR_LOG( "\x1b[33m" "WNG:" "\x1b[32m" "WLT:" FORMAT "\n", __VA_ARGS__ );
#else
    #define     WLT_WNG(FORMAT, ...)
#endif

#if  ( LIBGWR_LOG_WIDGET_LOG_TUNE > 2 )
    #define     WLT_ERR(FORMAT, ...)     LIBGWR_LOG( "\x1b[31m" "ERR:" "\x1b[32m" "WLT:" FORMAT "\n", __VA_ARGS__ );
#else
    #define     WLT_ERR(FORMAT, ...)
#endif

#define     GKCONFIG_LOG(A,B,C,D)
//  ############################################################################
//
//  static
//
//  ############################################################################
//  ############################################################################
//
//  LogTune::Preset
//
//  ############################################################################
LogTune::Preset::Preset(gboolean _b_editable)
{
    d_model                         = NULL;
    //  ........................................................................
    d_vbox                          = gwrgtk_vbox_new( FALSE, 0 );
        d_hbox                      = gwrgtk_hbox_new( FALSE, 0 );
            d_title_head            = gtk_label_new("Preset title :");
            d_title_label           = gtk_label_new("?");
            d_title_entry           = gtk_entry_new();
        d_tv                        = GWR_NEW_CAST( libgwr::widget::GwrTextView, FALSE );

    gtk_box_pack_start  ( GTK_BOX( d_hbox), d_title_head    , FALSE, FALSE, 0 );

    gtk_box_pack_start  ( GTK_BOX( d_vbox), d_hbox                  , FALSE, FALSE, 0 );
    gtk_box_pack_start  ( GTK_BOX( d_vbox), d_tv->widget()          , TRUE , TRUE , 0 );

    g_object_ref( d_title_label );
    g_object_ref( d_title_entry );

    set_editable( _b_editable );
}
LogTune::Preset::~Preset()
{
    g_object_unref( d_title_label );
    g_object_unref( d_title_entry );
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GtkWidget*  LogTune::Preset::title_widget()
{
    GList       *   l   = NULL;
    GtkWidget   *   p   = NULL;
    //  ........................................................................
    //  get the second child of d_hbox
    l = gtk_container_get_children( GTK_CONTAINER(d_hbox) );                    //  pointer on first child ( label )
    l = g_list_next( l );                                                       //  pointer on second child

    //  this tests for constructor's call : no widget is packed at construction time
    if ( l )
        p = (GtkWidget*)(l->data);

    return p;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void        LogTune::Preset::set_editable(gboolean _b)
{
    GtkWidget   *   p   = NULL;
    GtkWidget   *   wa  = NULL;                                                 //  widget to add
    GtkWidget   *   wr  = NULL;                                                 //  widget to remove
    //  ........................................................................
    p = title_widget();

    wa = _b ? d_title_entry : d_title_label;
    wr = _b ? d_title_label : d_title_entry;

    a_editable = _b;

    //  if second child packed, remove it
    if ( p )
    {
        if (  p == wa )                                                         //  already good child
            return;

        g_return_if_fail( p == wr );

        gtk_container_remove( GTK_CONTAINER(d_hbox), wr );
    }

    gtk_box_pack_start  ( GTK_BOX( d_hbox), wa , TRUE , TRUE , 0 );

    //  ........................................................................
    //  now GwrTextView
    gtk_text_view_set_editable( GTK_TEXT_VIEW(d_tv->widget_textview()), _b );
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
gchar*
LogTune::Preset::title()
{
    //  ........................................................................
    if ( editable() )
    {
        return g_strdup( gtk_entry_get_text( GTK_ENTRY(d_title_entry) ) );
    }
    else
    {
        return g_strdup( gtk_label_get_text( GTK_LABEL(d_title_label) ) );
    }
}
gchar*
LogTune::Preset::summary()
{
    //  ........................................................................
    return d_tv->get_text();
}
void
LogTune::Preset::title_update()
{
    //  ........................................................................
    if ( editable() )
    {
        return gtk_entry_set_text( GTK_ENTRY(d_title_entry), model()->title() );
    }
    else
    {
        return gtk_label_set_text( GTK_LABEL(d_title_label), model()->title() );
    }
}
void
LogTune::Preset::summary_update()
{
    //  ........................................................................
    d_tv->set_text( model()->summary() );
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
gboolean    LogTune::Preset::model_set( Preset::Model* _m)
{
    delete d_model;

    d_model = _m;

    title_update();
    summary_update();

    return TRUE;
}
//  ############################################################################
//
//  LogTune::Preset::Model
//
//  ############################################################################
LogTune::Preset::Model::Model()
{
    d_title         = NULL;
    d_summary       = NULL;
    d_element_sl    = NULL;
    a_element_card  = 0;
}
LogTune::Preset::Model::~Model()
{
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
gboolean    LogTune::Preset::Model::element_exist(Element* _e)
{
    GSList  *   sl = NULL;
    //  ........................................................................
    sl= esl() ; while ( sl )
    {
        if ( (static_cast < Element * > ( sl->data ))->uid_h() == _e->uid_h() )
            return TRUE;
    }
    return FALSE;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
gboolean    LogTune::Preset::Model::p0_Import_freadline(FILE* _f, int _n, gchar* _buffer)
{
            gchar   *   p   = NULL;
    //  ........................................................................
    if ( ! fgets(_buffer, _n, _f ) )                                            //  read line, including '\n'
        return FALSE;

    p = strchr(_buffer, '\n');                                                  //  remove trailing '\n'
    if ( ! p )
        return FALSE;
    (*p) = 0;

    return TRUE;
}
LogTune::Preset::Model*
LogTune::Preset::Model::Import(libgwr::file::File* _file)
{
            Preset::Model           *   m           = NULL;
            Preset::Model::Element  *   e           = NULL;

            gchar                   *   pn          = NULL;
            FILE                    *   f           = NULL;

            gchar                   exit        [128];
            gchar                   buffer      [512];
            gchar                   title       [128];
            gchar                   summary     [4096];
            gchar                   nsp         [512];
            gchar                   thm         [128];
            gchar                   fla         [16];
            gchar                   cc          [8];
            gchar                   cw          [8];
    //  ........................................................................
    g_return_val_if_fail( _file         , NULL );
    g_return_val_if_fail( _file->dn()   , NULL );
    g_return_val_if_fail( _file->fn()   , NULL );
    //  ........................................................................
    //  open file
    {

    f   = fopen( _file->str(), "r" );
    if ( ! f )
    {
        WLT_ERR("LogTune::Preset::Model::Import():fopen(%s) failed", pn);
        goto lab_failure;
    }

    m   = GWR_NEW_CAST( Preset::Model );

    }
    //  ........................................................................
    //  title
    {

    if ( ! p0_Import_freadline( f, 128, title )     )
    {
        WLT_ERR("%s", "LogTune::Preset::Model::Import():could not read title");
        goto lab_failure;
    }
    if ( ! p0_Import_freadline( f, 128, exit )      )
    {
        WLT_ERR("%s", "LogTune::Preset::Model::Import():could not read </title>");
        goto lab_failure;
    }
    if ( strcmp(exit, "</title>") )
    {
        WLT_ERR("LogTune::Preset::Model::Import():expected </title>, found [%s]", exit);
        goto lab_failure;
    }
    m->title_set( title );

    }
    //  ........................................................................
    //  summary
    {

    summary[0] = 0;                                                             //  because of strcat below
    while ( TRUE )
    {
        if ( ! p0_Import_freadline( f, 128, buffer )    )
        {
            WLT_ERR("%s", "LogTune::Preset::Model::Import():summary:could not read next line");
            goto lab_failure;
        }

        if ( ! strcmp(buffer, "</summary>") )               break;

        strcat(summary, buffer);
    }
    m->summary_set( summary );

    }
    //  ........................................................................
    //  elements
    while ( fscanf( f, "%s %s %s %s %s", nsp, thm, fla, cc, cw ) != EOF )
    {
        printf( "%s %s %s %s %s\n", nsp, thm, fla, cc, cw );

        e = GWR_NEW_CAST( Element, nsp, thm, fla,
            ( cc[0] == 'Y' ) ? TRUE : FALSE     ,
            ( cw[0] == 'Y' ) ? TRUE : FALSE     );

        m->element_add( e );
    }
    //  ........................................................................
lab_success:

    goto lab_end;
    //  ........................................................................
lab_failure:

    if ( m )
    {
        delete m;
        m = NULL;
    }
    goto lab_end;
    //  ........................................................................
lab_end:

    g_free_safe( pn );

    return m;
}//  ############################################################################
gboolean
LogTune::Preset::Model::Export(Model* _m, const gchar* _dirname, const gchar* _filename)
{
    /*
            Preset::Model::Element  *   e           = NULL;

            gchar                   *   pn          = NULL;
            FILE                    *   f           = NULL;
          //int                         r           = 0;

            gchar                   nsp         [512];
            gchar                   thm         [128];
            gchar                   fla         [16];
            gchar                   cc          [8];
            gchar                   cw          [8];
    //  ........................................................................
    */




    return TRUE;
}
//  ############################################################################
//
//  LogTune::Preset::Model::Element
//
//  ############################################################################

//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LogTune::Preset::Model::Element::Element(
    const   gchar   *   _nsp        ,
    const   gchar   *   _thm        ,
    const   gchar   *   _fla        ,
            gboolean    _console    ,
            gboolean    _widget     )
{
    gchar   *   t = NULL;
    //  ........................................................................
    d_nsp   = g_strdup( _nsp );
    d_thm   = g_strdup( _thm );
    d_fla   = g_strdup( _fla );

    a_console   = _console;
    a_widget    = _widget;

    a_nsp_h = gwr_str_hash( nsp() );
    a_thm_h = gwr_str_hash( thm() );
    a_fla_h = gwr_str_hash( fla() );

    //  uid
    libgwr::dstr::vset( t, nsp(), "~", thm(), "~", fla(), NULL );
    a_uid_h     = gwr_str_hash( t );
    g_free( t );
}
LogTune::Preset::Model::Element::~Element()
{
    g_free( d_nsp );
    g_free( d_thm );
    g_free( d_fla );
}
//  ############################################################################
//
//  LogTune::PresetLoad
//
//  ############################################################################
LogTune::PresetLoad::PresetLoad(
            libgwr::widget::LogTune *   _logtune)
{
    set_logtune ( _logtune              );
    set_logroot ( _logtune->logroot()   );
    set_logger  ( _logtune->logger()    );

    d_list_store                    = NULL;

    libgwr::file::Get_current_user_home_directory( selected_directory() );
    selected_directory()->string_dn()->append("/.gkconfig", NULL);
    //  ........................................................................
                                    // + ExtensibleHeader
    d_exh                           = GWR_NEW_CAST( libgwr::widget::ExtensibleHeaderF, "Load preset");

        d_vbox                      = gwrgtk_vbox_new(FALSE, 0);
            d_path_button           = gtk_file_chooser_button_new("Select a folder", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
            d_prp_hp                = gwrgtk_hpaned_new();
                d_prp_scr           = gtk_scrolled_window_new(NULL,NULL);
                    d_prp_trv       = gtk_tree_view_new();
                d_prp_preset        = GWR_NEW_CAST( LogTune::Preset );
            d_load_button           = gtk_button_new_with_label("Apply this preset");

    gtk_file_chooser_set_current_folder( GTK_FILE_CHOOSER(d_path_button), selected_directory()->dn() );

    d_prp_preset->set_editable( FALSE );

    gtk_container_add   ( GTK_CONTAINER(d_prp_scr), d_prp_trv );

    gtk_paned_pack1     ( GTK_PANED(d_prp_hp)   , d_prp_scr             , TRUE , FALSE);
    gtk_paned_pack2     ( GTK_PANED(d_prp_hp)   , d_prp_preset->widget(), TRUE , FALSE);

    gtk_box_pack_start  ( GTK_BOX( d_vbox), d_path_button       , FALSE, FALSE, 0 );
    gtk_box_pack_start  ( GTK_BOX( d_vbox), d_prp_hp            , FALSE, FALSE, 0 );
    gtk_box_pack_start  ( GTK_BOX( d_vbox), d_load_button       , FALSE, FALSE, 0 );

    d_exh->pack( d_vbox );

    //  ........................................................................
    d_list_store    = gtk_list_store_new(1, G_TYPE_STRING);
    list_store_refresh();
    d_trv_rnd       = gtk_cell_renderer_text_new();
    d_trv_col       = gtk_tree_view_column_new_with_attributes( "List Items", d_trv_rnd, "text", 0, NULL );
    gtk_tree_view_append_column( GTK_TREE_VIEW(d_prp_trv), d_trv_col );
    gtk_tree_view_set_model( GTK_TREE_VIEW(d_prp_trv), GTK_TREE_MODEL(d_list_store) );

    d_trv_sel       = gtk_tree_view_get_selection( GTK_TREE_VIEW(d_prp_trv) );

    g_signal_connect( d_load_button                         , "clicked"             , (GCallback)LogTune::PresetLoad::Sgn_button_clicked_apply          , (gpointer)this);
    g_signal_connect( GTK_FILE_CHOOSER_BUTTON(d_path_button), "selection-changed"   , (GCallback)LogTune::PresetLoad::Sgn_folder_changed                , (gpointer)this );
    g_signal_connect( d_trv_sel                             , "changed"             , (GCallback)LogTune::PresetLoad::Sgn_treeview_selection_changed    , (gpointer)this);
}
LogTune::PresetLoad::~PresetLoad()
{
    g_object_unref( d_list_store );

    g_object_unref( d_trv_rnd );
    g_object_unref( d_trv_col );
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
LogTune::PresetLoad::Sgn_treeview_selection_changed(
    GtkTreeSelection    *   _sel    ,
    gpointer                _data   )
{
            LogTune::PresetLoad     *   THIS    = NULL;
            GtkTreeIter                 iter    = { 0, NULL, NULL, NULL };
            GtkTreeModel            *   m       = NULL;
            gchar                   *   s       = NULL;

            LogTune::Preset::Model  *   pm      = NULL;

            libgwr::String              string;
    //  ........................................................................
    THIS = static_cast< LogTune::PresetLoad * > ( _data );

    if ( ! gtk_tree_selection_get_selected( _sel, &m, &iter ) )
        return;

    gtk_tree_model_get( GTK_TREE_MODEL(THIS->store()), &iter, 0, &s, -1);

    string.vset( THIS->selected_directory()->dn(), "/gkconfig.preset.", s, NULL );
    THIS->selected_file()->set( string.get() );

    pm = LogTune::Preset::Model::Import( THIS->selected_file() );
    if ( ! pm )
        return;

    THIS->d_prp_preset->model_set( pm );
}
void
LogTune::PresetLoad::Sgn_folder_changed(
    GtkFileChooser  *   _chooser    ,
    gpointer            _data)
{
    gchar               *   dir     = NULL;
    LogTune::PresetLoad *   THIS    = NULL;
    //  ........................................................................
    THIS = static_cast < LogTune::PresetLoad * > ( _data );

    dir = gtk_file_chooser_get_filename(_chooser);

    THIS->selected_directory()->set( dir );

    THIS->list_store_refresh();
}
gboolean
LogTune::PresetLoad::list_store_refresh()
{
            guint32         i       =   0;
    const   gchar       *   c       =   NULL;
            GtkTreeIter     iter;
            gchar       *   d       =   NULL;
    //  ........................................................................
    //  get files
    {

        selected_directory()->scan( libgwr::file::TYPE_REG );

    }
    //  ........................................................................
    //  fill list store
    gtk_list_store_clear( store() );                                            //  remove all entries

    for ( i = 0 ; i != selected_directory()->files()->card() ; i++ )                      //  add new entries
    {
        c = selected_directory()->files()->get(i)->fn();

        if ( strncmp( c, "gkconfig.preset.", 16 ) )
            continue;

        if ( strlen(c) == 16 )
            continue;

        d = (gchar*)c + 16;

        gtk_list_store_append( store(), &iter );

        gtk_list_store_set(
            store()                                         ,
            &iter                                           ,
            0 , d , -1                                      );
    }

    return TRUE;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  ----------------------------------------------------------------------------
//  LogTune::PresetLoad::Sgn_button_clicked_apply()
//  ----------------------------------------------------------------------------
void
LogTune::PresetLoad::Sgn_button_clicked_apply(
    GtkButton   *   _button ,
    gpointer        _data   )
{
            LogTune::PresetLoad                 *   THIS    = NULL;

            LogTune::Preset::Model              *   m       = NULL;
            GSList                              *   esl     = NULL;
            LogTune::Preset::Model::Element     *   el      = NULL;

            guint32                                 i   = 0;
            guint32                                 j   = 0;
            guint32                                 k   = 0;
            libgwr::log::Namespace              *   nsp = NULL;
            libgwr::log::Theme                  *   thm = NULL;
            libgwr::log::Flavour                *   fla = NULL;
            gboolean                                bc  = FALSE;
            gboolean                                bw  = FALSE;
    //  ........................................................................
    //  no need to re-base the channels & subchannels indexes, because we       //  _GWR_TODO_ warning to redirections !
    //  find them by their name.
    //  ........................................................................
    THIS = static_cast< LogTune::PresetLoad * > ( _data );

    m = THIS->d_prp_preset->model();

    if ( ! m )
    {
        WLT_ERR("%s", "LogTune::PresetLoad::Sgn_button_clicked_load():no model");
        return;
    }

    esl = m->esl() ; while ( esl )
    {
        el = static_cast < LogTune::Preset::Model::Element * > ( esl->data );

        //  ....................................................................
        //  find element's namespace in libkconfig::log::Namespaces()
        for ( i = 0 ; i != THIS->logroot()->nsp_card() ; i++ )
        {
            nsp = THIS->logroot()->nsp( i );

            if ( nsp->hname() == el->nsp_h() )
                goto lab_nsp_ok;
        }
        WLT_ERR("LogTune::PresetLoad::Sgn_button_clicked_load():namespace [%s] not found", el->nsp());
        goto lab_esl_next;

lab_nsp_ok:
        //  ....................................................................
        //  find element's theme in libkconfig::log::Namespace's themes
        for ( j = 0 ; j != nsp->thm_card() ; j++ )
        {
            thm = THIS->logroot()->thm( nsp->thm_ix( j ) );

            if ( thm->hname() == el->thm_h() )
                goto lab_thm_ok;
        }
        WLT_ERR("LogTune::PresetLoad::Sgn_button_clicked_load():theme [%s] not found in namespace [%s]", el->thm(), el->nsp());
        goto lab_esl_next;

lab_thm_ok:
        //  ....................................................................
        //  find element's flavour in libkconfig::log::Themes's flavours
        for ( k = 0 ; k != thm->fla_card() ; k++ )
        {
            fla = THIS->logroot()->fla( thm->fla_ix( k ) );
            if ( fla->hname() == el->fla_h() )
                goto lab_fla_ok;
        }
        WLT_ERR("LogTune::PresetLoad::Sgn_button_clicked_load():theme [%s] not found in namespace [%s]", el->thm(), el->nsp());
        goto lab_esl_next;

lab_fla_ok:
        //  ....................................................................
        //  get console & widget on/off values
        bc = el->c();
        bw = el->w();

        if ( bc )
            THIS->logtune()->m()->unmute    ( thm, fla, libgwr::Logger::eOutputConsole);
        else
            THIS->logtune()->m()->mute      ( thm, fla, libgwr::Logger::eOutputConsole);

        if ( bw )
            THIS->logtune()->m()->unmute    ( thm, fla, libgwr::Logger::eOutputTextView);
        else
            THIS->logtune()->m()->mute      ( thm, fla, libgwr::Logger::eOutputTextView);

        WLT_INF( "LogTune::PresetLoad::Sgn_button_clicked_load():nsp[%s] thm[%s] fla[%s] %c %c",
            el->nsp(), el->thm(), el->fla(), bc ? 'Y' : 'N', bw ? 'Y' : 'N' );

lab_esl_next:
        esl = g_slist_next( esl );
    }

    THIS->logtune()->v()->sync_to_model();
}
//  ############################################################################
//
//  LogTune::PresetSave
//
//  ############################################################################
LogTune::PresetSave::PresetSave(
            libgwr::widget::LogTune *   _logtune )
{
    //  ........................................................................
    set_logtune ( _logtune              );
    set_logroot ( _logtune->logroot()   );
    set_logger  ( _logtune->logger()    );

    d_exh                           = GWR_NEW_CAST( libgwr::widget::ExtensibleHeaderF, "Save to preset");

        d_vbox                      = gwrgtk_vbox_new(FALSE, 0);
            d_path_button           = gtk_file_chooser_button_new ("Select a file", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
            d_preset                = GWR_NEW_CAST( LogTune::Preset );
            d_save_button           = gtk_button_new_with_label("Save current settings to preset");

    d_preset->set_editable( TRUE );

    gtk_box_pack_start  ( GTK_BOX( d_vbox), d_path_button           , FALSE, FALSE, 0 );
    gtk_box_pack_start  ( GTK_BOX( d_vbox), d_preset->widget()      , FALSE, FALSE, 0 );
    gtk_box_pack_start  ( GTK_BOX( d_vbox), d_save_button           , FALSE, FALSE, 0 );

    d_exh->pack( d_vbox );

    g_signal_connect( GTK_BUTTON(d_save_button), "clicked", (GCallback)LogTune::PresetSave::Sgn_button_clicked_save, (gpointer)this );
}
LogTune::PresetSave::~PresetSave()
{
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void        LogTune::PresetSave::Sgn_button_clicked_save(GtkButton* _button, gpointer _data)
{
    LogTune::PresetSave     *   THIS    = NULL;
    //  ........................................................................
    THIS = static_cast< LogTune::PresetSave * > ( _data );

    THIS->save();
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
gboolean    LogTune::PresetSave::save()
{
            gboolean    b = TRUE;

            gchar   *   dn  = NULL;
            gchar   *   tn  = NULL;
            gchar   *   sn  = NULL;
            gchar   *   fn  = NULL;
            gchar   *   pn  = NULL;

            FILE                    *   f           = NULL;
          //int                         r           = 0;
            gchar                       buffer      [1024];

            guint32                         i   = 0;
            guint32                         j   = 0;
            guint32                         k   = 0;
            libgwr::log::Namespace      *   nsp = NULL;
            libgwr::log::Theme          *   thm = NULL;
            libgwr::log::Flavour        *   fla = NULL;
            libgwr::Logger::Channel     *   cha = NULL;
            libgwr::Logger::SubChannel  *   sub = NULL;
            guint32                         lc  = 0;                            //  log channel
            guint32                         ls  = 0;                            //  log subchannel
            gboolean                        bc  = FALSE;
            gboolean                        bt  = FALSE;
    //  ........................................................................
    //  get preset directory, name, summary
    dn = gtk_file_chooser_get_filename( GTK_FILE_CHOOSER(d_path_button) );
    if ( ! dn )
    {
        WLT_ERR("LogTune::PresetSave::save():%s", "no directory choosen");
        goto lab_failure;
    }

    tn = preset()->title();
    if ( ! tn )
    {
        WLT_ERR("LogTune::PresetSave::save():%s", "no title given");
        goto lab_failure;
    }

    sn = preset()->summary();
    //  ........................................................................
    //  open the file
    GKCONFIG_LOG( GKCONFIG_LOG_CHANNEL_MAIN, libgwr::Logger::eLogInf, "LogTune::PresetSave::save():dir     [%s]", dn);
    GKCONFIG_LOG( GKCONFIG_LOG_CHANNEL_MAIN, libgwr::Logger::eLogInf, "LogTune::PresetSave::save():title   [%s]", tn);
    GKCONFIG_LOG( GKCONFIG_LOG_CHANNEL_MAIN, libgwr::Logger::eLogInf, "LogTune::PresetSave::save():summary [%s]", sn);

    libgwr::dstr::vset( fn, "gkconfig.", tn, ".preset", NULL );                 //  filename
    libgwr::dstr::vset( pn, dn, "/", fn, NULL );                                //  full path

    f       =   fopen( pn, "w" );

    if ( ! f )
    {
        WLT_ERR("%s", "LogTune::PresetSave::save():fopen() failed");
        goto lab_failure;
    }
    //  ........................................................................
    //  write title
    fprintf( f, "%s\n", tn );
    fprintf( f, "%s\n", "</title>" );
    //  ........................................................................
    //  write summary
    fprintf( f, "%s\n", sn );
    fprintf( f, "%s\n", "</summary>" );
    //  ........................................................................
    //  write settings
    for ( i = 0 ; i != logroot()->nsp_card() ; i++ )
    {
        nsp = logroot()->nsp( i );

        for ( j = 0 ; j != nsp->thm_card() ; j++ )
        {
            thm = logroot()->thm( nsp->thm_ix( j ) );
            lc  = thm->cix();

            for ( k = 0 ; k != thm->fla_card() ; k++ )
            {
                fla = logroot()->fla( thm->fla_ix( k ) );
                ls  = fla->six();

                cha = logger()->channel( lc );
                if ( ! cha )
                {
                    WLT_ERR("LogTune::PresetSave::save():no channel [%i]", lc);
                    continue;
                }
                sub = cha->subchannel( ls );
                if ( ! sub )
                {
                    WLT_ERR("LogTune::PresetSave::save():no subchannel [%i] for channel [%i]", ls, lc);
                    continue;
                }
                bc = sub->enabled_console();
                bt = sub->enabled_textview();

                sprintf(buffer, "%s %s %s %c %c",
                   nsp->name(), thm->name(), fla->name()                    ,
                    bc ? 'Y' : 'N'                                          ,
                    bt ? 'Y' : 'N');

                fprintf(f, "%s\n", buffer );

                GKCONFIG_LOG( GKCONFIG_LOG_CHANNEL_MAIN, libgwr::Logger::eLogInf, "LogTune::PresetSave::save():%s", buffer );
            }
        }

    }

    fclose( f );
    //  ........................................................................
lab_success:
    goto lab_end;

    //  ........................................................................
lab_failure:
    b = FALSE;
    goto lab_end;

    //  ........................................................................
lab_end:
    g_free_safe( dn );
    g_free_safe( tn );
    g_free_safe( sn );

    g_free_safe( fn );
    g_free_safe( pn );

    return b;
}
//  ############################################################################
//
//  LogTune::Flavour
//
//  ############################################################################
LogTune::Flavour::Flavour(
    Theme                   *   _parent     ,
    libgwr::log::Flavour    *   _flavour    )
    :       a_parent( _parent )
{
    //  ........................................................................
    a_log_flavour           =   _flavour;
    d_label                 =   gtk_label_new( _flavour->name() );
    d_button_widget_toggle  =   gtk_check_button_new();
    d_button_console_toggle =   gtk_check_button_new();

    //  connect to "clicked" signal rather than "toggled", because model can change
    //  otherwise than toggling buttons in UI , so we need to toggle the buttons
    //  programmatically ; and this would lead to trigger a new "toggled" signal etc...
    //  "clicked" signal means only user action on UI.
    g_signal_connect( d_button_widget_toggle    , "clicked", (GCallback)LogTune::Flavour::Sgn_button_widget_toggled    , (gpointer)this );
    g_signal_connect( d_button_console_toggle   , "clicked", (GCallback)LogTune::Flavour::Sgn_button_console_toggled   , (gpointer)this );
}
LogTune::Flavour::~Flavour()
{
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void        LogTune::Flavour::toggle_textview(gboolean _b)
{
    gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(d_button_widget_toggle), _b );
}
void        LogTune::Flavour::toggle_console(gboolean _b)
{
    gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(d_button_console_toggle), _b );
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void        LogTune::Flavour::Sgn_button_widget_toggled(GtkToggleButton * _button, gpointer _data)
{
    Flavour *   THIS    =   NULL;
    //  ........................................................................
    THIS = static_cast< LogTune::Flavour * > ( _data );

    if ( gtk_toggle_button_get_active( _button ) )
        THIS->parent()->unmute  ( THIS, libgwr::Logger::eOutputTextView );
    else
        THIS->parent()->mute    ( THIS, libgwr::Logger::eOutputTextView );
}
void        LogTune::Flavour::Sgn_button_console_toggled(GtkToggleButton * _button, gpointer _data)
{
    Flavour *   THIS    = NULL;
    //  ........................................................................
    THIS = static_cast< LogTune::Flavour * > ( _data );

    if ( gtk_toggle_button_get_active( _button ) )
        THIS->parent()->unmute  ( THIS, libgwr::Logger::eOutputConsole );
    else
        THIS->parent()->mute    ( THIS, libgwr::Logger::eOutputConsole );
}
//  ############################################################################
//
//  LogTune::Theme
//
//  ############################################################################
//  in Gtk2 : sizes will be strictly respected
//  in Gtk3 : sizes will be modified depending on label's font size
guint32     LogTune::Theme::s_button_width  =   30;
guint32     LogTune::Theme::s_button_height =
    #if GWR_GTK_VERSION_GE(3,4)
        10;
    #else
        18;
    #endif
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LogTune::Theme::Theme(
    LogTune             *   _logtune    ,
    libgwr::log::Theme  *   _theme      )
    :   a_logtune( _logtune )
{
    //  ........................................................................
    a_log_theme                     = _theme;
    d_slist_flavours                = NULL;
    //  ........................................................................
    d_frame                                     = gtk_frame_new( _theme->name() );

        d_hbox                                  = gwrgtk_hbox_new( FALSE, 0 );

            d_grid                              = GWR_NEW_CAST( libgwr::widget::gtk::Matrix, 4, 3, TRUE );

                GtkWidget     * tmp             = gtk_label_new("ALL");
                d_button_all_text_view_mute     = gtk_button_new_with_label( "w -" );
                d_button_all_text_view_unmute   = gtk_button_new_with_label( "w +" );
                d_button_all_console_mute       = gtk_button_new_with_label( "c -" );
                d_button_all_console_unmute     = gtk_button_new_with_label( "c +" );

    gtk_box_set_spacing ( GTK_BOX(d_hbox), 10 );                                // space between flavours

    gtk_container_set_border_width( GTK_CONTAINER(d_frame), 2 );                // frame inner spacing

    gtk_widget_set_size_request( d_button_all_text_view_mute    , Theme::s_button_width, Theme::s_button_height );     // | reduce minimal size of buttons for
    gtk_widget_set_size_request( d_button_all_text_view_unmute  , Theme::s_button_width, Theme::s_button_height );     // | limiting Themes height
    gtk_widget_set_size_request( d_button_all_console_mute      , Theme::s_button_width, Theme::s_button_height );     // | reduce minimal size of buttons for
    gtk_widget_set_size_request( d_button_all_console_unmute    , Theme::s_button_width, Theme::s_button_height );     // | limiting Themes height

    gwrgtk_button_set_label_font( d_button_all_text_view_mute     , "Monospace", PANGO_STYLE_NORMAL, PANGO_WEIGHT_NORMAL, 10 );
    gwrgtk_button_set_label_font( d_button_all_text_view_unmute   , "Monospace", PANGO_STYLE_NORMAL, PANGO_WEIGHT_NORMAL, 10 );
    gwrgtk_button_set_label_font( d_button_all_console_mute       , "Monospace", PANGO_STYLE_NORMAL, PANGO_WEIGHT_NORMAL, 10 );
    gwrgtk_button_set_label_font( d_button_all_console_unmute     , "Monospace", PANGO_STYLE_NORMAL, PANGO_WEIGHT_NORMAL, 10 );

    d_grid->attach  ( tmp                           , 0, 0, GTK_FILL, GTK_FILL    , 0, 0);
    d_grid->attach  ( d_button_all_text_view_mute   , 0, 1, GTK_FILL, GTK_SHRINK  , 0, 0);
    d_grid->attach  ( d_button_all_text_view_unmute , 1, 1, GTK_FILL, GTK_SHRINK  , 0, 0);
    d_grid->attach  ( d_button_all_console_mute     , 0, 2, GTK_FILL, GTK_SHRINK  , 0, 0);
    d_grid->attach  ( d_button_all_console_unmute   , 1, 2, GTK_FILL, GTK_SHRINK  , 0, 0);

    gtk_box_pack_start  ( GTK_BOX(d_hbox), d_grid->widget() , FALSE, FALSE , 0 );

    g_signal_connect( d_button_all_text_view_mute   , "clicked", (GCallback)LogTune::Theme::Sgn_button_clicked_all_textview_mute    , (gpointer)this );
    g_signal_connect( d_button_all_text_view_unmute , "clicked", (GCallback)LogTune::Theme::Sgn_button_clicked_all_textview_unmute  , (gpointer)this );
    g_signal_connect( d_button_all_console_mute     , "clicked", (GCallback)LogTune::Theme::Sgn_button_toggled_all_console_mute     , (gpointer)this );
    g_signal_connect( d_button_all_console_unmute   , "clicked", (GCallback)LogTune::Theme::Sgn_button_toggled_all_console_unmute   , (gpointer)this );

    gtk_container_add   ( GTK_CONTAINER(d_frame), d_hbox );
}
LogTune::Theme::~Theme()
{
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void        LogTune::Theme::flavour_add(Flavour* _flavour)
{
    guint32     card    =   0;
    //  ........................................................................
    d_slist_flavours    = g_slist_append( d_slist_flavours, (gpointer)_flavour );
    card                = g_slist_length( d_slist_flavours );

    d_grid->attach( _flavour->wlabel()  , 1 + card, 0, GTK_FILL, GTK_SHRINK, 0, 0);
    d_grid->attach( _flavour->wbwt()    , 1 + card, 1, GTK_FILL, GTK_SHRINK, 0, 0);
    d_grid->attach( _flavour->wbct()    , 1 + card, 2, GTK_FILL, GTK_SHRINK, 0, 0);

}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void        LogTune::Theme::set_all_toggles_textview(gboolean _b)
{
    GSList      *   sl  = NULL;
    Flavour     *   f   = NULL;
    //  ........................................................................
    sl = d_slist_flavours ; while ( sl )
    {
        f = static_cast < Flavour * > ( sl->data );

        f->toggle_textview( _b );

        sl = g_slist_next( sl );
    }
}
void        LogTune::Theme::set_all_toggles_console(gboolean _b)
{
    GSList      *   sl  = NULL;
    Flavour     *   f   = NULL;
    //  ........................................................................
    sl = d_slist_flavours ; while ( sl )
    {
        f = static_cast < Flavour * > ( sl->data );

        f->toggle_console( _b );

        sl = g_slist_next( sl );
    }
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void        LogTune::Theme::Sgn_button_clicked_all_textview_mute    (GtkButton* _button , gpointer _data)
{
    Theme   *   THIS    = NULL;
    //  ........................................................................
    THIS = static_cast< LogTune::Theme * > ( _data );

    //libkconfig::log::Mute   ( THIS->log_theme(), libgwr::Logger::eOutputTextView );
    THIS->set_all_toggles_textview( FALSE );
}
void        LogTune::Theme::Sgn_button_clicked_all_textview_unmute  (GtkButton* _button , gpointer _data)
{
    Theme   *   THIS    = NULL;
    //  ........................................................................
    THIS = static_cast< LogTune::Theme * > ( _data );

    //libkconfig::log::Unmute ( THIS->log_theme(), libgwr::Logger::eOutputTextView );
    THIS->set_all_toggles_textview( TRUE );
}
void        LogTune::Theme::Sgn_button_toggled_all_console_mute     (GtkToggleButton*   , gpointer _data)
{
    Theme   *   THIS    = NULL;
    //  ........................................................................
    THIS = static_cast< LogTune::Theme * > ( _data );

    //libkconfig::log::Mute   ( THIS->log_theme(), libgwr::Logger::eOutputConsole );
    THIS->set_all_toggles_console( FALSE );
}
void        LogTune::Theme::Sgn_button_toggled_all_console_unmute   (GtkToggleButton*   , gpointer _data)
{
    Theme   *   THIS    = NULL;
    //  ........................................................................
    THIS = static_cast< LogTune::Theme * > ( _data );

    //libkconfig::log::Unmute ( THIS->log_theme(), libgwr::Logger::eOutputConsole );
    THIS->set_all_toggles_console( TRUE );
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void        LogTune::Theme::mute    (Flavour* _F, libgwr::Logger::eOutput _o)
{
    logtune()->m()->mute    ( log_theme(), _F->log_flavour(), _o );
}
void        LogTune::Theme::unmute  (Flavour* _F, libgwr::Logger::eOutput _o)
{
    logtune()->m()->unmute  ( log_theme(), _F->log_flavour(), _o );
}
//  ############################################################################
//
//  LogTune::Nsp
//
//  ############################################################################
LogTune::Nsp::Nsp(
    libgwr::log::Namespace  *   _nsp        )
{
    //  ........................................................................
    a_log_namespace                 = _nsp;
    d_name                          = g_strdup( _nsp->name() );
    d_slist_themes                  = NULL;
    //  ........................................................................
    d_exh           = GWR_NEW_CAST( libgwr::widget::ExtensibleHeader, name() );
    d_grid_themes   = GWR_NEW_CAST( libgwr::widget::gtk::Matrix, 1, 1, FALSE );
    //  ........................................................................
    d_exh->bg_color_if_shown_set("#cccccc");
    d_exh->pack( d_grid_themes->widget() );
}
LogTune::Nsp::~Nsp()
{
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void        LogTune::Nsp::theme_add(Theme* _theme)
{
    guint32     card    =   0;
    guint32     x       =   0;
    guint32     y       =   0;
    //  ........................................................................
    d_slist_themes = g_slist_append( d_slist_themes, (gpointer)_theme );

    card    = g_slist_length( d_slist_themes );

    y   =   ( card - 1 ) / 3;
    x   =   ( card - 1 ) % 3;

    d_grid_themes->attach(
        _theme->widget(),
        x, y,
        GTK_FILL, GTK_FILL, 0, 0 );
}
gboolean    LogTune::Nsp::is_empty()
{
    return d_slist_themes ? FALSE : TRUE;
}
//  ############################################################################
//
//  LogTune::Model
//
//  ############################################################################
LogTune::Model::Model(
    LogTune             *   _logtune    ,
    libgwr::log::Root   *   _logroot    ,
    libgwr::Logger      *   _logger     )
{
    //..........................................................................
    set_logroot ( _logroot  );
    set_logger  ( _logger   );
}
LogTune::Model::~Model()
{
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void    LogTune::Model::mute    (libgwr::log::Theme* _t, libgwr::log::Flavour* _f, libgwr::Logger::eOutput _o)
{
    guint32                                 lc  =   0;
    guint32                                 ls  =   0;
    libgwr::Logger::Channel             *   cha = NULL;
    libgwr::Logger::SubChannel          *   sub = NULL;
    //  ........................................................................
    //  get channel & subchannel
    lc  = _t->cix();
    ls  = _f->six();

    cha = logger()->channel( lc );
    if ( ! cha )
    {
        WLT_ERR("LogTune::Model::mute():no channel [%i]", lc);
        return;
    }
    sub = cha->subchannel( ls );
    if ( ! sub )
    {
        WLT_ERR("LogTune::Model::mute():no subchannel [%i] for channel [%i]", ls, lc);
        return;
    }

    logger()->mute( lc, ls, _o);
}
void    LogTune::Model::unmute  (libgwr::log::Theme* _t, libgwr::log::Flavour* _f, libgwr::Logger::eOutput _o)
{
    guint32                                 lc  =   0;
    guint32                                 ls  =   0;
    libgwr::Logger::Channel             *   cha = NULL;
    libgwr::Logger::SubChannel          *   sub = NULL;
    //  ........................................................................
    //  get channel & subchannel
    lc  = _t->cix();
    ls  = _f->six();

    cha = logger()->channel( lc );
    if ( ! cha )
    {
        WLT_ERR("LogTune::Model::unmute():no channel [%i]", lc);
        return;
    }
    sub = cha->subchannel( ls );
    if ( ! sub )
    {
        WLT_ERR("LogTune::Model::unmute():no subchannel [%i] for channel [%i]", ls, lc);
        return;
    }

    logger()->unmute( lc, ls, _o);
}
//  ############################################################################
//
//  LogTune::View
//
//  ############################################################################
LogTune::View::View(LogTune * _lt)
{
    guint32                             i,j,k;
    libgwr::log::Namespace          *   n   = NULL;
    libgwr::log::Theme              *   t   = NULL;
    libgwr::log::Flavour            *   f   = NULL;
    libgwr::widget::LogTune::Nsp    *   N   = NULL;
    libgwr::widget::LogTune::Theme  *   T   = NULL;
    libgwr::widget::LogTune::Flavour*   F   = NULL;
    //  ........................................................................
    set_control(_lt);
    //  ........................................................................
    //  main window + loader + saver
    d_dialog                        = gtk_dialog_new_with_buttons(
                                        "Log tuning", NULL                          ,
                                        (GtkDialogFlags)(GTK_DIALOG_MODAL)          ,
                                        GTK_STOCK_CANCEL    , GTK_RESPONSE_CANCEL   ,
                                        GTK_STOCK_OK        , GTK_RESPONSE_OK       ,
                                        NULL);
    d_dialog_content_area           = gtk_dialog_get_content_area( GTK_DIALOG(d_dialog) );
        d_ca_vbox                   = gwrgtk_vbox_new( FALSE, 0 );
            d_ca_prl                = GWR_NEW_CAST( libgwr::widget::LogTune::PresetLoad, _lt );
            d_ca_prs                = GWR_NEW_CAST( libgwr::widget::LogTune::PresetSave, _lt );
            d_ca_scrolled_window    = gtk_scrolled_window_new( NULL, NULL );
                d_ca_scr_vbox       = gwrgtk_vbox_new( FALSE, 0 );


    gtk_scrolled_window_add_with_viewport( GTK_SCROLLED_WINDOW(d_ca_scrolled_window), d_ca_scr_vbox );

    gtk_box_pack_start  ( GTK_BOX(d_ca_vbox), d_ca_prl->widget()                    , FALSE, FALSE, 0 );
    gtk_box_pack_start  ( GTK_BOX(d_ca_vbox), d_ca_prs->widget()                    , FALSE, FALSE, 0 );
    gtk_box_pack_start  ( GTK_BOX(d_ca_vbox), gwrgtk_hseparator_new()               , FALSE, FALSE, 5 );
    gtk_box_pack_start  ( GTK_BOX(d_ca_vbox), d_ca_scrolled_window                  , TRUE  ,TRUE , 0 );

    //  In gtk-2 d_ca_vbox expanded itself automatically, but no more in gtk-3 ;
    //  As in fact content_area is a GtkBox, we use gtk_box_pack_start instead
    //  gtk_container_add   ( GTK_CONTAINER(d_dialog_content_area), d_ca_vbox );
    gtk_box_pack_start  ( GTK_BOX(d_dialog_content_area), d_ca_vbox , TRUE , TRUE , 0 );

    g_signal_connect( d_dialog, "response"      , G_CALLBACK(Sgn_dialog_response)   , this );

    gtk_window_set_default_size(GTK_WINDOW (d_dialog), 600,600);
    //  ........................................................................
    //  all Nsp, Themes, Flavours

    //  libgwr::widget::log::Nsp loop
    for ( i = 0 ; i != c()->logroot()->nsp_card() ; i++ )
    {
        n = c()->logroot()->nsp( i );
        N = GWR_NEW_CAST( libgwr::widget::LogTune::Nsp, n );

        //  libgwr::widget::log::Theme loop
        for ( j = 0 ; j != n->thm_card() ; j++ )
        {
            t = c()->logroot()->thm( n->thm_ix( j ) );
            T = GWR_NEW_CAST( libgwr::widget::LogTune::Theme, c(), t);

            //  libgwr::widget::log::Flavour loop
            for ( k = 0 ; k != t->fla_card() ; k++ )
            {
                f = c()->logroot()->fla( t->fla_ix( k ) );
                F = GWR_NEW_CAST( libgwr::widget::LogTune::Flavour, T, f );
                T->flavour_add( F );
            }
            N->theme_add( T );
        }

        g_object_ref( N->widget() );
        nsp_add( N );
    }

    gtk_widget_show_all( d_dialog_content_area );
}
LogTune::View::~View()
{
    ////nsp_detach();
    gtk_widget_destroy( d_dialog );

    g_slist_free( d_slist_nsp );
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void        LogTune::View::nsp_add(LogTune::Nsp* _Nsp)
{
    gtk_box_pack_start( GTK_BOX(vbox()), _Nsp->widget(), FALSE, FALSE, 0 );     //  _GWR_GTK_BUG_ no expand does not work !

    gtk_widget_show_all( vbox() );                                              //  reshow all

    d_slist_nsp = g_slist_append( d_slist_nsp, (gpointer)_Nsp );
}
void        LogTune::View::sync_to_model()
{
    GSList                          *   ln  =   NULL;
    GSList                          *   lt  =   NULL;
    GSList                          *   lf  =   NULL;

    libgwr::log::Flavour            *   f   =   NULL;
    libgwr::log::Theme              *   t   =   NULL;

    libgwr::widget::LogTune::Nsp    *   N   =   NULL;
    libgwr::widget::LogTune::Theme  *   T   =   NULL;
    libgwr::widget::LogTune::Flavour*   F   =   NULL;

    libgwr::Logger::SubChannel      *   sub =   NULL;
    //  ........................................................................
    //  libgwr::widget::log::Nsp loop
    ln  =   slist_nsp() ; while ( ln )
    {
        N   =   static_cast < libgwr::widget::LogTune::Nsp* > ( ln -> data );

        lt  =   N->slist_themes() ; while ( lt )
        {
            T   =   static_cast < libgwr::widget::LogTune::Theme* > ( lt -> data );
            t   =   T->log_theme();

            lf  =   T->slist_flavours() ; while ( lf )
            {
                F   =   static_cast < libgwr::widget::LogTune::Flavour* > ( lf -> data );
                f   =   F->log_flavour();

                sub =   c()->logger()->channel( t->cix() )->subchannel( f->six() );

                gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(F->wbwt()), sub->enabled_textview() ? TRUE : FALSE );
                gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(F->wbct()), sub->enabled_console()  ? TRUE : FALSE );

                lf = g_slist_next( lf );
            }

            lt = g_slist_next( lt );
        }

        ln = g_slist_next( ln );
    }
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void        LogTune::View::Sgn_dialog_response(
    GtkDialog   *   _dialog,
    gint            _response_id,
    gpointer        _data)
{
    LogTune::View     *   THIS    = NULL;
    //  ........................................................................
    {
                                                                                /*
    GTK_RESPONSE_NONE
	Returned if an action widget has no response id, or if the dialog gets programmatically hidden or destroyed.

    GTK_RESPONSE_REJECT
	Generic response id, not used by GTK+ dialogs.

    GTK_RESPONSE_ACCEPT
	Generic response id, not used by GTK+ dialogs.

    GTK_RESPONSE_DELETE_EVENT
	Returned if the dialog is deleted.

    GTK_RESPONSE_OK
	Returned by OK buttons in GTK+ dialogs.

    GTK_RESPONSE_CANCEL
	Returned by Cancel buttons in GTK+ dialogs.

    GTK_RESPONSE_CLOSE
	Returned by Close buttons in GTK+ dialogs.

    GTK_RESPONSE_YES
	Returned by Yes buttons in GTK+ dialogs.

    GTK_RESPONSE_NO
	Returned by No buttons in GTK+ dialogs.

    GTK_RESPONSE_APPLY
	Returned by Apply buttons in GTK+ dialogs.

    GTK_RESPONSE_HELP
	Returned by Help buttons in GTK+ dialogs.
                                                                                */
    }

    THIS = static_cast < LogTune::View * > ( _data );

    if ( _response_id == GTK_RESPONSE_DELETE_EVENT )
    {
        gtk_dialog_response( GTK_DIALOG(THIS->dialog()), (GtkResponseType)1 );  //  return from gtk_dialog_run()
    }

    if ( _response_id == GTK_RESPONSE_CANCEL )
    {
        gtk_dialog_response( GTK_DIALOG(THIS->dialog()), (GtkResponseType)1 );  //  return from gtk_dialog_run()
    }

    if ( _response_id == GTK_RESPONSE_OK )
    {
        // ...
        gtk_dialog_response( GTK_DIALOG(THIS->dialog()), (GtkResponseType)1 );  //  return from gtk_dialog_run()
    }
}
//  ############################################################################
//
//  LogTune
//
//  ############################################################################
LogTune::LogTune(
    libgwr::log::Root   *   _logroot    ,
    libgwr::Logger      *   _logger     )
{
    //..........................................................................
    d_model     =   GWR_NEW_CAST( Model , this, _logroot, _logger );
    d_view      =   GWR_NEW_CAST( View  , this );

    v()->sync_to_model();
}
LogTune::~LogTune()
{
    delete d_view;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
gboolean    LogTune::run()
{
    gtk_dialog_run( GTK_DIALOG( view()->dialog() ) );

    return TRUE;
}


GWR_NAMESPACE_END(widget)
GWR_NAMESPACE_END(libgwr)

#undef  WLT_INF
#undef  WLT_WNG
#undef  WLT_ERR

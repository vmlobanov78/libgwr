/*
    *****************************************************************************
    *                                                                           *
    *   GtkFastTextView-demo.c                                                  *
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
    *   Purpose :   Illustrate speed of GtkFastTextView vs GtkTextView.         *
    *                                                                           *
    *****************************************************************************
*/
#include    <gtk/gtk.h>
#include    <string.h>
#include    <sys/times.h>
#include    "C/arrays/libgwrc-array-data-block.h"
#include    "C/gtk/GtkFastTextView/gtkfasttextview.h"

//  ............................................................................
FILE    *   Fd;
gchar       FileData    [128];
//  ............................................................................
gchar           SampleText  [1024];
gchar           SampleData  [1024];
guint32         Iterations          =   1;
gboolean        Use_colors          =   FALSE;
//  ............................................................................
GwrFastTextBuffer   *   tb1     =   NULL;
//  ............................................................................
GtkTextBuffer       *   tb2         =   NULL;
GtkTextTagTable     *   tb2_table   =   NULL;
GtkTextTag          *   tb2_tag_r   =   NULL;
GtkTextTag          *   tb2_tag_g   =   NULL;
GtkTextTag          *   tb2_tag_b   =   NULL;
//  ............................................................................
GtkWidget   *   w_win;
GtkWidget   *       w_vbox;
GtkWidget   *           w_frame;
GtkWidget   *               w_frm_grid;
GtkWidget   *               w_frm_chk_colors;
GtkWidget   *               w_frm_spin_nlines;
GtkWidget   *           w_hbox;
GtkWidget   *               w_vbox1;
GtkWidget   *                   w_button1;
GtkWidget   *                   w_tv1;
GtkWidget   *                   w_label1;
GtkWidget   *               w_vbox2;
GtkWidget   *                   w_button2;
GtkWidget   *                   w_sw2;
GtkWidget   *                       w_tv2;
GtkWidget   *                   w_label2;
//  ............................................................................
struct timespec T1;
struct timespec T2;
struct timespec Delta;
struct timespec Delta1;
struct timespec Delta2;
gchar           Str     [1024];
gchar           Temp    [1024];
gchar           Str_ns  [1024];
//  ############################################################################
//  main DEMO
//  ############################################################################
//  ............................................................................
void Format_ns(long _ns)
{
    if ( _ns < 1000 )
    {
        sprintf ( Temp      , "%ld", _ns);
        strcat  ( Str_ns    , Temp );
        return;
    }

    Format_ns(_ns / 1000 );

    sprintf ( Temp, ".%03ld", _ns % 1000);
    strcat  ( Str_ns, Temp );
}
//  ............................................................................
void    Get_user_settings()
{
    Iterations  =   (guint32)( gtk_spin_button_get_value( GTK_SPIN_BUTTON(w_frm_spin_nlines) ) );

    Use_colors  =   gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(w_frm_chk_colors) );
}
void    Chrono_start()
{
    clock_gettime(CLOCK_MONOTONIC, &T1);
}
void    Chrono_stop()
{
    clock_gettime(CLOCK_MONOTONIC, &T2);

	if ( ( T2.tv_nsec - T1.tv_nsec ) < 0 )
    {
		Delta.tv_sec    = T2.tv_sec     -   T1.tv_sec-1;
		Delta.tv_nsec   = 1000000000    +   T2.tv_nsec      -   T1.tv_nsec;
	}
    else
    {
		Delta.tv_sec    = T2.tv_sec     -   T1.tv_sec;
		Delta.tv_nsec   = T2.tv_nsec    -   T1.tv_nsec;
	}

    Str_ns[0] = 0;
    Format_ns( Delta.tv_nsec / 1000000 );

    sprintf(Str, "elapsed:(s):%ld (ms):%s", Delta.tv_sec, Str_ns);
}
//  ............................................................................
void    Signal_button1_clicked(GtkWidget* _w, gpointer _data)
{
                guint32     i       =   0;
    static      guint32     Count   =   0;
    //  ........................................................................
    Get_user_settings();

    Chrono_start();

    gwr_fast_text_buffer_reset(tb1);

    for ( i = 0 ; i != Iterations ; i++ )
    {
        sprintf(SampleText, "Line [%5i]:This is a sample text", i);

        if ( Use_colors )
        {

            sprintf(SampleData, "http://%05i", i);

            switch ( i % 3 )
            {

            //case    0   :   gwr_fast_text_buffer_add_line(tb1, SampleText, 1, 0, 0);    break;
            case    0   :   gwr_fast_text_buffer_add_line_with_extra_data(tb1, SampleText, 1, 0, 0, SampleData, 12);    break;
            case    1   :   gwr_fast_text_buffer_add_line(tb1, SampleText, 2, 0, 0);    break;
            case    2   :   gwr_fast_text_buffer_add_line(tb1, SampleText, 3, 0, 0);    break;

            }
        }
        else
        {
            gwr_fast_text_buffer_add_line(tb1, SampleText, 0, 0, 0);

            //fwrite( FileData, 34, 1, Fd );
            //printf("%i\n",x);
        }
    }

    gtk_fast_text_view_refresh( GTK_FAST_TEXT_VIEW(w_tv1) );

    Chrono_stop();

    Delta1  =   Delta;

    gtk_label_set_text( GTK_LABEL(w_label1), Str );

    Count++;
    gtk_fast_text_view_refresh( GTK_FAST_TEXT_VIEW(w_tv1) );
}
void    Signal_button2_clicked(GtkWidget* _w, gpointer _data)
{
    guint32     i   =   0;
    GtkTextIter iter;
    //  ........................................................................
    Get_user_settings();

    Chrono_start();

    gtk_text_buffer_set_text(tb2, "", -1);

    for ( i = 0 ; i != Iterations ; i++ )
    {
        sprintf(SampleText, "Line [%5i]:This is a sample text\n", i);

        gtk_text_buffer_get_end_iter(tb2, &iter);

        if ( Use_colors )
        {

            sprintf(SampleData, "http://%05i", i);

            switch ( i % 3 )
            {

            case    0   :   gtk_text_buffer_insert_with_tags(tb2, &iter, SampleText, -1, tb2_tag_r, NULL);  break;
            case    1   :   gtk_text_buffer_insert_with_tags(tb2, &iter, SampleText, -1, tb2_tag_g, NULL);  break;
            case    2   :   gtk_text_buffer_insert_with_tags(tb2, &iter, SampleText, -1, tb2_tag_b, NULL);  break;

            }
        }
        else
        {
            gtk_text_buffer_insert(tb2, &iter, SampleText, -1);
        }
    }

    gtk_fast_text_view_refresh( GTK_FAST_TEXT_VIEW(w_tv1) );

    Chrono_stop();

    Delta2  =   Delta;

    gtk_label_set_text( GTK_LABEL(w_label2), Str );
}
void    Signal_destroy(GtkWidget* _w)
{
    fflush(Fd);
}
//  ............................................................................
void    GftvXdataCallback(gpointer _data,guint16 _len)
{
    GtkWidget   *   dialog;
    gchar           str1        [256];
    gchar           str2        [128];
    //  ........................................................................
    if ( _len < 128 )
    {
        memcpy( str2, _data, _len );
        str2[ _len ] = 0;
    }
    else
    {
        strncpy( str2, _data, 127 );
        str2[128] = 0;
    }

    sprintf(str1, "The extra data is [%i] bytes long :\n", _len);
    strcat( str1, str2);

    dialog  =   gtk_message_dialog_new(
                    NULL,
                    GTK_DIALOG_DESTROY_WITH_PARENT,
                    GTK_MESSAGE_QUESTION,
                    GTK_BUTTONS_OK,
                    str1);

    gtk_window_set_title(GTK_WINDOW(dialog), "Extra data dialog");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
//  ............................................................................
int main( int argc, char *argv[])
{

    //  ........................................................................
    gtk_init(&argc, &argv);
    //  ........................................................................
    Fd  =   fopen("test.data", "w");
    //  ........................................................................
    //I extern          GwrFastTextBuffer   *   gwr_fast_text_buffer_new                (
    //I       guint32                         _lines_text_data_block_size                 ,
    //I       guint32                         _lines_text_data_blocks_storage_realloc     ,
    //I       guint32                         _lines_text_info_block_size                 ,
    //I       guint32                         _lines_text_info_blocks_storage_realloc     ,

    //I       guint32                         _lines_desc_info_blocks_storage_capacity    ,
    //I       guint32                         _lines_desc_infos_blocks_storage_realloc    ,

    //I       guint32                         _extra_data_data_block_size                 ,
    //I       guint32                         _extra_data_data_blocks_storage_realloc     ,
    //I       guint32                         _extra_data_info_block_size                 ,
    //I       guint32                         _extra_data_info_blocks_storage_realloc     );

    tb1 =   gwr_fast_text_buffer_new(
                70000 , 8 ,
                        8 ,

                1024 , 4 ,

                32768, 4   ,
                8192 , 4   );
    //  ........................................................................
    tb2_tag_r   =   gtk_text_tag_new("CR");
    tb2_tag_g   =   gtk_text_tag_new("CG");
    tb2_tag_b   =   gtk_text_tag_new("CB");

    tb2_table   =   gtk_text_tag_table_new();
    gtk_text_tag_table_add( tb2_table, tb2_tag_r);
    gtk_text_tag_table_add( tb2_table, tb2_tag_g);
    gtk_text_tag_table_add( tb2_table, tb2_tag_b);

    tb2 =   gtk_text_buffer_new(tb2_table);
    //  ........................................................................
    w_win                       =   gtk_window_new(GTK_WINDOW_TOPLEVEL);

        w_vbox                  =   gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

            w_frame             =   gtk_frame_new("Settings");

            w_frm_grid          =   gtk_grid_new();
            w_frm_chk_colors    =   gtk_check_button_new_with_label("Use colors");
            //w_frm_spin_nlines   =   gtk_spin_button_new_with_range(1000.0, 100000.0, 1000.0);
            w_frm_spin_nlines   =   gtk_spin_button_new_with_range(1.0, 100000.0, 1.0);

            w_hbox              =   gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

                w_vbox1         =   gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
                    w_button1   =   gtk_button_new_with_label("Write to GtkFastTextView");
                    w_tv1       =   gtk_fast_text_view_new(tb1);
                    w_label1    =   gtk_label_new("");

                w_vbox2         =   gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
                    w_button2   =   gtk_button_new_with_label("Write to GtkTextView");
                    w_sw2       =   gtk_scrolled_window_new(NULL, NULL);
                    w_tv2       =   gtk_text_view_new();
                    w_label2    =   gtk_label_new("");
    //  ........................................................................
    gtk_text_view_set_buffer( GTK_TEXT_VIEW(w_tv2), tb2 );
    gtk_spin_button_set_value( GTK_SPIN_BUTTON(w_frm_spin_nlines), (gdouble)Iterations);

    gtk_fast_text_view_set_color( GTK_FAST_TEXT_VIEW(w_tv1), 1, 255, 0, 0);
    gtk_fast_text_view_set_color( GTK_FAST_TEXT_VIEW(w_tv1), 2, 0, 255, 0);
    gtk_fast_text_view_set_color( GTK_FAST_TEXT_VIEW(w_tv1), 3, 0, 0, 255);
    gtk_fast_text_view_set_xd_callback( GTK_FAST_TEXT_VIEW(w_tv1), GftvXdataCallback );
    gtk_fast_text_view_set_font_size( GTK_FAST_TEXT_VIEW(w_tv1), 10 );

    g_object_set( G_OBJECT(tb2_tag_r), "foreground", "#FF0000", NULL );
    g_object_set( G_OBJECT(tb2_tag_g), "foreground", "#00FF00", NULL );
    g_object_set( G_OBJECT(tb2_tag_b), "foreground", "#0000FF", NULL );
    //  ........................................................................
    gtk_grid_attach( GTK_GRID(w_frm_grid), w_frm_chk_colors     , 0,0,1,1);
    gtk_grid_attach( GTK_GRID(w_frm_grid), w_frm_spin_nlines    , 1,0,1,1);
    gtk_container_add( GTK_CONTAINER(w_frame), w_frm_grid);

    gtk_box_pack_start( GTK_BOX(w_vbox1)    , w_button1 , FALSE, FALSE, 0 );
    gtk_box_pack_start( GTK_BOX(w_vbox1)    , w_tv1     , TRUE , TRUE , 0 );
    gtk_box_pack_start( GTK_BOX(w_vbox1)    , w_label1  , FALSE, FALSE, 0 );

    gtk_container_add( GTK_CONTAINER(w_sw2), w_tv2 );

    gtk_box_pack_start( GTK_BOX(w_vbox2)    , w_button2 , FALSE, FALSE, 0 );
    gtk_box_pack_start( GTK_BOX(w_vbox2)    , w_sw2     , TRUE , TRUE , 0 );
    gtk_box_pack_start( GTK_BOX(w_vbox2)    , w_label2  , FALSE, FALSE, 0 );

    gtk_box_set_homogeneous ( GTK_BOX(w_hbox)   , TRUE);
    gtk_box_pack_start      ( GTK_BOX(w_hbox)   , w_vbox1   , TRUE , TRUE , 0 );
    gtk_box_pack_start      ( GTK_BOX(w_hbox)   , w_vbox2   , TRUE , TRUE , 0 );

    gtk_box_pack_start      ( GTK_BOX(w_vbox)   , w_frame   , FALSE, FALSE, 0 );
    gtk_box_pack_start      ( GTK_BOX(w_vbox)   , w_hbox    , TRUE , TRUE , 0 );

    gtk_container_add( GTK_CONTAINER(w_win), w_vbox );

    g_signal_connect( w_button1, "clicked", G_CALLBACK(Signal_button1_clicked), NULL );
    g_signal_connect( w_button2, "clicked", G_CALLBACK(Signal_button2_clicked), NULL );
    g_signal_connect( w_win    , "destroy", G_CALLBACK(Signal_destroy), NULL);
    gtk_widget_show_all(w_vbox);

    gtk_widget_show(w_win);

    gtk_main();

    fclose(Fd);

    return 0;
}


//  ############################################################################
//  GwrCArrayEqual TEST
//  ############################################################################
/*
int main( int argc, char *argv[])
{
    //  ........................................................................
    guint32             i       =   0;
    gchar               tmp[256]  ;
    gchar               str[4]  ;
    gchar           *   ttr  ;
    GwrCADBlock24   *   dbk24   =   NULL;

    GwrCArrayEqual  *   aeq     =   NULL;
    //  ........................................................................
    aeq = gwr_array_equal_new("test", 4, 2, 2);

    for ( i = 0 ; i != 10 ; i ++ )
    {
        sprintf( tmp, "%04i", i);
        memcpy(str, tmp, 4 );
        gwr_array_equal_add_data( aeq, str );
        gwr_array_equal_dump( aeq );
    }

    for ( i = 0 ; i != 10 ; i ++ )
    {
        ttr = (gchar*)gwr_array_equal_get_data( aeq, i );

        printf("i[%03i]:%c%c%c%c\n", i, ttr[0], ttr[1], ttr[2], ttr[3] );
    }
    return 0;
}
*/


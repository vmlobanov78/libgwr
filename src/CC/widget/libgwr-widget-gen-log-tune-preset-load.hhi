    /// ************************************************************************
    //!
    //! \class  PresetLoad
    //!
    //! \brief  Load a log preset from a file
    //!
    /// ************************************************************************
    private:
    class   PresetLoad
    {
        LIBGWR_MEMBER_AUTO_TYPE(libgwr::widget::LogTune *, logtune  );
        LIBGWR_MEMBER_AUTO_TYPE(libgwr::log::Root       *, logroot  );
        LIBGWR_MEMBER_AUTO_TYPE(libgwr::Logger          *, logger   );

        LIBGWR_MEMBER_AUTO_OBJECT(libgwr::file::Directory , selected_directory    );
        LIBGWR_MEMBER_AUTO_OBJECT(libgwr::file::File      , selected_file         );
        //  --------------------------------------------------------------------
        private:
        libgwr::widget::IExtensibleHeader   *   d_exh;
        GtkWidget                           *       d_vbox;
        GtkWidget                           *           d_path_button;
        GtkWidget                           *           d_prp_hp;
        GtkWidget                           *               d_prp_scr;
        GtkWidget                           *                   d_prp_trv;
        Preset                              *               d_prp_preset;
        GtkWidget                           *           d_load_button;

        public:
                GtkWidget       *           widget()    { return d_exh->widget();   }
                void                        fold()      { d_exh->hide();            }
        //  --------------------------------------------------------------------
        //  file list
        private:
                GtkCellRenderer     *   d_trv_rnd;
                GtkTreeViewColumn   *   d_trv_col;
                GtkListStore        *   d_list_store;
                GtkTreeSelection    *   d_trv_sel;

        private:
                GtkListStore        *       store                           ()  { return d_list_store;      }
                gboolean                    list_store_refresh              ();
        static  void                        Sgn_folder_changed              (GtkFileChooser*, gpointer);
        static  void                        Sgn_treeview_selection_changed  (GtkTreeSelection*, gpointer);
        public:
        //  --------------------------------------------------------------------
        private:
        static  void                        Sgn_button_clicked_apply(GtkButton*, gpointer);
        //  --------------------------------------------------------------------
        //  ()~()
        public:
                 PresetLoad(libgwr::widget::LogTune*);
        virtual ~PresetLoad();

    };

    /// ************************************************************************
    //!
    //! \class  PresetSave
    //!
    //! \brief  Save current log settings to a preset file
    //!
    /// ************************************************************************
    private:
    class   PresetSave
    {
        LIBGWR_MEMBER_AUTO_TYPE(libgwr::widget::LogTune *, logtune);
        LIBGWR_MEMBER_AUTO_TYPE(libgwr::log::Root       *, logroot);
        LIBGWR_MEMBER_AUTO_TYPE(libgwr::Logger          *, logger);
        //  --------------------------------------------------------------------
        private:
        libgwr::widget::IExtensibleHeader   *   d_exh;
        GtkWidget                           *       d_vbox;
        GtkWidget                           *           d_path_button;
        Preset                              *           d_preset;
        GtkWidget                           *           d_save_button;

        private:
        Preset                  *           preset()    { return d_preset;      }

        public:
                GtkWidget       *           widget()    { return d_exh->widget();   }
                void                        fold()      { d_exh->hide();            }

        public:
                gboolean                    save();


        private:
        static  void                        Sgn_button_clicked_save(GtkButton*, gpointer);

        public:
                 PresetSave(libgwr::widget::LogTune*);
        virtual ~PresetSave();

    };

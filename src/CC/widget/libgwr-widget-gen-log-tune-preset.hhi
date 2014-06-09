    /// ************************************************************************
    //!
    //! \class  Preset
    //!
    //! \brief  Contain all infos for a log preset
    //!
    /// ************************************************************************
    private:
    class   Preset
    {
        //  ====================================================================
        //  inner classes
        //  ====================================================================
        /// ********************************************************************
        //!
        //! \namespace  gkconfig::widget
        //!
        //! \class  Model
        //!
        //! \brief  Contain a GSList of Elements.
        //!
        /// ********************************************************************
        public:
        class   Model
        {
            friend class Preset;
            //  ================================================================
            //  inner classes
            //  ================================================================
            /// ****************************************************************
            //!
            //! \namespace  gkconfig::widget
            //!
            //! \class  Element
            //!
            //! \brief  Contain :
            //!         - Namespace name    + Hash
            //!         - Theme name        + Hash
            //!         - Flavour name      + Hash
            //!         - Console & Widget display infos.
            //!
            /// ****************************************************************
            public:
            class   Element
            {
                private:
                gchar                       *       d_nsp;
                gchar                       *       d_thm;
                gchar                       *       d_fla;

                GwrCStrHash                         a_nsp_h;
                GwrCStrHash                         a_thm_h;
                GwrCStrHash                         a_fla_h;
                GwrCStrHash                     a_uid_h;

                gboolean                            a_console;
                gboolean                            a_widget;

                public:
                const   gchar               *       nsp()       { return d_nsp; }
                const   gchar               *       thm()       { return d_thm; }
                const   gchar               *       fla()       { return d_fla; }
                        gboolean            &       c()         { return a_console; }
                        gboolean            &       w()         { return a_widget;  }

                        GwrCStrHash                 nsp_h()     { return a_nsp_h;   }
                        GwrCStrHash                 thm_h()     { return a_thm_h;   }
                        GwrCStrHash                 fla_h()     { return a_fla_h;   }
                        GwrCStrHash                 uid_h()     { return a_uid_h;   }

                public:
                         Element(const gchar* _nsp, const gchar* _thm, const gchar* _fla, gboolean _console, gboolean _widget);
                virtual ~Element();
            };
            //  ################################################################
            private:
            gchar                           *   d_title;
            GSList                          *   d_element_sl;
            guint32                             a_element_card;
            gchar                           *   d_summary;

            public:
            const   gchar           *           title()     { return d_title;           }
            const   gchar           *           summary()   { return d_summary;         }
            private:
                    void                        title_set(const gchar* _s)
                                                {
                                                    g_free_safe( d_title );
                                                    d_title = g_strdup( _s );
                                                }
                    void                        summary_set(const gchar* _s)
                                                {
                                                    g_free_safe( d_summary );
                                                    d_summary = g_strdup( _s );
                                                }
            public:
                    GSList          *           esl     ()  { return d_element_sl;      }
                    guint32                     ecard   ()  { return a_element_card;    }
                    gboolean                    element_exist   (Element* _e);
                    void                        element_add     (Element* _e)
                                                {
                                                    d_element_sl = g_slist_append( d_element_sl, (gpointer)_e );
                                                    a_element_card++;
                                                }
            //  IO
            private:
            static  gboolean                    p0_Import_freadline(FILE* _f, int _n, gchar* _buffer);
            public:
            static  Model           *           Import(libgwr::file::File*);
            static  gboolean                    Export(Model*, const gchar* _dirname, const gchar* _filename);

            public:
                     Model();
            virtual ~Model();
        };
        //  ####################################################################
        private:
        GtkWidget                       *   d_vbox;
        GtkWidget                       *       d_hbox;
        GtkWidget                       *           d_title_head;
        GtkWidget                       *           d_title_label;
        GtkWidget                       *           d_title_entry;
        libgwr::widget::GwrTextView     *       d_tv;

        gboolean                            a_editable;

        Model                           *   d_model;


        public:
                Model           *           model()                 { return d_model;           }
                GtkWidget       *           widget()                { return d_vbox;            }
                GtkWidget       *           title_widget();
                gboolean                    editable()              { return a_editable;        }

                void                        set_editable(gboolean);
        //  --------------------------------------------------------------------
        public:
                gchar           *           title();
                gchar           *           summary();
        private:
                void                        title_update();
                void                        summary_update();
        //  --------------------------------------------------------------------
        public:
                gboolean                    model_set(Preset::Model*);


        public:
                 Preset(gboolean _b_editable = FALSE);
        virtual ~Preset();


    };

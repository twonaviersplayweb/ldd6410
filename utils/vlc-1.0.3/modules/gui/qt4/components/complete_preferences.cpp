/*****************************************************************************
 * preferences.cpp : "Normal preferences"
 ****************************************************************************
 * Copyright (C) 2006-2007 the VideoLAN team
 * $Id$
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <QApplication>
#include <QLabel>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVariant>
#include <QString>
#include <QFont>
#include <QGroupBox>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHeaderView>

#include "components/complete_preferences.hpp"
#include "components/preferences_widgets.hpp"

#include <vlc_config_cat.h>
#include <vlc_intf_strings.h>
#include <assert.h>

#define ITEM_HEIGHT 25

/*********************************************************************
 * The Tree
 *********************************************************************/
PrefsTree::PrefsTree( intf_thread_t *_p_intf, QWidget *_parent ) :
                            QTreeWidget( _parent ), p_intf( _p_intf )
{
    /* General Qt options */
    setColumnCount( 1 );
    setAlternatingRowColors( true );
    header()->hide();

    setIconSize( QSize( ITEM_HEIGHT,ITEM_HEIGHT ) );
    setTextElideMode( Qt::ElideNone );

    /* Nice icons */
#define BI( a,b) QIcon a##_icon = QIcon( QPixmap( b ))
    BI( audio, ":/advprefs_audio" );
    BI( video, ":/advprefs_video" );
    BI( input, ":/advprefs_codec" );
    BI( sout, ":/advprefs_sout" );
    BI( advanced, ":/advprefs_extended" );
    BI( playlist, ":/advprefs_playlist" );
    BI( interface, ":/advprefs_intf" );
#undef BI

    /* Build the tree for the main module */
    module_t *p_module = module_get_main();

    /* Initialisation and get the confsize */
    PrefsItemData *data = NULL;
    PrefsItemData *data_sub = NULL;
    QTreeWidgetItem *current_item = NULL;
    unsigned confsize;
    module_config_t *const p_config = module_config_get (p_module, &confsize);

    /* Go through the list of conf */
    for( size_t i = 0; i < confsize; i++ )
    {
        const char *psz_help;
        QIcon icon;

        /* Work on a new item */
        module_config_t *p_item = p_config + i;

        switch( p_item->i_type )
        {
        /* This is a category */
        case CONFIG_CATEGORY:
            if( p_item->value.i == -1 ) break;

            /* PrefsItemData Init */
            data = new PrefsItemData();
            data->name = qtr( config_CategoryNameGet( p_item->value.i ) );
            psz_help = config_CategoryHelpGet( p_item->value.i );
            if( psz_help )
                data->help = qtr( psz_help );
            else
                data->help.clear();
            data->i_type = TYPE_CATEGORY;
            data->i_object_id = p_item->value.i;

            /* This is a category, put a nice icon */
            switch( p_item->value.i )
            {
#define CI(a,b) case a: icon = b##_icon;break
            CI( CAT_AUDIO, audio );
            CI( CAT_VIDEO, video );
            CI( CAT_INPUT, input );
            CI( CAT_SOUT, sout );
            CI( CAT_ADVANCED, advanced );
            CI( CAT_PLAYLIST, playlist );
            CI( CAT_INTERFACE, interface );
            }
#undef CI

            /* Create a new QTreeItem to display it in the tree at top level */
            current_item = new QTreeWidgetItem();
            current_item->setText( 0, data->name );
            current_item->setIcon( 0 , icon );
            current_item->setSizeHint( 0, QSize( -1, ITEM_HEIGHT ) );
            current_item->setData( 0, Qt::UserRole,
                                   qVariantFromValue( data ) );
            addTopLevelItem( current_item );
            break;

        /* This is a subcategory */
        case CONFIG_SUBCATEGORY:
            if( p_item->value.i == -1 ) break;

            /* Special cases: move the main subcategories to the parent cat*/
            if( data &&
                ( p_item->value.i == SUBCAT_VIDEO_GENERAL ||
                  p_item->value.i == SUBCAT_ADVANCED_MISC ||
                  p_item->value.i == SUBCAT_INPUT_GENERAL ||
                  p_item->value.i == SUBCAT_INTERFACE_GENERAL ||
                  p_item->value.i == SUBCAT_SOUT_GENERAL||
                  p_item->value.i == SUBCAT_PLAYLIST_GENERAL||
                  p_item->value.i == SUBCAT_AUDIO_GENERAL ) )
            {
                /* Data still contains the correct thing */
                data->i_type = TYPE_CATSUBCAT;
                data->i_subcat_id = p_item->value.i;
                data->name = qtr( config_CategoryNameGet( p_item->value.i ) );
                psz_help = config_CategoryHelpGet( p_item->value.i );
                if( psz_help )
                    data->help = qtr( psz_help );
                else
                    data->help.clear();
                current_item->setData( 0, Qt::UserRole,
                                       QVariant::fromValue( data ) );
                continue;
            }

            /* Normal Subcategories */

            /* Process the Data */
            data_sub = new PrefsItemData();
            data_sub->name = qtr( config_CategoryNameGet( p_item->value.i) );
            psz_help = config_CategoryHelpGet( p_item->value.i );
            if( psz_help )
                data_sub->help = qtr( psz_help );
            else
                data_sub->help.clear();
            data_sub->i_type = TYPE_SUBCATEGORY;
            data_sub->i_object_id = p_item->value.i;

            /* Create a new TreeWidget */
            QTreeWidgetItem *subcat_item = new QTreeWidgetItem();
            subcat_item->setText( 0, data_sub->name );
            subcat_item->setData( 0, Qt::UserRole,
                                  qVariantFromValue( data_sub ) );
            subcat_item->setSizeHint( 0, QSize( -1, ITEM_HEIGHT ) );

            /* Add it to the parent */
            assert( current_item );
            current_item->addChild( subcat_item );
            break;

        /* Other items don't need yet a place on the tree */
        }
    }
    module_config_free( p_config );
    module_release( p_module );


    module_t **p_list = module_list_get( NULL );
    /* Build the tree of plugins */
    for( size_t i = 0; (p_module = p_list[i]) != NULL; i++ )
    {
        // Main module excluded
        if( module_is_main( p_module) ) continue;

        unsigned  confsize;
        int i_subcategory = 0, i_category = 0;

        bool b_options = false;
        module_config_t *const p_config = module_config_get (p_module, &confsize);

        /* Loop through the configurations items */
        for (size_t i = 0; i < confsize; i++)
        {
            const module_config_t *p_item = p_config + i;

            if( p_item->i_type == CONFIG_CATEGORY )
                i_category = p_item->value.i;
            else if( p_item->i_type == CONFIG_SUBCATEGORY )
                i_subcategory = p_item->value.i;

            if( p_item->i_type & CONFIG_ITEM )
                b_options = true;

            if( b_options && i_category && i_subcategory )
                break;
        }
        module_config_free (p_config);

        /* Dummy item, please proceed */
        if( !b_options || i_category == 0 || i_subcategory == 0 ) continue;


        // Locate the category item;
        QTreeWidgetItem *subcat_item = NULL;
        bool b_found = false;

        for( int i_cat_index = 0 ; i_cat_index < topLevelItemCount();
                                   i_cat_index++ )
        {
            /* Get the treeWidgetItem that correspond to the category */
            QTreeWidgetItem *cat_item = topLevelItem( i_cat_index );
            PrefsItemData *data = cat_item->data( 0, Qt::UserRole ).
                                             value<PrefsItemData *>();

            /* If we match the good category */
            if( data->i_object_id == i_category )
            {
                for( int i_sc_index = 0; i_sc_index < cat_item->childCount();
                         i_sc_index++ )
                {
                    subcat_item = cat_item->child( i_sc_index );
                    PrefsItemData *sc_data = subcat_item->data(0, Qt::UserRole).
                                                value<PrefsItemData *>();
                    if( sc_data && sc_data->i_object_id == i_subcategory )
                    {
                        b_found = true;
                        break;
                    }
                }
                if( !b_found )
                {
                    subcat_item = cat_item;
                    b_found = true;
                }
                break;
            }
        }
        if( !b_found ) continue;

        PrefsItemData *module_data = new PrefsItemData();
        module_data->i_type = TYPE_MODULE;
        module_data->psz_name = strdup( module_get_object( p_module ) );
        module_data->help.clear();
        QTreeWidgetItem *module_item = new QTreeWidgetItem();
        module_item->setText( 0, qtr( module_get_name( p_module, false ) ) );
        module_item->setData( 0, Qt::UserRole,
                              QVariant::fromValue( module_data) );
        module_item->setSizeHint( 0, QSize( -1, ITEM_HEIGHT ) );
        subcat_item->addChild( module_item );
    }

    /* We got everything, just sort a bit */
    sortItems( 0, Qt::AscendingOrder );

    module_list_free( p_list );
}

PrefsTree::~PrefsTree() {}

void PrefsTree::applyAll()
{
    doAll( false );
}

void PrefsTree::cleanAll()
{
    doAll( true );
}

void PrefsTree::doAll( bool doclean )
{
    for( int i_cat_index = 0 ; i_cat_index < topLevelItemCount();
             i_cat_index++ )
    {
        QTreeWidgetItem *cat_item = topLevelItem( i_cat_index );
        for( int i_sc_index = 0; i_sc_index < cat_item->childCount();
                 i_sc_index++ )
        {
            QTreeWidgetItem *sc_item = cat_item->child( i_sc_index );
            for( int i_module = 0 ; i_module < sc_item->childCount();
                     i_module++ )
            {
                PrefsItemData *data = sc_item->child( i_module )->
                               data( 0, Qt::UserRole).value<PrefsItemData *>();
                if( data->panel && doclean )
                {
                    delete data->panel;
                    data->panel = NULL;
                }
                else if( data->panel )
                    data->panel->apply();
            }
            PrefsItemData *data = sc_item->data( 0, Qt::UserRole).
                                            value<PrefsItemData *>();
            if( data->panel && doclean )
            {
                delete data->panel;
                data->panel = NULL;
            }
            else if( data->panel )
                data->panel->apply();
        }
        PrefsItemData *data = cat_item->data( 0, Qt::UserRole).
                                            value<PrefsItemData *>();
        if( data->panel && doclean )
        {
            delete data->panel;
            data->panel = NULL;
        }
        else if( data->panel )
            data->panel->apply();
    }
}

/*********************************************************************
 * The Panel
 *********************************************************************/
AdvPrefsPanel::AdvPrefsPanel( QWidget *_parent ) : QWidget( _parent )
{}

AdvPrefsPanel::AdvPrefsPanel( intf_thread_t *_p_intf, QWidget *_parent,
                        PrefsItemData * data ) :
                        QWidget( _parent ), p_intf( _p_intf )
{
    /* Find our module */
    module_t *p_module = NULL;
    if( data->i_type == TYPE_CATEGORY )
        return;
    else if( data->i_type == TYPE_MODULE )
        p_module = module_find( data->psz_name );
    else
    {
        p_module = module_get_main();
        assert( p_module );
    }

    unsigned confsize;
    module_config_t *const p_config = module_config_get (p_module, &confsize),
                    *p_item = p_config,
                    *p_end = p_config + confsize;

    if( data->i_type == TYPE_SUBCATEGORY || data->i_type ==  TYPE_CATSUBCAT )
    {
        while (p_item < p_end)
        {
            if(  p_item->i_type == CONFIG_SUBCATEGORY &&
                            ( ( data->i_type == TYPE_SUBCATEGORY &&
                              p_item->value.i == data->i_object_id ) ||
                            ( data->i_type == TYPE_CATSUBCAT &&
                              p_item->value.i == data->i_subcat_id ) ) )
                break;
            p_item++;
        }
    }

    /* Widgets now */
    global_layout = new QVBoxLayout();
    global_layout->setMargin( 2 );
    QString head;
    QString help;

    help = QString( data->help );

    if( data->i_type == TYPE_SUBCATEGORY || data->i_type ==  TYPE_CATSUBCAT )
    {
        head = QString( data->name );
        p_item++; // Why that ?
    }
    else
    {
        const char *psz_help = module_get_help (p_module);
        head = QString( qtr( module_GetLongName( p_module ) ) );
        if( psz_help )
        {
            help.append( "\n" );
            help.append( qtr( psz_help ) );
        }
    }

    QLabel *titleLabel = new QLabel( head );
    QFont titleFont = QApplication::font( static_cast<QWidget*>(0) );
    titleFont.setPointSize( titleFont.pointSize() + 6 );
    titleFont.setFamily( "Verdana" );
    titleLabel->setFont( titleFont );

    // Title <hr>
    QFrame *title_line = new QFrame;
    title_line->setFrameShape(QFrame::HLine);
    title_line->setFrameShadow(QFrame::Sunken);

    QLabel *helpLabel = new QLabel( help, this );
    helpLabel->setWordWrap( true );

    global_layout->addWidget( titleLabel );
    global_layout->addWidget( title_line );
    global_layout->addWidget( helpLabel );

    QGroupBox *box = NULL;
    QGridLayout *boxlayout = NULL;

    QScrollArea *scroller= new QScrollArea;
    scroller->setFrameStyle( QFrame::NoFrame );
    QWidget *scrolled_area = new QWidget;

    QGridLayout *layout = new QGridLayout();
    int i_line = 0, i_boxline = 0;
    bool has_hotkey = false;

    if( p_item ) do
    {
        if( ( ( data->i_type == TYPE_SUBCATEGORY &&
                p_item->value.i != data->i_object_id ) ||
              ( data->i_type == TYPE_CATSUBCAT  &&
                p_item->value.i != data->i_subcat_id ) ) &&
            ( p_item->i_type == CONFIG_CATEGORY ||
              p_item->i_type == CONFIG_SUBCATEGORY ) )
            break;
        if( p_item->b_internal == true ) continue;

        if( p_item->i_type == CONFIG_SECTION )
        {
            if( box )
            {
                box->setLayout( boxlayout );
                box->show();
                layout->addWidget( box, i_line, 0, 1, -1 );
                i_line++;
            }
            box = new QGroupBox( qtr( p_item->psz_text ), this );
            box->hide();
            boxlayout = new QGridLayout();
        }
        /* Only one hotkey control */
        if( has_hotkey && p_item->i_type & CONFIG_ITEM && p_item->psz_name &&
                                         strstr( p_item->psz_name, "key-" ) )
            continue;
        if( p_item->i_type & CONFIG_ITEM && p_item->psz_name &&
                                            strstr( p_item->psz_name, "key-" ) )
            has_hotkey = true;

        ConfigControl *control;
        if( ! box )
            control = ConfigControl::createControl( VLC_OBJECT( p_intf ),
                                        p_item, this, layout, i_line );
        else
            control = ConfigControl::createControl( VLC_OBJECT( p_intf ),
                                    p_item, this, boxlayout, i_boxline );
        if( !control )
            continue;

        if( box ) i_boxline++;
        else i_line++;
        controls.append( control );
    }
    while( !( ( data->i_type == TYPE_SUBCATEGORY ||
               data->i_type == TYPE_CATSUBCAT ) &&
             ( p_item->i_type == CONFIG_CATEGORY ||
               p_item->i_type == CONFIG_SUBCATEGORY ) )
        && ( ++p_item < p_end ) );

    if( box )
    {
        box->setLayout( boxlayout );
        box->show();
        layout->addWidget( box, i_line, 0, 1, -1 );
    }

    module_release (p_module);

    scrolled_area->setSizePolicy( QSizePolicy::Preferred,QSizePolicy::Fixed );
    scrolled_area->setLayout( layout );
    scroller->setWidget( scrolled_area );
    scroller->setWidgetResizable( true );
    global_layout->addWidget( scroller );
    setLayout( global_layout );
}

void AdvPrefsPanel::apply()
{
    QList<ConfigControl *>::Iterator i;
    for( i = controls.begin() ; i != controls.end() ; i++ )
    {
        ConfigControl *c = qobject_cast<ConfigControl *>(*i);
        c->doApply( p_intf );
    }
}

void AdvPrefsPanel::clean()
{}

AdvPrefsPanel::~AdvPrefsPanel()
{
    qDeleteAll( controls ); controls.clear();
}


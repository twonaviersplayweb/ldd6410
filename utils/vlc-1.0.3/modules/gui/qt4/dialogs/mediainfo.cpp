/*****************************************************************************
 * mediainfo.cpp : Information about an item
 ****************************************************************************
 * Copyright (C) 2006-2008 the VideoLAN team
 * $Id$
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *          Jean-Baptiste Kempf <jb@videolan.org>
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
 ******************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "dialogs/mediainfo.hpp"
#include "input_manager.hpp"

#include <QTabWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>

MediaInfoDialog *MediaInfoDialog::instance = NULL;

/* This Dialog has two main modes:
    - General Mode that shows the current Played item, and the stats
    - Single mode that shows the info on ONE SINGLE Item on the playlist
   Please be Careful of not breaking one the modes behaviour... */

MediaInfoDialog::MediaInfoDialog( intf_thread_t *_p_intf,
                                  input_item_t *p_item ) :
                                  QVLCFrame( _p_intf )
{
    isMainInputInfo = ( p_item == NULL );

    setWindowTitle( qtr( "Media Information" ) );

    /* TabWidgets and Tabs creation */
    infoTabW = new QTabWidget;

    MP = new MetaPanel( infoTabW, p_intf );
    infoTabW->addTab( MP, qtr( "&General" ) );
    EMP = new ExtraMetaPanel( infoTabW, p_intf );
    infoTabW->addTab( EMP, qtr( "&Extra Metadata" ) );
    IP = new InfoPanel( infoTabW, p_intf );
    infoTabW->addTab( IP, qtr( "&Codec Details" ) );
    if( isMainInputInfo )
    {
        ISP = new InputStatsPanel( infoTabW, p_intf );
        infoTabW->addTab( ISP, qtr( "&Statistics" ) );
    }

    QGridLayout *layout = new QGridLayout( this );

    /* No need to use a QDialogButtonBox here */
    saveMetaButton = new QPushButton( qtr( "&Save Metadata" ) );
    saveMetaButton->hide();
    QPushButton *closeButton = new QPushButton( qtr( "&Close" ) );
    closeButton->setDefault( true );

    QLabel *uriLabel = new QLabel( qtr( "Location:" ) );
    QLineEdit *uriLine = new QLineEdit;

    layout->addWidget( infoTabW, 0, 0, 1, 8 );
    layout->addWidget( uriLabel, 1, 0, 1, 1 );
    layout->addWidget( uriLine, 1, 1, 1, 7 );
    layout->addWidget( saveMetaButton, 2, 6 );
    layout->addWidget( closeButton, 2, 7 );

    BUTTONACT( closeButton, close() );

    /* The tabs buttons are shown in the main dialog for space and cosmetics */
    BUTTONACT( saveMetaButton, saveMeta() );

    /* Let the MetaData Panel update the URI */
    CONNECT( MP, uriSet( const QString& ), uriLine, setText( const QString& ) );
    CONNECT( MP, editing(), saveMetaButton, show() );

    /* Display the buttonBar according to the Tab selected */
    CONNECT( infoTabW, currentChanged( int ), this, updateButtons( int ) );

    /* If using the General Mode */
    if( isMainInputInfo )
    {
        msg_Dbg( p_intf, "Using a general info windows" );
        /**
         * Connects on the various signals of input_Manager
         * For the currently playing element
         **/
        CONNECT( THEMIM->getIM(), infoChanged( input_item_t* ),
                 IP, update( input_item_t* ) );
        CONNECT( THEMIM->getIM(), metaChanged( input_item_t* ),
                 MP, update( input_item_t* ) );
        CONNECT( THEMIM->getIM(), metaChanged( input_item_t* ),
                 EMP, update( input_item_t* ) );
        CONNECT( THEMIM->getIM(), statisticsUpdated( input_item_t* ),
                 ISP, update( input_item_t* ) );

        if( THEMIM->getInput() )
            p_item = input_GetItem( THEMIM->getInput() );
    }
    else
        msg_Dbg( p_intf, "Using an item specific info windows" );

    /* Call update at start, so info is filled up at begginning */
    if( p_item )
        updateAllTabs( p_item );

    readSettings( "Mediainfo", QSize( 600 , 480 ) );
}

MediaInfoDialog::~MediaInfoDialog()
{
    writeSettings( "Mediainfo" );
}

void MediaInfoDialog::showTab( int i_tab = 0 )
{
    infoTabW->setCurrentIndex( i_tab );
    show();
}

void MediaInfoDialog::saveMeta()
{
    MP->saveMeta();
    saveMetaButton->hide();
}

void MediaInfoDialog::updateAllTabs( input_item_t *p_item )
{
    IP->update( p_item );
    MP->update( p_item );
    EMP->update( p_item );

    if( isMainInputInfo ) ISP->update( p_item );
}

void MediaInfoDialog::clearAllTabs()
{
    IP->clear();
    MP->clear();
    EMP->clear();

    if( isMainInputInfo ) ISP->clear();
}

void MediaInfoDialog::close()
{
    hide();

    /* if dialog is closed, revert editing if not saved */
    if( MP->isInEditMode() )
    {
        MP->setEditMode( false );
        updateButtons( 0 );
    }

    if( !isMainInputInfo )
        deleteLater();
}

void MediaInfoDialog::updateButtons( int i_tab )
{
    if( MP->isInEditMode() && i_tab == 0 )
        saveMetaButton->show();
    else
        saveMetaButton->hide();
}


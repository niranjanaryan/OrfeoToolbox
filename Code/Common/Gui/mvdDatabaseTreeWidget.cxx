/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "Gui/mvdDatabaseTreeWidget.h"
//#include "Gui/ui_mvdDatabaseTreeWidget.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdAlgorithm.h"

namespace mvd
{

/*
  TRANSLATOR mvd::DatabaseTreeWidget

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
DatabaseTreeWidget
::DatabaseTreeWidget( QWidget* parent  ):
  QTreeWidget( parent),
  m_DatasetFilename("")
{
  //setMouseTracking(true);
  setDragEnabled(true);

  // setup contextual menu
  InitializeContextualMenu();
}

/*******************************************************************************/
DatabaseTreeWidget
::~DatabaseTreeWidget()
{
}

/*******************************************************************************/
void
DatabaseTreeWidget
::InitializeContextualMenu()
{
  setContextMenuPolicy(Qt::CustomContextMenu);
  
  QObject::connect(this,
                   SIGNAL(customContextMenuRequested(const QPoint&)),
                   SLOT(OnCustomContextMenuRequested(const QPoint&))
    );
}

/*******************************************************************************/
void 
DatabaseTreeWidget::mouseMoveEvent( QMouseEvent * event )
{
  if ( !(event->buttons() &  Qt::LeftButton ))
    return;
  
  //start Drag ?
  int distance = ( event->pos() - m_StartDragPosition ).manhattanLength();
  if (distance < QApplication::startDragDistance())
    return;
    

  //Get current selection
  QTreeWidgetItem *selectedItem = currentItem();

  if ( selectedItem && selectedItem->parent() )
    {
    //TODO : get the image filename  of the selected dataset
    QByteArray itemData( ToStdString (m_DatasetFilename ).c_str() );
 
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-qabstractitemmodeldatalist", itemData);

    //Create drag
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    drag->exec(Qt::CopyAction | Qt::MoveAction);
    }
}

/*******************************************************************************/
void
DatabaseTreeWidget::mousePressEvent(QMouseEvent *event)
{   
  if (event->button() == Qt::LeftButton)
     {
     //
     // superclass event handling
     QTreeWidget::mousePressEvent(event);
     
     // remember start drag position
     m_StartDragPosition = event->pos();
     }
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
DatabaseTreeWidget::OnSelectedDatasetFilenameChanged(const QString& filename)
{
  //
  // update the dataset image filename to be used in the dragged mime data
  m_DatasetFilename = filename;
}

/*******************************************************************************/
void
DatabaseTreeWidget::OnDeleteTriggered( const QString & id)
{
  emit DatasetToDeleteSelected( id );
}

/*******************************************************************************/
void
DatabaseTreeWidget::OnCustomContextMenuRequested(const QPoint& pos)
{
  // get the item
  QTreeWidgetItem* item = itemAt(pos);
  
  // if not the root item 
  if ( item && item->parent() ) 
    {
    // menu 
    QMenu  menu;
    
    // create the desired action
    QAction * deleteNodeChild = new QAction(tr ("Delete Dataset") , &menu);

    // use a QSignalMapper to bundle parameterless signals and re-emit
    // them with parameters (QString here)
    QSignalMapper *signalMapper = new QSignalMapper( this );
    signalMapper->setMapping( deleteNodeChild, item->text(0) );
    
    QObject::connect( deleteNodeChild , 
                      SIGNAL(triggered()), 
                      signalMapper, 
                      SLOT( map() ) 
      );

    // add action to the menu
    menu.addAction( deleteNodeChild );

    // connect the re-emitted signal to our slot 
    QObject::connect( 
      signalMapper, 
      SIGNAL(mapped(const QString &)), 
      SLOT( OnDeleteTriggered(const QString &)) 
      );

    // show menu
    menu.exec( viewport()->mapToGlobal(pos) );
    }
}

} // end namespace 'mvd'


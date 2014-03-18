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
#include "mvdDatabaseTreeWidget.h"


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
#include "Core/mvdDataStream.h"
#include "Gui/mvdTreeWidgetItem.h"

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
  TreeWidget( parent ),
  m_DatasetFilename(),
  m_EditionActive( false )
{
  // setMouseTracking(true);

  //
  // do some connection
  QObject::connect(
    this,
    SIGNAL( itemChanged( QTreeWidgetItem* , int ) ),
    SLOT( OnItemChanged( QTreeWidgetItem* , int ) )
  );

  QObject::connect(
    this,
    SIGNAL( customContextMenuRequested( const QPoint& ) ),
    SLOT( OnCustomContextMenuRequested( const QPoint& ) )
  );
}

/*******************************************************************************/
DatabaseTreeWidget
::~DatabaseTreeWidget()
{
}

/*******************************************************************************/
void 
DatabaseTreeWidget::mouseMoveEvent( QMouseEvent * event )
{
#if BYPASS_MOUSE_EVENTS
  TreeWidget::mouseMoveEvent( event );

#else // BYPASS_MOUSE_EVENTS
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
#endif // BYPASS_MOUSE_EVENTS
}

/*******************************************************************************/
void
DatabaseTreeWidget::mousePressEvent(QMouseEvent *event)
{
#if BYPASS_MOUSE_EVENTS
  TreeWidget::mousePressEvent( event );

#else // BYPASS_MOUSE_EVENTS
  if (event->button() == Qt::LeftButton)
     {
     //
     // superclass event handling
     TreeWidget::mousePressEvent(event);
     
     // remember start drag position
     m_StartDragPosition = event->pos();
     }

#endif // BYPASS_MOUSE_EVENTS
}

/*******************************************************************************/
void
DatabaseTreeWidget::dropEvent(QDropEvent *event)
{
  // qDebug() << this << "::dropEvent(" << event << ")";

  /*
  if( event->mimeData()->hasFormat( "application/x-qabstractitemmodeldatalist" ) )
    {
    QByteArray byteArray(
      event->mimeData()->data( "application/x-qabstractitemmodeldatalist" )
    );

    QDataStream stream( &byteArray, QIODevice::ReadOnly );

    int count = 0;

    //
    // http://www.qtcentre.org/threads/8756-QTreeWidgetItem-mime-type

    typedef QMap< int, QVariant > QIntToVariantMap;

    struct Item
    {
      Item() : row( -1 ), col( -1 ), var() {}

      int row;
      int col;
      QIntToVariantMap var;
    };

    while( !stream.atEnd() )
      {
      Item item;

      stream >> item.row >> item.col >> item.var;

      qDebug() << item.row << item.col << item.var;

      ++ count;
      }

    qDebug() << count << "items.";
    }
  */

  /*
  if( event->mimeData()->hasFormat( "application/x-qtreewidgetitemptrlist" ) )
    {
    QByteArray byteArray(
      event->mimeData()->data( "application/x-qtreewidgetitemptrlist" )
    );

    QDataStream stream( &byteArray, QIODevice::ReadOnly );

    int count = 0;

    //
    // http://www.qtcentre.org/threads/8756-QTreeWidgetItem-mime-type

    QTreeWidgetItem* varItem = NULL;

    while( !stream.atEnd() )
      {
      QVariant variant;

      stream >> variant;

      qDebug() << "Variant:" << variant;

      // http://www.qtfr.org/viewtopic.php?id=9630

      varItem = variant.value< QTreeWidgetItem* >();

      qDebug()
        << "Item (variant):"
        << varItem
        << varItem->text( 0 )
        << varItem->text( 1 )
        << varItem->text( 2 )
        << varItem->parent();

      ++ count;
      }

    qDebug() << count << "items.";
    }
  */

  TreeWidget::dropEvent( event );
}

/*******************************************************************************/
bool
DatabaseTreeWidget::dropMimeData( QTreeWidgetItem* parent,
                                  int index,
                                  const QMimeData* data,
                                  Qt::DropAction action )
{
  qDebug()
    << this << "::dropMimeData("
    << parent << ","
    << index << ","
    << data << ","
    << action << ")";

  bool result = TreeWidget::dropMimeData( parent, index, data, action );

  qDebug() << "->" << result;

  return result;
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
DatabaseTreeWidget
::OnSelectedDatasetFilenameChanged( const QString& filename )
{
#if BYPASS_MOUSE_EVENTS
#else // BYPASS_MOUSE_EVENTS
  //
  // update the dataset image filename to be used in the dragged mime data
  m_DatasetFilename = filename;
#endif // BYPASS_MOUSE_EVENTS
}

/*******************************************************************************/
void
DatabaseTreeWidget::OnDeleteTriggered( const QString & id)
{
  emit DatasetToDeleteSelected( id );
}

/*******************************************************************************/
void
DatabaseTreeWidget::OnRenameTriggered()
{
  // find the QTreeWidgetItem and make it editable
  m_ItemToEdit = itemAt( m_ContextualMenuClickedPosition );
  m_PreviousItemText = m_ItemToEdit->text(0);
  // m_DefaultItemFlags  = m_ItemToEdit->flags(); 
  // m_ItemToEdit->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);

  // edit item
  // inspired from here:
  // http://www.qtcentre.org/archive/index.php/t-45857.html?s=ad4e7c45bbd9fd4bf5cc32853dd3fe82  
  m_EditionActive = true;
  editItem(m_ItemToEdit, 0);
}

/*******************************************************************************/
void
DatabaseTreeWidget::OnItemChanged( QTreeWidgetItem* item , int column)
{
  if (m_EditionActive )
    {
    if (!item->text(column).isEmpty() )
      {
      // dynamic cast to get the id
      TreeWidgetItem* dsItem = 
        dynamic_cast<TreeWidgetItem *>( item );

      // send the new alias of the dataset / it identifier
      emit DatasetRenamed(dsItem->text(column), dsItem->GetHash()  );
      }
    else
      {
      m_ItemToEdit->setText( column, m_PreviousItemText );
      }

    // initialize all needed variables
    m_PreviousItemText.clear();
    m_EditionActive = false;

    // set back default item flags
    // m_ItemToEdit->setFlags( m_DefaultItemFlags );
    }
}

/*******************************************************************************/
void
DatabaseTreeWidget
::OnAddItemTriggered()
{
  QTreeWidgetItem* item =  itemAt( m_ContextualMenuClickedPosition );
  assert( item!=NULL );

  emit AddItemRequested( item );
}

/******************************************************************************/
void
DatabaseTreeWidget
::keyPressEvent( QKeyEvent * event )
{
  // triggered only if an item (and not the root one) is selected
  if ( currentItem() && currentItem()->parent() )
    {
    switch (event->key())
      {
      case Qt::Key_F2:
      {
      // find the QTreeWidgetItem and make it editable
      m_ItemToEdit = currentItem();
      m_PreviousItemText = m_ItemToEdit->text(0);
      // m_DefaultItemFlags  = m_ItemToEdit->flags(); 
      // m_ItemToEdit->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);

      // edit item
      // inspired from here:
      // http://www.qtcentre.org/archive/index.php/t-45857.html?s=ad4e7c45bbd9fd4bf5cc32853dd3fe82  
      m_EditionActive = true;
      editItem(m_ItemToEdit, 0);
      
      break;
      }
      case Qt::Key_Delete:
      {
      // cast to DatasetTreeItem
      TreeWidgetItem* item 
        = dynamic_cast<TreeWidgetItem *>( currentItem());
        
      emit DatasetToDeleteSelected( item->GetHash() );

      break;
      }
      }

    }
}

/*******************************************************************************/
void
DatabaseTreeWidget::OnCustomContextMenuRequested(const QPoint& pos)
{
  // Get  item.
  QTreeWidgetItem* qtwi = itemAt( pos );
  assert( qtwi!=NULL );

  TreeWidgetItem* item =
    dynamic_cast< TreeWidgetItem* >( itemAt( pos ) );
  assert( item!=NULL );


  /*
  QString itemType;

  switch( item->type() )
    {
    case TreeWidgetItem::ITEM_TYPE_NONE:
      break;

    case TreeWidgetItem::ITEM_TYPE_NODE:
      itemType = tr( "group" );
      break;

    case TreeWidgetItem::ITEM_TYPE_LEAF:
      itemType = tr( "dataset" );
      break;

    default:
      assert( false && "Unhandled TreeWidgetItem::ItemType value!" );
      break;
    };
  */

  // Remember context-menu clicked position.
  //
  // This is done to later find cicked item because QTreeWidgetItem is
  // not derived from QObject and QSignalMapper only supports int,
  // QString, QObject and QWiget.
  //
  // This is also the reason why delete and rename triggers are
  // duplicated.
  m_ContextualMenuClickedPosition = pos;

  // Create context-menu.
  QMenu menu;

  // Insert actions into context-menu.
  if( item->parent()!=NULL &&
      item->type()==TreeWidgetItem::ITEM_TYPE_LEAF )
    {
    AddMappedAction(
      &menu,
      tr( "Delete dataset" ),
      item->GetHash(),
      this,
      SIGNAL( DatasetToDeleteSelected( const QString& ) )
    );

    if( item->flags().testFlag( Qt::ItemIsEditable ) )
      AddAction(
        &menu,
        tr ("Rename dataset"),
        this, 
        SLOT( OnRenameTriggered() )
      );
    }

  if( item->type()==TreeWidgetItem::ITEM_TYPE_NODE )
    {
    AddAction(
      &menu,
      tr( "Add group" ),
      this,
      SLOT( OnAddItemTriggered() )
    );

    /*
    AddMappedAction(
      &menu,
      tr( "Delete group" ),
      // Pass ID by string because it is a 'long long' and
      // QSignalMapper only supports int.
      item->GetId().toString(),
      this,
      SIGNAL( DeleteGroupTriggered( const QString& ) )
    );
    */

    /*
    if( item->flags().testFlag( Qt::ItemIsEditable ) )
      AddMappedAction(
        &menu,
        tr( "Rename group" ),
        // Pass ID by string because it is a 'long long' and
        // QSignalMapper only supports int.
        item->GetId().toString(),
        this,
        SLOT( OnRenameGroupTriggered( const QString& ) )
      );
    */
    }

  /*
  AddMappedAction(
    &menu,
    tr( "Delete" ) + " " + itemType,
    item,
    this,
    SIGNAL( DeletedItemRequested( QTreeWidgetItem* ) )
  );
  */

  /*
  AddAction(
    &menu,
    tr( "Rename" ) + " " + itemType,
    item,
    this,
    SLOT( OnEditItemText( QTreeWidgetItem * ) )
  );
  */

  // Display and activate context-menu.
  menu.exec( viewport()->mapToGlobal( pos ) );
}

/*******************************************************************************/
QAction*
DatabaseTreeWidget::AddAction( QMenu* menu,
                               const QString& text,
                               QObject* receiver,
                               const char* method,
                               Qt::ConnectionType type )
{
  assert( menu!=NULL );
  assert( receiver!=NULL );
  assert( method!=NULL );

  QAction* action = menu->addAction( text );

  QObject::connect(
    action,
    SIGNAL( triggered() ),
    // to:
    receiver,
    method,
    // with:
    type
  );

  return action;
}

/*******************************************************************************/
QAction*
DatabaseTreeWidget::AddMappedAction( QMenu* menu,
                                     const QString& text,
                                     const QString& data,
                                     QObject* receiver,
                                     const char* slot,
                                     Qt::ConnectionType type )
{
  assert( menu!=NULL );
  assert( receiver!=NULL );
  assert( slot!=NULL );

  // Add signal-mapped action into menu.
  QSignalMapper* signalMapper = AddMappedAction( menu, text );
  assert(
    signalMapper->parent()==qobject_cast< QAction* >( signalMapper->parent() )
  );

  // Get action back.
  QAction* action = qobject_cast< QAction* >( signalMapper->parent() );
  assert( action!=NULL );

  // Set signal mapping.
  signalMapper->setMapping( action, data );

  // Connect mapped signal.
  QObject::connect(
    signalMapper,
    SIGNAL( mapped( const QString& ) ),
    // to:
    receiver,
    slot,
    // with:
    type
  );


  return action;
}

/*******************************************************************************/
QSignalMapper *
DatabaseTreeWidget::AddMappedAction( QMenu* menu,
                                     const QString& text )
{
  assert( menu!=NULL );

  // Add menu action.
  QAction * action = menu->addAction( text );

  // Pre-create signal-mapper.
  QSignalMapper* signalMapper = new QSignalMapper( action );

  // Connect menu-action trigger to signal-mapper.
  QObject::connect(
    action,
    SIGNAL( triggered() ),
    // to:
    signalMapper,
    SLOT( map() )
  );

  // Return parented signal-mapper.
  return signalMapper;
}

} // end namespace 'mvd'

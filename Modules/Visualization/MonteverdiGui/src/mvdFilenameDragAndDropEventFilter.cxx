/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mvdFilenameDragAndDropEventFilter.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)
#include <cassert>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)

namespace mvd
{

/*
  TRANSLATOR mvd::FilenameDragAndDropEventFilter

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*****************************************************************************/
FilenameDragAndDropEventFilter::FilenameDragAndDropEventFilter(QObject* p) : AbstractDragAndDropEventFilter(p)
{
}

/*****************************************************************************/
FilenameDragAndDropEventFilter::~FilenameDragAndDropEventFilter()
{
}

/*****************************************************************************/
bool FilenameDragAndDropEventFilter::DragEnterEvent(QObject* watched, QDragEnterEvent* e)
{
  // qDebug() << this << "::DragEnterEvent(" << object << "," << e << ")";
  // qDebug() << e->mimeData()->formats();

  //
  // Bypass event its MIME data does not contain any URL(s).
  if (!e->mimeData()->hasUrls())
    return false;

  //
  // Bypass event if MIME data URL(s) are not all local filenames.
  typedef QList<QUrl> QUrlList;

  QUrlList urls(e->mimeData()->urls());

  for (QUrlList::const_iterator it(urls.begin()); it != urls.end(); ++it)
  {
    // qDebug() << *it;
    // qDebug() << it->scheme()
    // 	     << it->scheme().compare( "file", Qt::CaseInsensitive );

    if (
#if QT_VERSION < QT_VERSION_CHECK(4, 8, 0)
        it->scheme().compare("file", Qt::CaseInsensitive) != 0
#else  // QT_VERSION < QT_VERSION_CHECK( 4, 8, 0 )
        !it->isLocalFile()
#endif // QT_VERSION < QT_VERSION_CHECK( 4, 8, 0 )
        || e->source() == watched)
    {
      return false;
    }
  }

  //
  // Accept event if its MIME data contains some URL(s) and they are
  // all local filenames.
  e->acceptProposedAction();

  //
  // Eatup event.
  return true;
}

/*****************************************************************************/
bool FilenameDragAndDropEventFilter::DragLeaveEvent(QObject* /* object */, QDragLeaveEvent* /* e */)
{
  // qDebug() << this << "::DragLeaveEvent(" << object << "," << event << ")";

  //
  // Nothing to do: bypass event & let default behaviour occur.
  return false;
}

/*****************************************************************************/
bool FilenameDragAndDropEventFilter::DragMoveEvent(QObject*, QDragMoveEvent*)
{
  // qDebug() << this << "::DragMoveEvent(" << object << "," << event << ");";

  // Nothing to do: bypass event & let default behaviour occur.
  return false;
}

/*****************************************************************************/
bool FilenameDragAndDropEventFilter::DropEvent(QObject* watched, QDropEvent* e)
{
  // qDebug() << this << "::DropEvent(" << object << "," << e << ")";

  assert(e != NULL);
  assert(e->mimeData() != NULL);

  //
  // Bypass event its MIME data does not contain not URL(s).
  if (!e->mimeData()->hasUrls())
    return false;

  //
  // Bypass event if MIME data URL(s) are not all local filenames.
  typedef QList<QUrl> QUrlList;

  QUrlList urls(e->mimeData()->urls());

  QStringList filenames;

  for (QUrlList::const_iterator it(urls.begin()); it != urls.end(); ++it)
  {
#if QT_VERSION < QT_VERSION_CHECK(4, 8, 0)
    if (it->scheme().compare("file", Qt::CaseInsensitive) == 0)
#else  // QT_VERSION < QT_VERSION_CHECK( 4, 8, 0 )
    if (it->isLocalFile())
#endif // QT_VERSION < QT_VERSION_CHECK( 4, 8, 0 )
    {
      if (e->source() != watched)
      {
        filenames.push_back(it->toLocalFile());

        Q_EMIT FilenameDropped(filenames.back());
      }
    }
    else
    {
      qWarning() << "Dropped URL is not a local filename:" << *it;
    }
  }

  Q_EMIT FilenamesDropped(filenames);

  //
  // Eatup event.
  return true;
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

} // end namespace 'mvd'

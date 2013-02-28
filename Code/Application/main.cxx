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

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

// Monteverdi includes (sorted by alphabetic order)
#include "mvdApplication.h"
#include "mvdMainWindow.h"
#include "mvdDatasetModel.h"

//
// OTB includes (sorted by alphabetic order)

//
// MAIN
//
int
main( int argc, char* argv[] )
{
  mvd::Application application( argc, argv );

  // TODO: Move in correct place when UI is displayed.
  application.MakeCacheDir();

  //
  // Force numeric options of locale to "C"
  // See issue #635
  //
  // TODO: Move into I18nApplication.
  setlocale( LC_NUMERIC, "C" );
    
  mvd::MainWindow mainWindow;


#if 0
  // TODO: Correctly manage main-window state via application settings.
  mainWindow.showMaximized();
#else
  // Usefull when developping/debugging to avoid overlapping other windows.
  mainWindow.show();
#endif

  // This code is here to propagate events from maximization to child
  // widgets, so that an image loaded from command-line will get the
  // appropriate widget size and occupy as much space as possible on screen.
  application.processEvents();

  // TODO: Move into mvd::Application.
  // Handle passing image filename from command-line
  if(argc>1)
    {
    try
      {
      // TODO: Replace with complex model (list of DatasetModel) when implemented.
      mvd::DatasetModel* model = mvd::Application::LoadDatasetModel(
        argv[1],
        // TODO: Remove width and height from dataset model loading.
        mainWindow.centralWidget()->width(),
        mainWindow.centralWidget()->height());

      mvd::Application::Instance()->SetModel( model );
      }
    catch( std::exception& exc )
      {
      // TODO: Report something usefull here
      }
    }
  
  return application.exec();
}

//
// Main functions implementations.
//

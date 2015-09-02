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

#include "mvdTypes.h"

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

namespace mvd
{
/*
  TRANSLATOR mvd::Types

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*******************************************************************************/
const char*
BOUND_NAMES[ BOUND_COUNT ] = {
  "BOUND_LOWER",
  "BOUND_UPPER"
};

char const * const
EFFECT_NAME[ EFFECT_COUNT ] =
{
  // QT_TRANSLATE_NOOP( "mvd", "None" ),
  QT_TRANSLATE_NOOP( "mvd", "Chessboard" ),
  QT_TRANSLATE_NOOP( "mvd", "Gradient" ),
  QT_TRANSLATE_NOOP( "mvd", "Local contrast" ),
  QT_TRANSLATE_NOOP( "mvd", "Local translucency" ),
  QT_TRANSLATE_NOOP( "mvd", "Normal" ),
  QT_TRANSLATE_NOOP( "mvd", "Spectral angle" ),
  QT_TRANSLATE_NOOP( "mvd", "Swipe (horizontal)" ),
  QT_TRANSLATE_NOOP( "mvd", "Swipe (vertical)" ),
};

} // end namespace 'mvd'
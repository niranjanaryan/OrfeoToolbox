/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbWrapperInputImageParameter.h"
#include "itksys/SystemTools.hxx"
#include "otbWrapperTypes.h"
#include "otbWrapperInputImageParameterMacros.h"
#include <boost/algorithm/string.hpp>

namespace otb
{
namespace Wrapper
{
otbGetImageMacro(UInt16Image);
otbGetImageMacro(UInt16VectorImage)
otbGenericCastImageMacro(UInt16ImageType, SimpleCastImage, )
otbGenericCastImageMacro(UInt16VectorImageType, SimpleCastImage, Vector)
otbGenericCastImageMacro(UInt16ImageType, CastVectorImageFromImage, Vector)
}
}

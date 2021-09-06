/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbOpticalDefaultImageMetadataInterface_h
#define otbOpticalDefaultImageMetadataInterface_h


#include "otbMacro.h"

#include "otbOpticalImageMetadataInterface.h"

namespace otb
{
/** \class OpticalDefaultImageMetadataInterface
 *
 * \brief Creation of an "otb" DefaultImageMetadataInterface that gets metadata.
 *
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT OpticalDefaultImageMetadataInterface : public OpticalImageMetadataInterface
{
public:
  typedef OpticalDefaultImageMetadataInterface Self;
  typedef OpticalImageMetadataInterface        Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OpticalDefaultImageMetadataInterface, OpticalImageMetadataInterface);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType     ImageKeywordlistType;

  /** Get the first wavelength for the spectral band definition */
  VariableLengthVectorType GetFirstWavelengths() const override
  {
    itkExceptionMacro("GetFirstWavelengths not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the last wavelength for the spectral band definition */
  VariableLengthVectorType GetLastWavelengths() const override
  {
    itkExceptionMacro("GetLastWavelengths not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the enhanced band names (here nothing because the sensor is not identify) */
  std::vector<std::string> GetEnhancedBandNames() const override
  {
    std::vector<std::string> nothing;
    return nothing;
  }

  bool CanRead() const override
  {
    // This class is the default one, it has to be able to call every metadata
    return false;
  }

protected:
  OpticalDefaultImageMetadataInterface() = default;
  ~OpticalDefaultImageMetadataInterface() override = default;

private:
  OpticalDefaultImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif

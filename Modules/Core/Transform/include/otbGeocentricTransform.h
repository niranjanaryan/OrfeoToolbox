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

#ifndef otbGeocentricTransform_h
#define otbGeocentricTransform_h

#include "otbCoordinateTransformation.h"
#include "otbTransform.h"

namespace otb
{
/** \class GeocentricTransform
 *
 * \brief Convert coordinates from geocentric (X, Y, Z) to geographic (lon, lat, h)
 *
 *
 * \ingroup OTBTransform
 */
template <TransformDirection TDirectionOfMapping, class TScalarType = double>
class ITK_EXPORT       GeocentricTransform : public Transform<TScalarType, // Data type for scalars
                                                        3,  // Number of dimensions in the input space
                                                        3> // Number of dimensions in the output space
{
public:
  /** Standard class typedefs. */
  typedef GeocentricTransform Self;
  typedef Transform<TScalarType, 3, 3> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef typename Superclass::ScalarType ScalarType;
  typedef itk::Point<ScalarType, 3>  InputPointType;
  typedef itk::Point<ScalarType, 3> OutputPointType;


  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GeocentricTransform, Transform);

  static const TransformDirection DirectionOfMapping = TDirectionOfMapping;

  itkStaticConstMacro(InputSpaceDimension, unsigned int, 3);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, 3);
  itkStaticConstMacro(SpaceDimension, unsigned int, 3);
  itkStaticConstMacro(ParametersDimension, unsigned int, 3*(3 + 1));

  OutputPointType TransformPoint(const InputPointType& point) const override;

protected:
  GeocentricTransform();
  ~GeocentricTransform() override = default;

private:
  GeocentricTransform(const Self&) = delete;
  void operator=(const Self&) = delete;

  std::unique_ptr<CoordinateTransformation> m_MapProjection;

  double m_a;
  double m_b;
  double m_f;
  double m_es;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGeocentricTransform.hxx"
#endif

#endif

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
#ifndef otbStreamingBufferGenerator_h
#define otbStreamingBufferGenerator_h

#include "itkImageToImageFilter.h"
#include "itkProgressReporter.h"
#include "otbRAMDrivenAdaptativeStreamingManager.h"

namespace otb
{

/**
 * \class StreamingBufferGenerator
 * \brief This filter generates an output image with an internal
 * explicit streaming mechanism.
 *
 * \ingroup OTBImageIO
 */
template <class TInputImage>
class ITK_EXPORT StreamingBufferGenerator : public itk::ImageToImageFilter<TInputImage, TInputImage>
{

public:
  /** Standard class typedefs. */
  using Self = StreamingBufferGenerator;
  using Superclass = itk::ImageToImageFilter<TInputImage, TInputImage>;
  using Pointer = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(StreamingBufferGenerator, itk::ImageToImageFilter);

  /** Images typedefs */
  using ImageType = typename Superclass::InputImageType;
  using RegionType = typename Superclass::InputImageRegionType;

  /** Streaming manager base class pointer */
  using RAMDrivenAdaptativeStreamingManagerType = RAMDrivenAdaptativeStreamingManager<TInputImage>;
  using StreamingManagerPointerType = typename RAMDrivenAdaptativeStreamingManagerType::Pointer;

protected:
  StreamingBufferGenerator(){
    this->ReleaseDataFlagOn();
    this->ReleaseDataBeforeUpdateFlagOn();
    m_StreamingManager = RAMDrivenAdaptativeStreamingManagerType::New();
  };
  ~StreamingBufferGenerator() override = default;

  void UpdateOutputData(itk::DataObject * output) override
  {
    (void) output;
    this->GenerateData();
  }

  void GenerateData() override;

  StreamingManagerPointerType m_StreamingManager;

private:
  StreamingBufferGenerator(const Self &) = delete;
  Self & operator=(const Self &) = delete;

}; // end class


} // end namespace otb

#include "otbStreamingBufferGenerator.hxx"

#endif
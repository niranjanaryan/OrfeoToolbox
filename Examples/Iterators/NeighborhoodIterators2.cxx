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


#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"

// In this example, the Sobel edge-detection routine is rewritten using
// convolution filtering.  Convolution filtering is a standard image processing
// technique that can be implemented numerically as the inner product of all
// image neighborhoods with a convolution kernel \cite{Gonzalez1993}
// \cite{Castleman1996}.  In ITK, we use a class of objects called
// \emph{neighborhood operators} as convolution kernels and a special function
// object called \doxygen{itk}{NeighborhoodInnerProduct} to calculate inner
// products.
//
// The basic ITK convolution filtering routine is to step through the image
// with a neighborhood iterator and use NeighborhoodInnerProduct to
// find the inner product of each neighborhood with the desired kernel. The
// resulting values are written to an output image.  This example uses a
// neighborhood operator called the \doxygen{itk}{SobelOperator}, but all
// neighborhood operators can be convolved with images using this basic
// routine.  Other examples of neighborhood operators include derivative
// kernels, Gaussian kernels, and morphological
// operators. \doxygen{itk}{NeighborhoodOperatorImageFilter} is a generalization of
// the code in this section to ND images and arbitrary convolution kernels.
//
// We start writing this example by including the header files for the Sobel
// kernel and the inner product function.

#include "itkSobelOperator.h"
#include "itkNeighborhoodInnerProduct.h"

int main(int argc, char* argv[])
{
  if (argc < 4)
  {
    std::cerr << "Missing parameters. " << std::endl;
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile outputImageFile direction" << std::endl;
    return -1;
  }

  using PixelType  = float;
  using ImageType  = otb::Image<PixelType, 2>;
  using ReaderType = otb::ImageFileReader<ImageType>;

  using NeighborhoodIteratorType = itk::ConstNeighborhoodIterator<ImageType>;
  using IteratorType             = itk::ImageRegionIterator<ImageType>;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  try
  {
    reader->Update();
  }
  catch (itk::ExceptionObject& err)
  {
    std::cout << "ExceptionObject caught !" << std::endl;
    std::cout << err << std::endl;
    return -1;
  }

  ImageType::Pointer output = ImageType::New();
  output->SetRegions(reader->GetOutput()->GetRequestedRegion());
  output->Allocate();

  IteratorType out(output, reader->GetOutput()->GetRequestedRegion());

  // \index{convolution!kernels}
  // \index{convolution!operators}
  // \index{iterators!neighborhood!and convolution}
  //
  // Refer to the previous example for a description of reading the input image and
  // setting up the output image and iterator.
  //
  // The following code creates a Sobel operator.  The Sobel operator requires
  // a direction for its partial derivatives.  This direction is read from the command line.
  // Changing the direction of the derivatives changes the bias of the edge
  // detection, i.e. maximally vertical or maximally horizontal.

  itk::SobelOperator<PixelType, 2> sobelOperator;
  sobelOperator.SetDirection(::atoi(argv[3]));
  sobelOperator.CreateDirectional();

  // The neighborhood iterator is initialized as before, except that now it takes
  // its radius directly from the radius of the Sobel operator.  The inner
  // product function object is templated over image type and requires no
  // initialization.

  NeighborhoodIteratorType::RadiusType radius = sobelOperator.GetRadius();
  NeighborhoodIteratorType             it(radius, reader->GetOutput(), reader->GetOutput()->GetRequestedRegion());

  itk::NeighborhoodInnerProduct<ImageType> innerProduct;

  // Using the Sobel operator, inner product, and neighborhood iterator objects,
  // we can now write a very simple \code{for} loop for performing convolution
  // filtering.  As before, out-of-bounds pixel values are supplied automatically
  // by the iterator.

  for (it.GoToBegin(), out.GoToBegin(); !it.IsAtEnd(); ++it, ++out)
  {
    out.Set(innerProduct(it, sobelOperator));
  }

  // The output is rescaled and written as in the previous example.  Applying
  // this example in the $x$ and $y$ directions produces the images at the center
  // and right of Figure~\ref{fig:NeighborhoodExamples1}. Note that x-direction
  // operator produces the same output image as in the previous example.

  using WritePixelType = unsigned char;
  using WriteImageType = otb::Image<WritePixelType, 2>;
  using WriterType     = otb::ImageFileWriter<WriteImageType>;

  using RescaleFilterType = itk::RescaleIntensityImageFilter<ImageType, WriteImageType>;

  RescaleFilterType::Pointer rescaler = RescaleFilterType::New();

  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  rescaler->SetInput(output);

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[2]);
  writer->SetInput(rescaler->GetOutput());
  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject& err)
  {
    std::cout << "ExceptionObject caught !" << std::endl;
    std::cout << err << std::endl;
    return -1;
  }

  return EXIT_SUCCESS;
}

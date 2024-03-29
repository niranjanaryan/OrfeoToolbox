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

#include "mvdColorSetupController.h"


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
#include "mvdVectorImageModel.h"
#include "mvdColorSetupWidget.h"

namespace mvd
{
/*
  TRANSLATOR mvd::ColorSetupController

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
ColorSetupController::ColorSetupController(ColorSetupWidget* widget, QObject* p) : AbstractModelController(widget, p)
{
}

/*******************************************************************************/
ColorSetupController::~ColorSetupController()
{
}

/*******************************************************************************/
void ColorSetupController::Connect(AbstractModel* model)
{
  ColorSetupWidget* colorSetupWidget = GetWidget<ColorSetupWidget>();

  //
  // Connect GUI to controller.
  QObject::connect(colorSetupWidget, SIGNAL(CurrentRgbIndexChanged(RgbwChannel, int)),
                   // to:
                   this, SLOT(OnCurrentRgbIndexChanged(RgbwChannel, int)));

  QObject::connect(colorSetupWidget, SIGNAL(CurrentGrayIndexChanged(int)),
                   // to:
                   this, SLOT(OnCurrentGrayIndexChanged(int)));

  QObject::connect(colorSetupWidget, SIGNAL(GrayscaleActivated(bool)),
                   // to:
                   this, SLOT(OnGrayscaleActivated(bool)));

  QObject::connect(colorSetupWidget, SIGNAL(AlphaValueChanged(double)),
                   // to:
                   this, SLOT(OnAlphaValueChanged(double)));

  //
  // Connect controller to model.
  QObject::connect(this, SIGNAL(ModelUpdated()),
                   // to:
                   model, SLOT(OnModelUpdated()));
}

/*******************************************************************************/
void ColorSetupController::Disconnect(AbstractModel* model)
{
  ColorSetupWidget* colorSetupWidget = GetWidget<ColorSetupWidget>();

  //
  // Disconnect controller to model.
  QObject::disconnect(this, SIGNAL(ModelUpdated()),
                      // from:
                      model, SLOT(OnModelUpdated()));

  //
  // Disconnect GUI from controller.
  QObject::disconnect(colorSetupWidget, SIGNAL(CurrentRgbIndexChanged(RgbwChannel, int)),
                      // from:
                      this, SLOT(OnCurrentRgbIndexChanged(RgbwChannel, int)));

  QObject::disconnect(colorSetupWidget, SIGNAL(CurrentGrayIndexChanged(int)),
                      // from:
                      this, SLOT(OnCurrentGrayIndexChanged(int)));

  QObject::disconnect(colorSetupWidget, SIGNAL(GrayscaleActivated(bool)),
                      // from:
                      this, SLOT(OnGrayscaleActivated(bool)));

  QObject::disconnect(colorSetupWidget, SIGNAL(AlphaValueChanged(double)),
                      // from:
                      this, SLOT(OnAlphaValueChanged(double)));
}

/*******************************************************************************/
void ColorSetupController::ClearWidget()
{
  //
  // Calculate loop bounds. Return if nothing to do.
  CountType begin = 0;
  CountType end   = 0;

  if (!mvd::RgbwBounds(begin, end, RGBW_CHANNEL_ALL))
    return;

  //
  // Access color-dynamics widget.
  ColorSetupWidget* colorSetupWidget = GetWidget<ColorSetupWidget>();

  // Block this controller's signals to prevent display refreshes
  // but let let widget(s) signal their changes so linked values
  // will be correctly updated.
  bool thisSignalsBlocked = this->blockSignals(true);
  {
    // Block widget's signals...
    //...but force call to valueChanged() slot to force refresh.
    bool widgetSignalsBlocked = colorSetupWidget->blockSignals(true);
    {
      // Reset list of component names.
      colorSetupWidget->SetComponents(QStringList(tr("BAND 0")));

      //
      // RGBW-mode.

      // Reset current-indices of RGB channels widgets.
      for (CountType i = begin; i < end; ++i)
      {
        RgbwChannel channel = static_cast<RgbwChannel>(i);

        // Set current-index of channel.
        colorSetupWidget->SetCurrentRgbIndex(channel, 0);
      }

      //
      // Grayscale-mode.
      colorSetupWidget->SetGrayscaleActivated(true);

      // Allow user-selectable grayscale-mode.
      colorSetupWidget->SetGrayscaleEnabled(false);

      // Set current-index of white (gray).
      colorSetupWidget->SetCurrentGrayIndex(0);

      //
      // Alpha
      colorSetupWidget->SetAlpha(1.0);
    }
    colorSetupWidget->blockSignals(widgetSignalsBlocked);
  }
  this->blockSignals(thisSignalsBlocked);
}

/*******************************************************************************/
void ColorSetupController::virtual_ResetWidget(bool)
{
  // Reset color-setup widget.
  ResetIndices(RGBW_CHANNEL_RGB);

  // Reset alpha.
  ResetAlpha();
}

/*******************************************************************************/
void ColorSetupController::ResetIndices(RgbwChannel channels)
{
  //
  // Calculate loop bounds. Return if nothing to do.
  CountType begin = 0;
  CountType end   = 0;

  if (!mvd::RgbBounds(begin, end, channels))
    return;

  //
  // Access color-dynamics widget.
  ColorSetupWidget* colorSetupWidget = GetWidget<ColorSetupWidget>();

  //
  // Access image-model.
  VectorImageModel* imageModel = GetModel<VectorImageModel>();
  assert(imageModel != NULL);

  // Block this controller's signals to prevent display refreshes
  // but let let widget(s) signal their changes so linked values
  // will be correctly updated.
  bool thisSignalsBlocked = this->blockSignals(true);
  {
    // Block widget's signals...
    //...but force call to valueChanged() slot to force refresh.
    bool widgetSignalsBlocked = colorSetupWidget->blockSignals(true);
    {
      // Reset list of component names.
      colorSetupWidget->SetComponents(imageModel->GetBandNames(true));

      //
      // RGB-mode.

      // Reset current-indices of RGB channels widgets.
      for (CountType i = begin; i < end; ++i)
      {
        RgbwChannel channel = static_cast<RgbwChannel>(i);

        VectorImageSettings::ChannelVector::value_type band = imageModel->GetSettings().GetRgbChannel(i);

        // Set current-index of channel.
        colorSetupWidget->SetCurrentRgbIndex(channel, band);
#if 0
    OnCurrentRgbIndexChanged( channel, band );
#endif
      }

      //
      // Grayscale-mode.
      if (channels == RGBW_CHANNEL_RGB)
      {
        // Activated grayscale-mode.
        colorSetupWidget->SetGrayscaleActivated(imageModel->GetSettings().IsGrayscaleActivated());

        // Allow user-selectable grayscale-mode.
        colorSetupWidget->SetGrayscaleEnabled(imageModel->GetNbComponents() > 1);

        // Set current-index of white (gray).
        colorSetupWidget->SetCurrentGrayIndex(imageModel->GetSettings().GetGrayChannel());
      }
    }
    colorSetupWidget->blockSignals(widgetSignalsBlocked);
  }
  this->blockSignals(thisSignalsBlocked);
}

/*******************************************************************************/
void ColorSetupController::ResetAlpha()
{

  //
  // Access color-dynamics widget.
  ColorSetupWidget* colorSetupWidget = GetWidget<ColorSetupWidget>();

  //
  // Access image-model.
  VectorImageModel* imageModel = GetModel<VectorImageModel>();
  assert(imageModel != NULL);

  // Block this controller's signals to prevent display refreshes
  // but let let widget(s) signal their changes so linked values
  // will be correctly updated.
  bool thisSignalsBlocked = this->blockSignals(true);
  {
    // Block widget's signals...
    //...but force call to valueChanged() slot to force refresh.
    bool widgetSignalsBlocked = colorSetupWidget->blockSignals(true);
    {
      colorSetupWidget->SetAlpha(imageModel->GetSettings().GetAlpha());
    }
    colorSetupWidget->blockSignals(widgetSignalsBlocked);
  }
  this->blockSignals(thisSignalsBlocked);
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void ColorSetupController::OnCurrentRgbIndexChanged(RgbwChannel channel, int index)
{
  /*
  qDebug()
    << this
    << "::OnCurrentRgbIndexChanged("
    << RGBW_CHANNEL_NAMES[ channel ] << ", " << index
    << ")";
  */

  // Get image-model.
  VectorImageModel* imageModel = GetModel<VectorImageModel>();
  assert(imageModel != NULL);

  // Update channel index.
  imageModel->GetSettings().SetRgbChannel(channel, index);

  // Signal band-index of RGB channel has changed to other
  // controllers.
  Q_EMIT RgbChannelIndexChanged(channel, index);

  // Signal model has been updated.
  Q_EMIT ModelUpdated();
}

/*******************************************************************************/
void ColorSetupController::OnCurrentGrayIndexChanged(int index)
{
  // qDebug() << this << "::OnCurrentGrayIndexChanged(" << index << ")";

  // Get image-model.
  VectorImageModel* imageModel = GetModel<VectorImageModel>();
  assert(imageModel != NULL);

  // Update channel indices.
  imageModel->GetSettings().SetGrayChannel(index);

  // Signal band-index of gray channel has changed to other
  // controllers.
  Q_EMIT GrayChannelIndexChanged(index);

  // Signal model has been updated.
  Q_EMIT ModelUpdated();
}

/*******************************************************************************/
void ColorSetupController::OnGrayscaleActivated(bool activated)
{
  // qDebug() << this << "::OnGrayscaleActivated(" << activated << ")";

  // Get image-model.
  VectorImageModel* imageModel = GetModel<VectorImageModel>();
  assert(imageModel != NULL);

  // Change grayscale-mode activation state.
  imageModel->GetSettings().SetGrayscaleActivated(activated);

  // Signal grayscale-mode activation-state has changed to other
  // controllers.
  Q_EMIT GrayscaleActivated(activated);

  // Signal model has been updated.
  Q_EMIT ModelUpdated();
}

/*******************************************************************************/
void ColorSetupController::OnAlphaValueChanged(double alpha)
{
  // Get image-model.
  VectorImageModel* imageModel = GetModel<VectorImageModel>();
  assert(imageModel != NULL);

  // Change grayscale-mode activation state.
  imageModel->GetSettings().SetAlpha(alpha);

  // Signal model has been updated.
  Q_EMIT ModelUpdated();
}

} // end namespace 'mvd'

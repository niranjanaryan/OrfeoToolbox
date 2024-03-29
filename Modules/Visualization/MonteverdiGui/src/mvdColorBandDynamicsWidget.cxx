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

#include "mvdColorBandDynamicsWidget.h"
#include "ui_mvdColorBandDynamicsWidget.h"

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
#include "mvdAlgorithm.h"
#include "mvdDoubleValidator.h"

// Used for QString::number() calls. By default, QString::number()
// uses a 6 digit precision.
#define MAX_SIGNIFICANT_DIGITS 17

namespace mvd
{
/*
  TRANSLATOR mvd::ColorBandDynamicsWidget

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*******************************************************************************/
ColorBandDynamicsWidget::ColorBandDynamicsWidget(QWidget* p, Qt::WindowFlags flags)
  : QWidget(p, flags), m_UI(new mvd::Ui::ColorBandDynamicsWidget()), m_LowIntensityValidator(NULL), m_HighIntensityValidator(NULL), m_Channel(RGBW_CHANNEL_RGB)
{
  m_UI->setupUi(this);

  m_LowIntensityValidator = new DoubleValidator(m_UI->lowIntensityLineEdit);
  m_UI->lowIntensityLineEdit->setValidator(m_LowIntensityValidator);

  m_HighIntensityValidator = new DoubleValidator(m_UI->highIntensityLineEdit);
  m_UI->highIntensityLineEdit->setValidator(m_HighIntensityValidator);
}

/*******************************************************************************/
ColorBandDynamicsWidget::~ColorBandDynamicsWidget()
{
  delete m_UI;
  m_UI = NULL;
}

/*****************************************************************************/
double ColorBandDynamicsWidget::GetMinIntensity() const
{
  return m_UI->lowIntensityLineEdit->text().toDouble();
}

/*****************************************************************************/
void ColorBandDynamicsWidget::SetMinIntensity(double value)
{
  m_LowIntensityValidator->setBottom(value);
  m_HighIntensityValidator->setBottom(value);
}

/*****************************************************************************/
double ColorBandDynamicsWidget::GetMaxIntensity() const
{
  return m_LowIntensityValidator->bottom();
}

/*****************************************************************************/
void ColorBandDynamicsWidget::SetMaxIntensity(double value)
{
  m_LowIntensityValidator->setTop(value);
  m_HighIntensityValidator->setTop(value);
}

/*****************************************************************************/
double ColorBandDynamicsWidget::GetLowIntensity() const
{
  return m_HighIntensityValidator->top();
}

/*****************************************************************************/
void ColorBandDynamicsWidget::SetLowIntensity(double value)
{
  // qDebug() << this << "::SetLowIntensity(" << value << ")";

  QString number(QString::number(value, 'g', MAX_SIGNIFICANT_DIGITS));

  assert(!number.isEmpty());

  m_UI->lowIntensityLineEdit->setText(number);

  m_UI->lowIntensityLineEdit->setCursorPosition(0);
}

/*****************************************************************************/
double ColorBandDynamicsWidget::GetHighIntensity() const
{
  return m_UI->highIntensityLineEdit->text().toDouble();
}

/*****************************************************************************/
void ColorBandDynamicsWidget::SetHighIntensity(double value)
{
  // qDebug() << this << "::SetHighIntensity(" << value << ")";

  QString number(QString::number(value, 'g', MAX_SIGNIFICANT_DIGITS));

  assert(!number.isEmpty());

  m_UI->highIntensityLineEdit->setText(number);

  m_UI->highIntensityLineEdit->setCursorPosition(0);
}

/*****************************************************************************/
double ColorBandDynamicsWidget::GetLowQuantile() const
{
  return m_UI->lowQuantileSpinBox->value();
}

/*****************************************************************************/
void ColorBandDynamicsWidget::SetLowQuantile(double value)
{
  m_UI->lowQuantileSpinBox->setValue(value);
}

/*****************************************************************************/
double ColorBandDynamicsWidget::GetHighQuantile() const
{
  return m_UI->highQuantileSpinBox->value();
}

/*****************************************************************************/
void ColorBandDynamicsWidget::SetHighQuantile(double value)
{
  m_UI->highQuantileSpinBox->setValue(value);
}

/*****************************************************************************/
bool ColorBandDynamicsWidget::IsBounded() const
{
  return m_UI->linkButton->isChecked();
}

/*****************************************************************************/
void ColorBandDynamicsWidget::SetBounded(bool enabled)
{
  return m_UI->linkButton->setChecked(enabled);
}

/*****************************************************************************/
void ColorBandDynamicsWidget::SetLinkButtonEnabled(bool enabled)
{
  return m_UI->linkButton->setEnabled(enabled);
}

/*****************************************************************************/
void ColorBandDynamicsWidget::SetDefaultsButtonEnabled(bool enabled)
{
  return m_UI->defaultsButton->setEnabled(enabled);
}

/*****************************************************************************/
void ColorBandDynamicsWidget::SetChannelLabel(RgbwChannel channel)
{
  m_Channel = channel;

  m_UI->rLabel->setVisible(channel == RGBW_CHANNEL_RED || channel == RGBW_CHANNEL_RGB || channel == RGBW_CHANNEL_ALL);

  m_UI->gLabel->setVisible(channel == RGBW_CHANNEL_GREEN || channel == RGBW_CHANNEL_RGB || channel == RGBW_CHANNEL_ALL);

  m_UI->bLabel->setVisible(channel == RGBW_CHANNEL_BLUE || channel == RGBW_CHANNEL_RGB || channel == RGBW_CHANNEL_ALL);

  m_UI->wLabel->setVisible(channel == RGBW_CHANNEL_WHITE);

  m_UI->applyAllButton->setEnabled(channel == RGBW_CHANNEL_RED || channel == RGBW_CHANNEL_GREEN || channel == RGBW_CHANNEL_BLUE || channel == RGBW_CHANNEL_RGB);
}

/*****************************************************************************/
RgbwChannel ColorBandDynamicsWidget::GetChannelLabel() const
{
  return m_Channel;
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void ColorBandDynamicsWidget::on_lowIntensityLineEdit_editingFinished()
{
  // qDebug() << this << "::on_lowIntensityLineEdit_editingFinished()";

  assert(m_UI != NULL);
  assert(m_UI->lowIntensityLineEdit != NULL);

  bool   isOk  = true;
  double value = m_UI->lowIntensityLineEdit->text().toDouble(&isOk);

  if (!isOk)
  {
#if 0
    throw std::invalid_argument(
      ToStdString( tr( "Invalid argument '%1'." ).arg( text ) )
    );
#else
    return;
#endif
  }

  Q_EMIT LowIntensityChanged(m_Channel, value);
}

/*****************************************************************************/
void ColorBandDynamicsWidget::on_highIntensityLineEdit_editingFinished()
{
  // qDebug() << this << "::on_highIntensityLineEdit_editingFinished()";

  assert(m_UI != NULL);
  assert(m_UI->highIntensityLineEdit != NULL);

  bool   isOk  = true;
  double value = m_UI->highIntensityLineEdit->text().toDouble(&isOk);

  if (!isOk)
  {
#if 0
    throw std::invalid_argument(
      ToStdString( tr( "Invalid argument '%1'." ).arg( text ) )
    );
#else
    return;
#endif
  }

  Q_EMIT HighIntensityChanged(m_Channel, value);
}

/*****************************************************************************/
void ColorBandDynamicsWidget::on_lowQuantileSpinBox_valueChanged(double value)
{
  Q_EMIT LowQuantileChanged(m_Channel, value);
}

/*****************************************************************************/
void ColorBandDynamicsWidget::on_highQuantileSpinBox_valueChanged(double value)
{
  Q_EMIT HighQuantileChanged(m_Channel, value);
}

/*****************************************************************************/
void ColorBandDynamicsWidget::on_minMaxButton_clicked()
{
  Q_EMIT ResetIntensityClicked(m_Channel);
}

/*****************************************************************************/
void ColorBandDynamicsWidget::on_defaultsButton_clicked()
{
  Q_EMIT ResetQuantileClicked(m_Channel);
}

/*****************************************************************************/
void ColorBandDynamicsWidget::on_applyAllButton_clicked()
{
  Q_EMIT ApplyAllClicked(m_Channel, m_UI->lowQuantileSpinBox->value(), m_UI->highQuantileSpinBox->value());
}

/*****************************************************************************/
void ColorBandDynamicsWidget::on_linkButton_toggled(bool checked)
{
  /*
  if( checked )
    {
    QString text( m_UI->lowIntensityLineEdit->text() );
    m_LowIntensityValidator->fixup( text );
    m_UI->lowIntensityLineEdit->setText( text );

    text = m_UI->lowIntensityLineEdit->text();
    m_HighIntensityValidator->fixup( text );
    m_UI->highIntensityLineEdit->setText( text );
    }
  */

  Q_EMIT LinkToggled(m_Channel, checked);
}

} // end namespace 'mvd'

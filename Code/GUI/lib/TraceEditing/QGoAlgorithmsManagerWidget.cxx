/*=========================================================================
 Authors: The GoFigure Dev. Team.
 at Megason Lab, Systems biology, Harvard Medical school, 2009-11

 Copyright (c) 2009-11, President and Fellows of Harvard College.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 Neither the name of the  President and Fellows of Harvard College
 nor the names of its contributors may be used to endorse or promote
 products derived from this software without specific prior written
 permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#include "QGoAlgorithmsManagerWidget.h"
#include <QStackedLayout>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QPainter>
#include <QCheckBox>


QGoAlgorithmsManagerWidget::
QGoAlgorithmsManagerWidget( std::string iModeName,
  QWidget *iParent,
  std::vector<QString> iVectChannels,
  QStringList iListTime,
  bool iOnlyOneMethod,
  bool NeedApplyResetButton ) : QWidget(iParent), m_MethodComboBox(NULL),
    m_ChannelComboBox(NULL), m_TimeComboBox(NULL)
{
  this->m_ModeName = iModeName;
  this->Initialize(iVectChannels, iListTime, iOnlyOneMethod, NeedApplyResetButton);
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QGoAlgorithmsManagerWidget::~QGoAlgorithmsManagerWidget()
{
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoAlgorithmsManagerWidget::Initialize(std::vector<QString> iVectChannels,
  QStringList iListTime, bool iOnlyOneMethod, bool NeedApplyResetButton)
{
  this->m_VBoxLayout = new QVBoxLayout;
  QHBoxLayout* HBox = new QHBoxLayout;
  QFormLayout* FormLayout = new QFormLayout;
  if (!iListTime.empty() )
    {
    this->m_ListTimePoints = iListTime;
    this->m_TimeComboBox = new QComboBox(this);
    this->m_TimeComboBox->addItems(iListTime);
    QLabel* Label= new QLabel("TSlice",this);
    HBox->addWidget(Label);
    HBox->addWidget(this->m_TimeComboBox);
    }

  this->m_InvertBox = new QCheckBox("Invert", this);
  HBox->addWidget(this->m_InvertBox);

  this->m_VBoxLayout->addLayout(HBox);
  if (!iVectChannels.empty() )
    {
    this->m_ChannelComboBox = new QComboBox(this);
    std::vector<QString>::iterator iter = iVectChannels.begin();
    while (iter != iVectChannels.end())
      {
      this->m_ChannelComboBox->addItem(*iter);
      ++iter;
      }

    FormLayout->addRow(tr("Channel:"), this->m_ChannelComboBox);
    }

  this->m_MethodWidgets = new QStackedWidget(this);

  if (!iOnlyOneMethod)
    {
    this->m_MethodComboBox = new QComboBox(this);
    FormLayout->addRow(tr("Method:"), this->m_MethodComboBox);

    QObject::connect(this->m_MethodComboBox, SIGNAL(activated(int)),
            this->m_MethodWidgets, SLOT(setCurrentIndex(int)));
    }

   this->m_VBoxLayout->addLayout(FormLayout);
   this->m_VBoxLayout->addWidget(this->m_MethodWidgets);

  if (NeedApplyResetButton)
    {
    QHBoxLayout* ButtonLayout = new QHBoxLayout;
    QPushButton* ApplyButton = new QPushButton(tr("Apply"),this);
    ApplyButton->setShortcut(tr("A", "Apply Algorithm"));
    ApplyButton->setShortcut(tr("Ctrl+A", "Apply Algorithm"));
    ApplyButton->setToolTip("Apply Algorithm");
    QPushButton* ResetButton = new QPushButton(tr("Delete"), this);
    ResetButton->setShortcut(tr("D", "Delete the seeds"));
    ResetButton->setShortcut(tr("Ctrl+D", "Delete the seeds"));
    ResetButton->setToolTip("Delete the seeds");
    ButtonLayout->addWidget(ApplyButton);
    ButtonLayout->addWidget(ResetButton);
    this->m_VBoxLayout->addLayout(ButtonLayout);

     QObject::connect(ApplyButton, SIGNAL(clicked()),
    this, SLOT(EmitApplyAlgo()));

    QObject::connect(ResetButton, SIGNAL(clicked()), this, SIGNAL(ResetClicked()));

    }

  this->setLayout(this->m_VBoxLayout);
  this->m_VBoxLayout->setSizeConstraint(QLayout::SetFixedSize);
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoAlgorithmsManagerWidget::AddMethod(QGoAlgorithmWidget* iAlgoWidget)
{
  this->m_MethodWidgets->addWidget(iAlgoWidget);
  int Index = this->m_MethodWidgets->indexOf(iAlgoWidget);
  this->m_MethodComboBox->insertItem(Index, iAlgoWidget->GetMethodName().c_str());
  iAlgoWidget->show();
  this->m_MethodComboBox->setCurrentIndex(Index);
  this->m_MethodWidgets->setCurrentWidget(iAlgoWidget);
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoAlgorithmsManagerWidget::SetCurrentIndex(int iIndex)
{
  if (this->m_MethodComboBox != NULL)
    {
    this->m_MethodComboBox->setCurrentIndex(iIndex);
    }
  this->m_MethodWidgets->setCurrentIndex(iIndex);
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
std::string QGoAlgorithmsManagerWidget::GetModeName()
{
  return this->m_ModeName;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoAlgorithmsManagerWidget::SetCurrentChannel(QString iChannel)
{
  this->m_ChannelComboBox->setCurrentIndex(
    this->m_ChannelComboBox->findText(iChannel) );
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoAlgorithmsManagerWidget::SetTSliceForClassicView(QString iTimePoint)
{
  if (this->m_TimeComboBox)
    {
    this->m_TimeComboBox->clear();
    this->m_TimeComboBox->addItems(this->m_ListTimePoints);
    this->m_TimeComboBox->setCurrentIndex(
      this->m_TimeComboBox->findText(iTimePoint) );
    this->m_TimeComboBox->setEnabled(false);
    }
  if (this->m_ChannelComboBox)
    {
    this->m_ChannelComboBox->setEnabled(true);
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoAlgorithmsManagerWidget::SetTSliceForDopplerView(
  QHash<QString, QColor> iListTimePoints, int iIndexChannel)
{
  if (this->m_TimeComboBox)
    {
    this->m_TimeComboBox->clear();
    if (!iListTimePoints.empty() )
    {
    QHash<QString, QColor>::iterator iter = iListTimePoints.begin();
    while(iter != iListTimePoints.end() )
      {
      QPixmap  pix(12, 12);
      QPainter painter(&pix);
      painter.setPen(Qt::gray);
      QColor color = iter.value();
      painter.setBrush( QBrush(color) );
      painter.drawRect(0, 0, 12, 12);
      QIcon Icon;
      Icon.addPixmap(pix);

      this->m_TimeComboBox->addItem(Icon, iter.key());

      ++iter;
      }
    }
    if (iListTimePoints.size() > 0)
      {
      this->m_TimeComboBox->setCurrentIndex(1);
      }
    this->m_TimeComboBox->setEnabled(true);
    }
  if (this->m_ChannelComboBox)
    {
    this->m_ChannelComboBox->setCurrentIndex(iIndexChannel);
    this->m_ChannelComboBox->setEnabled(false);
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
bool QGoAlgorithmsManagerWidget::HasMethod()
{
  return this->m_MethodComboBox->count();
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoAlgorithmsManagerWidget::EmitApplyAlgo()
{
  QGoAlgorithmWidget* iCurrentWidget =
    dynamic_cast<QGoAlgorithmWidget*>(m_MethodWidgets->currentWidget());
  iCurrentWidget->EmitApplyAlgo();
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
std::string QGoAlgorithmsManagerWidget::GetCurrentImageName()
{
  std::string imageName;
  if(this->m_ChannelComboBox->isEnabled())
  {
    imageName = this->m_ChannelComboBox->currentText().toStdString();
  }
  else
  {
    imageName = this->m_TimeComboBox->currentText().toStdString();
  }
  return imageName;
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
int QGoAlgorithmsManagerWidget::GetSelectedTimePoint()
{
  return this->m_TimeComboBox->currentText().toInt();
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoAlgorithmsManagerWidget::AddWidgetForOnlyOneMethod(
  QWidget* iWidget)
{
  this->m_MethodWidgets->addWidget(iWidget);
  //int Index = this->m_MethodWidgets->indexOf(iWidget);
  this->m_MethodWidgets->setCurrentWidget(iWidget);
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
bool QGoAlgorithmsManagerWidget::IsInvertChecked()
{
  if (this->m_InvertBox->checkState() == Qt::Checked)
    {
    return true;
    }
  else
    {
    return false;
    }
}
/*void QGoAlgorithmsManagerWidget::AddMethod(std::string iNameMethod,
  QWidget* iParametersWidget, QWidget* iAdvParamWidget)
{
  QWidget* MethodWidget = new QWidget(this);

  QVBoxLayout* MethodLayout = new QVBoxLayout;
  MethodLayout->addWidget(iParametersWidget);

  QGoAdvancedParametersWidget* AdvParamWidget =
    new QGoAdvancedParametersWidget(this);
  AdvParamWidget->AddAdvancedParamWidget(iAdvParamWidget);


  MethodLayout->addWidget(AdvParamWidget);

  MethodWidget->setLayout(MethodLayout);
  this->m_MethodWidgets->addWidget(MethodWidget);

  int Index = this->m_MethodWidgets->indexOf(MethodWidget);
  this->m_MethodComboBox->insertItem(Index,iNameMethod.c_str());
}*/

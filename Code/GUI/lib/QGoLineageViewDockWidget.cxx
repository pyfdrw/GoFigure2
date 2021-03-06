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
#include "QGoLineageViewDockWidget.h"

#include <iostream>
#include <QVBoxLayout>
#include <QAction>

//-------------------------------------------------------------------------
QGoLineageViewDockWidget::QGoLineageViewDockWidget(
  QWidget *iParent) : QGoDockWidget(iParent)
{
  this->SetUpUi();
  QIcon Lineageicon;
  Lineageicon.addPixmap(QPixmap( QString::fromUtf8(":/fig/LineageView.png") ),
                      QIcon::Normal, QIcon::Off);
  this->m_ToggleAction->setIcon(Lineageicon);
  this->m_ToggleAction->setToolTip("Lineage View");
  this->setWindowTitle("Lineage View");

  
  // color code
  QObject::connect( this->m_depthLineage, SIGNAL( toggled(bool) ),
                    this, SLOT( ColorCodeLineagesByDepth(bool) ) );
  QObject::connect( this->m_real, SIGNAL( toggled(bool) ),
                    this, SLOT( ColorCodeLineagesByOriginalColor(bool) ) );
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QGoLineageViewDockWidget::
~QGoLineageViewDockWidget()
{
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoLineageViewDockWidget::ColorCodeLineagesByDepth(bool iChecked)
{
  if ( iChecked )
    {
    emit ChangeDivisionsColorCode("DepthInformation");
    }
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoLineageViewDockWidget::ColorCodeLineagesByOriginalColor(bool iChecked)
{
  if ( iChecked )
    {
    emit ChangeDivisionsColorCode("Original");
    }
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoLineageViewDockWidget::SetUpUi()
{
  QWidget* LineageViewWidget = new QWidget;

  this->m_depthLineage = new QRadioButton(tr("Depth Color Code") );  
  this->m_real = new QRadioButton(tr("Original Color") );
  this->m_real->setChecked(true);

  QVBoxLayout* VLayout = new QVBoxLayout;
  VLayout->addWidget(this->m_real);
  VLayout->addWidget(this->m_depthLineage);

  LineageViewWidget->setLayout(VLayout);
  this->setWidget(LineageViewWidget);
  LineageViewWidget->setSizePolicy(
    QSizePolicy::Expanding, QSizePolicy::Fixed);

}

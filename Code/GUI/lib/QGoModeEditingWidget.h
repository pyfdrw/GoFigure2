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

#ifndef __QGoModeEditingWidget_h
#define __QGoModeEditingWidget_h

#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include "QGoAlgorithmsManagerWidget.h"

/**
 * \class QGoModeEditingWidget
 * \ingroup GUI
 * \brief widget that manages the different modes for the TraceEditingWidget,
 has a combobox with the mode names and a corresponging stackedWidgets which
 display the right widget according to the mode selected in the combobox
*/
class QGoModeEditingWidget:
  public QWidget
{
  Q_OBJECT
public:
  explicit QGoModeEditingWidget(QWidget *iParent = 0);
  ~QGoModeEditingWidget();

  /**
  \brief add a widget in the StackedWidget with the mode name that will
  be added in the combobox.
  */
  void AddWidgetWithModeName (std::string iModeName, QWidget* iWidget= 0);

  /**
  \brief add a QGoAlgorithmsManagerWidget and set the current index of this algo widget
  to iCurrentIndex
  \param[in] iAlgoManagerWidget
  \param[in] iCurrentIndex default index for the algo widget
  */
  void AddAlgoManagerWidget(QGoAlgorithmsManagerWidget* iAlgoManagerWidget, int iCurrentIndex = 0);

protected:
  QVBoxLayout*            m_VBoxLayout;
  QComboBox*              m_ModeComboBox;
  QStackedWidget*         m_ModeWidgets;  
  
  void Initialize();

};
#endif

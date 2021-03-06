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

#ifndef __QGoAlgorithmWidget_h
#define __QGoAlgorithmWidget_h

#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <map>
#include "ctkCollapsibleGroupBox.h"
#include "QGoAlgoParameter.h"

/**
 * \class QGoAlgorithmWidget
 * \ingroup GUI 
 * \brief 
*/
class QGoAlgorithmWidget:
  public QWidget
{
  Q_OBJECT
public:
  explicit QGoAlgorithmWidget(std::string iMethodName, QWidget *iParent = 0);
  ~QGoAlgorithmWidget();

  /**
  \brief
  \return the name of the algorithms 
  */
  std::string GetMethodName();

  /**
  \brief add the Advanced parameters box if there are parameters inside and reduce it
  before showing the widget    
  */
  void show();

  template<typename T>
  void AddParameter(QGoAlgoParameter<T>* iParameter)
    {
    if (iParameter->m_AdvParam)
      {
        this->m_AdvParamLayout->addRow(tr("%1:").arg(iParameter->m_ParamName.c_str() ), 
        iParameter->m_Box);
      }    
    else
      {
        this->m_ParamLayout->addRow(tr("%1:").arg(iParameter->m_ParamName.c_str() ), 
        iParameter->m_Box);
      }
    }

  /**
  \brief method called by the QGoAlgoManagerWidget when the apply button is clicked, 
  emit the signal applyAlgo()
  */
  void EmitApplyAlgo();

signals:
    void ApplyAlgo();

protected:
  QVBoxLayout*                             m_VBoxLayout;
  std::string                              m_MethodName;
  QFormLayout*                             m_ParamLayout;
  QFormLayout*                             m_AdvParamLayout;
  bool                                     m_AdvParamAlreadySetUp;

  void Initialize();

};
#endif

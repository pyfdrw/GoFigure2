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

#ifndef __QGoDockWidgetStatus_h
#define __QGoDockWidgetStatus_h

#include <Qt>
#include <QObject>
#include <QAction>
#include "QGoDockWidget.h"

class QGoDockWidgetStatus:public QObject
{
  Q_OBJECT
public:
  explicit QGoDockWidgetStatus(QGoDockWidget *iW);

  explicit QGoDockWidgetStatus(const QGoDockWidgetStatus & iS);

  explicit QGoDockWidgetStatus(QGoDockWidget *iW,
                               Qt::DockWidgetArea iArea,
                               const bool & iVisibility,
                               const bool & iAttached,
                               QMainWindow* iMainWindow = 0);

  virtual ~QGoDockWidgetStatus();

  QGoDockWidget *m_DockWidget;

  /** \brief Position */
  Qt::DockWidgetArea m_Area;
  Qt::DockWidgetArea m_DefaultArea;

  /** \brief is Visible */
  bool m_Visibility;

  /** \brief Attached to QGoMainWindow*/
  bool m_Attached;

  /** \brief which main window the dock widget belongs to*/
  QMainWindow* m_MainWindow;

public slots:
  /** \brief set the area of the m_dockwidget*/
  void SetArea(Qt::DockWidgetArea iArea);

  /** \brief set the visibility of the m_dockwidget*/
  void SetVisibility(bool iVisibility);

  /** \brief set the floated status of the m_dockwidget*/
  void SetAttached(bool iAttached);

protected:
  /** \brief set the signal slots connections to update m_visibility, m_Attached and 
  m_Area*/
  void SetConnections();
};
#endif

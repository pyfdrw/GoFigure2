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

#include "QGoTabElementBase.h"

#include <QApplication>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QDockWidget>
#include <QPluginLoader>

#include <iostream>

//--------------------------------------------------------------------------
QGoTabElementBase::QGoTabElementBase(QWidget *iParent)
  : QMainWindow(iParent),
  m_StatusBar(NULL)
  //m_TracesActions(NULL),
  //m_TraceSettingsToolBar(NULL),
{
  //this->m_TracesActions = new QGoToolBarStatus;
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
QGoTabElementBase::~QGoTabElementBase()
{
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
std::vector< QAction * > QGoTabElementBase::ViewActions()
{
  return m_ViewActions;
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
std::vector< QAction * > QGoTabElementBase::ViewNoToolBarActions()
{
  return this->m_ViewNoToolBarActions;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
std::vector< QAction * > QGoTabElementBase::SegmentationActions()
{
  return m_SegmentationActions;
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
std::vector< QAction * > QGoTabElementBase::ToolsActions()
{
  return m_ToolsActions;
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
std::vector< QAction * > QGoTabElementBase::BookmarkActions()
{
  return m_BookmarkActions;
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
//std::vector< QAction * > QGoTabElementBase::ModeActions()
//{
//  return m_ModeActions;
//}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
//std::vector< QAction * > QGoTabElementBase::TracesActions()
//QGoToolBarStatus* QGoTabElementBase::TracesActions()
//{
//  return m_TracesActions;
//}

////--------------------------------------------------------------------------

////--------------------------------------------------------------------------
//QGoTraceSettingsWidget*  QGoTabElementBase::TraceSettingsWidget()
//{
//  return m_TraceSettingsWidgetForToolBar;
//}
////--------------------------------------------------------------------------

////--------------------------------------------------------------------------
//void QGoTabElementBase::SetTraceSettingsToolBar(
//  QToolBar* iToolBar)
//{
//  this->m_TraceSettingsToolBar = iToolBar;
//}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
std::list< QGoTabElementBase::QGoDockWidgetStatusPair > &
QGoTabElementBase::DockWidget()
{
  return m_DockWidgetList;
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
std::list< QAction * > QGoTabElementBase::GetPluginActions()
{
  return m_PluginActionList;
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTabElementBase::SetPluginActions(std::list< QAction * > iList)
{
  m_PluginActionList = iList;
}

//--------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoTabElementBase::CreateModeToolBar(
  QMenu* iMenu, QToolBar* iToolBar)
{
  QActionGroup* ModeGroup = new QActionGroup(this);
  ModeGroup->setObjectName("ModeGroup");

  this->m_ModeToolBar = new QGoToolBarStatus(iToolBar, iMenu, Qt::TopToolBarArea,
      true, true, this);
  //---------------------------------//
  //           default mode          //
  //---------------------------------//

  // Create/initialize the default action
  QAction *DefaultAction = new QAction(tr("Default"), this);
  DefaultAction->setShortcut(tr("1", "Default Mode"));
  DefaultAction->setObjectName("DefaultMode");

  DefaultAction->setCheckable(true);
  DefaultAction->setChecked(true);

  QIcon DefaultIcon;
  DefaultIcon.addPixmap(QPixmap( QString::fromUtf8(":/fig/mouse-cursor.png") ),
                        QIcon::Normal, QIcon::Off);
  DefaultAction->setIcon(DefaultIcon);

  ModeGroup->addAction(DefaultAction);

  // it also updates the interactor behaviour
  QObject::connect( DefaultAction, SIGNAL( toggled(bool) ),
                    this, SLOT( DefaultInteractorBehavior(bool) ) );

  this->m_ModeToolBar->m_VectorAction.push_back(DefaultAction);

  //---------------------------------//
  //            Zoom mode            //
  //---------------------------------//

  QAction *ZoomAction = new QAction(tr("Zoom"), this);
  ZoomAction->setShortcut(tr("Z", "Zoom Mode"));
  ZoomAction->setCheckable(true);
  ZoomAction->setChecked(false);

  QIcon ZoomIcon;
  ZoomIcon.addPixmap(QPixmap( QString::fromUtf8(":/fig/zoom.png") ),
                     QIcon::Normal, QIcon::Off);
  ZoomAction->setIcon(ZoomIcon);

  ModeGroup->addAction(ZoomAction);

  this->m_ModeToolBar->m_VectorAction.push_back(ZoomAction);
  // it also updates the interactor behaviour
  QObject::connect( ZoomAction, SIGNAL( toggled(bool) ),
                    this, SLOT( ZoomInteractorBehavior(bool) ) );

  //---------------------------------//
  //            Translate  mode            //
  //---------------------------------//

  QAction *TranslateAction = new QAction(tr("Translate"), this);
  TranslateAction->setShortcut(tr("T", "Translate Mode"));
  TranslateAction->setCheckable(true);
  TranslateAction->setChecked(false);

  QIcon TranslateIcon;
  TranslateIcon.addPixmap(QPixmap( QString::fromUtf8(":/fig/Hand.png") ),
                    QIcon::Normal, QIcon::Off);
  TranslateAction->setIcon(TranslateIcon);

  ModeGroup->addAction(TranslateAction);

  this->m_ModeToolBar->m_VectorAction.push_back(TranslateAction);
  // it also updates the interactor behaviour
  QObject::connect( TranslateAction, SIGNAL( toggled(bool) ),
                    this, SLOT( TranslateInteractorBehavior(bool) ) );

  this->m_ToolBarList.push_back(this->m_ModeToolBar);
}

//--------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoTabElementBase::CreateViewToolBar(QMenu* iMenu, QToolBar* iToolBar)
{
}
//--------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoTabElementBase::SetStatusBarPointer(QStatusBar *iStatusbar)
{
  this->m_StatusBar = iStatusbar;
}
//--------------------------------------------------------------------------

//-------------------------------------------------------------------------
std::list< QGoToolBarStatus* > QGoTabElementBase::GetToolBarsStatus()
{
  return this->m_ToolBarList;
}

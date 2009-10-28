#include "QGoTabManager.h"

#include <QTabWidget>

#include "QGoMainWindow.h"
#include "QGoTabElementBase.h"

QGoTabManager::QGoTabManager( QGoMainWindow* iMW, QTabWidget* iTW )
{
  m_MainWindow = iMW;
  m_TabWidget = iTW;
  m_PreviousTabIndex = -1;
}

QGoTabManager::~QGoTabManager()
{
}

void QGoTabManager::SetMainWindow( QGoMainWindow* iMW )
{
  m_MainWindow = iMW;
}

void QGoTabManager::SetTabWidget( QTabWidget* iTW )
{
  m_TabWidget = iTW;
}

void QGoTabManager::ChangeCurrentTab( int iIdx )
{
  if( m_PreviousTabIndex != -1 )
    {
    QGoTabElementBase* w =
      dynamic_cast< QGoTabElementBase* >(
        m_TabWidget->widget( m_PreviousTabIndex ) );
    if( w )
      {
      // First remove all toolbar related to the previous tab
      m_MainWindow->ClearViewToolBar();

      // Then remove all actions related to the previous tab from menuView
      m_MainWindow->ClearViewMenu();

      std::list< QDockWidget* > dock_list = w->DockWidget();

      for( std::list< QDockWidget* >::iterator
        dck_it = dock_list.begin();
        dck_it != dock_list.end();
        ++dck_it )
        {
        m_MainWindow->removeDockWidget( *dck_it );
        }

      GoFigure::TabDimensionType dim = w->GetTabDimensionType();

      std::map< GoFigure::TabDimensionType, std::list< QAction* > >::iterator
        map_it = m_MainWindow->m_TabDimPluginActionMap.find( dim );

      if( map_it != m_MainWindow->m_TabDimPluginActionMap.end() )
        {
        for( std::list< QAction* >::iterator list_it = (map_it->second).begin();
          list_it != (map_it->second).end();
          list_it++ )
          {
          (*list_it)->setDisabled( true );
          }
        }
      }
    }

  if( iIdx != -1 )
    {
    QGoTabElementBase* w2 =
      dynamic_cast< QGoTabElementBase* >(
        m_TabWidget->widget( iIdx ) );

    if( w2 )
      {
      // Then add all actions related to the new tab from menuView
      std::vector< QAction* > action_vector2 = w2->ViewActions();

      for( std::vector< QAction* >::iterator it = action_vector2.begin();
        it != action_vector2.end();
        ++it )
        {
        m_MainWindow->AddActionToViewMenu( *it );
        }

      std::list< QDockWidget* > dock_list = w2->DockWidget();

      for( std::list< QDockWidget* >::iterator
        dck_it = dock_list.begin();
        dck_it != dock_list.end();
        ++dck_it )
        {
        m_MainWindow->addDockWidget( Qt::LeftDockWidgetArea, *dck_it );
        (*dck_it)->show();
        }

      GoFigure::TabDimensionType dim = w2->GetTabDimensionType();

      std::map< GoFigure::TabDimensionType, std::list< QAction* > >::iterator
        map_it = m_MainWindow->m_TabDimPluginActionMap.find( dim );

      if( map_it != m_MainWindow->m_TabDimPluginActionMap.end() )
        {
        for( std::list< QAction* >::iterator list_it = (map_it->second).begin();
          list_it != (map_it->second).end();
          list_it++ )
          {
          (*list_it)->setEnabled( true );
          }
        }
      }
    }

  m_PreviousTabIndex = iIdx;
}

void QGoTabManager::CloseTab( int idx )
{
  if( idx >= 0 )
    {
    QGoTabElementBase* w =
      dynamic_cast< QGoTabElementBase* >( m_TabWidget->widget( idx ) );

    m_TabWidget->removeTab( idx );

    std::list< QDockWidget* > dock_list = w->DockWidget();

    for( std::list< QDockWidget* >::iterator
      dck_it = dock_list.begin();
      dck_it != dock_list.end();
      ++dck_it )
      {
      m_MainWindow->removeDockWidget( (*dck_it) );
      }

    if( w )
      {
      w->WriteSettings();
      delete w;
      }
    }
}

void QGoTabManager::CloseAllTabs( )
{
  int NumberOfTabs = m_TabWidget->count();

  for( int i = 0; i < NumberOfTabs; i++ )
    {
    int k = NumberOfTabs - 1 - i;
    QGoTabElementBase* w =
      dynamic_cast< QGoTabElementBase* >( m_TabWidget->widget( k ) );

    if( w )
      {
      std::list< QDockWidget* > dock_list = w->DockWidget();

      for( std::list< QDockWidget* >::iterator
        dck_it = dock_list.begin();
        dck_it != dock_list.end();
        ++dck_it )
        {
        m_MainWindow->removeDockWidget( (*dck_it) );
        }

      w->WriteSettings();
      delete w;
      }
    }

    m_MainWindow->ClearViewToolBar();
    m_TabWidget->clear( );
}

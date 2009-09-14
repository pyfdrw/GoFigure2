/*=========================================================================
  Author: $Author: arnaudgelas $  // Author of last commit
  Version: $Revision: 503 $  // Revision of last commit
  Date: $Date: 2009-08-03 17:42:52 -0400 (Mon, 03 Aug 2009) $  // Date of last commit
=========================================================================*/

/*=========================================================================
 Authors: The GoFigure Dev. Team.
 at Megason Lab, Systems biology, Harvard Medical school, 2009

 Copyright (c) 2009, President and Fellows of Harvard College.
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

#include "QGoMainWindow.h"
#include "QGoTabElementBase.h"
#include "QGoTabImageView2D.h"

#include <QDesktopServices>
#include <QUrl>

#include <iostream>

#include <QFileDialog>
#include <QMessageBox>

#include "itkQtAdaptor.h"
#include "itkQtProgressBar.h"

#include <itkImageFileReader.h>
#include <QSettings>

#include <vtkImageAppendComponents.h>
#include <vtkMarchingCubes.h>
#include <vtkPolyDataReader.h>
#include <vtkPLYReader.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <QDialog>

// *************************************************************************
QGoMainWindow::QGoMainWindow( )
{
  this->setupUi( this );
  this->setCentralWidget( this->CentralTabWidget );
  this->setWindowTitle( tr( "<*)0|00|0>< ~~ <*)0|00|0><     GoFigure    ><0|00|0(*> ~~ ><0|00|0(*>") );
  this->statusbar->showMessage( tr( "No data" ) );

  this->CentralTabWidget->clear();
  this->CentralTabWidget->setTabsClosable( true );

  this->statusbar->addPermanentWidget( &m_Bar );
  m_Bar.hide();
  SetCurrentSingleFile( QString() );
  SetCurrentMultiFile( QString() );

  CreateSignalSlotsConnection();
  ReadSettings();
}

// *************************************************************************
QGoMainWindow::~QGoMainWindow()
{
  this->WriteSettings();
}

// *************************************************************************
void QGoMainWindow::CreateSignalSlotsConnection()
{
  //QObject::connect( this->actionOpen, SIGNAL( activated( ) ),
    //this, SLOT( showprogressloading() ) );
  QObject::connect(this->CentralTabWidget,
    SIGNAL(tabCloseRequested(int)),
    this, SLOT( on_actionClose_activated( ) ) );

  QObject::connect( &m_SignalAdaptor, SIGNAL(Signal()),
    &(this->m_Bar), SLOT(hide()) );

  for( int i = 0; i < MaxRecentFiles; ++i )
    {
    recentSingleFileActions[i] = new QAction(this);
    recentSingleFileActions[i]->setVisible(false);
    QObject::connect(this->recentSingleFileActions[i], SIGNAL(triggered()),
      this, SLOT(openRecentSingleFile()));

    recentMultipleFileActions[i] = new QAction(this);
    recentMultipleFileActions[i]->setVisible(false);
    QObject::connect(this->recentMultipleFileActions[i], SIGNAL(triggered()),
      this, SLOT(openRecentMultipleFile()));
    }

}

// *************************************************************************
void QGoMainWindow::on_actionOpen_Single_File_activated( )
{
  QString filename = QFileDialog::getOpenFileName(
    this,
    tr( "Select Image" ),"",
    tr( "Images (*.png *.bmp *.jpg *.jpeg *.tiff *.mha *.mhd *.img *.lsm)" )
    );

  if( !filename.isEmpty( ) )
    {
    this->SetSingleFileName( filename );
    }
}

// *************************************************************************
void QGoMainWindow::on_actionOpen_Multiple_Files_activated( )
{
  QString filename = QFileDialog::getOpenFileName(
    this,
    tr( "Select one Image from the Dataset" ),"",
    tr( "Images (*.jpg *.jpeg *.lsm)" )
    );

//   if( !filename.isEmpty( ) )
//     {
//     SetFileName( filename, true );
//     }
}


//--------------------------------------------------------------------------------
void QGoMainWindow::on_actionUse_DataBase_activated()
{
}
//--------------------------------------------------------------------------------

// *************************************************************************
void QGoMainWindow::openFilesfromDB()
{
}
// *************************************************************************
void QGoMainWindow::on_actionOpen_Mesh_activated( )
{
  if( this->CentralTabWidget->count() > 0 )
    {
    int idx = this->CentralTabWidget->currentIndex();

    if( idx >= 0 )
    {
      QStringList filename = QFileDialog::getOpenFileNames(
        this,
        tr( "Select meshes or contours"), "",
        tr( "vtk - vtkPolyData (*.vtk);;ply - Polygon File Format (*.ply)" ) );

      if( !filename.isEmpty() )
        {
        QStringList::Iterator it = filename.begin();
        std::list< vtkPolyData* > mesh_list;
        std::list< vtkProperty* > property_list;
        property_list.push_back( 0 );

        while( it != filename.end() )
          {
          if( QFile::exists( *it ) )
            {
            QString extension = QFileInfo( *it ).suffix();

            vtkPolyData* mesh = vtkPolyData::New();

            if( extension.compare( "vtk", Qt::CaseInsensitive ) == 0 )
              {
              vtkPolyDataReader* mesh_reader = vtkPolyDataReader::New();
              mesh_reader->SetFileName( (*it).toAscii( ).data( ) );
              mesh_reader->Update();

              mesh->ShallowCopy( mesh_reader->GetOutput() );
              mesh_list.push_back( mesh );
              mesh_reader->Delete();
              }
            else
              {
              if( extension.compare( "ply", Qt::CaseInsensitive ) == 0 )
                {
                vtkPLYReader* mesh_reader = vtkPLYReader::New();
                mesh_reader->SetFileName( (*it).toAscii( ).data( ) );
                mesh_reader->Update();

                mesh->ShallowCopy( mesh_reader->GetOutput() );
                mesh_list.push_back( mesh );
                mesh_reader->Delete();
                }
              }
            }
          ++it;
          }

        if( !mesh_list.empty() )
          {
          double mesh_bounds[6];
          mesh_list.front()->GetBounds( mesh_bounds );
          bool IsContour = false;

          for( int i = 0; ( i < 3 ) && ( !IsContour ); ++i )
            {
            if( mesh_bounds[2*i] == mesh_bounds[2*i+1] )
              {
              IsContour = true;
              }
            }

//           QImagePageViewTracer* myPageView =
//             dynamic_cast<QImagePageViewTracer*>( m_PageView[idx] );
//           if( myPageView )
//             {
//             myPageView->AddContours( mesh_list, property_list, true, true );
//             // if( IsContour )
//             //{
//             //
//             // }
//             }
//           else
//             {
//             QImagePageView4DTracer* myPageView4D =
//               dynamic_cast<QImagePageView4DTracer*>( m_PageView[ idx ] );
//             if( myPageView4D )
//               {
//   //             myPageView4D->AddContours( mesh_list, property_list, true, true );
//               // if( IsContour )
//               //   {
//               //   }
//               // else
//               //  {
//               //  }
//               }
//             }
          }

        while( !mesh_list.empty() )
          {
          if( mesh_list.back() )
            {
            mesh_list.back()->Delete();
            }
          mesh_list.pop_back();
          }
        }
      }
    }
  else
    {
    /// \note Do we need to create a view mesh if there is no opened image?
    QMessageBox::warning( this, tr( "Open Mesh / Contour Warning" ),
      tr( "One image needs to be opened first to be able to load contours or meshes" ) );
    }

}//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
void QGoMainWindow::on_actionClose_activated( )
{
  int idx = this->CentralTabWidget->currentIndex();

  if( idx >= 0 )
    {
    QGoTabElementBase* w =
      dynamic_cast< QGoTabElementBase* >(
        this->CentralTabWidget->widget( idx ) );
    w->WriteSettings();
    delete w;
    }

  this->CentralTabWidget->removeTab( idx );
}
//--------------------------------------------------------------------------------

// *************************************************************************
void QGoMainWindow::on_actionClose_all_activated( )
{
  int NumberOfTabs = this->CentralTabWidget->count();

  for( int i = 0; i < NumberOfTabs; i++ )
    {
    QGoTabElementBase* w =
      dynamic_cast< QGoTabElementBase* >(
        this->CentralTabWidget->widget( i ) );
    w->WriteSettings();
    delete w;
    }
  this->CentralTabWidget->clear();
}
//--------------------------------------------------------------------------------

// *************************************************************************
void QGoMainWindow::on_actionQuit_activated( )
{
  this->close();
  this->WriteSettings();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoMainWindow::SetSingleFileName( const QString& iFile )
{
  if( QFile::exists( iFile ) )
    {
    this->SetCurrentSingleFile( iFile );

    // parse extension
    QString ext = QFileInfo( iFile ).suffix();
    if( ext.compare( "lsm", Qt::CaseInsensitive ) == 0 )
      {
      this->OpenLSMImage( m_CurrentFile, 0 );
      }
    else
      {
      this->OpenImageWithITK( m_CurrentFile );
//       this->DisplayImage( m_CurrentFile );
      }
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoMainWindow::OpenLSMImage( const QString& iFile, const int& iTimePoint )
{
  vtkLSMReader* reader = vtkLSMReader::New();
  reader->SetFileName( iFile.toAscii().data() );
  reader->SetUpdateTimePoint( iTimePoint );
  reader->Update();

  int dim[5];
  reader->GetDimensions( dim );

  int ImageDimensionality = 4;
//   bool Color = ( dim[4] > 1 );

  if( dim[3] == 1 ) // only one time point
    {
    if( dim[2] == 1 ) // only one z slice
      {
      ImageDimensionality = 2;
      }
    else
      {
      ImageDimensionality = 3;
      }
    }
  else
    {
    ImageDimensionality = 4;
    }

  switch( ImageDimensionality )
    {
    case 2:
      {
      QGoTabImageView2D* w2 = new QGoTabImageView2D;
      w2->SetImage( reader->GetOutput() );
      w2->setWindowTitle( iFile );
//       w2->SetLSMReader( reader );
      w2->Update();

      this->CentralTabWidget->addTab( w2, iFile );
      std::vector< QMenu* > w2_menus = w2->Menus();

      for( size_t i = 0; i < w2_menus.size(); i++ )
        {
        this->menubar->addMenu( w2_menus[i] );
        }
      break;
      }
    case 3:
      {
//       QGoTabImageView3D* w3 = new QGoTabImageView3D;
//       w3->SetImage( reader->GetOutput() );
//       w3->SetLSMReader( reader );
//       w3->Update();
      break;
      }
    case 4:
      {
//       QGoTabImageView3Dt* w3dt = new QGoTabImageView3Dt;
//       w3dt->SetImage( reader->GetOutput() );
//       w3dt->SetTimePoint( iTimePoint );
//       w3dt->SetLSMReader( reader );
//       w3dt->Update();
      break;
      }
    }

  reader->Delete();
}

//--------------------------------------------------------------------------------
void QGoMainWindow::OpenImageWithITK( const QString& iFile )
{
  typedef itk::ImageFileReader< ImageType > ImageReaderType;
  typedef ImageReaderType::Pointer          ImageReaderPointer;

  ImageReaderPointer reader = ImageReaderType::New();
  reader->SetFileName( iFile.toAscii( ).data( ) );

  //BUG 03/23: This next line is commented for the time being since
  // it makes gofigure crashing.
  // this->ShowProgressLoading( reader );
  reader->Update();

  ImageType::Pointer itkImage = reader->GetOutput();
  itkImage->DisconnectPipeline();

  VTKConvertImagePointer convert = VTKConvertImageType::New();
  convert->SetInput( itkImage );
//   this->ShowProgressLoading( convert );
  convert->Update();
//   this->HideProgressLoading();

  vtkImageData* VTKImage = convert->GetOutput();

  int dim[3];
  VTKImage->GetDimensions( dim );

  if( ( dim[0] == 1 ) || ( dim[1] == 1 ) || ( dim[2] == 1 ) )
    {
    QGoTabImageView2D* w2 = new QGoTabImageView2D;
    w2->SetImage( VTKImage );
    w2->setWindowTitle( iFile );
    w2->Update();
    this->CentralTabWidget->addTab( w2, iFile );
    }
  else
    {
//     QGoTabImageView3D* w3 = new QGoTabImageView3D;
//     w3->SetImage( VTKImage );
//     w2->setWindowTitle( title );
//     w3->Update();
    }
}
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
void QGoMainWindow::on_actionAbout_activated( )
{
  QString version( tr( "v0.5" ) );

  QString about_gofigure( tr( "GoFigure V2 \n\n" ) );

  QString authors( tr( "Authors in alphabetical order:\n" ) );
  authors.append( " * 2008 ~ A. Gelas\n" );
  authors.append( " * 2007-2009 ~ A. Gouaillard\n" );
  authors.append( " * 2008 ~ K. Mosaliganti\n" );
  authors.append( " * 2008 ~ L. Souhait\n\n" );
  authors.append( " * Principal Investigator\n" );
  authors.append( " * S. Megason\n" );

  QString message = QString( "GoFigure V2 %1\n\n" ).arg( version );
  message.append( about_gofigure );
  message.append( authors );

  QMessageBox::about( this, tr( "<*)0|00|0>< About GoFigure" ), message );
}
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
void QGoMainWindow::on_actionAbout_Qt_activated( )
{
  QMessageBox::aboutQt( this, tr( "About Qt" ) );
}
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoMainWindow::on_actionGoFigure2_Website_activated( )
{
  QDesktopServices::openUrl( QUrl("https://sourceforge.net/projects/gofigure2/") );
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoMainWindow::on_actionUser_mailing_list_activated( )
{
  QDesktopServices::openUrl( QUrl("mailto:users@gofigure2.com?subject=About GoFigure2") );
}
//--------------------------------------------------------------------------
void QGoMainWindow::on_actionDeveloper_mailing_list_activated( )
{
  QDesktopServices::openUrl( QUrl("mailto:developers@gofigure2.com?subject=About Gofigure2" ) );
}

//--------------------------------------------------------------------------
void QGoMainWindow::SetCurrentSingleFile( const QString &fileName )
{
  m_CurrentFile = fileName;
  this->setWindowModified( false );
  QString shownName = "Untitled";
  if( !m_CurrentFile.isEmpty() )
    {
    shownName = strippedName( m_CurrentFile );
    m_RecentSingleFiles.removeAll( m_CurrentFile );
    m_RecentSingleFiles.prepend( m_CurrentFile );
    UpdateRecentFileActions( m_RecentSingleFiles, menuSingle_Files,
      recentSingleFileActions );
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoMainWindow::SetCurrentMultiFile( const QString &fileName )
{
  m_CurrentFile = fileName;
  this->setWindowModified( false );
  QString shownName = "Untitled";
  if( !m_CurrentFile.isEmpty() )
    {
    shownName = strippedName( m_CurrentFile );
    m_RecentMultipleFiles.removeAll( m_CurrentFile );
    m_RecentMultipleFiles.prepend( m_CurrentFile );
    UpdateRecentFileActions( m_RecentMultipleFiles, menuMultiple_Files,
      recentMultipleFileActions );
    }
}

//--------------------------------------------------------------------------
QString QGoMainWindow::strippedName(const QString &fullFileName)
{
  return QFileInfo(fullFileName).fileName();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoMainWindow::UpdateRecentFileActions( QStringList list,
  QMenu *menu,
  QAction *recentFileActions[MaxRecentFiles] )
{
  QMutableStringListIterator i(list);
  while (i.hasNext())
    {
    if (!QFile::exists(i.next()))
      i.remove();
    }
  if (!list.isEmpty())
    {
    menu->setEnabled(true);
    }

  for (int j = 0; j < MaxRecentFiles; ++j)
    {
    if (j < list.count())
      {
      QString text = tr("&%1 %2 ")
        .arg(j + 1)
        .arg(strippedName(list[j]));

      recentFileActions[j]->setText(text);
      recentFileActions[j]->setData(list[j]);
      recentFileActions[j]->setVisible(true);
      menu->addAction(recentFileActions[j]);
      }
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoMainWindow::openRecentSingleFile()
{
  QAction* action = qobject_cast< QAction* >( sender() );
  if( action )
    {
    this->SetSingleFileName( action->data().toString() );
    }
}
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
void QGoMainWindow::openRecentMultipleFile()
{
  QAction* action = qobject_cast< QAction* >( sender() );
  if( action )
    {
//     this->SetMultiFileName( action->data().toString() );
    }
}
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
void QGoMainWindow::ReadSettings()
{
  QSettings settings;
  m_RecentSingleFiles =
    settings.value( "RecentSingleFiles" ).toStringList( );
  m_RecentMultipleFiles =
    settings.value( "RecentMultipleFiles" ).toStringList( );

  this->UpdateRecentFileActions( m_RecentSingleFiles,
    this->menuSingle_Files, this->recentSingleFileActions );
  this->UpdateRecentFileActions( m_RecentMultipleFiles,
    this->menuMultiple_Files, this->recentMultipleFileActions );

  settings.beginGroup( "MainWindow" );
  QSize size = settings.value( "size" ).toSize();

  if( size.isValid() )
    {
    this->resize( size );
    this->move( settings.value("pos").toPoint() );
    }
  else
    {
    this->resize( 1450, 750 );
    }

  //  settings.setValue("vsplitterSizes", vSplitter->saveState());
  settings.endGroup();
}
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
void QGoMainWindow::WriteSettings()
{
  QSettings settings;
  settings.setValue("RecentSingleFiles", m_RecentSingleFiles);
  settings.setValue("RecentMultipleFiles", m_RecentMultipleFiles);
  settings.beginGroup("MainWindow");
  settings.setValue("size", size());
  settings.setValue("pos", pos());
  settings.endGroup();
}
//--------------------------------------------------------------------------------
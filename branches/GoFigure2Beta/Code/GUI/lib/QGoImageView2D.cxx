#include "QGoImageView2D.h"

#include "QVTKWidget.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkLookupTable.h"
#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"
#include "vtkImageData.h"
#include "vtkTextProperty.h"
#include "vtkViewImage2DCollection.h"
#include "vtkViewImage2D.h"

//--------------------------------------------------------------------------
QGoImageView2D::QGoImageView2D( QWidget* iiParent ) : QGoImageView( iiParent )
{
  m_VTKEventQtConnector = vtkEventQtSlotConnect::New();

  this->setupUi( this );

  vtkViewImage2D* View = vtkViewImage2D::New();

  SetupViewGivenQVTKWidget( View, m_QVTKWidgetXY );

  m_Pool->AddItem( View );
  View->Delete();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
QGoImageView2D::~QGoImageView2D()
{
  m_VTKEventQtConnector->Delete();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoImageView2D::setupUi( QWidget* iiParent )
{
  if(iiParent->objectName().isEmpty())
    {
    iiParent->resize(800, 800);
    }

  m_QVTKWidgetXY = new QVTKWidget( this );
  m_QVTKWidgetXY->resize(800,800);

  m_LayOut = new QHBoxLayout( iiParent );
  m_LayOut->addWidget( m_QVTKWidgetXY  );

  retranslateUi(iiParent);

  QMetaObject::connectSlotsByName(iiParent);
} // setupUi
//--------------------------------------------------------------------------

//------------------------------------------------------------------------------
void QGoImageView2D::retranslateUi(QWidget *iParent)
{
  iParent->setWindowTitle( this->m_Tag );
  Q_UNUSED(iParent);
}
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoImageView2D::SetImage( vtkImageData* iImage )
{
  int dim[3];
  iImage->GetDimensions( dim );

  if( ( dim[0] > 1 ) && ( dim[1] > 1 ) && ( dim[2] > 1 ) )
    {
    std::cout <<"void QGoTabImageView2D::SetImage( vtkImageData* iImage )"
      <<std::endl;
    std::cout <<"This widget only display 2D images." <<std::endl;
    return;
    }
  else
    {
    m_Image = iImage;
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
vtkImageData* QGoImageView2D::GetImage( )
{
  return m_Image;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
vtkImageActor* QGoImageView2D::GetImageActor( const int& i )
{
  if( i == 0 )
    {
    vtkViewImage2D* View = m_Pool->GetItem( i );
    return View->GetImageActor();
    }
  else
    {
    return 0;
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
QVTKInteractor* QGoImageView2D::GetInteractor( const int& i )
{
  if( i == 0 )
    {
    return m_QVTKWidgetXY->GetInteractor();
    }
  else
    {
    return 0;
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoImageView2D::Update( )
{
  vtkViewImage2D* View = m_Pool->GetItem( 0 );
  View->SetInput( this->m_Image );
  View->GetTextProperty()->SetFontFamilyToArial();
  View->GetTextProperty()->SetFontSize( 20 );
  View->SetViewOrientation( vtkViewImage2D::VIEW_ORIENTATION_AXIAL );
  View->SetViewConvention( vtkViewImage2D::VIEW_CONVENTION_NEUROLOGICAL );

  m_Pool->Initialize();
  m_Pool->InitializeAllObservers();
  m_Pool->SyncSetShowAnnotations( true );
  m_Pool->SyncSetShowScalarBar( false );
  m_Pool->SyncRender();
  m_Pool->SyncReset();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoImageView2D::SetLookupTable( vtkLookupTable* iLut )
{
  m_Pool->SyncSetLookupTable( iLut );
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoImageView2D::ShowScalarBar( const bool& iShow )
{
  m_Pool->SyncSetShowScalarBar( iShow );
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
QString QGoImageView2D::SnapshotViewXY( const GoFigure::SnapshotImageType& iType,
      const QString& iBaseName )
{
  QString filename = SnapshotView( this->m_QVTKWidgetXY, iType,
    iBaseName, m_SnapshotId );

  m_SnapshotId++;
  return filename;
}
//--------------------------------------------------------------------------

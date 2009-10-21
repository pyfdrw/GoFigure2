#include "QGoManualSegmentationDockWidget.h"

#include <QColorDialog>
#include <cstdlib>

QGoManualSegmentationDockWidget::
QGoManualSegmentationDockWidget( QWidget* parent ) :
  QDockWidget( parent ),
  m_OrderVector( 5, 0 ),
  m_ValidatedColor( Qt::red )
{
  m_SettingsDialog = new QGoManualSegmentationSettingsDialog( this );

  this->setupUi( this );

  QObject::connect( this->SettingsBtn, SIGNAL( pressed() ),
    m_SettingsDialog, SLOT( exec() ) );

  QObject::connect( m_SettingsDialog, SIGNAL( accepted() ),
    this, SLOT( SetContourColor() ) );

  QObject::connect( this->IdSpinBox, SIGNAL( valueChanged( int ) ),
    this, SLOT( GenerateRandomColorForGivenId( int ) ) );

  QObject::connect( this->ReinitializeBtn, SIGNAL( pressed() ),
    this, SIGNAL( ReinitializePressed() ) );

  QObject::connect( this->ValidateBtn, SIGNAL( pressed() ),
    this, SIGNAL( ValidatePressed() ) );

  QObject::connect( this->ReinitializeIncrementBtn, SIGNAL( pressed() ),
    this, SIGNAL( ReinitializeAndIncrementPressed() ) );
}

QGoManualSegmentationDockWidget::
~QGoManualSegmentationDockWidget()
{
}

void
QGoManualSegmentationDockWidget::
OnSetColorPressed()
{
  QColor temp = QColorDialog::getColor( m_ValidatedColor, this,
    tr( "Pick a color for validated contours" ) );

  if( temp != m_ValidatedColor )
    {
    m_ValidatedColor = temp;
//     SetContourColor( m_ValidatedColor );
    }
}

void QGoManualSegmentationDockWidget::
SetContourColor( )
{
  QColor color = m_SettingsDialog->GetLineColor();
  GenerateOrderVector( color );
}

void QGoManualSegmentationDockWidget::
GenerateOrderVector( const QColor& iSelectedColor )
{
  int h, s, v;
  iSelectedColor.getHsv( &h, &s, &v );

  unsigned int closest_id = static_cast< unsigned int >(
    floor( static_cast< double >( h ) / 60. + 0.5 ) );

  unsigned int k = 0;
  for( unsigned int i = 0; i < 6; i++ )
    {
    if( i != closest_id )
      {
      m_OrderVector[k] = i;
      k++;
      }
    }
}

void
QGoManualSegmentationDockWidget::
GenerateRandomColorForGivenId( int iId )
{
  unsigned int k = static_cast< unsigned int >( iId ) % 5;
  int t = 60 * static_cast< int >( m_OrderVector[k] );

  m_ValidatedColor.setHsv( rand() % 60 + t, rand() % 256, rand() % 128 + 127 );
}

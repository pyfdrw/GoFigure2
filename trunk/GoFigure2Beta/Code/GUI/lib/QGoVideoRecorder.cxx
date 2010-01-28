#include "QGoVideoRecorder.h"

#include <QFileDialog>

#include "QGoImageView3D.h"

#include "vtkRenderWindow.h"
#include "vtkFFMPEGRenderWindowRecorder.h"


//-------------------------------------------------------------------------

/**
 * \brief Constructor
 */

//-------------------------------------------------------------------------

QGoVideoRecorder::
QGoVideoRecorder( QWidget *iParent ) : QDockWidget( iParent ), m_XMin( 0 ),
  m_YMax( 100 ), m_ZMin( 0 ), m_ZMax( 100 ), m_TMin( 0 ), m_TMax( 100 ),
  m_WindowSelected( 0 ), m_VideoName2( "" ),m_FrameRate2( 30 ),
  m_VideoQuality2 ( 1 ), m_FrameCounter( 0 ), m_RenderWindowSelected( false )
{
  this->setupUi( this );

  m_VideoRecorder = vtkFFMPEGRenderWindowRecorder::New();

  m_InternalTimer = new QTimer( this );
  QObject::connect( m_InternalTimer, SIGNAL(timeout()),this, SLOT(timeout()) );

  //Initalize chrono with .00 (not 0)
  QString value = "0.00";
  this->videoLenght->display( value );
  this->endRecord->setEnabled( false );

  this->videoName->setReadOnly( true );
  this->videoName_2->setReadOnly( true );

  this->tabVideoWidget->setEnabled( false );
}

//-------------------------------------------------------------------------

/**
 * \brief Destructor
 */

//-------------------------------------------------------------------------

QGoVideoRecorder::
~QGoVideoRecorder( )
{
  if( m_VideoRecorder )
    {
	m_VideoRecorder->Delete();
    }

  delete m_InternalTimer;
}

//-------------------------------------------------------------------------

/**
 * \brief Set value XMin and XMax for a given image (useful for spin boxes)
 * update content of spin box (X default Slice)
 */

//-------------------------------------------------------------------------

void
QGoVideoRecorder::
SetXMinAndMax( int XMin, int XMax )
{
  m_XMin = XMin;
  m_XMax = XMax;

  m_XMinForVideo = XMin;
  m_XMaxForVideo = XMax;

  //Initialization of the spinboxes
  this->tSpinMin_2->setMinimum( m_XMin );
  this->tSpinMin_2->setMaximum( m_XMax );
  this->tSpinMin_2->setValue( m_XMin );

  this->tSpinMax_2->setMinimum( m_XMin );
  this->tSpinMax_2->setMaximum( m_XMax );
  this->tSpinMax_2->setValue( m_XMax );
}


//-------------------------------------------------------------------------

/**
 * \brief Set value YMin and YMax for a given image (useful for spin boxes)
 */

//-------------------------------------------------------------------------

void
QGoVideoRecorder::
SetYMinAndMax( int YMin, int YMax )
{
  m_YMin = YMin;
  m_YMax = YMax;

  m_YMinForVideo = YMin;
  m_YMaxForVideo = YMax;
}


//-------------------------------------------------------------------------

/**
 * \brief Set value ZMin and ZMax for a given image (useful for spin boxes)
 */

//-------------------------------------------------------------------------

void
QGoVideoRecorder::
SetZMinAndMax( int ZMin, int ZMax )
{
  m_ZMin = ZMin;
  m_ZMax = ZMax;

  m_ZMinForVideo = ZMin;
  m_ZMaxForVideo = ZMax;
}

//-------------------------------------------------------------------------

/**
 * \brief Set value TMin and TMax for a given image (useful for spin boxes)
 */

//-------------------------------------------------------------------------

void
QGoVideoRecorder::
SetTMinAndMax( int TMin, int TMax )
{
  m_TMin = TMin;
  m_TMax = TMax;

  m_TMinForVideo = TMin;
  m_TMaxForVideo = TMax;

  this->tSpinMin->setMinimum( m_TMin );
  this->tSpinMin->setMaximum( m_TMax );
  this->tSpinMin->setValue( m_TMin );

  this->tSpinMax->setMinimum( m_TMin );
  this->tSpinMax->setMaximum( m_TMax );
  this->tSpinMax->setValue( m_TMax );
}

//-------------------------------------------------------------------------

/**
 * \brief Update content of spin box depending of moving slice (X,Y,Z) ( T Fixed )
 */

//-------------------------------------------------------------------------

void
QGoVideoRecorder::
UpdateQSpinBoxFT( int value )
{
  switch ( value ) {
    case 0 :
    // X Slice
    this->tSpinMin_2->setMinimum(m_XMin);
    this->tSpinMin_2->setMaximum(m_XMax);
    this->tSpinMin_2->setValue(m_XMin);

    this->tSpinMax_2->setMinimum(m_XMin);
    this->tSpinMax_2->setMaximum(m_XMax);
    this->tSpinMax_2->setValue(m_XMax);

    m_SliceFT = 0;
    break;

    case 1 :
    // Y Slice
    this->tSpinMin_2->setMinimum(m_YMin);
    this->tSpinMin_2->setMaximum(m_YMax);
    this->tSpinMin_2->setValue(m_YMin);

    this->tSpinMax_2->setMinimum(m_YMin);
    this->tSpinMax_2->setMaximum(m_YMax);
    this->tSpinMax_2->setValue(m_YMax);

    m_SliceFT = 1;
    break;

    default :
    // Z Slice
    this->tSpinMin_2->setMinimum(m_ZMin);
    this->tSpinMin_2->setMaximum(m_ZMax);
    this->tSpinMin_2->setValue(m_ZMin);

    this->tSpinMax_2->setMinimum(m_ZMin);
    this->tSpinMax_2->setMaximum(m_ZMax);
    this->tSpinMax_2->setValue(m_ZMax);

    m_SliceFT = 2;
	}
}

//-------------------------------------------------------------------------

/**
 * \brief Update content of spin box depending of moving slice (X,Y,Z)
 */

//-------------------------------------------------------------------------

void
QGoVideoRecorder::
on_SliceFT_activated( int value)
{
  UpdateQSpinBoxFT( value );
}

//-------------------------------------------------------------------------

/**
 * \brief Update value for the video length ( T Fixed )
 */

//-------------------------------------------------------------------------

void
QGoVideoRecorder::
on_tSpinMin_2_valueChanged( int value )
{
  switch ( m_SliceFT )
    {
    case 0 :
    // X Slice
    m_XMinForVideo = value;
    break;

    case 1 :
    // Y Slice
    m_YMinForVideo = value;
    break;

    default :
    // Z Slice
    m_ZMinForVideo = value;
    }
}

//-------------------------------------------------------------------------

/**
 * \brief Update value for the video length ( T Fixed )
 */

//-------------------------------------------------------------------------
void
QGoVideoRecorder::
on_tSpinMax_2_valueChanged( int value )
{
  switch ( m_SliceFT )
    {
    case 0 :
    // X Slice
    m_XMaxForVideo = value;
    break;

	    case 1 :
    // Y Slice
    m_YMaxForVideo = value;
    break;

    default :
    // Z Slice
    m_ZMaxForVideo = value;
    }
}

//-------------------------------------------------------------------------

/**
 * \brief Get and print the location to store MegaCapture file
 */

//-------------------------------------------------------------------------

void
QGoVideoRecorder::
on_createFile_clicked()
{
	m_VideoName2 = QFileDialog::getSaveFileName( this,
	      tr( "Folder to Save Video" ), "fileName", 0 );

	this->videoName->setText( m_VideoName2 );
}

//-------------------------------------------------------------------------

/**
 * \brief Function called when FrameRate changes
 */

//-------------------------------------------------------------------------

void
QGoVideoRecorder::
on_frameRate_valueChanged( int value )
{
	m_FrameRate2= value;
	this->frameRate_2->setValue(value);
    // update video length

}

//-------------------------------------------------------------------------

/**
 * \brief Function called when VideoQuality changes
 */

//-------------------------------------------------------------------------

void
QGoVideoRecorder::
on_videoQuality_valueChanged( int value )
{
	m_VideoQuality2= value;
	this->videoQuality_2->setValue(value);
}

//-------------------------------------------------------------------------

/**
 * \brief Function called when "Create video" clicked in Create Video Tab
 */

//-------------------------------------------------------------------------

void
QGoVideoRecorder::
on_startVideo_clicked()
{

//something to record first view
  if( ( m_VideoName2 == NULL ) || ( !m_RenderWindowSelected ))
    {
    std::cout<<"Please select a videoName and a good rendering window"<<std::endl;
    }
  else if( this->tabWidget->currentIndex () == 0 )
    {
    m_VideoRecorder->Setm_FrameRate( m_FrameRate2 );
    m_VideoRecorder->Setm_VideoQuality( m_VideoQuality2 );

    // Get actual position of Slices
    emit GetSlicePosition( m_SliceFT );

    if( m_SliceFT == 0 )
      {
      QString fileName = m_VideoName2;

      fileName.insert( fileName.size(), QString("-X-"));
      fileName.insert( fileName.size(), QString::number( m_XMinForVideo, 10 ) );
      fileName.insert( fileName.size(), QString("-"));
      fileName.insert( fileName.size(), QString::number( m_XMaxForVideo, 10 ) );
      fileName.insert( fileName.size(), QString(".avi"));
      m_VideoRecorder->SetFileName( fileName.toStdString() );

      m_VideoRecorder->StartCapture();

      for(unsigned int i = m_XMinForVideo; i < m_XMaxForVideo+1; i++)
        {
        //send signal to gofigure to change slice
      std::cout << "emit X"<<std::endl;
        emit XSliceChanged(i);
        //capture screen
        m_VideoRecorder->TakeSnapshot();
        }

        m_VideoRecorder->EndCapture();
      }

    if( m_SliceFT == 1 )
      {
      QString fileName = m_VideoName2;

      fileName.insert( fileName.size(), QString("-Y-"));
      fileName.insert( fileName.size(), QString::number( m_YMinForVideo, 10 ) );
      fileName.insert( fileName.size(), QString("-"));
      fileName.insert( fileName.size(), QString::number( m_YMaxForVideo, 10 ) );
      fileName.insert( fileName.size(), QString(".avi"));

       m_VideoRecorder->SetFileName( fileName.toStdString() );

       m_VideoRecorder->StartCapture();

       for(unsigned int i = m_YMinForVideo; i < m_YMaxForVideo+1; i++)
         {
         //send signal to gofigure to change slice
       std::cout << "emit Y"<<std::endl;
         emit YSliceChanged(i);
         //capture screen
         m_VideoRecorder->TakeSnapshot();
         }
       m_VideoRecorder->EndCapture();
       }

    if( m_SliceFT == 2 )
      {
      QString fileName = m_VideoName2;

      fileName.insert( fileName.size(), QString("-Z-"));
      fileName.insert( fileName.size(), QString::number( m_ZMinForVideo, 10 ) );
      fileName.insert( fileName.size(), QString("-"));
      fileName.insert( fileName.size(), QString::number( m_ZMaxForVideo, 10 ) );
      fileName.insert( fileName.size(), QString(".avi"));

       m_VideoRecorder->SetFileName( fileName.toStdString() );

       m_VideoRecorder->StartCapture();

       for(unsigned int i = m_ZMinForVideo; i < m_ZMaxForVideo+1; i++)
         {
         //send signal to gofigure to change slice
       std::cout << "emit Z"<<std::endl;
         emit ZSliceChanged(i);
         //capture screen
         m_VideoRecorder->TakeSnapshot();
         }
       m_VideoRecorder->EndCapture();
       }
    }
  else
    {
    QString fileName = m_VideoName2;

    fileName.insert( fileName.size(), QString("-T-"));
    fileName.insert( fileName.size(), QString::number( m_TMinForVideo, 10 ) );
    fileName.insert( fileName.size(), QString("-"));
    fileName.insert( fileName.size(), QString::number( m_TMaxForVideo, 10 ) );
    fileName.insert( fileName.size(), QString(".avi"));

    m_VideoRecorder->SetFileName( fileName.toStdString() );

    m_VideoRecorder->StartCapture();

    //problem with 3d view?

    for(unsigned int i = m_TMinForVideo; i < m_TMaxForVideo+1; i++)
      {
      //send signal to gofigure to change slice
      std::cout << "emit T"<<std::endl;
      emit TSliceChanged(i);
      //capture screen
      m_VideoRecorder->TakeSnapshot();
      }
    m_VideoRecorder->EndCapture();

    }

}

//-------------------------------------------------------------------------

/**
 * \brief Update value for the video length ( T Fixed )
 */

//-------------------------------------------------------------------------

void
QGoVideoRecorder::
on_tSpinMin_valueChanged( int value )
{
  m_TMinForVideo = value;
}

//-------------------------------------------------------------------------

/**
 * \brief Update value for the video length ( T moving )
 */

//-------------------------------------------------------------------------
void
QGoVideoRecorder::
on_tSpinMax_valueChanged( int value )
{
  m_TMaxForVideo = value;
}

//-------------------------------------------------------------------------

/**
 * \brief Function called to choose name/path of output file
 */

//-------------------------------------------------------------------------


void
QGoVideoRecorder::
on_createFile_2_clicked()
{
  m_VideoName2 = QFileDialog::getSaveFileName( this,
    tr( "Folder to Save Video" ), "fileName", 0 );

  this->videoName_2->setText( m_VideoName2 );
}

//-------------------------------------------------------------------------

/**
 * \brief Update value of frame rate
 */

//-------------------------------------------------------------------------

void
QGoVideoRecorder::
on_frameRate_2_valueChanged( int value )
{
  m_FrameRate2 = value;
  this->frameRate->setValue(value);
}

//-------------------------------------------------------------------------

/**
 * \brief Update value of video quality
 */

//-------------------------------------------------------------------------

void
QGoVideoRecorder::
on_videoQuality_2_valueChanged( int value )
{
  m_VideoQuality2 = value;
  this->videoQuality->setValue(value);
}

//-------------------------------------------------------------------------

/**
 * \brief Function called when "Create video" clicked in Record Video Tab
 */

//-------------------------------------------------------------------------

void
QGoVideoRecorder::
on_startRecord_clicked()
{

  if( ( m_VideoName2 == NULL ) || ( !m_RenderWindowSelected ))
  {
  // TODO: display pop up window
    std::cout<<"Please select a videoName and a good rendering window"<<std::endl;
  }
  else
  {
  this->videoName_2->setEnabled(false);
  this->createFile_2->setEnabled(false);
  this->frameRate_2->setEnabled(false);
  this->videoQuality_2->setEnabled(false);
  this->startRecord->setEnabled(false);
  this->tabVideoMethod1->setEnabled(false);
  this->endRecord->setEnabled(true);

  QString fileName = m_VideoName2;
  fileName.insert( fileName.size(), QString(".avi"));

  m_VideoRecorder->SetFileName( fileName.toStdString() );
  m_VideoRecorder->Setm_FrameRate( m_FrameRate2 );
  m_VideoRecorder->Setm_VideoQuality( m_VideoQuality2 );
  m_VideoRecorder->StartCapture();
  m_InternalTimer->start( 1000/m_FrameRate2 );
  }
}

//-------------------------------------------------------------------------

/**
 * \brief Function called when "End" clicked in Record Video Tab
 */

//-------------------------------------------------------------------------

void
QGoVideoRecorder::
on_endRecord_clicked()
{
  m_VideoRecorder->EndCapture();
  m_InternalTimer->stop();
  m_FrameCounter = 0;

  this->videoName_2->setEnabled(true);
  this->createFile_2->setEnabled(true);
  this->frameRate_2->setEnabled(true);
  this->videoQuality_2->setEnabled(true);
  this->startRecord->setEnabled(true);
  this->tabVideoMethod1->setEnabled(true);
  this->endRecord->setEnabled(false);
}

//-------------------------------------------------------------------------

/**
 * \brief Update the value of the timer whilr recording
 */

//-------------------------------------------------------------------------

void
QGoVideoRecorder::
timeout()
{
  m_VideoRecorder->TakeSnapshot();

  ++m_FrameCounter;

  // for a better visualisation, always show 2 decimal
  double doubleCounter;
  doubleCounter = (double)m_FrameCounter/(double)m_FrameRate2;

  int test = 100*doubleCounter;
  QString value = QString::number( test, 10 );

  if ( test >= 10 )
  	{
    value.insert( value.size()-2, QString("."));
  	}
  else
  	{
    value.insert( value.size()-1, QString(".0"));
  	}

  if(test<100)
  	{
    value.insert( value.size()-3, QString("0"));
  	}
  this->videoLenght->display(value);
}

//-------------------------------------------------------------------------

/**
 * \brief Function called when "Create video" clicked in Create Video Tab
 */

//-------------------------------------------------------------------------

void
QGoVideoRecorder::
SetRenderingWindow( vtkRenderWindow* iRenderingWindow )
{
  if(iRenderingWindow != NULL)
    {
    m_RenderWindowSelected = true;
    m_VideoRecorder->SetRenderingWindow(iRenderingWindow);
    this->tabVideoWidget->setEnabled(true);
    }
    else
    {
    m_RenderWindowSelected = false;
    this->tabVideoWidget->setEnabled(false);
    }

  // TODO Resize image with the first one if we want to change views during record
}

//----------------------------------------------------------------------------//

void
QGoVideoRecorder::
SetInitialPosition( int iInitialPosition )
{
  m_InitialPosition = iInitialPosition;
}

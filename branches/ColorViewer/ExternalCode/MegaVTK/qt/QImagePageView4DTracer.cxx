/*=========================================================================
 Authors: The GoFigure Dev. Team.
 while at Megason Lab, Systems biology, Harvard Medical school, 2009

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

#include "QImagePageViewTracer.h"
#include "QimagePageView4DTracer.h"

#include "QSplitterchild.h"
#include "vtkViewImage2DCommand.h"

#include <QResizeEvent>
#include <vtkImageAppendComponents.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <vtkTextProperty.h>
#include <vtkProperty.h>

#include <vtkImageData.h>
#include <vtkImageClip.h>
#include <vtkImagePermute.h>
#include <vtkImageResample.h>
#include <vtkWindowToImageFilter.h>

#include <vtkLSMReader.h>
#include <vtkBMPWriter.h>
#include <vtkPostScriptWriter.h>
#include <vtkJPEGWriter.h>
#include <vtkPNGWriter.h>
#include <vtkTIFFWriter.h>

#include <vtkPolyDataWriter.h>

#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkMath.h>

QImagePageView4DTracer::QImagePageView4DTracer( QWidget* parent ) : QWidget( parent )
{
  this->NumberOfTimePoints = 0;
  this->Image = (vtkImageData*)(0);
 
  this->Whatever = new QImagePageViewTracer( );

  this->slider1 = new QSlider( Qt::Horizontal );

  this->button = new QPushButton("Run &Movie");
	
  this->LayOut1 = new QVBoxLayout;
  this->LayOut1->addWidget( this->Whatever );
  this->LayOut1->addWidget( this->slider1 );
  this->LayOut1->addWidget( this->button );
  
  this->LayOutWidget1 = new QWidget( this );
  this->LayOutWidget1->setLayout( this->LayOut1 );

  QObject::connect( this->slider1, SIGNAL( valueChanged( int ) ),
    this, SLOT( SetView( int ) ) );
  QObject::connect( this->button, SIGNAL( clicked( ) ),
    this, SLOT( RunMovie( ) ) );

}

QImagePageView4DTracer::~QImagePageView4DTracer()
{
  delete this->Whatever;
  delete this->LayOut1;
  delete this->LayOutWidget1;
  delete this->slider1;
}

void 
QImagePageView4DTracer::SetFileName( char* name )
{
  this->FileName = name;
  this->SetView( 0 );
}

void QImagePageView4DTracer::ReadLSMFile( int TimePoint )
{
  vtkImageData* myImage_ch1 = vtkImageData::New();
  vtkLSMReader* reader=vtkLSMReader::New();
  reader->SetFileName( this->FileName );
  reader->SetUpdateTimePoint( TimePoint );
  reader->Update();
  this->slider1->setMinimum( 0 );
  this->NumberOfTimePoints = reader->GetNumberOfTimePoints();
  this->slider1->setMaximum( this->NumberOfTimePoints );
	
  int NumberOfChannels = reader->GetNumberOfChannels();
  myImage_ch1->ShallowCopy( reader->GetOutput() );
  reader->Delete();

  vtkImageData* myImage_ch2 = vtkImageData::New(); 
  if( NumberOfChannels == 1 ) 
    {
	myImage_ch2->ShallowCopy( myImage_ch1 );
    }
  else
    {
	vtkLSMReader* reader2=vtkLSMReader::New();
	reader2->SetFileName( this->FileName );
        reader2->SetUpdateTimePoint( TimePoint );
	reader2->SetUpdateChannel( 1 );
	reader2->Update();
	myImage_ch2->ShallowCopy( reader2->GetOutput() );
	reader2->Delete();
    }

  vtkImageData* myImage2 = vtkImageData::New(); 
  vtkImageAppendComponents* appendFilter1 = vtkImageAppendComponents::New();
  appendFilter1->AddInput( myImage_ch1 );
  appendFilter1->AddInput( myImage_ch2 );
  appendFilter1->Update();
  myImage2->ShallowCopy( appendFilter1->GetOutput() );
  appendFilter1->Delete();
  myImage_ch2->Delete();

  vtkImageData* myImage_ch3 = vtkImageData::New();
  if( NumberOfChannels == 2 )
    {
	myImage_ch3->ShallowCopy( myImage_ch1 );
    }
  else
    {
	vtkLSMReader* reader3=vtkLSMReader::New();
	reader3->SetFileName( this->FileName );
        reader3->SetUpdateTimePoint( TimePoint );
	reader3->SetUpdateChannel( 2 );
	reader3->Update();
	myImage_ch3->ShallowCopy( reader3->GetOutput() );
	reader3->Delete();
    }
  myImage_ch1->Delete();
	
  vtkImageData* myImage3 = vtkImageData::New(); 
  vtkImageAppendComponents* appendFilter2 = vtkImageAppendComponents::New();
  appendFilter2->AddInput( myImage2    );
  appendFilter2->AddInput( myImage_ch3 );
  appendFilter2->Update(); 
  myImage3->ShallowCopy( appendFilter2->GetOutput() );
  appendFilter2->Delete();
  myImage2->Delete();
  myImage_ch3->Delete();

  if( this->Image ) this->Image->Delete();

  this->Image = myImage3;
}

void QImagePageView4DTracer::SetView( int value )
{
  this->ReadLSMFile( value );
  this->LayOut1->remove( this->Whatever );
  delete this->Whatever;
  this->Whatever = new QImagePageViewTracer( );
  this->LayOut1->insertWidget( 0, this->Whatever );
  this->Whatever->SetImage( this->Image );
}

void QImagePageView4DTracer::resizeEvent( QResizeEvent* event )
{
  QWidget::resizeEvent( event );
  this->LayOutWidget1->resize( event->size() );
}

void QImagePageView4DTracer::RunMovie( )
{
  std::cout << "Enjoy movie mode." << std::endl;

  //for( unsigned int i = 0; i < this->NumberOfTimePoints; i++)
    {
    this->SetView( 0 ); //i );
    this->Render();
    char buffer;
    std::cin >> buffer;
    }
}


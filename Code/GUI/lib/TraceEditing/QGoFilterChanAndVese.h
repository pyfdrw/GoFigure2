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
#ifndef __QGoFilterChanAndVese_h
#define __QGoFilterChanAndVese_h

#include "QGoFilterSemiAutoBase.h"
#include <QObject>
#include "QGoGUILibConfigure.h"
#include "itkImage.h"
#include "itkChanAndVeseSegmentationFilter.h"
#include "itkVTKImageImport.h"
#include "vtkSmartPointer.h"
#include "vtkPolyData.h"

/**
 * \class QGoFilterChanAndVese
 * \brief Levelset segmentation algorithm implementation.
 * Can generate contours and meshes.
 * Will generate 2D objects if m_Dimension<2, 3D objects in the other case.
 */
class QGOGUILIB_EXPORT QGoFilterChanAndVese : public QGoFilterSemiAutoBase
{
  Q_OBJECT
public:
  typedef float                   OutputPixelType;

  typedef itk::Image< OutputPixelType, 3 >  Output3DType;
  typedef typename Output3DType::Pointer    Output3DPointer;

  typedef itk::Image< OutputPixelType, 2 >  Output2DType;
  typedef typename Output2DType::Pointer    Output2DPointer;

  /** \brief Constructor */
  explicit QGoFilterChanAndVese(QObject *iParent = NULL, int iDimension = 2);

  /** \brief Destructor */
  ~QGoFilterChanAndVese();

  virtual vtkPolyData * Apply();

  //virtual void ConnectSignals(int iFilterNumber);

  std::vector<vtkPolyData*> ApplyFilterLevelSet3D(double iRadius, vtkPoints* iPoints,
    int iIterations, int iCurvature,
    std::vector<vtkSmartPointer< vtkImageData > >* iImages,
    int iChannel);

  template< typename TPixel, unsigned int VImageDimension >
  void ApplyFilter(
    typename itk::Image< TPixel, VImageDimension >::Pointer iITKInput,
    typename itk::Image< OutputPixelType, VImageDimension >::Pointer & oITKOutput,
    const std::vector< double >& iCenter,
    const double& iRadius,
    const int& iIterations,
    const int& iCurvature)
  {
    typedef itk::Image< TPixel, VImageDimension > FeatureImageType;
    typedef itk::ChanAndVeseSegmentationFilter< FeatureImageType >
      SegmentationFilterType;
    typedef typename SegmentationFilterType::Pointer SegmentationFilterPointer;

    typedef typename SegmentationFilterType::InternalPointType ITKPointType;
    typedef typename SegmentationFilterType::InternalCoordRepType ITKCoordType;

    // convert center into ITKPointType
    ITKPointType itk_center;

    for( unsigned int dim = 0; dim < VImageDimension; dim++ )
      {
      itk_center[dim] = static_cast< ITKCoordType >( iCenter[dim] );
      }

    // convert radius into ITKCoordType
    ITKCoordType itk_radius = static_cast< ITKCoordType >( iRadius );

    // ITK filter
    SegmentationFilterPointer filter = SegmentationFilterType::New();
    filter->SetCenter( itk_center );
    filter->SetRadius( itk_radius );
    filter->SetFeatureImage( iITKInput );
    filter->SetPreprocess( 1 );
    filter->SetNumberOfIterations( iIterations );
    filter->SetCurvatureWeight( iCurvature );

    try
      {
      filter->Update();
      }
    catch (itk::ExceptionObject & e)
      {
      std::cerr << "Error: " << e <<std::endl;
      }

    oITKOutput->Graft( filter->GetOutput() );
  }

  std::vector<std::vector<vtkPolyData*> > ApplyFilterSetOf2D(
    double iRadius, std::vector< vtkPoints* >* iPoints,
    int iIterations, int iCurvature, int iSampling,
    std::vector<vtkSmartPointer< vtkImageData > >* iImages, int iChannel);

public slots:
  //void setIterations(int iIterations);

  //void setCurvature(int iCurvature);

protected:
  void Filter2D(double *iCenter, const int & iOrientation);

  vtkPolyData * Filter3D(double *iCenter, int iCurvature, int iIterations,
    double iRadius, std::vector< vtkSmartPointer< vtkImageData > >* iImages,
    int iChannel);

private:
  int m_Iterations;
  int m_Curvature;
  int m_Dimension;

};
#endif

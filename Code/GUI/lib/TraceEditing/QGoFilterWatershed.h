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
#ifndef __QGoFilterWatershed_h
#define __QGoFilterWatershed_h

#include "QGoFilterSemiAutoBase.h"

#include "itkWatershedBasedCellSegmentation.h"

#include "QGoGUILibConfigure.h"

class GoImageProcessor;
/**
 * \class QGoFilterWatershed
 * \brief Watershed segmentation algorithm implementation.
 * Can generate contours and meshes.
 * Will generate 2D objects if m_Dimension<2, 3D objects in the other case.
 */

class QGOGUILIB_EXPORT QGoFilterWatershed:public QGoFilterSemiAutoBase
{
  Q_OBJECT
public:
  typedef int                   OutputPixelType;

  typedef itk::Image< int, 3 >  Output3DType;
  typedef Output3DType::Pointer   Output3DPointer;

  typedef itk::Image< int, 2 >  Output2DType;
  typedef Output2DType::Pointer   Output2DPointer;

  /** \brief Constructor */
  explicit QGoFilterWatershed(QObject *iParent = NULL, int iDimension = 2);

  /** \brief Destructor */
  ~QGoFilterWatershed();

  template< typename TPixel >
  void Apply3DFilter(
    typename itk::Image< TPixel, 3 >::Pointer iITKInput,
    const int& iThresMin,
    const int& iThresMax,
    const double& iCorrTresh,
    const double& iAlpha,
    const double& iBeta)
  {
  const unsigned int Dimension = 3;
  typedef itk::Image< TPixel, Dimension >    FeatureImageType;
  typedef typename FeatureImageType::Pointer FeatureImagePointer;
  typedef itk::Image< double, Dimension >    InputImageType;
  typedef typename InputImageType::IndexType InputImageIndexType;
  typedef typename InputImageType::Pointer   InputImagePointer;
  typedef itk::Image< int, Dimension >       SegmentImageType;
  typedef typename SegmentImageType::Pointer SegmentImagePointer;

  // Apply filter
  // Apply watershed segmentation filter
  //---------------------------------------------------------
  typedef itk::Image< TPixel, 3 > FeatureImageType;
  typedef itk::WatershedBasedCellSegmentation
      <FeatureImageType, InputImageType,SegmentImageType>
    SegmentationFilterType;
  typedef typename SegmentationFilterType::Pointer SegmentationFilterPointer;

  // ITK filter
  SegmentationFilterPointer filter = SegmentationFilterType::New();
  filter->SetInput(iITKInput);
  filter->SetNucleusThresholdMin(iThresMin);
  filter->SetNucleusThresholdMax(iThresMax);
  filter->SetCorrelationThreshold1(iCorrTresh);
  filter->SetAlpha(iAlpha);
  filter->SetBeta(iBeta);
  filter->Update();

  typename Output3DType::Pointer resulting_image = filter->GetOutput();

  if( resulting_image.IsNotNull() )
    {
    m_Image3D->Graft( resulting_image );
    }
  else
    {
    itkGenericExceptionMacro(
          <<"WaterShedSegmentationFilter's output is NULL" );
    }
  }


  template< typename TPixel >
  void Apply2DFilter(
    typename itk::Image< TPixel, 2 >::Pointer iITKInput,
    const int& iThresMin,
    const int& iThresMax,
    const double& iCorrTresh,
    const double& iAlpha,
    const double& iBeta)
  {
  const unsigned int Dimension = 2;
  typedef itk::Image< TPixel, Dimension >    FeatureImageType;
  typedef typename FeatureImageType::Pointer FeatureImagePointer;
  typedef itk::Image< double, Dimension >    InputImageType;
  typedef typename InputImageType::IndexType InputImageIndexType;
  typedef typename InputImageType::Pointer   InputImagePointer;
  typedef itk::Image< int, Dimension >       SegmentImageType;
  typedef typename SegmentImageType::Pointer SegmentImagePointer;

  // Apply filter
  // Apply watershed segmentation filter
  //---------------------------------------------------------
  typedef itk::Image< TPixel, 2 > FeatureImageType;
  typedef itk::WatershedBasedCellSegmentation
      <FeatureImageType, InputImageType,SegmentImageType>
    SegmentationFilterType;
  typedef typename SegmentationFilterType::Pointer SegmentationFilterPointer;

  // ITK filter
  SegmentationFilterPointer filter = SegmentationFilterType::New();
  filter->SetInput(iITKInput);
  filter->SetNucleusThresholdMin(iThresMin);
  filter->SetNucleusThresholdMax(iThresMax);
  filter->SetCorrelationThreshold1(iCorrTresh);
  filter->SetAlpha(iAlpha);
  filter->SetBeta(iBeta);
  filter->Update();

  typename Output2DType::Pointer resulting_image = filter->GetOutput();

  if( resulting_image.IsNotNull() )
    {
    m_Image2D->Graft( resulting_image );
    }
  else
    {
    itkGenericExceptionMacro(
          <<"WaterShedSegmentationFilter's output is NULL" );
    }
  }

  std::vector<std::vector<vtkPolyData*> > ApplyFilterSetOf2D(double iRadius,
    int iThresMin, int iThresMax, double iCorrTresh, double iAlpha,
    double iBeta,  int iSampling,
    std::vector< vtkPoints* >* iPoints,
    GoImageProcessor* iImages, int iChannel);

  Output3DType::Pointer GetOutput3D()
    {
    return m_Image3D;
    }

  Output2DType::Pointer GetOutput2D()
    {
    return m_Image2D;
    }

private:
  void Filter2D(double *iCenter, const int & iOrientation);

  Output3DType::Pointer m_Image3D;
  Output2DType::Pointer m_Image2D;

  int m_TreshMin;
  int m_TreshMax;
  double m_CorrTresh;
  double m_Alpha;
  double m_Beta;
};
#endif

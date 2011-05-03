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
#ifndef __QGoMeshLevelSetAlgo_h
#define __QGoMeshLevelSetAlgo_h

#include "QGoLevelSetAlgo.h"
#include "QGoFilterChanAndVese.h"
#include "QGoAlgorithmWidget.h"
#include "QGoAlgoParameter.h"
#include "QGoGUILibConfigure.h"
#include "vtkSmartPointer.h"
#include "vtkPolyData.h"
#include "vtkImageData.h"
#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"


/**
\class QGoMeshLevelSetAlgo
\brief class to be the interface between the levelset algo for meshes
and GoFigure
*/
class QGoMeshLevelSetAlgo: public QGoLevelSetAlgo
{
public:
  QGoMeshLevelSetAlgo(std::vector< vtkPoints* >* iSeeds, QWidget *iParent = 0);
  ~QGoMeshLevelSetAlgo();

  std::vector<vtkPolyData*> ApplyAlgo(
    std::vector<vtkSmartPointer< vtkImageData > >* iImages,
    int iChannel);

protected:

  template < class TPixel, unsigned int VImageDimension >
  vtkPolyData * ApplyLevelSetFilter(const std::vector<double>& iCenter,
  std::vector<vtkSmartPointer< vtkImageData > >* iImages,
    int iChannel)
    {
    assert( iCenter.size() == VImageDimension );

    typedef TPixel PixelType;

    typedef itk::Image< PixelType, VImageDimension >  ImageType;
    typedef typename ImageType::Pointer               ImagePointer;

    // Since the pipeline is in ITK, first let's convert the image into ITK
    ImagePointer ItkInput =
        this->ConvertVTK2ITK< PixelType, VImageDimension>(
          ( *iImages )[iChannel] );


    // let's compute the bounds of the region of interest
    double radius = this->m_Radius->GetValue();

    std::vector< double > bounds( 2 * VImageDimension, 0. );
    unsigned int k = 0;
    for( unsigned int dim = 0; dim < VImageDimension; dim++ )
      {
      bounds[k++] = iCenter[dim] - 2. * radius;
      bounds[k++] = iCenter[dim] + 2. * radius;
      }

    // then let's extract the Region of Interest
    ImagePointer ITK_ROI_Image =
        this->ITKExtractROI< PixelType, VImageDimension >( bounds, ItkInput );

    int curvature_weight = this->m_Curvature->GetValue();
    int nb_iterations = this->m_Iterations->GetValue();

    typedef itk::Image< QGoFilterChanAndVese::OutputPixelType, VImageDimension >
        OutputImageType;

    typename OutputImageType::Pointer ItkOutput = OutputImageType::New();

    // Compute the segmentation in 3D
    QGoFilterChanAndVese Filter;
    Filter.ApplyFilter< PixelType, VImageDimension >( ITK_ROI_Image,
          ItkOutput,
          iCenter,
          radius,
          nb_iterations,
          curvature_weight );

    // Here it would be better if the mesh extraction would be performed directly
    // in ITK instead.
    vtkImageData * FilterOutPutToVTK =
        this->ConvertITK2VTK<
          typename QGoFilterChanAndVese::OutputPixelType,
          VImageDimension >( ItkOutput );

    vtkPolyData* temp_output = this->ExtractPolyData(FilterOutPutToVTK, 0);

    double temp_bounds[6];
    temp_output->GetBounds( temp_bounds );

    double temp_center[3];
    temp_center[0] = ( temp_bounds[0] + temp_bounds[1] ) * 0.5;
    temp_center[1] = ( temp_bounds[2] + temp_bounds[3] ) * 0.5;
    temp_center[2] = ( temp_bounds[4] + temp_bounds[5] ) * 0.5;

    vtkSmartPointer< vtkTransform > translation =
        vtkSmartPointer< vtkTransform >::New();

    /// \todo fix it to get the real center!!!
    translation->Translate(iCenter[0] - temp_center[0],
                           iCenter[1] - temp_center[1],
                           iCenter[2] - temp_center[2] );

    vtkSmartPointer< vtkTransformPolyDataFilter > mesh_transform =
        vtkSmartPointer< vtkTransformPolyDataFilter >::New();
    mesh_transform->SetTransform(translation);
    mesh_transform->SetInput( temp_output );
    mesh_transform->Update();

    vtkPolyData* mesh = vtkPolyData::New();
    mesh->DeepCopy( mesh_transform->GetOutput() );

    return mesh;
    }
};

#endif

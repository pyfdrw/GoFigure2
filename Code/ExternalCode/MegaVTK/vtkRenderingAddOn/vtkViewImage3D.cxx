/*========================================================================
 Copyright (c) INRIA - ASCLEPIOS Project (http://www-sop.inria.fr/asclepios).
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 * Neither the name of INRIA or ASCLEPIOS, nor the names of any contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 * Modified source versions must be plainly marked as such, and must not be
 misrepresented as being the original software.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 =========================================================================*/

/*=========================================================================
 Modifications were made by the GoFigure Dev. Team.
 while at Megason Lab, Systems biology, Harvard Medical school, 2009-11

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

#include "vtkViewImage3D.h"

#include "vtkSmartPointer.h"

#include "vtkInteractorStyleImage3D.h"
#include "vtkViewImage3DCommand.h"

#include "vtkImageAppendComponents.h"
#include "vtkImageExtractComponents.h"
#include "vtkCamera.h"
#include "vtkCommand.h"
#include "vtkImageActor.h"
#include "vtkImageData.h"
#include "vtkImageMapToWindowLevelColors.h"
#include "vtkInteractorStyleImage.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkMatrix4x4.h"
#include "vtkTransform.h"
#include "vtkScalarBarActor.h"
#include "vtkOrientationAnnotation.h"
#include "vtkCornerAnnotation.h"
#include "vtkTextProperty.h"
#include "vtkLookupTable.h"
#include "vtkMath.h"
#include "vtkPlane.h"
#include "vtkCutter.h"
// #include "vtkQuadricLODActor.h"
#include "vtkActor.h"
#include "vtkPolyDataMapper.h"
#include "vtkPoints.h"
#include "vtkIdList.h"
#include "vtkOutlineSource.h"
#include "vtkMatrixToLinearTransform.h"
#include "vtkPointData.h"
#include "vtkUnsignedCharArray.h"
#include "vtkIntArray.h"
#include "vtkImageAccumulate.h"
#include "vtkQuadricClustering.h"

#include <vtkClipPolyData.h>
#include <vtkInteractorStyleTrackball.h>
#include <vtkRendererCollection.h>
#include <vtkImageData.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeTextureMapper3D.h>
#include <vtkVolumeMapper.h>
#include <vtkFiniteDifferenceGradientEstimator.h>
#include <vtkVolumeTextureMapper2D.h>
#include <vtkProperty.h>
#include <vtkVolume.h>
#include <vtkImageDataGeometryFilter.h>
#include <vtkImageActor.h>
#include <vtkAxes.h>
#include <vtkMatrix4x4.h>
#include <vtkTubeFilter.h>
#include <vtkAnnotatedCubeActor.h>
#include <vtkPropAssembly.h>
#include <vtkAxesActor.h>
#include <vtkTextProperty.h>
#include <vtkCaptionActor2D.h>
#include <vtkPointData.h>
#include <vtkImageReslice.h>
#include "vtkRenderWindow.h"
#include "vtkScalarsToColors.h"
#include "vtkColorTransferFunction.h"
#include <vtkImageShiftScale.h>
#include <vtkDataSet.h>
#include <vtkDataSetMapper.h>
#include <vtkPlane.h>
#include <vtkPlaneCollection.h>
#include <vtkGeometryFilter.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkCellData.h>

#include <vector>
#include <string>
#include <algorithm>

#include "vtkPlanes.h"
#include "vtkPlane.h"

vtkCxxRevisionMacro(vtkViewImage3D, "$Revision: 501 $");
vtkStandardNewMacro(vtkViewImage3D);

//----------------------------------------------------------------------------
class VTK_RENDERINGADDON2_EXPORT ImageActorCallback : public vtkCommand
{
public:

  static ImageActorCallback * New() {
    return new ImageActorCallback;
  }

  void Execute( vtkObject *caller,
                unsigned long event,
                void *vtkNotUsed(callData) )
  {
    if ( !this->Actor ) { return; }
    vtkImageActor *imagecaller = vtkImageActor::SafeDownCast (caller);

    if ( !imagecaller ) { return; }
    if ( !imagecaller->GetInput() ) { return; }

    if ( event == vtkCommand::ModifiedEvent )
      {
      this->Actor->SetInput( imagecaller->GetInput() );
      this->Actor->SetInterpolate( imagecaller->GetInterpolate() );
      this->Actor->SetOpacity( imagecaller->GetOpacity() );
      this->Actor->SetDisplayExtent ( imagecaller->GetDisplayExtent() );
      }
  }

//----------------------------------------------------------------------------

  vtkImageActor *Actor;
protected:
  ImageActorCallback() : Actor(0) {
  }

  ~ImageActorCallback() {
  }

};

//----------------------------------------------------------------------------
/**
 *
 */
vtkViewImage3D::vtkViewImage3D()
{
  this->VolumeProperty  = vtkVolumeProperty::New();
  this->VolumeActor     = vtkVolume::New();
  this->Callback = vtkImage3DCroppingBoxCallback::New();
  this->VolumeMapper3D = vtkVolumeTextureMapper3D::New();

  this->Phantom.push_back( vtkImageActor::New() );
  this->Phantom.push_back( vtkImageActor::New() );
  this->Phantom.push_back( vtkImageActor::New() );

  this->PhantomCallback.push_back( ImageActorCallback::New() );
  this->PhantomCallback.push_back( ImageActorCallback::New() );
  this->PhantomCallback.push_back( ImageActorCallback::New() );

  this->BoundsActor.push_back( vtkActor::New() );
  this->BoundsActor.push_back( vtkActor::New() );
  this->BoundsActor.push_back( vtkActor::New() );

  this->Command = vtkViewImage3DCommand::New();
  this->Command->SetVtkImageView3D(this);
  // the new interactor style
  this->InteractorStyle3D = vtkInteractorStyleImage3D::New();

//   this->SetupVolumeRendering();
  this->SetupWidgets();
}

//----------------------------------------------------------------------------
/**
 *
 */
vtkViewImage3D::~vtkViewImage3D()
{
  // delete all vtk objetcts:
  this->VolumeMapper3D->Delete();
  this->VolumeProperty->Delete();
  this->VolumeActor->Delete();
  this->Callback->Delete();
  this->Cube->Delete();
  this->Marker->Delete();
  this->Phantom[0]->Delete();
  this->Phantom[1]->Delete();
  this->Phantom[2]->Delete();
  this->PhantomCallback[0]->Delete();
  this->PhantomCallback[1]->Delete();
  this->PhantomCallback[2]->Delete();
  this->BoundsActor[0]->Delete();
  this->BoundsActor[1]->Delete();
  this->BoundsActor[2]->Delete();

  this->Command->Delete();
  this->InteractorStyle3D->Delete();
}

//----------------------------------------------------------------------------
/**
 *
 */
void vtkViewImage3D::SetupVolumeRendering()
{
  // MAPPER
  // crop volume into 27? small regions
  // for efficiency?
  this->VolumeMapper3D->CroppingOn();
  this->VolumeMapper3D->SetCroppingRegionFlagsToSubVolume();
  this->VolumeMapper3D->SetCroppingRegionFlags (0x7ffdfff);

  // PROPERTY
  // opacity TF
  vtkPiecewiseFunction* opacityfunction = vtkPiecewiseFunction::New();
  opacityfunction->AddPoint (0, 0.0);
  opacityfunction->AddPoint (255, 1.0);
  this->VolumeProperty->SetScalarOpacity(opacityfunction);
  opacityfunction->Delete();

  // one dataset-1 tf, not 1 tf for each component
  this->VolumeProperty->IndependentComponentsOff();
  this->VolumeProperty->SetInterpolationTypeToLinear();
  this->VolumeProperty->ShadeOff();
  this->VolumeProperty->SetDiffuse (0.9);
  this->VolumeProperty->SetAmbient (0.2);
  this->VolumeProperty->SetSpecular (0.3);
  this->VolumeProperty->SetSpecularPower (15.0);

  // ACTOR
  this->VolumeActor->SetProperty (this->VolumeProperty);
  this->VolumeActor->SetMapper (this->VolumeMapper3D);
  this->VolumeActor->PickableOff();
  this->VolumeActor->DragableOff();
  this->VolumeActor->SetVisibility (0);

  // set up the boxwidget/ callback
  this->Callback->SetVolumeMapper (this->VolumeMapper3D);

  this->Renderer->AddViewProp (this->VolumeActor);
}

//----------------------------------------------------------------------------
/**
 *
 */
void vtkViewImage3D::SetupWidgets()
{
  // Create an annotated cube actor (directions)
  this->Cube = vtkAnnotatedCubeActor::New();
  this->Cube->SetXPlusFaceText( this->DirectionAnnotationMatrix[0][1].c_str() );
  this->Cube->SetXMinusFaceText( this->DirectionAnnotationMatrix[0][0].c_str() );
  this->Cube->SetYPlusFaceText( this->DirectionAnnotationMatrix[1][1].c_str() );
  this->Cube->SetYMinusFaceText( this->DirectionAnnotationMatrix[1][0].c_str() );
  this->Cube->SetZPlusFaceText( this->DirectionAnnotationMatrix[2][1].c_str() );
  this->Cube->SetZMinusFaceText( this->DirectionAnnotationMatrix[2][0].c_str() );
  this->Cube->SetZFaceTextRotation(90);
  this->Cube->SetFaceTextScale(0.65);
  this->Cube->GetCubeProperty()->SetColor(0.5, 1, 1);
  this->Cube->GetTextEdgesProperty()->SetLineWidth(1);
  this->Cube->GetTextEdgesProperty()->SetDiffuse(0);
  this->Cube->GetTextEdgesProperty()->SetAmbient(1);
  this->Cube->GetTextEdgesProperty()->SetColor(0.18, 0.28, 0.23);
  this->Cube->SetTextEdgesVisibility (1);
  this->Cube->SetCubeVisibility(1);
  this->Cube->SetFaceTextVisibility(1);

  this->Cube->GetXPlusFaceProperty()->SetColor (1, 0, 0);
  this->Cube->GetXPlusFaceProperty()->SetInterpolationToFlat();
  this->Cube->GetXMinusFaceProperty()->SetColor (1, 0, 0);
  this->Cube->GetXMinusFaceProperty()->SetInterpolationToFlat();
  this->Cube->GetYPlusFaceProperty()->SetColor (0, 1, 0);
  this->Cube->GetYPlusFaceProperty()->SetInterpolationToFlat();
  this->Cube->GetYMinusFaceProperty()->SetColor (0, 1, 0);
  this->Cube->GetYMinusFaceProperty()->SetInterpolationToFlat();
  this->Cube->GetZPlusFaceProperty()->SetColor (0, 0, 1);
  this->Cube->GetZPlusFaceProperty()->SetInterpolationToFlat();
  this->Cube->GetZMinusFaceProperty()->SetColor (0, 0, 1);
  this->Cube->GetZMinusFaceProperty()->SetInterpolationToFlat();

  this->Marker = vtkOrientationMarkerWidget::New();
  this->Marker->SetOutlineColor (0.93, 0.57, 0.13);
  this->Marker->SetOrientationMarker (this->Cube);
  this->Marker->SetViewport (0.0, 0.05, 0.15, 0.15);
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *
 */
void vtkViewImage3D::Render()
{
  if ( this->FirstRender )
    {
    // Initialize the size if not set yet
    vtkImageData *input = this->GetInput();
    if ( this->RenderWindow->GetSize()[0] == 0 && input )
      {
      if ( this->Renderer )
        {
        this->ResetCamera();
        }
      this->FirstRender = 0;
      }
    }
  if ( this->GetInput() )
    {
    this->RenderWindow->Render();
    }
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *
 */
void vtkViewImage3D::SetVolumeRenderingOff()
{
  this->VolumeActor->SetVisibility (false);
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *
 */
void vtkViewImage3D::SetVolumeRenderingOn()
{
  int *size = this->GetInput()->GetDimensions();

  if ( ( size[0] < 2 )
    || ( size[1] < 2 )
    || ( size[2] < 2 ) )
    {
    vtkWarningMacro (<< "Cannot do volume rendering for a single slice, skipping" << endl);
    return;
    }

  // check if RGB already
  // or create RGB using LUT if single channel input
  int NbOfComp = this->GetInput()->GetNumberOfScalarComponents();

  vtkSmartPointer<vtkImageData> image;

  if ( NbOfComp > 1 && NbOfComp != 4 && NbOfComp < 5 )
    {
    image = this->GetInput();
    }
  else
    {
    // color single channel image with current LUT
    vtkImageData* temp = this->GetInput();
    vtkSmartPointer<vtkImageMapToColors> map =
        vtkSmartPointer<vtkImageMapToColors>::New();
    map->SetLookupTable(this->LookupTable);
    map->SetInput(temp);
    map->SetOutputFormatToRGB();
    map->Update();
    image = map->GetOutput();
    }

  this->SetupVolumeRendering();

  // get the index of the first non-NULL component
  int i(0);
  for(i=0; i<3;++i)
    {
    double range[2];
    image->GetPointData()->GetScalars()->GetRange(range,i);
    if(range[1]>0)
      {
      break;
      }
    }

  // mix components
  // dont't really get the point but has sth to do with the alpha component
  // create a "FAKE" 4th alpha channel...??
  vtkSmartPointer< vtkImageExtractComponents > extComp =
    vtkSmartPointer< vtkImageExtractComponents >::New();
  extComp->SetInput(image.GetPointer());
  extComp->SetComponents(i);
  extComp->Update();

  vtkSmartPointer< vtkImageAppendComponents > addComp =
    vtkSmartPointer< vtkImageAppendComponents >::New();
  addComp->AddInput(image);
  addComp->AddInput( extComp->GetOutput() );
  addComp->Update();

  // add output to mapper
  this->VolumeMapper3D->SetInput( addComp->GetOutput() );

  // check if 3d mapper is supported
  this->SetupTextureMapper();

  this->VolumeActor->SetVisibility (true);
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *
 */
void vtkViewImage3D::SetTriPlanarRenderingOn()
{
  this->VolumeActor->SetVisibility(false);
  for ( int i = 0; i < 3; i++ )
    {
    this->Phantom[i]->SetVisibility(true);
    this->BoundsActor[i]->SetVisibility(true);
    }
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void vtkViewImage3D::SetTriPlanarRenderingOff()
{
  this->VolumeActor->SetVisibility(true);
  for ( int i = 0; i < 3; i++ )
    {
    this->Phantom[i]->SetVisibility(false);
    this->BoundsActor[i]->SetVisibility(false);
    }
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *
 * @param i
 * @param input
 * @param in_bounds
 */
void vtkViewImage3D::Add2DPhantom(const unsigned int & i,
                                  vtkImageActor *input,
                                  vtkPolyData *in_bounds)
{
  if ( i >= 3 )
    {
    return;
    }

  vtkRenderer *ren = this->GetRenderer();
  if ( ren )
    {
    ren->RemoveActor(this->Phantom[i]);

    this->Phantom[i]->SetInput ( input->GetInput() );
    this->Phantom[i]->SetDisplayExtent ( input->GetDisplayExtent() );
    this->Phantom[i]->SetUserMatrix ( input->GetUserMatrix() );
    this->PhantomCallback[i]->Actor = this->Phantom[i];
    input->AddObserver (vtkCommand::ModifiedEvent, this->PhantomCallback[i]);
    ren->AddActor (this->Phantom[i]);

    /**
       IMPORTANT NOTE

       Adding a 2D actor in the 3D scene should be as simple as the next line
       instead of the code above...

       Unfortunately it does not seem to work properly. But this is something
       we should investigate in because it would be much simpler
    */
    //     this->GetRenderer()->AddActor (input);

    if ( in_bounds )
      {
      ren->RemoveActor(this->BoundsActor[i]);

      vtkSmartPointer< vtkPolyDataMapper > bounds_mapper =
        vtkSmartPointer< vtkPolyDataMapper >::New();
      bounds_mapper->SetInput(in_bounds);
      bounds_mapper->StaticOn();

      this->BoundsActor[i]->SetMapper(bounds_mapper);
      this->BoundsActor[i]->GetProperty()->SetRepresentationToWireframe();
      this->BoundsActor[i]->GetProperty()->SetLineWidth(2.);

      ren->AddActor(this->BoundsActor[i]);
      }
    }
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void
vtkViewImage3D::SetBoundsActorsVisibility(bool iVisibility)
{
  vtkstd::vector< vtkActor * >::iterator
    BoundsActorIterator = BoundsActor.begin();

  while ( BoundsActorIterator != BoundsActor.end() )
    {
    ( *BoundsActorIterator )->SetVisibility(iVisibility);
    ++BoundsActorIterator;
    }
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *
 */
void vtkViewImage3D::InstallPipeline()
{
  if ( this->RenderWindow && this->Renderer )
    {
    this->RenderWindow->AddRenderer(this->Renderer);
    }

  if ( this->Interactor )
    {
    // Add observers
    this->InteractorStyle3D->AddObserver(
      vtkViewImage3DCommand::MeshPickingEvent, this->Command);
    this->InteractorStyle3D->AddObserver(
      vtkViewImage3DCommand::BoxPickingEvent, this->Command);

    this->Interactor->SetInteractorStyle(this->InteractorStyle3D);
    this->Interactor->SetRenderWindow(this->RenderWindow);

    //this->PlaneWidget->SetInteractor ( this->Interactor );
    this->Marker->SetInteractor (this->Interactor);

    this->Marker->On();
    this->Marker->InteractiveOff ();
    }
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *
 * @param dataset
 * @param property
 * @param intersection
 * @param iDataVisibility
 * @return
 */
// vtkQuadricLODActor*
vtkActor *
vtkViewImage3D::AddDataSet(vtkDataSet *dataset,
                           vtkProperty *property,
                           const bool & intersection,
                           const bool & iDataVisibility)
{
  (void)intersection;

  if ( !this->Renderer )
    {
    return NULL;
    }

  vtkCamera *cam = this->Renderer->GetActiveCamera();

  if ( !cam )
    {
    return NULL;
    }

  vtkSmartPointer< vtkPolyDataMapper > mapper =
    vtkSmartPointer< vtkPolyDataMapper >::New();
  mapper->SetInput( dynamic_cast< vtkPolyData * >( dataset ) );
  mapper->SetScalarVisibility(iDataVisibility);
  mapper->StaticOn();

  vtkActor *actor3d = vtkActor::New();
  actor3d->SetMapper(mapper);

  if ( property )
    {
    // Generates bug in visu
    //actor3d->SetProperty( property );
    actor3d->GetProperty()->SetColor( property->GetColor() );
    actor3d->GetProperty()->SetOpacity( property->GetOpacity() );
    actor3d->GetProperty()->SetLineWidth(this->IntersectionLineWidth);
    }

  return actor3d;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *
 * @param matrix
 */
void vtkViewImage3D::SetOrientationMatrix(vtkMatrix4x4 *matrix)
{
  this->Superclass::SetOrientationMatrix (matrix);

  this->VolumeActor->SetUserMatrix (matrix);
  //this->BoxWidget->SetOrientationMatrix (matrix);
//   this->PlaneWidget->SetTransform (transform);
}

//----------------------------------------------------------------------------
/**
 *
 */
void vtkViewImage3D::SetupTextureMapper()
{
  if ( !this->GetInput() )
    {
    return;
    }

  vtkVolumeTextureMapper3D *mapper3D =
    vtkVolumeTextureMapper3D::SafeDownCast( this->VolumeActor->GetMapper() );

  if ( mapper3D && !this->GetRenderWindow()->GetNeverRendered() )
    {
#if VTK_MAJOR_VERSION == 5 && VTK_MINOR_VERSION == 6 && VTK_BUILD_VERSION == 0
    if ( !mapper3D->IsRenderSupported (this->VolumeProperty) )
#else
    if ( !mapper3D->IsRenderSupported( this->VolumeProperty,
                                       this->GetRenderer() ) )
#endif
      {
      //try the ATI fragment program implementation
      // mapper3D->SetPreferredMethodToFragmentProgram();

#if VTK_MAJOR_VERSION == 5 && VTK_MINOR_VERSION == 6 && VTK_BUILD_VERSION == 0
      if ( !mapper3D->IsRenderSupported (this->VolumeProperty) )
#else
      if ( !mapper3D->IsRenderSupported( this->VolumeProperty,
                                         this->GetRenderer() ) )
#endif
        {
        vtkWarningMacro (
          << "Warning: 3D Texture volume rendering is not supported by your"
          << " hardware, I switch to 2D Texture rendering." << endl);

        /// \todo FIX LEAK
        vtkVolumeTextureMapper2D *newMapper =
          vtkVolumeTextureMapper2D::New();
        newMapper->CroppingOn();
        newMapper->SetCroppingRegionFlags (0x7ffdfff);

        double rangeR[2];
        this->GetInput()->GetPointData()->GetScalars()->GetRange(rangeR, 0);
        double rangeG[2];
        this->GetInput()->GetPointData()->GetScalars()->GetRange(rangeG, 1);
        double rangeB[2];
        this->GetInput()->GetPointData()->GetScalars()->GetRange(rangeB, 2);

        double rangeMax = std::max(rangeB[1], std::max(rangeR[1], rangeG[1]));
        double rangeMin = std::min(rangeB[0], std::min(rangeR[0], rangeG[0]));


        double  shift = 0 - rangeMin;
        double  scale = 65535.0 / ( rangeMax - rangeMin );

        vtkSmartPointer< vtkImageShiftScale > scaler =
          vtkSmartPointer< vtkImageShiftScale >::New();
        scaler->SetInput ( this->GetInput() );
        scaler->SetShift (shift);
        scaler->SetScale (scale);
        scaler->SetOutputScalarTypeToUnsignedShort();
        scaler->Update();
        newMapper->SetInput ( scaler->GetOutput() );
        scaler->Delete();
        this->Callback->SetVolumeMapper (newMapper);

        mapper3D->Delete();
        this->VolumeMapper3D = newMapper;
        this->VolumeActor->SetMapper (this->VolumeMapper3D);
        }
      }
    }
}

//----------------------------------------------------------------------------
/**
 *
 */
vtkInteractorStyleImage3D *
vtkViewImage3D::GetInteractorStyle3D()
{
  return this->InteractorStyle3D;
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void
vtkViewImage3D::ComputeDistances(double *n, double *origin)
{
  // go through all actors
  // relative distance from point to plane
  vtkActorCollection *test = this->Renderer->GetActors();
  test->InitTraversal();
  vtkProp3D *prop_temp = test->GetNextActor();

  while ( prop_temp )
    {
    double *point = prop_temp->GetCenter();
    double  distance = n[0] * ( point[0] - origin[0] )
      + n[1] * ( point[1] - origin[1] )
      + n[2] * ( point[2] - origin[2] );

    bool state;
    if ( distance < 0 )
      {
      state = false;
      }
    else
      {
      state = true;
      }

    this->GetInteractorStyle3D()->SetCurrentProp(prop_temp);
    this->GetInteractorStyle3D()->SetCurrentState(state);
    this->InvokeEvent(vtkViewImage3DCommand::VisibilityUpdatedEvent);

    prop_temp = test->GetNextActor();
    }
  // emit signal to say to render
  this->InvokeEvent(vtkViewImage3DCommand::UpdateRenderEvent);
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void
vtkViewImage3D::ComputeDistancesToSquare(vtkPlanes *planes)
{
  // go through all actors
  // relative distance from point to plane
  vtkActorCollection *test = this->Renderer->GetActors();
  test->InitTraversal();
  vtkProp3D *prop_temp = test->GetNextActor();

  while ( prop_temp )
    {
    double *point(prop_temp->GetCenter());
    bool    show(true);

    for ( int i = 0; i < 6; ++i )
      {
      double *n = planes->GetPlane(i)->GetNormal();
      double *origin = planes->GetPlane(i)->GetOrigin();
      double  distance = n[0] * ( point[0] - origin[0] )
        + n[1] * ( point[1] - origin[1] )
        + n[2] * ( point[2] - origin[2] );

      if ( distance > 0 )
        {
        show = false;
        break;
        }
      }

    this->GetInteractorStyle3D()->SetCurrentProp(prop_temp);
    this->GetInteractorStyle3D()->SetCurrentState(show);
    this->InvokeEvent(vtkViewImage3DCommand::VisibilityUpdatedEvent);

    prop_temp = test->GetNextActor();
    }

  planes->Delete();
  // emit signal to say to render
  this->InvokeEvent(vtkViewImage3DCommand::UpdateRenderEvent);
}

//----------------------------------------------------------------------------

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
#include "vtkSphereSource.h"
#include "vtkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"

#include "MeshToContourFilter.h"

int main(int argc, char *argv[])
{
  if ( argc != 2 )
    {
    std::cerr << "./ContourToMeshFilter(.exe) takes 1 arguments" << std::endl;
    std::cerr << "1- boolean value (test)" << std::endl;
    return EXIT_FAILURE;
    }

  // create sphere
  vtkSmartPointer<vtkSphereSource> sphereSource2 =
    vtkSmartPointer<vtkSphereSource>::New();
  sphereSource2->SetRadius(10);
  sphereSource2->SetCenter(25,0,0);
  sphereSource2->SetThetaResolution(10);
  sphereSource2->SetPhiResolution(10);
  sphereSource2->Update();


  // split sphere
  MeshToContourFilter filter;

  filter.SetInput(sphereSource2->GetOutput());
  filter.SetSpacing(1, 1, 1);

  std::map<double, vtkPolyData* > output =
      filter.ExtractPolyData( MeshToContourFilter::XY );

  vtkSmartPointer< vtkRenderer > renderer =
    vtkSmartPointer< vtkRenderer >::New();

  std::map<double, vtkPolyData* >::iterator it = output.begin();

  while (it!=output.end())
    {
    vtkSmartPointer<vtkPolyDataMapper> mapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInput( it->second );

    vtkSmartPointer<vtkActor> actor =
      vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    renderer->AddActor(actor);

    ++it;
    }

  vtkSmartPointer< vtkRenderWindow > renwin =
    vtkSmartPointer< vtkRenderWindow >::New();
  renwin->AddRenderer(renderer);

  vtkSmartPointer< vtkRenderWindowInteractor > iren =
    vtkSmartPointer< vtkRenderWindowInteractor >::New();
  iren->SetRenderWindow(renwin);

  renwin->Render();

  if ( atoi(argv[1]) == 1 )
    {
    iren->CreateOneShotTimer(1);
    }
  else
    {
    iren->Start();
    }

  // clean map
  it = output.begin();
  while (it!=output.end())
    {
    (it->second)->Delete();
    ++it;
    }

  return EXIT_SUCCESS;
}
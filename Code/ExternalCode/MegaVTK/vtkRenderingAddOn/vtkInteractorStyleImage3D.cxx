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

#include "vtkInteractorStyleImage3D.h"

#include <vtkObjectFactory.h>
#include <vtkRenderWindowInteractor.h>
#include "vtkRenderer.h"
#include "vtkProperty.h"
#include "vtkViewImage3DCommand.h"

#include <vtkProp.h>

// For picking
#include "vtkAbstractPropPicker.h"
#include "vtkAssemblyPath.h"

vtkCxxRevisionMacro (vtkInteractorStyleImage3D, "$Revision: 1 $");
vtkStandardNewMacro (vtkInteractorStyleImage3D);

//----------------------------------------------------------------------------
vtkInteractorStyleImage3D::vtkInteractorStyleImage3D()
{
  this->m_Mode = InteractionTypeDefault;

  m_State = false;
}

//----------------------------------------------------------------------------
vtkInteractorStyleImage3D::
~vtkInteractorStyleImage3D()
{
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::OnMouseMove()
{
  switch ( this->State )
    {
    case VTKIS_PICK3D:
      HighlightCurrentActor();
      break;
    }

  this->Superclass::OnMouseMove();
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::OnLeftButtonDown()
{
  switch ( this->m_Mode )
    {
    case InteractionTypeZoom:
      this->InvokeEvent(vtkViewImage3DCommand::ZoomEvent);
      this->Superclass::OnRightButtonDown();
      break;
    case InteractionTypePan:
      this->InvokeEvent(vtkViewImage3DCommand::PanEvent);
      this->Superclass::OnMiddleButtonDown();
      break;
    case InteractionTypeMeshPicking:
      this->SetCurrentProp(this->CurrentProp);
      this->InvokeEvent(vtkViewImage3DCommand::MeshPickingEvent);
      this->State = VTKIS_NONE;
      this->Superclass::OnLeftButtonDown();
      break;
    default:
      this->Superclass::OnLeftButtonDown();
      break;
    }
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::OnLeftButtonUp()
{
  switch ( this->m_Mode )
    {
    case InteractionTypeZoom:
      this->Superclass::OnRightButtonUp();
      break;
    case InteractionTypePan:
      this->Superclass::OnMiddleButtonUp();
      break;
    case InteractionTypeMeshPicking:
      this->State = VTKIS_NONE;
      this->StartPick();
      break;
    default:
      this->Superclass::OnLeftButtonUp();
      break;
    }
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::OnRightButtonDown()
{
  switch ( this->m_Mode )
    {
    case InteractionTypePan:
      this->Superclass::OnMiddleButtonDown();
      break;
    case InteractionTypeMeshPicking:
      this->State = VTKIS_NONE;
      this->Superclass::OnRightButtonDown();
      break;
    default:
      this->Superclass::OnRightButtonDown();
      break;
    }
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::OnRightButtonUp()
{
  switch ( this->m_Mode )
    {
    case InteractionTypePan:
      this->Superclass::OnMiddleButtonUp();
      break;
    case InteractionTypeMeshPicking:
      this->State = VTKIS_NONE;
      this->StartPick();
      break;
    default:
      this->Superclass::OnRightButtonUp();
      break;
    }
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::OnMiddleButtonDown()
{
  switch ( this->m_Mode )
    {
    case InteractionTypeZoom:
      this->Superclass::OnRightButtonDown();
      break;
    case InteractionTypeMeshPicking:
      this->State = VTKIS_NONE;
      this->Superclass::OnMiddleButtonDown();
      break;
    default:
      this->Superclass::OnMiddleButtonDown();
      break;
    }
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::OnMiddleButtonUp()
{
  switch ( this->m_Mode )
    {
    case InteractionTypeZoom:
      this->Superclass::OnRightButtonUp();
      break;
    case InteractionTypeMeshPicking:
      this->State = VTKIS_NONE;
      this->StartPick();
      break;
    default:
      this->Superclass::OnMiddleButtonUp();
      break;
    }
}
//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::OnChar()
{
  vtkRenderWindowInteractor *rwi = this->Interactor;

  if ( ( rwi->GetKeyCode() == 'w' ) || ( rwi->GetKeyCode() == 'W' ) )
      {
      vtkActorCollection *ac;
      vtkActor *anActor, *aPart;
      vtkAssemblyPath *path;
      this->FindPokedRenderer(rwi->GetEventPosition()[0],
                              rwi->GetEventPosition()[1]);
      if(this->CurrentRenderer!=0)
        {
        ac = this->CurrentRenderer->GetActors();
        vtkCollectionSimpleIterator ait;
        for (ac->InitTraversal(ait); (anActor = ac->GetNextActor(ait)); )
          {
          for (anActor->InitPathTraversal(); (path=anActor->GetNextPath()); )
            {

            // make sure we don't modify the planes actors
            if ( path != NULL )
              {
              std::vector< vtkProp3D * >::iterator it2 = m_PlanesActors.begin();
              while(it2!=m_PlanesActors.end())
                {
                if(path && dynamic_cast<vtkProp*>(*it2) == path->GetLastNode()->GetViewProp())
                  {
                  path = NULL;
                  }
                ++it2;
                }
              }

            // go on if target is not a plane
            if(path)
              {
              aPart=static_cast<vtkActor *>(path->GetLastNode()->GetViewProp());
              aPart->GetProperty()->SetRepresentationToWireframe();
              }
            }
          }
        }
      else
        {
        vtkWarningMacro(<<"no current renderer on the interactor style.");
        }
      rwi->Render();
      return;
      }
  else if ( ( rwi->GetKeyCode() == 's' ) || ( rwi->GetKeyCode() == 'S' ) )
      {
      vtkActorCollection *ac;
      vtkActor *anActor, *aPart;
      vtkAssemblyPath *path;
      this->FindPokedRenderer(rwi->GetEventPosition()[0],
                              rwi->GetEventPosition()[1]);
      if(this->CurrentRenderer!=0)
        {
        ac = this->CurrentRenderer->GetActors();
        vtkCollectionSimpleIterator ait;
        for (ac->InitTraversal(ait); (anActor = ac->GetNextActor(ait)); )
          {
          for (anActor->InitPathTraversal(); (path=anActor->GetNextPath()); )
            {

            // make sure we don't modify the planes actors
            if ( path != NULL )
              {
              std::vector< vtkProp3D * >::iterator it2 = m_PlanesActors.begin();
              while(it2!=m_PlanesActors.end())
                {
                if(path && dynamic_cast<vtkProp*>(*it2) == path->GetLastNode()->GetViewProp())
                  {
                  path = NULL;
                  }
                ++it2;
                }
              }

            // go on if target is not a plane
            if(path)
              {
              aPart=static_cast<vtkActor *>(path->GetLastNode()->GetViewProp());
              aPart->GetProperty()->SetRepresentationToSurface();
              }
            }
          }
        }
      else
        {
        vtkWarningMacro(<<"no current renderer on the interactor style.");
        }
      rwi->Render();
      return;
      }

  this->Superclass::OnChar();
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::SetCurrentProp(vtkProp *iCurrent)
{
  this->m_CurrentProp = iCurrent;
}

//----------------------------------------------------------------------------
vtkProp *
vtkInteractorStyleImage3D::GetCurrentProp()
{
  return this->m_CurrentProp;
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::SetCurrentState(bool iState)
{
  m_State = iState;
}

//----------------------------------------------------------------------------
bool
vtkInteractorStyleImage3D::GetCurrentState()
{
  return m_State;
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::StartPick()
{
  if ( this->State != VTKIS_NONE )
    {
    return;
    }
  this->StartState(VTKIS_PICK3D);
  this->InvokeEvent(vtkCommand::StartPickEvent, this);
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::HighlightCurrentActor()
{
  vtkRenderWindowInteractor *rwi = this->Interactor;

  if ( this->CurrentRenderer != 0 )
    {
    vtkAssemblyPath *path = NULL;
    int *            eventPos = rwi->GetEventPosition();
    this->FindPokedRenderer(eventPos[0], eventPos[1]);
    rwi->StartPickCallback();
    vtkAbstractPropPicker *picker =
      vtkAbstractPropPicker::SafeDownCast( rwi->GetPicker() );

    if ( picker != NULL )
      {
      picker->Pick(eventPos[0], eventPos[1],
                   0.0, this->CurrentRenderer);
      path = picker->GetPath();
      }

    // check if item does does not belong to phantom[]
    if ( path != NULL )
      {
      std::vector< vtkProp3D * >::iterator it2 = m_PlanesActors.begin();
      while(it2!=m_PlanesActors.end())
        {
        if(path && dynamic_cast<vtkProp*>(*it2) == path->GetFirstNode()->GetViewProp())
          {
          path = NULL;
          }
        ++it2;
        }
      }

    if ( path == NULL )
      {
      this->HighlightProp(NULL);
      this->PropPicked = 0;
      }
    else
      {
      this->HighlightProp( path->GetFirstNode()->GetViewProp() );
      this->PropPicked = 1;
      }
    rwi->EndPickCallback();
    }
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::EnablePickMode()
{
  this->State = VTKIS_NONE;
  this->m_Mode = InteractionTypeMeshPicking;
  this->StartPick();
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::EnableZoomMode()
{
  this->State = VTKIS_NONE;
  this->m_Mode = InteractionTypeZoom;
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::EnablePanMode()
{
  this->State = VTKIS_NONE;
  this->m_Mode = InteractionTypePan;
}

//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::EnableDefaultMode()
{
  this->State = VTKIS_NONE;
  this->m_Mode = InteractionTypeDefault;
}
//----------------------------------------------------------------------------
void
vtkInteractorStyleImage3D::
SetPlanesActors( std::vector< vtkProp3D * > iBounds)
{
  this->m_PlanesActors = iBounds;
}
//----------------------------------------------------------------------------

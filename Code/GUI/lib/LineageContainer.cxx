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

#include "LineageContainer.h"

#include "vtkPolyData.h"
#include "vtkCellArray.h"
#include "vtkTriangle.h"

//-------------------------------------------------------------------------
LineageContainer::
LineageContainer(QObject *iParent,QGoImageView3D *iView):Superclass(iParent, iView)
{}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
LineageContainer::
~LineageContainer()
{
  MultiIndexContainerType::iterator it = m_Container.begin();

  while ( it != m_Container.end() )
    {
    it->ReleaseData();
    ++it;
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
LineageContainer::
createDumbLineageFromCurrentElement( double* iMother, double* iDaughter1, double* iDaughter2)
{
  // Create the triangle polydata
  //setup points (geometry)
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  points->InsertNextPoint ( iMother[0], iMother[1], iMother[2] );
  points->InsertNextPoint ( iDaughter1[0], iDaughter1[1], iDaughter1[2] );
  points->InsertNextPoint ( iDaughter2[0], iDaughter2[1], iDaughter2[2] );

  vtkSmartPointer<vtkCellArray> triangles = vtkSmartPointer<vtkCellArray>::New();

  //create a triangle on the three points in the polydata
  vtkSmartPointer<vtkTriangle> triangle = vtkSmartPointer<vtkTriangle>::New();

  //Unfortunately in this simple example the following lines are ambiguous.
  //The first 0 is the index of the triangle vertex which is ALWAYS 0-2.
  //The second 0 is the index into the point (geometry) array, so this can range from 0-(NumPoints-1)
  //i.e. a more general statement is triangle->GetPointIds()->SetId(0, PointId);
  triangle->GetPointIds()->SetId ( 0, 0 );
  triangle->GetPointIds()->SetId ( 1, 1 );
  triangle->GetPointIds()->SetId ( 2, 2 );

  //add the triangle to the list of triangles (in this case there is only 1)
  triangles->InsertNextCell ( triangle );

  //add the geometry and topology to the polydata
  this->m_CurrentElement.Nodes = vtkPolyData::New();
  this->m_CurrentElement.Nodes->SetPoints ( points );
  this->m_CurrentElement.Nodes->SetPolys ( triangles );

  // Add the actors
  if ( this->m_ImageView )
    {
    //Create new actors (new address)
    vtkProperty *trace_property = vtkProperty::New();
    double       r = this->m_CurrentElement.rgba[0];
    double       g = this->m_CurrentElement.rgba[1];
    double       b = this->m_CurrentElement.rgba[2];
    double       a = this->m_CurrentElement.rgba[3];

    trace_property->SetColor(r,
                             g,
                             b);
    trace_property->SetOpacity(a);

    // Add contour
    std::vector< vtkActor * > lineageActors =
      m_ImageView->AddContour(this->m_CurrentElement.Nodes, trace_property);

    this->m_CurrentElement.ActorXY = lineageActors[0];
    this->m_CurrentElement.ActorXZ = lineageActors[1];
    this->m_CurrentElement.ActorYZ = lineageActors[2];
    this->m_CurrentElement.ActorXYZ = lineageActors[3];
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
LineageContainer::
createLineage( unsigned int iLineageID,
               unsigned int iMoTrackID, double* iMotherFirst, double* iMotherLast,
               unsigned int iD1TrackID, double* iD1First, double* iD1Last,
               unsigned int iD2TrackID, double* iD2First, double* iD2Last)
{
/*
  // Create temp structure
  TrackStructure tempStructure(*iIterator);

  //add the point in the map
  bool pointDeleted = tempStructure.DeleteElement(iTime);

  // build the new polydata if a point has been deleted
  if ( pointDeleted && iReconstructPolyData )
    {
    UpdateTrackStructurePolyData(tempStructure);
    }

  // Replace
  m_Container.get< TraceID >().replace(iIterator, tempStructure);*/
/*
  LineageStructure motherStructure;

  // Create Mother structure
  motherStructure.m_LineageID = iLineageID;
  motherStructure.m_FirstPoint = iMotherFirst;
  motherStructure.m_LastPoint = iMotherLast;
  motherStructure.TrackID = iMoTrackID;
  motherStructure.Root = true;
  m_Container.insert(motherStructure);

  // Create Daughter structures
  LineageStructure daughter1Structure;
  daughter1Structure.m_LineageID = iLineageID;
  daughter1Structure.m_FirstPoint = iD1First;
  daughter1Structure.m_LastPoint = iD1Last;
  daughter1Structure.TrackID = iD1TrackID;
  daughter1Structure.Root = false;
  m_Container.insert(daughter1Structure);

  LineageStructure daughter2Structure;
  daughter2Structure.m_LineageID = iLineageID;
  daughter2Structure.m_FirstPoint = iD2First;
  daughter2Structure.m_LastPoint = iD2Last;
  daughter2Structure.TrackID = iD2TrackID;
  daughter2Structure.Root = false;
  m_Container.insert(daughter2Structure);
*/
  // insert elements (to lock "address")

  /*
  // Set up relationship
  // For mother
  SetRoot(Self* iRoot);
  SetMother(Self* iMother);
  SetDaughter1(Self* iDaughter1);
  SetDaughter2(Self* iDaughter2);

  // Create Data for root
  m_CurrentElement.ActorXY = NULL;
  m_CurrentElement.ActorXZ = NULL;
  m_CurrentElement.ActorYZ = NULL;
  m_CurrentElement.ActorXYZ = NULL;*/
}
//-------------------------------------------------------------------------

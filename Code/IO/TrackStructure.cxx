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

#include "TrackStructure.h"

#include <iostream>
#include "vtkPolyData.h"
#include "vtkActor.h"

#include "vtkDoubleArray.h"
#include "vtkPointData.h"

#include "vtkSphereSource.h"
#include "vtkGlyph3D.h"
#include "vtkTubeFilter.h"
#include "vtkAppendPolyData.h"
#include "vtkMath.h"

//--------------------------------------------------------------------------
TrackStructure::
TrackStructure():TraceStructure()
{
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
TrackStructure::
TrackStructure(const TrackStructure & iE):
  TraceStructure( iE ), PointsMap(iE.PointsMap)
{}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
TrackStructure::
~TrackStructure()
{
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
bool
TrackStructure::
InsertElement(const unsigned int& iTime, double* iPoint)
{
  // check if there is something at the iTime time point
  PointsMapIterator pointsMapIterator = this->PointsMap.find(iTime);

  // if there is no point, insert it and return true
  if ( pointsMapIterator == this->PointsMap.end() )
    {
    this->PointsMap.insert(
          std::pair< unsigned int, double* >( iTime, iPoint ) );
    return true;
    }

  // else do nothing and return false
  std::cout << "in: " << __FILE__ << " at line: " << __LINE__ << std::endl;
  std::cout << "can't insert a point at this time point" << std::endl;
  return false;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
bool
TrackStructure::
DeleteElement(const unsigned int& iTime)
{
  // check if there is something at the iTime time point
  PointsMapIterator pointsMapIterator = this->PointsMap.find(iTime);

  // if there is a point, delete it and return true
  if ( pointsMapIterator != this->PointsMap.end() )
    {
    // free memory
    delete[] pointsMapIterator->second;
    // clear map
    this->PointsMap.erase(pointsMapIterator);
    return true;
    }

  // else do nothing and return false
  std::cout << "in: " << __FILE__ << " at line: " << __LINE__ << std::endl;
  std::cout << "can't delete a point at this time point" << std::endl;
  return false;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
bool
TrackStructure::
ReplaceElement(const unsigned int& iTime, double* iPoint)
{
  // delete the existing element
  bool deleteElement = DeleteElement(iTime);

  // if sth has been deleted, insert the point and return true
  if(deleteElement)
    {
    return InsertElement(iTime,iPoint);
    }

  // else do nothing and return false
  std::cout << "in: " << __FILE__ << " at line: " << __LINE__ << std::endl;
  std::cout << "can't replace a point at this time point" << std::endl;
  return deleteElement;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
TrackStructure::ReleaseData() const
{
  TraceStructure::ReleaseData();

  PointsMapConstIterator begin = this->PointsMap.begin();
  PointsMapConstIterator end = this->PointsMap.end();

  while( begin != end )
    {
    delete[] begin->second;
    ++begin;
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
TrackStructure::
UpdateTracksRepresentation( bool iGlyph, bool iTube ) const
{
  vtkPolyData* glyph_pd = NULL;
  vtkSphereSource* sphere = NULL;
  vtkGlyph3D* glyph = NULL;

  if( iGlyph )
    {
    // Glyph shape
    sphere = vtkSphereSource::New();
    sphere->SetThetaResolution( 8 );
    sphere->SetPhiResolution( 8 );

    glyph = vtkGlyph3D::New();
    glyph->SetInput( this->Nodes );
    glyph->SetSource( sphere->GetOutput() );
    glyph->Update();

    glyph_pd = glyph->GetOutput();
    }

  vtkPolyData* tube_pd = NULL;
  vtkTubeFilter* tube = NULL;

  if( iTube )
    {
    tube = vtkTubeFilter::New();
    tube->SetNumberOfSides( 8 );
    tube->SetInput( this->Nodes );
    tube->SetRadius( .2  );
    tube->Update();

    tube_pd = tube->GetOutput();
    }
  else
    {
    tube_pd = this->Nodes;
    }

  vtkPolyData* temp = NULL;
  vtkAppendPolyData* apd = NULL;

  if( glyph_pd && tube_pd )
    {
    // append both polydata sets
    apd = vtkAppendPolyData::New();;
    apd->AddInput( glyph_pd );
    apd->AddInput( tube_pd );
    apd->Update();

    temp = apd->GetOutput();
    }
  else
    {
    if( glyph_pd )
      {
      temp = glyph_pd;
      }
    else
      {
      if( tube_pd )
        {
        temp = tube_pd;
        }
      else
        {
        temp = this->Nodes;
        }
      }
    }

  this->Nodes->DeepCopy( temp );

  if( sphere )
    {
    sphere->Delete();
    }
  if( glyph )
    {
    glyph->Delete();
    }
  if( tube )
    {
    tube->Delete();
    }
  if( apd )
    {
    apd->Delete();
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
GoFigureTrackAttributes
TrackStructure::
ComputeAttributes()
{
  GoFigureTrackAttributes attributes;

  attributes.total_length = 0.;
  attributes.distance = 0.;
  attributes.avg_speed = 0.;
  attributes.max_speed = 0.;
  unsigned int t0, t1;
  attributes.theta = 0.;
  attributes.phi = 0.;

  PointsMapConstIterator it = this->PointsMap.begin();
  unsigned int tmin = it->first;
  t0 = tmin;
  t1 = tmin;
  double* org = it->second;
  double* p = it->second;
  double* q = it->second; // if we only have one point in the map
  ++it;

  vtkDoubleArray* newArray =
      dynamic_cast<vtkDoubleArray*>(this->Nodes->GetPointData()->GetArray("SpeedInformation"));
  newArray->Reset();
  newArray->InsertNextValue(0.0);

  while( it != this->PointsMap.end() )
    {
    t1 = it->first;
    q = it->second;
    attributes.distance = sqrt( vtkMath::Distance2BetweenPoints( p, q ) );
    attributes.total_length += attributes.distance;
    attributes.max_speed = std::max( attributes.max_speed,
        attributes.distance / (static_cast< double >( t1 - t0 ) ) );

    double speed = attributes.distance / (static_cast< double >( t1 - t0 ) );
    newArray->InsertNextValue( speed );

    p = q;
    t0 = t1;
    ++it;
    }

  if (t1 == tmin)
    {
      attributes.avg_speed = 0;
    }
  else
    {
    attributes.avg_speed = attributes.total_length /
                           static_cast< double >( t1 - tmin );
    }

  attributes.distance = sqrt( vtkMath::Distance2BetweenPoints( org, q ) );

  if(attributes.distance)
    {
    attributes.theta = vtkMath::DegreesFromRadians( atan2( ( q[1] - org[1] ),
                                                           ( q[0] - org[0] ) ) );
    attributes.phi   = vtkMath::DegreesFromRadians( acos( ( q[2] - org[2] ) /
                                                         attributes.distance ) );
    }

  return attributes;
}

//--------------------------------------------------------------------------

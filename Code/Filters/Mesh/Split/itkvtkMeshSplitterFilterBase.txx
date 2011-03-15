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

#ifndef __itkvtkMeshSplitterFilterBase_txx
#define __itkvtkMeshSplitterFilterBase_txx

#include "itkvtkMeshSplitterFilterBase.h"

namespace itk
{
template< class TFeatureImage >
vtkMeshSplitterFilterBase< TFeatureImage >::
vtkMeshSplitterFilterBase() : Superclass(), m_Mesh( NULL ), m_Outputs( 0 )
  {
  for( unsigned int dim = 0; dim < 3; ++dim )
    {
    m_Bounds[2 * dim] = NumericTraits< double >::max();
    m_Bounds[2 * dim + 1] = NumericTraits< double >::min();
    }
  }

template< class TFeatureImage >
void
vtkMeshSplitterFilterBase< TFeatureImage >::
SetMesh( vtkPolyData* iMesh )
{
  if( ( iMesh ) && ( iMesh != m_Mesh ) )
    {
    m_Mesh = iMesh;
    m_Mesh->GetBounds( m_Bounds );
    this->Modified();
    }
}

template< class TFeatureImage >
std::vector< vtkPolyData* >
vtkMeshSplitterFilterBase< TFeatureImage >::
GetOutputs()
{
  return m_Outputs;
}

template< class TFeatureImage >
void
vtkMeshSplitterFilterBase< TFeatureImage >::
GenerateData()
{
  if( !m_Mesh )
    {
    itkGenericExceptionMacro( << "m_Mesh is NULL" );
    }

  this->Split();
}

}
#endif

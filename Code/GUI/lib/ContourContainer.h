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

#ifndef __ContourContainer_h
#define __ContourContainer_h

#include "ContourMeshContainer.h"
#include "QGoGUILibConfigure.h"

/**
 * \class ContourContainer
 * \brief Wraps a boost::multi_index_container of ContourMeshStructure.
 * This class is specialized for the means of 2D Contour.
 * */
class QGOGUILIB_EXPORT ContourContainer : public ContourMeshContainer
{
  Q_OBJECT

public:
  /** \brief Constructor */
  explicit ContourContainer( QObject* iParent, QGoImageView3D *iView);

  /** \brief Destructor */
  ~ContourContainer();

protected:

  /** \brief Add a contour in the visualization given the spline nodes and the
   * property for the given contour. It returns a vector of size 4, of each
   * element represents one view in the Quad-View.  */
  std::vector< vtkActor* > AddTrace( vtkPolyData* iNode, vtkProperty* iProperty );

private:

  Q_DISABLE_COPY(ContourContainer);

};

#endif // CONTOURCONTAINER_H
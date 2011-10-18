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
#ifndef __QGoFilterShape_h
#define __QGoFilterShape_h

#include "QGoFilterSemiAutoBase.h"

#include "QGoGUILibConfigure.h"

class GoImageProcessor;
class vtkPolyData;

/**
 * \class QGoFilterShape
 * \brief Shape segmentation algorithm implementation.
 * Can generate circles, spheres, squares and cubes.
  * Will generate 2D objects if m_Dimension<2, 3D objects in the other case.
 */

class QGOGUILIB_EXPORT QGoFilterShape:public QGoFilterSemiAutoBase
{
  Q_OBJECT
public:
  /** \brief Constructor */
  explicit QGoFilterShape(QObject *iParent = NULL, int iDimension = 2);

  /** \brief Destructor */
  ~QGoFilterShape();

  virtual vtkPolyData * Apply();

  virtual void ConnectSignals(int iFilterNumber);

  //integration algo
  /**
  \brief return the polydatas corresponding to the new created Shapes
  */
  std::vector<vtkPolyData *> ApplyFilter3D( double iRadius,
    std::vector< vtkPoints* >* iPoints, std::string iShape,
    GoImageProcessor* iImages,
    int iChannel);

  std::vector<std::vector<vtkPolyData*> > ApplyFilterSetOf2D(double iRadius,
    std::string iShape, int iSampling,
    std::vector< vtkPoints* >* iPoints,
    GoImageProcessor* iImages, int iChannel);

public slots:

  void setShape(int);

protected:

  int m_Shape;

  vtkPolyData * GenerateSphere(double *iCenter, double iRadius,
    vtkSmartPointer< vtkImageData > iImage);

  vtkPolyData * GenerateCube(double *iCenter, double iRadius,
    vtkSmartPointer< vtkImageData > iImage);

  vtkPolyData * GenerateCylinder(double *iCenter);

private:
  Q_DISABLE_COPY(QGoFilterShape);
};
#endif
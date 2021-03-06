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
#ifndef __QGoSemiAutoSegmentationAlgo_h
#define __QGoSemiAutoSegmentationAlgo_h

#include "QGoSegmentationAlgo.h"
#include "QGoAlgorithmWidget.h"
#include "QGoGUILibConfigure.h"
#include "vtkSmartPointer.h"
#include "vtkPolyData.h"
#include "vtkImageData.h"
#include "QGoGUILibConfigure.h"

class GoImageProcessor;

/**
\class QGoSemiAutoSegmentationAlgo
\brief abstract class to be the interface between the semi automatic
algorithms for meshes and contours and GoFigure
*/
class QGOGUILIB_EXPORT QGoSemiAutoSegmentationAlgo:public QGoSegmentationAlgo
{
  Q_OBJECT
public:
  explicit QGoSemiAutoSegmentationAlgo(std::vector< vtkPoints* >* iSeeds, QWidget *iParent = 0);
  virtual ~QGoSemiAutoSegmentationAlgo();


  /**
  \brief return the vtkpolydata created by the algorithm
  */
  virtual std::vector<vtkPolyData*> ApplyAlgo(
    GoImageProcessor* iImages,
    std::string iChannel,
    bool iIsInvertedOn = false) = 0;

protected:
  std::vector< vtkPoints* >*      m_Seeds;
  QGoAlgoParameter<double>*       m_Radius;

  /**
  \brief construct the algowidget with the different parameters
  */
  virtual void SetAlgoWidget(QWidget* iParent = 0);


  /**
  \brief delete the different parameters
  */
  virtual void DeleteParameters() = 0;

  /*
   * \brief Get boundingBox from a center and a radius
   * \param[in] iCenter center of the box
   * \param[in] iRadius radius of the box
   * \param[in] iOrientation 0-xy, 1-xz, 2-yz, 3-xyz
   * \return vector[6] containing the bounding box (xmin, xmax, ymin, imax, ...)
   */
  std::vector<double> GetBounds(
      const std::vector<double>& iCenter,
      const double& iRadius,
      const unsigned int& iOrientation = 3);

};

#endif

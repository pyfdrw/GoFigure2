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
#ifndef __QGoWaterShedAlgo_h
#define __QGoWaterShedAlgo_h

#include "QGoSemiAutoSegmentationAlgo.h"
#include "QGoAlgorithmWidget.h"
#include "QGoAlgoParameter.h"
#include "QGoGUILibConfigure.h"
#include "vtkSmartPointer.h"
#include "vtkPolyData.h"
#include "vtkImageData.h"

#include "GoImageProcessor.h"


/**
\class QGoWaterShedAlgo
\brief class to be the interface between the watershed algo for meshes
and GoFigure
*/
class QGoWaterShedAlgo: public QGoSemiAutoSegmentationAlgo
{
public:
  QGoWaterShedAlgo(std::vector< vtkPoints* >* iSeeds,
                   int iMaxThreshold = 0,
                   QWidget* iParent = 0);
  ~QGoWaterShedAlgo();

  virtual std::vector<vtkPolyData*> ApplyAlgo(
    GoImageProcessor* iImages,
    std::string iChannel,
    bool iIsInvertedOn = false) = 0;

protected:

  QGoAlgoParameter<int>*          m_ThresMin;
  QGoAlgoParameter<int>*          m_ThresMax;
  QGoAlgoParameter<double>*       m_CorrThres;
  QGoAlgoParameter<double>*       m_Alpha;
  QGoAlgoParameter<double>*       m_Beta;

  int                             m_MaxThreshold;

  virtual void SetAlgoWidget(QWidget* iParent);

  void DeleteParameters();
};

#endif

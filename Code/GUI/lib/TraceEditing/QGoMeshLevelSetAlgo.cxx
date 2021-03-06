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
#include "QGoMeshLevelSetAlgo.h"
#include "QGoFilterChanAndVese.h"


QGoMeshLevelSetAlgo::
QGoMeshLevelSetAlgo(std::vector< vtkPoints* >* iSeeds, QWidget* iParent)
  :QGoLevelSetAlgo(iSeeds, iParent)
{
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QGoMeshLevelSetAlgo::~QGoMeshLevelSetAlgo()
{
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
std::vector<vtkPolyData*> QGoMeshLevelSetAlgo::ApplyAlgo(
  GoImageProcessor* iImages,
    std::string iChannel, 
    bool iIsInvertedOn)
{
  std::vector<vtkPolyData*> oNewMeshes = std::vector<vtkPolyData*>();

  if ( this->m_Radius->GetValue() <= 0 )
    {
    std::cerr << "Radius should be > 0 " << std::endl;
    return oNewMeshes;
    }

  double Center[3];
  std::vector<double> CenterVect(3);

  // LOOP  FOR EACH SEED
  for( size_t id = 0; id < this->m_Seeds->size(); id++ )
    {
    for ( int i = 0; i < (*this->m_Seeds)[id]->GetNumberOfPoints(); i++ )
      {
      (*this->m_Seeds)[id]->GetPoint(i, Center);

      CenterVect[0] = Center[0];
      CenterVect[1] = Center[1];
      CenterVect[2] = Center[2];

      vtkPolyData* temp_output =
          this->ApplyLevelSetFilter< unsigned char >(
            CenterVect,
            iImages->getImageITK< unsigned char, 3>(iChannel)); //input raw image

      if(temp_output->GetNumberOfCells() > 0)
        {
        oNewMeshes.push_back( temp_output );
        }
      /*else
        {
        std::cout << "No polydata could be generated - check parameters"
                  << std::endl;
        temp_output->Delete();
        }*/
      }
    }

  return oNewMeshes;
}
//-------------------------------------------------------------------------

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

#ifndef __vtkPolyDataMySQLMeshReader_h
#define __vtkPolyDataMySQLMeshReader_h

#include "vtkObject.h"
#include <string>

#include "vtkSmartPointer.h"
class vtkPolyData;

#include "QGoIOConfigure.h"

/**
\defgroup MySQLReader MySQLReader
\defgroup Contour Contour
\defgroup Mesh Mesh
\defgroup Trace Trace
*/

/**
\class vtkPolyDataMySQLMeshReader
\brief Reads a string and convert it into a mesh polydata
\ingroup MySQLReader Mesh Trace
*/

class QGOIO_EXPORT vtkPolyDataMySQLMeshReader:public vtkObject
{
public:
  /*
   * \brief Public constructor
   */
  static vtkPolyDataMySQLMeshReader * New();

  vtkTypeRevisionMacro(vtkPolyDataMySQLMeshReader, vtkObject);

  /*
   * \brief Generate a mesh from a string
   * \param[in] iString base string to generate the polydata
   * \return pointer to the generated contour/mesh
   */
  vtkSmartPointer<vtkPolyData> GetPolyData(const std::string & iString);

protected:
  vtkPolyDataMySQLMeshReader();
  ~vtkPolyDataMySQLMeshReader();

private:
  vtkPolyDataMySQLMeshReader(const vtkPolyDataMySQLMeshReader &);
  void operator=(const vtkPolyDataMySQLMeshReader &);
};

#endif

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

#ifndef __GoFigureTrackAttributes_h
#define __GoFigureTrackAttributes_h

#include <map>
#include <string>
#include "QGoIOConfigure.h"

/*
\struct GoFigureTrackAttributes
\brief Track attributes to be displayed in the table widget
*/

struct QGOIO_EXPORT GoFigureTrackAttributes {
  // total distance (add each segment size) = deplacement
  double total_length;
  // average speed
  double avg_speed;
  // maximum speed
  double max_speed;
  // euclidian distance between first and last points
  double distance;
  // theta in spherical coordinate system
  double theta;
  // phi in spherical coordinate system
  double phi;

  // Constructors
  GoFigureTrackAttributes(){}
  GoFigureTrackAttributes(const GoFigureTrackAttributes & iE):
  total_length(iE.total_length), avg_speed(iE.avg_speed),
  max_speed(iE.max_speed), distance(iE.distance),
  theta(iE.theta), phi(iE.phi){}
};
#endif
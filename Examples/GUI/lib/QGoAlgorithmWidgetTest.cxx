/*=========================================================================
 Authors: The GoFigure Dev. Team.
 at Megason Lab, Systems biology, Harvard Medical school, 2009

 Copyright (c) 2009, President and Fellows of Harvard College.
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
#include <QApplication>
#include <QTimer>
#include <QStringList>
#include "QGoAlgorithmWidget.h"




//**************************************************************************//
//                               MAIN                                       //
//**************************************************************************//

int main(int argc, char *argv[])
{
  if ( argc != 1 )
    {
    return EXIT_FAILURE;
    }

  QApplication app(argc, argv);
  QTimer *     timer = new QTimer;
  timer->setSingleShot(true);

  QGoAlgorithmWidget* AlgoWidget = new QGoAlgorithmWidget("Test", NULL);
  QStringList ChannelName;
  ChannelName.append("Channel 1");
  ChannelName.append("Channel 2");
  ChannelName.append("All Channels");
  AlgoWidget->AddParameter("Channel", ChannelName);
  AlgoWidget->AddParameter("IntParam", 0, 100, 50);
  AlgoWidget->AddParameter("DoubleParam", 20.56, 53.21, 24, 2);
  AlgoWidget->AddAdvParameter("IntParam", 20, 50, 40);
  AlgoWidget->AddAdvParameter("DoubleParam", 11, 23.00, 15, 3);


  //QObject::connect( timer, SIGNAL( timeout() ), AlgoWidget, SLOT( close() ) );

  AlgoWidget->show();
  timer->start(1000);


  app.processEvents();
  int output = app.exec();

  app.closeAllWindows();

  delete timer;
  delete AlgoWidget;

  return output;
}

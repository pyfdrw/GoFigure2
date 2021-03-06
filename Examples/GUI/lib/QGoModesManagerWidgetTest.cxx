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

#include "QGoModesManagerWidget.h"
#include "QGoAlgorithmWidget.h"
#include "QGoAlgoParameter.h"




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
  //QTimer *     timer = new QTimer;
  //timer->setSingleShot(true);

  QStringList ChannelName;
    ChannelName.append("Channel 1");
    ChannelName.append("Channel 2");
  QStringList ListTimePoint;
  ListTimePoint.append("t-1");
  ListTimePoint.append("t");
  ListTimePoint.append("t+1");

  QGoModesManagerWidget* MeshMode = new QGoModesManagerWidget(ChannelName, ListTimePoint, NULL);
  
  
  //QGoAlgorithmsManagerWidget* SemiAutoModeMeshEditingAlgoWidget = new QGoAlgorithmsManagerWidget(
  //  "Semi Automated",ChannelName, ListTimePoint, NULL);
  QGoAlgorithmWidget* LevelSetWidget = new QGoAlgorithmWidget("LevelSet", MeshMode);
    //SemiAutoModeMeshEditingAlgoWidget);
  
  QGoAlgoParameter<double> Radius ("Radius", false, 0, 10, 2, 3);
  LevelSetWidget->AddParameter<double>(&Radius);
  QGoAlgoParameter<int> Curvature ("Curvature", true, 0, 100, 20);
  LevelSetWidget->AddParameter<int>(&Curvature);
  QGoAlgoParameter<int> Iteration ("Iterations", true, 0, 1000, 100);
  LevelSetWidget->AddParameter<int>(&Iteration);
  
  //LevelSetWidget->AddParameter("Channel", ChannelName);
  //LevelSetWidget->AddParameter("Radius", 0, 10, 3);
  //LevelSetWidget->AddAdvParameter("Curvature", 0, 100, 20);
  //LevelSetWidget->AddAdvParameter("Iterations", 0, 1000, 100);

  QGoAlgorithmWidget* Shape3D = new QGoAlgorithmWidget("Shape 3D", MeshMode);//SemiAutoModeMeshEditingAlgoWidget);
  //Shape3D->AddParameter("Channel", ChannelName);
  //Shape3D->AddParameter("Radius", 0, 10, 3);
  Shape3D->AddParameter<double>(&Radius);
  QStringList ShapeList;
  ShapeList.append("Sphere");
  ShapeList.append("Cube");
  QGoAlgoParameter<std::string> Shape("Shape",true, ShapeList, "Sphere");
  //Shape3D->AddAdvParameter("Shape", ShapeList);
  Shape3D->AddParameter<std::string>(&Shape);
  
  QGoAlgorithmWidget* WaterShedWidget = new QGoAlgorithmWidget("WaterShed", MeshMode);//SemiAutoModeMeshEditingAlgoWidget);
  //WaterShedWidget->AddParameter("Channel", ChannelName);
  //WaterShedWidget->AddParameter("Radius", 0, 10, 3);
  WaterShedWidget->AddParameter<double>(&Radius);
  QGoAlgoParameter<int> ThresMin ("Thres.Min.", true, 0, 100, 20);
  WaterShedWidget->AddParameter<int>(&ThresMin);
  //WaterShedWidget->AddAdvParameter ("Thres.Min.", 0, 100, 20);
  QGoAlgoParameter<int> ThresMax ("Thres.Max.", true, 0, 30, 50);
  WaterShedWidget->AddParameter<int>(&ThresMax);
 // WaterShedWidget->AddAdvParameter("Thres.Min.", 0, 30, 50);
  QGoAlgoParameter<double> CorrThres ("Corr.Thres.", true, 0, 5, 2, 2);
  WaterShedWidget->AddParameter<double>(&CorrThres);
  //WaterShedWidget->AddAdvParameter("Corr.Thres.", 0, 5, 2, 2);
  QGoAlgoParameter<double> Alpha ("Alpha", true, 0, 5, 1.50, 2);
  WaterShedWidget->AddParameter<double>(&Alpha);
  //WaterShedWidget->AddAdvParameter("Alpha", 0, 5, 1.50, 2);
  QGoAlgoParameter<double> Beta ("Beta", true, 0, 5, 3, 1);
  WaterShedWidget->AddParameter<double>(&Beta);
  //WaterShedWidget->AddAdvParameter("Beta", 0, 5, 3, 1);

  //SemiAutoModeMeshEditingAlgoWidget->AddMethod(LevelSetWidget);
  //SemiAutoModeMeshEditingAlgoWidget->AddMethod(WaterShedWidget);
  //SemiAutoModeMeshEditingAlgoWidget->AddMethod(Shape3D);
  MeshMode->AddAlgoWidgetForSemiAutomatedMode(LevelSetWidget);
  MeshMode->AddAlgoWidgetForSemiAutomatedMode(WaterShedWidget);
  MeshMode->AddAlgoWidgetForSemiAutomatedMode(Shape3D);

  MeshMode->CheckDefaultModes();
  
  
  //MeshMode->AddAlgoManagerWidget(SemiAutoModeMeshEditingAlgoWidget);
  //MeshMode->AddWidgetWithModeName("Test", SemiAutoModeMeshEditingAlgoWidget);
  //SemiAutomatedMode->AddMethod(WaterShedWidget);
  //SemiAutomatedMode->AddMethod(Shape3D);
  //QObject::connect( timer, SIGNAL( timeout() ), SemiAutoModeMeshEditingAlgoWidget, SLOT( close() ) );

  
  //timer->start(1000);

  MeshMode->show();

  app.processEvents();
  int output = app.exec();

  app.closeAllWindows();

  //delete timer;
 // delete SemiAutoModeMeshEditingAlgoWidget;
  delete MeshMode;

  return output;
}

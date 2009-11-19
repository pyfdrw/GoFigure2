/*=========================================================================
  Author: $Author: lsouhait $  // Author of last commit
  Version: $Revision: 549 $  // Revision of last commit
  Date: $Date: 2009-08-11 10:14:35 -0400 (Tue, 11 Aug 2009) $  // Date of last commit
=========================================================================*/

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
#ifndef __GoDBCollectionOfTraces_h
#define __GoDBCollectionOfTraces_h

#include <QString>
#include <QStringList>
#include <QObject>
#include "MegaVTK2Configure.h"
#include "GoDBRecordSetHelper.h"
#include "vtkMySQLDatabase.h"
#include "GoDBCoordinateRow.h"
#include "GoDBTraceInfoForTableWidget.h"
#include <QStringList>

class GoDBCollectionOfTraces
{

public:

  explicit QMEGAVTKADDON2_EXPORT GoDBCollectionOfTraces();
  explicit QMEGAVTKADDON2_EXPORT GoDBCollectionOfTraces(
    std::string CollectionName,std::string Traces);
  virtual  QMEGAVTKADDON2_EXPORT ~GoDBCollectionOfTraces();
 
  //void QMEGAVTKADDON2_EXPORT SetDatabaseVariables(
    //QString Server,QString User,QString Password, QString NameDB);
  void QMEGAVTKADDON2_EXPORT SetImgSessionID (unsigned int iImgSessionID);

  /** \brief Delete in the Database all the traces listed in the QStringList */
  void QMEGAVTKADDON2_EXPORT DeleteTraces(QStringList TracesToDelete,
  vtkMySQLDatabase* DatabaseConnector);

  /** \brief Update the collectionID of the selected traces in the DB traces table
  with the new collectionID: */
  void QMEGAVTKADDON2_EXPORT AddSelectedTracesToCollection(
    QStringList ListSelectedTraces,int newCollectionID,
    vtkMySQLDatabase* DatabaseConnector);
  
  /** \brief Fill a vector of GoDBTraceInfoForTableWidget with the info
  needed to fill the table widget for all the traces*/ 
  std::vector<GoDBTraceInfoForTableWidget> GetCommonColumnsInfoForTraceTable();

  template< class myT >
  void QMEGAVTKADDON2_EXPORT CreateNewCollectionFromSelection(
    QStringList ListSelectedTraces, vtkMySQLDatabase* DatabaseConnector,
    myT& NewObject)
    {
    //the following fields are common to all the collections, the one
    //that are different are specified in QGoPrintDatabase:
    NewObject.SetField("ImagingSessionID",this->m_ImgSessionID);
      //the CollectionID is set to 0 as it is a new Collection that will be created, not
       //contours to be added to an existing collection:
    NewObject.SetField("CoordIDMax",this->GetCoordMaxID(
      DatabaseConnector,0,ListSelectedTraces));
    NewObject.SetField("CoordIDMin",this->GetCoordMinID(
      DatabaseConnector,0,ListSelectedTraces));

    int NewCollectionID = this->CreateNewCollection<myT>(DatabaseConnector,NewObject);
    AddSelectedTracesToCollection(ListSelectedTraces,
      NewCollectionID, DatabaseConnector);
    }


  QStringList QMEGAVTKADDON2_EXPORT ListCollectionID(
    vtkMySQLDatabase* DatabaseConnector);

  std::string CollectionName()
    { return m_CollectionName;}
  std::string TracesName()
    { return m_TracesName;}



protected:

  std::string  m_CollectionName;
  std::string  m_CollectionIDName;
  std::string  m_TracesName;
  std::string  m_TracesIDName;
  unsigned int m_ImgSessionID;

  /** \brief Create a new collection Row in the collection table and
  return the collectionID from the created row: */
  int  CreateNewCollection();

  template< class myT >
  int CreateNewCollection(vtkMySQLDatabase* DatabaseConnector, myT& myNewObject)
    {
    return AddOnlyOneNewObjectInTable<myT>(
      DatabaseConnector,m_CollectionName,myNewObject, m_CollectionIDName);
    }

  /** \brief compare all the coordinate for all the traces inside the collection,
  create the coordinate in the database with all the max and return it*/
  int GetCoordMaxID(vtkMySQLDatabase* DatabaseConnector,
    int CollectionID,QStringList ListSelectedTraces);

  /** \brief compare all the coordinate for all the traces inside the collection,
  create the coordinate in the database with all the min and return it*/
  int GetCoordMinID(vtkMySQLDatabase* DatabaseConnector,
    int CollectionID,QStringList ListSelectedTraces);

  /** \brief return the coordinate min of all the coordinates of the 
  selected traces*/
  GoDBCoordinateRow GetSelectingTracesCoordMin(
  vtkMySQLDatabase* DatabaseConnector, std::vector<std::string> ListSelectedTracesID);

  /** \brief return the coordinate max of all the coordinates of the 
  selected traces*/
  GoDBCoordinateRow GetSelectingTracesCoordMax(
  vtkMySQLDatabase* DatabaseConnector, std::vector<std::string> ListSelectedTracesID);
  
  /** \brief return the coordinate min for the existing Collection*/
  GoDBCoordinateRow GetExistingCoordMin(
  vtkMySQLDatabase* DatabaseConnector, int CollectionCoordIDMin,
  int CollectionID );

   /** \brief return the coordinate max for the existing Collection*/
  GoDBCoordinateRow GetExistingCoordMax(
  vtkMySQLDatabase* DatabaseConnector, int CollectionCoordIDMax,
  int CollectionID );

  /** \brief Fill the vector of GoDBTraceInfoForTableWidget with the info
  common to 2 traces only*/  
  void GetSpecificInfoForTraceTable(
    std::vector<GoDBTraceInfoForTableWidget> &ioSpecificInfos);
  
};
#endif

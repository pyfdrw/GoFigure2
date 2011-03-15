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

#ifndef __QGoDBLineageManager_h
#define __QGoDBLineageManager_h

#include "QGoTableWidget.h"
#include "GoDBCollectionOfTraces.h"
#include "GoDBTWContainerForLineage.h"
#include "QGoDBTraceManager.h"
#include "QGoGUILibConfigure.h"
#include "LineageContainer.h"

/**
\class QGoDBLineageManager
\brief This class manages the database queries, the table widget and
the data from the database in the Container for visu for the lineages
\ingroup DB, GUI
*/
class QGOGUILIB_EXPORT QGoDBLineageManager:public QGoDBTraceManager
{
  Q_OBJECT
public:
  QGoDBLineageManager(int iImgSessionID, QWidget *iparent);
  ~QGoDBLineageManager();

  virtual void DisplayInfoForLastCreatedTrace(vtkMySQLDatabase *iDatabaseConnector);

  virtual void DisplayInfoForExistingTrace(vtkMySQLDatabase *iDatabaseConnector,
                                           int iTraceID);

  /**
  \brief set the m_LineageContainerInfoForVisu to the iContainerForVisu
  \param[in] iContainerForVisu common container for the visu and database
  */
  void SetLineagesInfoContainerForVisu(LineageContainer *iContainerForVisu);

  /**
  \brief get all the data from the database to load all the lineages for the imagingsession
  into the table widget and the container for the visu
  \param[in] iDatabaseConnector connection to the database
  */
  void DisplayInfoAndLoadVisuContainerForAllLineages(
    vtkMySQLDatabase *iDatabaseConnector);

  /**
  \brief display in the TW the values extracted from iTrackAttributes
  * called when loading all tracks from database when opening an imagingsession
  \param[in] iTrackAttributes computed values for a track
  \param[in] iTrackID trackID of the track we want to display the values
  */
  //void DisplayOnlyCalculatedValuesForExistingTrack(
  //  GoFigureTrackAttributes *iTrackAttributes, unsigned iTrackID);

  /**
  \brief create a new lineage with IDRoot in the database, add it in the
  TW and in the visu container
  \param[in] iDatabaseConnector connection to the database
  \param[in] iTrackRoot ID of the track that is the root of the new lineage to create
  \return the ID of the new lineage just created
  */
  unsigned int CreateNewLineageWithTrackRoot(
      vtkMySQLDatabase *iDatabaseConnector, unsigned int iTrackRoot);

  //virtual pure method in QGoDBTraceManager
  std::list< unsigned int > UpdateTheTracesColor(vtkMySQLDatabase *iDatabaseConnector);

  //virtual pure method in QGoDBTraceManager
  virtual void UpdateTWAndContainerForImportedTraces(std::vector< int > iVectorImportedTraces,
                                                     vtkMySQLDatabase *iDatabaseConnector);
  
  /**
  \brief delete the traces of the list from the database, the TW and the
  container for visu
  \param[in] iDatabaseConnector connection to the database
  \param[in] iListTraces list of the tracesIDs to be deleted
  */
  void DeleteListTraces(vtkMySQLDatabase *iDatabaseConnector,
    std::list<unsigned int> iListTraces);

  //virtual pure method in QGoDBTraceManager
  virtual void DeleteCheckedTraces( vtkMySQLDatabase *iDatabaseConnector);

   //virtual pure method in QGoDBTraceManager
  virtual std::list< unsigned int > GetListHighlightedIDs();

  /**
  \brief get the data from the TrackContainer corresponding to the user selected TrackID
  and put them in the current element of the track container
  */
  //void UpdateCurrentElementTrackContainer();

  /**
  \brief update the points of the imported track in current_element
  with the info from the meshes and save them in the database
  */
  //void UpdatePointsOfCurrentElementForImportedTrack(
	//  std::map<unsigned int,double*> iMeshesInfo, vtkMySQLDatabase* iDatabaseConnector);

  //method in QGoDBTraceManager
  //void UpdateBoundingBoxes(
  //vtkMySQLDatabase *iDatabaseConnector,std::list< unsigned int > iListTracesIDs);

public slots:

  /**
  \brief update the trackID root for the lineage with iTrackIDRoot
  */
  void UpdateTrackRootSelectedLineage(unsigned int iLineageID, 
    unsigned int iTrackIDRoot);

  //void CreateBasicLineageInVisuFromCurrentElement(
  //  double* iMotherTrackPoint, double* iDaughterOneTrackPoint, 
  //  double* iDaughterTwoTrackPoint);
  /**
  \brief create a new division in visu and update the ID root of the last created lineage
  with the iTrackIDRoot
  */
  /*void CreateNewDivisionInVisuForNewLineage(
    unsigned int iTrackIDRoot,
    double* iMotherTrackPoint, 
    unsigned int iDaughterOneID,
    double* iDaughterOneTrackPoint,
    unsigned int iDaughterTwoID,
    double* iDaughterTwoTrackPoint);*/

  /**
  \brief create a new division in visu.
  */
  /*void UpdateExistingLineageWithNewDivision(
    unsigned int iLineageID,
    unsigned int iMotherTrackID,
    double* iMotherTrackPoint, 
    unsigned int iDaughterOneID,
    double* iDaughterOneTrackPoint,
    unsigned int iDaughterTwoID,
    double* iDaughterTwoTrackPoint);*/

signals:
  //void NeedMeshesInfoForImportedTrack(unsigned int iTrackID);
  //void TrackToSplit(unsigned int iTrackID, std::list<unsigned int> iListMeshIDs);
  //void TrackIDToBeModifiedWithWidget(std::list<unsigned int> iListTracksID);
  //void MeshesToAddToTrack(std::list<unsigned int> iListMeshes, unsigned int iTrackID);

protected:
  GoDBTWContainerForLineage      *m_TWContainer;
  LineageContainer               *m_LineageContainerInfoForVisu;

  //virtual pure method in QGoDBTraceManager
  virtual void SetCollectionsTraceNames();

  //virtual pure method in QGoDBTraceManager
  virtual void DisplayInfoForAllTraces(vtkMySQLDatabase *iDatabaseConnector);

  //virtual pure method in QGoDBTraceManager
  virtual void GetTracesInfoFromDBAndModifyContainerForVisu(
    vtkMySQLDatabase* iDatabaseConnector,
    std::list<unsigned int> iListTraceIDs = std::list< unsigned int >());

  /**
  \brief get the center of bounding boxes from the database for the meshes
  belonging to the iTrackID, update the polydata in the container for visu and save it in
  the database
  \param[in] iDatabaseConnector connection to the database
  \param[in] iTrackID ID for the track the polydata needs to be recalculated
  */
  //void UpdateTrackPolydataForVisu(vtkMySQLDatabase *iDatabaseConnector,unsigned int iTrackID);

  //virtual in QGoDBTraceManager
  //void AddActionsContextMenu(QMenu *iMenu);

protected slots:
  //virtual pure method in QGoDBTraceManager
  virtual void UpdateHighlightedElementsInVisuContainer(int iTraceID);

  //virtual pure method in QGoDBTraceManager
  virtual void UpdateVisibleElementsInVisuContainer(int iTraceID);

  /**
  \brief create or update the track contained in the current element of the
  track container into the database, the table widget and insert the current
  element into the track container
  \param[in] iDatabaseConnector connection to the database
  */
  //void SaveTrackCurrentElement(vtkMySQLDatabase* iDatabaseConnector);

 //virtual pure method in QGoDBTraceManager
  virtual void SetColorCoding(bool IsChecked);

  /**
  \brief get the trackIDs checked in the TW that will be modified with the
  widget and emit a signal with them as info on meshes are needed.
  */
  //void SplitMergeTrackWithWidget();

  /**
  \brief check that there is only one checked track in the TW and emit a
  signal with it as the checked mesh is needed also to split the track
  */
  //void TrackIDToEmit();

  /**
  \brief check that only 2 tracks are checked in the TW, if not display 
  a message to the user, check that the 2 tracks are not overlapping, 
  if yes, display a message to the user, get the meshes of the track with 
  the lowest timepoints, delete this track and send a signal for the list of 
  meshes of the previous track to be reassigned to the track with the 
  highest timepoints
  */
  //void MergeTracks();

  /**
  \brief check that the 2 tracks are not overloaping, if not, return the
  trackID to keep for the merge and the one to delete
  \param[in] iTrackIDs IDs of both tracks to check
  \param[in,out] ioTraceIDToKeep ID of the trace to keep after the merge
  \param[in,out] ioTraceIDToDelete ID of the trace to delete after the merge
  \param[in] iDatabaseConnector connection to the database
  \return false of the tracks are not overlapping, true if they are
  */
  //bool CheckOverlappingTracks(std::list<unsigned int> iTrackIDs,
   // unsigned int &ioTraceIDToKeep, unsigned int &ioTraceIDToDelete,
   // vtkMySQLDatabase* iDatabaseConnector);

};
#endif

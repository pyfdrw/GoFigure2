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

#ifndef __QGoDBTrackManager_h
#define __QGoDBTrackManager_h

#include "QGoTableWidget.h"
#include "GoDBCollectionOfTraces.h"
#include "GoDBTWContainerForTrack.h"
#include "QGoDBTraceManager.h"
#include "QGoGUILibConfigure.h"
#include "TrackContainer.h"
#include "GoDBTrackFamilyRow.h"

class TrackStructure;

/**
\class QGoDBTrackManager
\brief This class manages the database queries, the table widget and
the data from the database in the Container for visu for the tracks
\ingroup DB, GUI
*/
class QGOGUILIB_EXPORT QGoDBTrackManager:public QGoDBTraceManager
{
  Q_OBJECT
public:
  QGoDBTrackManager(int iImgSessionID, QWidget *iparent);
  ~QGoDBTrackManager();

  virtual void DisplayInfoForLastCreatedTrace(vtkMySQLDatabase *iDatabaseConnector);

  virtual void DisplayInfoForExistingTrace(vtkMySQLDatabase *iDatabaseConnector,
                                           int iTraceID);

  /**
  \brief set the m_TrackContainerInfoForVisu to the iContainerForVisu
  \param[in] iContainerForVisu common container for the visu and database
  */
  void SetTracksInfoContainerForVisu(TrackContainer *iContainerForVisu);

  /**
  \brief get all the data from the database to load all the tracks for the imagingsession
  into the table widget and the container for the visu
  \param[in] iDatabaseConnector connection to the database
  */
  void DisplayInfoAndLoadVisuContainerForAllTracks(
    vtkMySQLDatabase *iDatabaseConnector);

  /**
  \brief get all the data from the database of the track family table and give the corresponding
  info to the track container for visu
  \param[in] iDatabaseConnector connection to the database
  */
  void LoadInfoVisuContainerForTrackFamilies(vtkMySQLDatabase *iDatabaseConnector);

  /**
  \brief display in the TW the values extracted from iTrackAttributes
  * called when loading all tracks from database when opening an imagingsession
  \param[in] iTrackAttributes computed values for a track
  \param[in] iTrackID trackID of the track we want to display the values
  */
  void DisplayOnlyCalculatedValuesForExistingTrack(
    GoFigureTrackAttributes *iTrackAttributes, unsigned int iTrackID);

  /**
  \brief create a new track with no mesh and no points in the database, add it in the
  TW and in the visu container
  \param[in] iDatabaseConnector connection to the database
  \return the ID of the new track just created
  */
  unsigned int CreateNewTrackWithNoMesh(
    vtkMySQLDatabase *iDatabaseConnector);

  //virtual pure method in QGoDBTraceManager
  std::list< unsigned int > UpdateTheTracesColor(vtkMySQLDatabase *iDatabaseConnector);

  //virtual pure method in QGoDBTraceManager
  virtual void UpdateTWAndContainerForImportedTraces(
    const std::vector< int > & iVectorImportedTraces,
    vtkMySQLDatabase *iDatabaseConnector);

  /**
  \brief delete the traces of the list from the database, the TW and the
  container for visu
  \param[in] iDatabaseConnector connection to the database
  \param[in] iListTraces list of the tracesIDs to be deleted

  */
  void DeleteListTraces(vtkMySQLDatabase *iDatabaseConnector,
    const std::list<unsigned int> & iListTraces);

  //virtual pure method in QGoDBTraceManager
  virtual void DeleteCheckedTraces( vtkMySQLDatabase *iDatabaseConnector);


   //virtual pure method in QGoDBTraceManager
  virtual std::list< unsigned int > GetListHighlightedIDs();

  /**
  \brief get the data from the TrackContainer corresponding to the user selected TrackID
  and put them in the current element of the track container
  */
  void UpdateCurrentElementTrackContainer();

  /**
  \brief update the points of the imported track in current_element
  with the info from the meshes and save them in the database
  */
  void UpdatePointsOfCurrentElementForImportedTrack(
    std::map<unsigned int,double*> iMeshesInfo,
    vtkMySQLDatabase* iDatabaseConnector);

  //method in QGoDBTraceManager
  void UpdateBoundingBoxes(
    vtkMySQLDatabase *iDatabaseConnector,
    const std::list< unsigned int > & iListTracesIDs);

  /**
  \brief check if the track belongs to a division and if it is possible to add the mesh
  without making the track overlapping the other tracks of the divisions, if so return
  a message to the user, if not, return the trackID of the divisions to be updated in the
  visu, if return empty message and empty list, the track doesn't belong to any division
  */
  std::string CheckMeshCanBeAddedToTrack( vtkMySQLDatabase* iDatabaseConnector,
                                          unsigned int iTrackID,
                                          unsigned int iMeshTimePoint,
                                          std::list<unsigned int> &ioMotherTrackDivisionToUpdate);

  /**
  \brief update the track container for visu and consequently the divisions in the visu
  */
  void UpdateDivisions(const std::list<unsigned int> & iListMotherTrackIDs);

  /**
  \brief Modify volume of the given track ID
  */
  void AddVolume(const unsigned int& iTrackID, const double& iVolume);

  /**
  \brief Modify volume of the given track ID
  */
  void AddVolumes(const std::list< std::pair<unsigned int, double> > & iVolumes);

  /**
  \brief Modify volume of the given track ID
  */
  void RemoveVolumes(const std::list< std::pair<unsigned int, double> > & iVolumes);

  /**
  \brief Modify volume of the given track ID
  */
  void AddVolumes(const std::list< std::pair<unsigned int, double> > & iVolumes,
                  unsigned int iTrackID);

  /**
  \brief Modify volume of the given track ID
  */
  void RemoveVolumes(const std::list< std::pair<unsigned int, double> > & iVolumes,
                     unsigned int iTrackID);

  std::vector<unsigned int> GetTrackFamily(vtkMySQLDatabase* iDatabaseConnector,
                                           unsigned int iTrackID);

  bool isMother(vtkMySQLDatabase* iDatabaseConnector, unsigned int iTrackID);

                                             /**
  \brief check that the mothertrackID is not already a mother in another
  trackfamily, create the trackfamily if not and return the trackfamilyID
  \param[in] iDatabaseConnector connection to the database
  \param[in] iMotherTrackID
  \param[in] iDaughtersID
  \return ID of the new created trackfamily
  */
  int CreateTrackFamily(vtkMySQLDatabase* iDatabaseConnector,
                        unsigned int iMotherTrackID,
                        const std::list<unsigned int> & iDaughtersID);

signals:
  void NeedMeshesInfoForImportedTrack(unsigned int iTrackID);
  void TrackToSplit(unsigned int iTrackID, std::list<unsigned int> iListMeshIDs);
  void TrackIDToBeModifiedWithWidget(std::list<unsigned int> iListTracksID);
  void MeshesToAddToTrack(std::list<unsigned int> iListMeshes, unsigned int iTrackID);

  void CheckedTracksToAddToSelectedLineage(std::list<unsigned int> iDaughtersID, unsigned int iLineageID,
    std::list<unsigned> iLineagesToDelete);

  void NewLineageToCreateFromTracks( std::list<unsigned int> iCheckedTracksIDs, unsigned int iTrackIDRoot,
    std::list<unsigned> iLineagesToDelete);

  void NeedToGoToTheRealLocation(double, double, double, int);

public slots:
  /**
  \brief slot called when the user chose "Delete the division for this tracks"
  */
  void DeleteTheDivisions(std::list<unsigned int> iDivisions = std::list<unsigned int>());

    /**
  \brief slot called when the user chose "Create a new division from checked tracks"
  */
  void CreateCorrespondingTrackFamily(std::list<unsigned int> iDivisions = std::list<unsigned int>());

protected:
  GoDBTWContainerForTrack *m_TWContainer;
  TrackContainer          *m_TrackContainerInfoForVisu;

  //virtual pure method in QGoDBTraceManager
  virtual void SetCollectionsTraceNames();

  //virtual pure method in QGoDBTraceManager
  virtual void DisplayInfoForAllTraces(vtkMySQLDatabase *iDatabaseConnector);

  virtual void DisplayInfoForTracesForSpecificTPs(
    vtkMySQLDatabase *iDatabaseConnector, const std::list<unsigned int> & iListTPs);

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
  void UpdateTrackPolydataForVisu(vtkMySQLDatabase *iDatabaseConnector,unsigned int iTrackID);

  //virtual in QGoDBTraceManager
  void AddActionsContextMenu(QMenu *iMenu);

  /**
  \brief create or update the track contained in the current element of the
  track container into the database, the table widget and insert the current
  element into the track container
  \param[in] iDatabaseConnector connection to the database
  */
  void SaveTrackCurrentElement(vtkMySQLDatabase* iDatabaseConnector);

  void SaveTrackStructure(vtkMySQLDatabase* iDatabaseConnector,
      TrackStructure* iStructure);

  /**
  \brief check that the 2 tracks are not overloaping, if not, return the
  trackID to keep for the merge and the one to delete
  \param[in] iTrackIDs IDs of both tracks to check
  \param[in,out] ioTraceIDToKeep ID of the trace to keep after the merge
  \param[in,out] ioTraceIDToDelete ID of the trace to delete after the merge
  \param[in] iDatabaseConnector connection to the database
  \return false of the tracks are not overlapping, true if they are
  */
  bool CheckOverlappingTracks(const std::list<unsigned int> & iTrackIDs,
    unsigned int &ioTraceIDToKeep, unsigned int &ioTraceIDToDelete,
    vtkMySQLDatabase* iDatabaseConnector);

  /**
  \brief update the trackFamilyID in the database for the track corresponding
  to iDaughterID, if the trackfamilyID is 0, then the lineageID will be set to
  0 also for this track
  \param[in] iDatabaseconnector connection to the database
  \param[in] iDaughterID ID of the track to be updated
  \param[in] iTrackFamilyID
  */
  void UpdateTrackFamilyIDForDaughter(vtkMySQLDatabase* iDatabaseConnector,
    unsigned int iDaughterID,unsigned int iTrackFamilyID);

  /**
  \brief get the trackID with the lowest timepoint as the mother trackID,
  if several tracks have the lowest timepoint, return false. if other tracks
  from the list are overlapping the mother trackID, return false.
  \param[in] iListTracksID tracks ID to be identified
  \param[in,out] ioMotherID to be modified with the identified mother TrackID
  \param[in,out] ioDaughtersID to be modified with the identified daughters TrackID
  \return false if it was not possible to identify them based on the timepoints
  */
  bool IdentifyMotherDaughtersToCreateTrackFamily(
    vtkMySQLDatabase* iDatabaseConnector,
    const std::list<unsigned int> & iListTracksID,
    int &ioMotherID,
    std::list<unsigned int> &ioDaughtersID);

  /**
  \brief get the lineage ids of the 2 daughters, then get all the tracks belonging
  to these 2 lineages and push them into the ioTrackIDsOfTheFamilies.
  \param[in] iDatabaseConnector connection to the database
  \param[in,out] ioTrackIDsOfTheFamilies IDs of all the tracks that will belong to
  the same lineage as the mother of the new created division
  \return the lineage ids of the 2 daughters
  */
  std::list<unsigned int> GetTrackIDFromDaughtersFamilies( vtkMySQLDatabase* iDatabaseConnector,
    std::list<unsigned int> &ioTrackIDsOfTheFamilies);

  /**
  \brief set the trackfamilyid of the daughters to 0, delete the trackfamily
  from the database and from the visu and check if the daughters are mothers, if not
  fill the ioTrackIDsNoLineage, if yes, update the all family
  \
  */
  void DeleteOneDivision(GoDBTrackFamilyRow iDivision, vtkMySQLDatabase* iDatabaseConnector,
    std::list<unsigned int> &ioTrackIDsNoLineage, std::list<unsigned int> &ioMotherLineageToDelete);

  /**
  \brief build a message for the user to know which ones of the selected tracks have no division
  and emit a signal for it to be printed into the status bar
  \param[in] iTracksNoDivision IDs of the selected tracks that are not mothers
  */
  void PrintAMessageForTracksWithNoDivision(std::list<unsigned int> iTracksNoDivision);

  /**
  \brief set the trackfamilyID of the daughter to 0, get all the tracks with the same lineage
  and emit a signal to create a lineage from these tracks with the daughter as trackIDRoot
  if ioPartOfHigherLineage, the lineage of the division will not be deleted as higher tracks belong to it
  */
  void CreateALineageWithFormerDaughterOfADeletedDivision(
    unsigned int iDaughterID, vtkMySQLDatabase* iDatabaseConnector, bool &ioPartOfHigherLineage);

  /**
  \brief return the trackfamilyID of the division the track is a mother of or 0 if the
  track is not a mother
  */
  unsigned int IsTheTrackAMother(unsigned int iDaughterID,
    vtkMySQLDatabase* iDatabaseConnector);

  /**
  \brief return the trackfamilyID of the division the track is a daughter of or 0 if the
  track is not a daughter
  */
  unsigned int IsTheTrackADaughter(unsigned int iTrackID,
  vtkMySQLDatabase* iDatabaseConnector);

  /**
  \brief check if the daughters are mothers, if yes, create a new lineage for them,
  if not, update the track familyID to 0 and fill the ioTrackIDsNoLineage with them,
  if ioPartOfHigherLineage is false, delete the lineage after creating a new one
  for the daughter family if the daughter is a mother and set the ioPartOfHigherLineage
  to true
  */
  void UpdateFormerDaughtersOfADeletedDivision(
    std::list<unsigned int> iDaughtersID,
    std::list<unsigned int> &ioTrackIDsNoLineage,
    bool &ioPartOfHigherLineage);

  /**
  \brief get the division IDs the track belongs to as a mother or as a daughter
  */
  std::list<unsigned int> GetDivisionIDsTheTrackBelongsTo(
    vtkMySQLDatabase* iDatabaseConnector, unsigned int iTrackID );
  /**
  \brief check that the iTimePoint is < to the mintimepoint of the daughter from the
  division where trackID is a mother
  */
  unsigned int CheckBoundingBoxDivisionAsAMother(vtkMySQLDatabase* iDatabaseConnector,
    unsigned int iTimePoint, unsigned int iTrackFamilyID );

   /**
  \brief check that the iTimePoint is > to the maxtimepoint of the mother from the
  division where trackID is a daughter
  */
  unsigned int CheckBoundingBoxDivisionAsADaughter(vtkMySQLDatabase* iDatabaseConnector,
    unsigned int iTimePoint, unsigned int iTrackFamilyID );

protected slots:

  //virtual pure method in QGoDBTraceManager
  virtual void UpdateHighlightedElementsInVisuContainer(int iTraceID);

  //virtual pure method in QGoDBTraceManager
  virtual void UpdateVisibleElementsInVisuContainer(int iTraceID);


 //virtual pure method in QGoDBTraceManager
  virtual void SetColorCoding(bool IsChecked);

  /**
  \brief get the trackIDs checked in the TW that will be modified with the
  widget and emit a signal with them as info on meshes are needed.
  */
  void SplitMergeTrackWithWidget();

  /**
  \brief check that there is only one checked track in the TW and emit a
  signal with it as the checked mesh is needed also to split the track
  */
  void TrackIDToEmit();

  /**
  \brief check that only 2 tracks are checked in the TW, if not display
  a message to the user, check that the 2 tracks are not overlapping,
  if yes, display a message to the user, get the meshes of the track with
  the lowest timepoints, delete this track and send a signal for the list of
  meshes of the previous track to be reassigned to the track with the
  highest timepoints
  */
  void MergeTracks();

  void GoToTrackEnd();
  void GoToTrackBegin();
};
#endif

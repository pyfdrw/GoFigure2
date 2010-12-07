/*=========================================================================
 Authors: The GoFigure Dev. Team.
 at Megason Lab, Systems biology, Harvard Medical school, 2009-10

 Copyright (c) 2009-10, President and Fellows of Harvard College.
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

#include "QGoDBMeshManager.h"
#include "GoDBMeshRow.h"
#include <iostream>
#include <sstream>

QGoDBMeshManager::QGoDBMeshManager(int iImgSessionID, QWidget *iparent):
QGoDBTraceManager(),m_MeshContainerInfoForVisu(NULL),m_SelectedCellType(NULL),
m_SelectedSubCellType(NULL)
{
  this->SetInfo(iImgSessionID, iparent);
  this->m_TWContainer = new GoDBTWContainerForMesh(iImgSessionID);
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QGoDBMeshManager::~QGoDBMeshManager()
{
  if ( this->m_TWContainer )
    {
    delete this->m_TWContainer;
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoDBMeshManager::SetMeshesInfoContainerForVisu(
  MeshContainer *iContainerForVisu)
{
  this->SetTracesInfoContainerForVisuTemplate<MeshContainer>(
    iContainerForVisu,&this->m_MeshContainerInfoForVisu);
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

void QGoDBMeshManager::SetCollectionsTraceNames()
{
  this->m_TraceName = "mesh";
  this->m_CollectionName = "track";
  this->m_CollectionOf = "contour";
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoDBMeshManager::DisplayInfoAndLoadVisuContainerForAllMeshes(
  vtkMySQLDatabase *iDatabaseConnector,unsigned int iTimePoint)
{
  this->DisplayInfoAndLoadVisuContainerWithAllTraces< GoDBTWContainerForMesh >
    (this->m_TWContainer, iDatabaseConnector);
  //this->UpdateTracesVisibilityForGivenTimePoint(iTimePoint);
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoDBMeshManager::DisplayInfoForAllTraces(
  vtkMySQLDatabase *iDatabaseConnector)
{
  int IndexShowColumn = this->m_TWContainer->GetIndexShowColumn();
  this->DisplayInfoForAllTracesTemplate< GoDBTWContainerForMesh >(
    this->m_TWContainer, iDatabaseConnector,Qt::Unchecked,IndexShowColumn);
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
/*void QGoDBMeshManager::UpdateTracesVisibilityForGivenTimePoint(unsigned int iTimePoint)
{
  std::list<unsigned int> ListMeshes =
    this->m_MeshContainerInfoForVisu->GetElementsTraceIDForGivenTimePoint(
    iTimePoint);
  this->m_Table->SetVisibleStateForListTraceIDs(
    ListMeshes,Qt::Checked,this->m_TraceName);
}*/

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoDBMeshManager::DisplayInfoForLastCreatedTrace(
  vtkMySQLDatabase *iDatabaseConnector)
{
  this->DisplayInfoForLastCreatedTraceTemplate< GoDBTWContainerForMesh >(
    this->m_TWContainer, iDatabaseConnector);
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoDBMeshManager::DisplayOnlyVolumeAreaForExistingMesh(
  GoFigureMeshAttributes *iMeshAttributes, unsigned iMeshID)
{
  if ( iMeshAttributes != 0 )
    {
    std::vector< std::string > ColumnNames (2);
    std::vector< std::string > Values (2);
    ColumnNames.at(0) = "SurfaceArea";
    Values.at(0) = ConvertToString< double >(iMeshAttributes->m_Area);
    ColumnNames.at(1) = "Volume";
    Values.at(1) = ConvertToString< double >(iMeshAttributes->m_Volume);
    this->m_Table->AddValuesForID(ColumnNames, Values, iMeshID, "meshID");
    }
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoDBMeshManager::DisplayInfoForLastCreatedMesh(
  vtkMySQLDatabase *iDatabaseConnector, GoFigureMeshAttributes *iMeshAttributes)
{
  this->m_TWContainer->SetMeshAttributes(iMeshAttributes);
  this->DisplayInfoForLastCreatedTrace(iDatabaseConnector);
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoDBMeshManager::DisplayInfoForExistingTrace(
  vtkMySQLDatabase *iDatabaseConnector, int iTraceID)
{
  this->DisplayInfoForExistingTraceTemplate< GoDBTWContainerForMesh >(
    this->m_TWContainer, iDatabaseConnector, iTraceID);
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoDBMeshManager::DisplayInfoForExistingTraceForMesh(
  vtkMySQLDatabase *iDatabaseConnector, int iTraceID,
  GoFigureMeshAttributes *iMeshAttributes)
{
  this->m_TWContainer->SetMeshAttributes(iMeshAttributes);
  this->DisplayInfoForExistingTrace(iDatabaseConnector, iTraceID);
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoDBMeshManager::AddActionsContextMenu(QMenu *iMenu)
{
  QGoDBTraceManager::AddActionsContextMenu(iMenu);
  this->AddSpecificActionsForContourMesh(iMenu);
  this->m_CheckedTracesMenu->addAction( 
    tr("Add the selected CellType '%1' to the checked meshes")
    .arg(this->m_SelectedCellType->c_str() ), this, SLOT( UpdateCellType() ) );
  this->m_CheckedTracesMenu->addAction( 
    tr("Add the selected SubCellType '%1' to the checked meshes")
    .arg(this->m_SelectedSubCellType->c_str() ), this, SLOT( UpdateSubCellType() ) );
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
unsigned int QGoDBMeshManager::SaveNewMeshFromVisu(
  unsigned int iXCoordMin, unsigned int iYCoordMin, unsigned int iZCoordMin,
  unsigned int iTCoord, unsigned int iXCoordMax, unsigned int iYCoordMax,
  unsigned int iZCoordMax, vtkPolyData *iTraceNodes,
  vtkMySQLDatabase *iDatabaseConnector, NameWithColorData iColor,
  unsigned int iTrackID, GoFigureMeshAttributes *iMeshAttributes)
{
  GoDBMeshRow NewMesh(this->m_ImgSessionID);
  NewMesh.SetCellType(iDatabaseConnector, *this->m_SelectedCellType);
  NewMesh.SetSubCellType(iDatabaseConnector, *this->m_SelectedSubCellType);
  this->SetMeshBoundingBoxAndPoints(iXCoordMin, iYCoordMin, iZCoordMin, iTCoord,
                                    iXCoordMax, iYCoordMax, iZCoordMax, iTraceNodes, iDatabaseConnector, NewMesh,
                                    iMeshAttributes);
  //save the intensities for each channel !!!
  unsigned int NewMeshID = this->m_CollectionOfTraces->CreateNewTraceInDB< GoDBMeshRow >(
    NewMesh, iDatabaseConnector, iColor, iTrackID);
  double *rgba = this->GetVectorFromQColor(iColor.second);
  this->m_MeshContainerInfoForVisu->UpdateCurrentElementFromDB(
    NewMeshID, rgba);
  this->DisplayInfoForLastCreatedMesh(iDatabaseConnector, iMeshAttributes);
  return NewMeshID;
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoDBMeshManager::SaveGeneratedMeshFromVisu(unsigned int iXCoordMin,
                                                 unsigned int iYCoordMin,
                                                 unsigned int iZCoordMin,
                                                 unsigned int iTCoord,
                                                 unsigned int iXCoordMax,
                                                 unsigned int iYCoordMax,
                                                 unsigned int iZCoordMax,
                                                 vtkPolyData *iTraceNodes,
                                                 vtkMySQLDatabase *iDatabaseConnector,
                                                 GoFigureMeshAttributes *iMeshAttributes)
{
  unsigned int TraceID =
    this->m_MeshContainerInfoForVisu->m_CurrentElement.TraceID;
  GoDBMeshRow GeneratedMesh;

  GeneratedMesh.SetValuesForSpecificID(TraceID, iDatabaseConnector);

  this->SetMeshBoundingBoxAndPoints(iXCoordMin,
                                    iYCoordMin,
                                    iZCoordMin,
                                    iTCoord,
                                    iXCoordMax,
                                    iYCoordMax,
                                    iZCoordMax,
                                    iTraceNodes,
                                    iDatabaseConnector,
                                    GeneratedMesh,
                                    iMeshAttributes);
  //save the intensity for each channel !!!
  GeneratedMesh.SaveInDB(iDatabaseConnector);
  this->DisplayInfoForExistingTraceForMesh(iDatabaseConnector, TraceID, iMeshAttributes);
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
unsigned int QGoDBMeshManager::CreateNewMeshWithNoContourNoPoints(
  vtkMySQLDatabase *iDatabaseConnector, NameWithColorData iColor, unsigned int iTimePoint,
  //std::string iCellType, std::string iSubCellType, 
  unsigned int iTrackID)
{
  GoDBMeshRow NewMesh;

 // NewMesh.SetCellType(iDatabaseConnector, iCellType);
  //NewMesh.SetSubCellType(iDatabaseConnector, iSubCellType);
  NewMesh.SetCellType(iDatabaseConnector, *this->m_SelectedCellType);
  NewMesh.SetSubCellType(iDatabaseConnector, *this->m_SelectedSubCellType);
  if ( iTrackID != 0 )
    {
    NewMesh.SetCollectionID(iTrackID);
    }
  unsigned int NewMeshID =
    this->m_CollectionOfTraces->CreateCollectionWithNoTracesNoPoints< GoDBMeshRow >(
      iDatabaseConnector, iColor, NewMesh, iTimePoint);

  double *color = this->GetVectorFromQColor(iColor.second);
  this->m_MeshContainerInfoForVisu->UpdateCurrentElementFromDB(
    NewMeshID, color);
  delete[] color;

  this->m_MeshContainerInfoForVisu->InsertCurrentElement();
  this->DisplayInfoForLastCreatedTrace(iDatabaseConnector);
  return NewMeshID;
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
std::list< unsigned int > QGoDBMeshManager::UpdateTheTracesColor(
  vtkMySQLDatabase *iDatabaseConnector, NameWithColorData iNewColor)
{
  return this->UpdateTheTracesColorTemplate< GoDBMeshRow,ContourMeshContainer >(
    iDatabaseConnector,this->m_MeshContainerInfoForVisu,iNewColor);
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoDBMeshManager::UpdateBoundingBoxes(vtkMySQLDatabase *iDatabaseConnector,
                                           std::list< unsigned int > iListTracesIDs)
{
  std::list< unsigned int > ListMeshesWithNoPoints =
    this->m_CollectionOfTraces->GetListTracesIDWithNoPoints(
      iListTracesIDs, iDatabaseConnector);
  if ( !ListMeshesWithNoPoints.empty() )
    {
    QGoDBTraceManager::UpdateBoundingBoxes(iDatabaseConnector, ListMeshesWithNoPoints);
    }
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoDBMeshManager::SetMeshBoundingBoxAndPoints(unsigned int iXCoordMin,
                                                   unsigned int iYCoordMin,
                                                   unsigned int iZCoordMin,
                                                   unsigned int iTCoord,
                                                   unsigned int iXCoordMax,
                                                   unsigned int iYCoordMax,
                                                   unsigned int iZCoordMax,
                                                   vtkPolyData *iTraceNodes,
                                                   vtkMySQLDatabase *iDatabaseConnector,
                                                   GoDBMeshRow & iMesh,
                                                   GoFigureMeshAttributes *iMeshAttributes)
{
  GoDBCoordinateRow coord_min = this->GetCoordinateFromInt(iXCoordMin,
                                                           iYCoordMin, iZCoordMin, iTCoord);
  GoDBCoordinateRow coord_max = this->GetCoordinateFromInt(iXCoordMax,
                                                           iYCoordMax, iZCoordMax, iTCoord);

  iMesh.SetTheDataFromTheVisu(iDatabaseConnector, iTraceNodes,
                              coord_min, coord_max, iMeshAttributes);
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoDBMeshManager::UpdateTWAndContainerForImportedTraces(
  std::vector< int > iVectorImportedTraces, vtkMySQLDatabase *iDatabaseConnector)
{
  this->UpdateTWAndContainerWithImportedTracesTemplate<
    GoDBTWContainerForMesh>(this->m_TWContainer,
   iVectorImportedTraces, iDatabaseConnector);
  //update the visualization and the data from visu in the container for visu:
  this->m_MeshContainerInfoForVisu->
    UpdateVisualizationForGivenIDs< std::vector< int > >(
      iVectorImportedTraces );
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoDBMeshManager::DeleteTraces(vtkMySQLDatabase *iDatabaseConnector)
{
  this->DeleteTracesTemplate<ContourMeshContainer>(iDatabaseConnector,
    this->m_MeshContainerInfoForVisu);
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
std::list< unsigned int > QGoDBMeshManager::GetListHighlightedIDs()
{
  return this->m_MeshContainerInfoForVisu->GetHighlightedElementsTraceID();
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoDBMeshManager::UpdateHighlightedElementsInVisuContainer(
  int iTraceID)
{
  this->m_MeshContainerInfoForVisu->
    UpdateElementHighlightingWithGivenTraceID(iTraceID);
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoDBMeshManager::UpdateVisibleElementsInVisuContainer(int iTraceID)
{
  this->m_MeshContainerInfoForVisu->
    UpdateElementVisibilityWithGivenTraceID(iTraceID);
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoDBMeshManager::GetTracesInfoFromDBAndModifyContainerForVisu(
  vtkMySQLDatabase* iDatabaseConnector,std::vector<int> iVectIDs)
{
  std::list<ContourMeshStructure> list_of_traces;
  GetTracesInfoFromDBAndModifyContainer(
      list_of_traces,
      iDatabaseConnector, this->m_TraceName, this->m_CollectionName,
      this->m_ImgSessionID, -1, iVectIDs);

  std::list< ContourMeshStructure >::iterator it = list_of_traces.begin();

  while ( it != list_of_traces.end() )
    {
    this->m_MeshContainerInfoForVisu->Insert(*it);
    ++it;
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
std::map<unsigned int,double*> QGoDBMeshManager::GetMeshesInfoForImportedMesh(
	std::list<unsigned int> iMeshesIDs)
{
	return this->m_MeshContainerInfoForVisu->GetMeshesPoints(iMeshesIDs);
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoDBMeshManager::SetColorCoding(bool IsChecked)
{
  this->SetColorCodingTemplate<ContourMeshContainer>(
    this->m_MeshContainerInfoForVisu,IsChecked);
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoDBMeshManager::SetSelectedCellType(std::string* iCellType)
{
  this->m_SelectedCellType = iCellType;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoDBMeshManager::SetSelectedSubCellType(std::string* iSubCellType)
{
  this->m_SelectedSubCellType = iSubCellType;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoDBMeshManager::UpdateCellType()
{
  emit NeedToGetDatabaseConnection();
  std::list<unsigned int> ListCheckedMeshes =
    this->m_MeshContainerInfoForVisu->GetHighlightedElementsTraceID();
  int CellTypeID = GoDBMeshRow::GetCellTypeID(this->m_DatabaseConnector, 
    *this->m_SelectedCellType);
  this->m_CollectionOfTraces->UpdateValueForListTraces(
    this->m_DatabaseConnector,"CellTypeID",ConvertToString<int>(CellTypeID),
    ListCheckedMeshes);
  this->DisplayInfoForExistingTraces(this->m_DatabaseConnector,ListCheckedMeshes);
  emit DBConnectionNotNeededAnymore();
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoDBMeshManager::UpdateSubCellType()
{
  emit NeedToGetDatabaseConnection();
  std::list<unsigned int> ListCheckedMeshes =
    this->m_MeshContainerInfoForVisu->GetHighlightedElementsTraceID();
  int SubCellTypeID = GoDBMeshRow::GetSubCellTypeID(this->m_DatabaseConnector, 
    *this->m_SelectedSubCellType);
  this->m_CollectionOfTraces->UpdateValueForListTraces(
    this->m_DatabaseConnector,"SubCellularID",ConvertToString<int>(SubCellTypeID),
    ListCheckedMeshes);
  this->DisplayInfoForExistingTraces(this->m_DatabaseConnector,ListCheckedMeshes);
  emit DBConnectionNotNeededAnymore();
}

 unsigned int QGoDBMeshManager::
   ReassignTrackIDForPreviousMeshWithSameTimePoint( vtkMySQLDatabase *iDatabaseConnector,
    unsigned int iTrackID,unsigned int iTimePoint)
 {
   unsigned int oExistingMeshID = 0;
   //get meshID with same timepoint and same TrackID:
   std::list<unsigned int> ExistingMeshID =
     this->m_CollectionOfTraces->GetTraceIDsWithTimePointAndCollectionID(
     iDatabaseConnector,iTrackID, iTimePoint);
   if (ExistingMeshID.empty())
    {
    return oExistingMeshID;
    }
   if (ExistingMeshID.size() > 1)
    {
    std::cout<<"there is more than 1 existing mesh for this track at this timepoint ";
    std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
    std::cout << std::endl;
    return oExistingMeshID;
    }
   oExistingMeshID = ExistingMeshID.front();
   GoDBMeshRow ExistingMesh(oExistingMeshID,iDatabaseConnector);
   ExistingMesh.SetCollectionID(0); //reassign the track ID to 0:
   ExistingMesh.SaveInDB(iDatabaseConnector);
   this->DisplayInfoForExistingTrace(iDatabaseConnector,oExistingMeshID); //update the TW
   return oExistingMeshID;
 }
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QString QGoDBMeshManager::CheckExistingMeshesForTheTrack(
   unsigned int iTrackID,int iTimePoint,vtkMySQLDatabase* iDatabaseConnector)
{
  QString MessageToPrint("");
  unsigned int MeshIDKickedOut =
    this->ReassignTrackIDForPreviousMeshWithSameTimePoint(
      iDatabaseConnector, iTrackID,iTimePoint);
  if (MeshIDKickedOut != 0)
    {
      MessageToPrint =
        tr("Warning: existing mesh at this timepoint for this track !!The track of the mesh with the meshID %1 has been reassigned to 0")
      .arg(MeshIDKickedOut);
    }
  return MessageToPrint;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QString QGoDBMeshManager::CheckExistingMeshesForTheTrack(
   unsigned int iTrackID,vtkMySQLDatabase* iDatabaseConnector,std::list<unsigned int> iListMeshIDs)
{
  QString MessageQString("");
  std::string MessageToPrint = "";
  std::list<unsigned int> ListTimePoints =
    this->m_CollectionOfTraces->GetListTimePointsFromTraceIDs(iDatabaseConnector,iListMeshIDs);
  if (!ListTimePoints.empty())
  {
  std::string MeshIDToPrint = "";
  std::list<unsigned int>::iterator iter = ListTimePoints.begin();
  while(iter!= ListTimePoints.end())
    {
    unsigned int MeshIDKickedOut =
      this->ReassignTrackIDForPreviousMeshWithSameTimePoint(
        iDatabaseConnector, iTrackID,*iter);
    if (MeshIDKickedOut != 0)
      {
        MeshIDToPrint += ConvertToString<unsigned int>(MeshIDKickedOut);
        MeshIDToPrint += ", ";
      }
    iter++;
    }
  if (!MeshIDToPrint.empty())
    {
      MeshIDToPrint = MeshIDToPrint.substr(0, MeshIDToPrint.size()-2);
      MessageToPrint += "The trackID of the meshes ";
      MessageToPrint += MeshIDToPrint;
      MessageToPrint += " have been reassigned to 0";
    }
  }
  MessageQString = MessageToPrint.c_str();
  return MessageQString;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
std::string QGoDBMeshManager::CheckListMeshesFromDifferentTimePoints(
  vtkMySQLDatabase *iDatabaseConnector,std::list< unsigned int > iListMeshIDs,
  std::list<unsigned int> & ioListMeshIDsToBePartOfTrack,
  std::list<unsigned int> & ioListMeshIDsToReassign)
{
  std::string MessageToPrint = "";
  ioListMeshIDsToBePartOfTrack = iListMeshIDs;
  if (!iListMeshIDs.empty())
    {
    std::list< unsigned int > TimePointsWithSeveralMeshes =
      this->m_CollectionOfTraces->GetTimePointWithSeveralTracesFromTheList(
      iDatabaseConnector,iListMeshIDs);
    std::list< unsigned int >::iterator iter = TimePointsWithSeveralMeshes.begin();
    while (iter != TimePointsWithSeveralMeshes.end())
      {
      int MaxMeshIDForTimePoint =
        this->m_CollectionOfTraces->GetMaxTraceIDsForSpecificTimePoint(
        iDatabaseConnector,iListMeshIDs,*iter);
      if (MaxMeshIDForTimePoint != -1)
        {
        std::list<unsigned int> TraceIDs =
          this->m_CollectionOfTraces->GetNonMaxTraceIDsForSpecificTimePoint(
          iDatabaseConnector, iListMeshIDs, *iter,MaxMeshIDForTimePoint);
        std::copy(TraceIDs.begin(),TraceIDs.end(),std::back_inserter(ioListMeshIDsToReassign) );
        std::list<unsigned int>::iterator iterTraceIDToRemove = TraceIDs.begin();
        while (iterTraceIDToRemove != TraceIDs.end())
          {
          std::list <unsigned int>::iterator Find =
            std::find(ioListMeshIDsToBePartOfTrack.begin(), ioListMeshIDsToBePartOfTrack.end(),
            *iterTraceIDToRemove);
          ioListMeshIDsToBePartOfTrack.erase(Find);
          iterTraceIDToRemove++;
          }
        }
      iter++;
       }
    if (!ioListMeshIDsToReassign.empty())
      {
        MessageToPrint = "Warning: the meshIDs ";
        std::list<unsigned int>::iterator iterIDs = ioListMeshIDsToReassign.begin();
        while (iterIDs != ioListMeshIDsToReassign.end() )
          {
          std::string temp = ConvertToString<unsigned int>(*iterIDs);
          MessageToPrint += temp;
          MessageToPrint += ", ";
          iterIDs++;
          }
        MessageToPrint  = MessageToPrint.substr(0,MessageToPrint.size()-1);
        MessageToPrint  += "have not been reassigned ";
        MessageToPrint += "to the trackID because several meshes were selected for the same ";
        MessageToPrint += "timepoints ";
        std::list<unsigned int>::iterator iter = TimePointsWithSeveralMeshes.begin();
        while(iter != TimePointsWithSeveralMeshes.end())
          {
          std::string temp = ConvertToString<unsigned int>(*iter);
          MessageToPrint += temp;
          MessageToPrint += ", ";
          iter++;
          }
        MessageToPrint = MessageToPrint.substr(0,MessageToPrint.size()-2);
      }
     }
  return MessageToPrint;
}

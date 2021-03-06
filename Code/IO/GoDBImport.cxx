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

#include "GoDBImport.h"
#include "SelectQueryDatabaseHelper.h"
#include "QueryDataBaseHelper.h"
#include "ConvertToStringHelper.h"
#include "GoDBColorRow.h"
#include "GoDBCellTypeRow.h"
#include "GoDBSubCellTypeRow.h"
#include "GoDBCoordinateRow.h"
#include "GoDBContourRow.h"
#include "GoDBMeshRow.h"
#include "GoDBTrackRow.h"
#include "GoDBTrackFamilyRow.h"
#include "GoDBLineageRow.h"
#include "GoDBChannelRow.h"
#include "GoDBIntensityRow.h"

//--------------------------------------------------------------------------
GoDBImport::GoDBImport(const std::string & iServerName, const std::string & iLogin,
                       const std::string & iPassword, int iImagingSessionID,
                       const std::string & iFilename, int iCurrentTimePoint)
{
  this->m_ServerName = iServerName;
  this->m_Login = iLogin;
  this->m_Password = iPassword;
  this->m_ImagingSessionID = iImagingSessionID;
  this->m_CurrentTimePoint = iCurrentTimePoint;
  this->m_InFile.open(iFilename.c_str(), std::ifstream::in);
}

//--------------------------------------------------------------------------
GoDBImport::~GoDBImport()
{
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBImport::ImportContours()
{
  this->OpenDBConnection();
  IntMapType MapColorIDs;
  IntMapType MapCellTypeIDs;
  IntMapType MapSubCellTypeIDs;
  IntMapType MapCoordIDs;
  std::string          LineContent;

  LineContent = this->SaveNoTracesEntities(MapColorIDs, MapCellTypeIDs,
                                           MapSubCellTypeIDs, MapCoordIDs);

  this->SaveTracesEntities(MapColorIDs, MapCoordIDs, LineContent, MapCellTypeIDs,
                           MapSubCellTypeIDs);
  //this->FillContourInfoForVisu(this->m_NewContourIDs);
  this->CloseDBConnection();
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBImport::ImportMeshes()
{
  this->OpenDBConnection();
  this->m_NewContourIDs.clear();
  IntMapType MapColorIDs;
  IntMapType MapCellTypeIDs;
  IntMapType MapSubCellTypeIDs;
  IntMapType MapCoordIDs;
  std::string          LineContent;

  LineContent = this->SaveNoTracesEntities(MapColorIDs, MapCellTypeIDs,
                                           MapSubCellTypeIDs, MapCoordIDs);
  this->SaveTracesEntities(MapColorIDs, MapCoordIDs, LineContent, MapCellTypeIDs,
                           MapSubCellTypeIDs, true);
  //this->FillMeshInfoForVisu(this->m_NewMeshIDs);
  this->CloseDBConnection();
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBImport::ImportTracks()
{
  this->ImportMeshes();
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
std::string GoDBImport::SaveNoTracesEntities(IntMapType & ioMapColorIDs,
                                             IntMapType & ioMapCellTypeIDs,
                                             IntMapType & ioMapSubCellTypeIDs,
                                             IntMapType & ioMapCoordIDs)
{
  std::string LineContent;

  getline(this->m_InFile, LineContent);
  while ( !this->IsLineForNumberOfEntities(LineContent) )
    {
    getline (this->m_InFile, LineContent);
    }
  while ( this->FindFieldName(LineContent) != "NumberOftrack" )
    {
    int EntitiesNumber = atoi( this->GetValueForTheLine(LineContent).c_str() );
    getline(this->m_InFile, LineContent);
    //if there is nothing to be saved in the database for this group, just go
    //to the next line in the file:
    if ( EntitiesNumber != 0 )
      {
      if ( this->GetValueForTheLine(LineContent) != "NoValueOnTheLine" )
        {
        std::cout << "There was supposed to be only the name of the entity to save,the entity will not be saved";
        std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
        std::cout << std::endl;
        return LineContent;
        }
      std::string NameEntity = this->FindFieldName(LineContent);
      if ( NameEntity == "color" )
        {
        LineContent = this->SaveImportedEntitiesInDatabase< GoDBColorRow >(
            EntitiesNumber, ioMapColorIDs);
        }
      if ( NameEntity == "celltype" )
        {
        LineContent = this->SaveImportedEntitiesInDatabase< GoDBCellTypeRow >(
            EntitiesNumber, ioMapCellTypeIDs);
        }
      if ( NameEntity == "subcellulartype" )
        {
        LineContent = this->SaveImportedEntitiesInDatabase< GoDBSubCellTypeRow >(
            EntitiesNumber, ioMapSubCellTypeIDs);
        }
      if ( NameEntity == "coordinate" )
        {
        LineContent = this->SaveImportedEntitiesInDatabase< GoDBCoordinateRow >(
            EntitiesNumber, ioMapCoordIDs);
        }
      if ( NameEntity != "color" && NameEntity != "celltype"
           && NameEntity != "subcellulartype" && NameEntity != "coordinate" )
        {
        std::cout << "The name of the entity doesn't correspond to any of the no traces entity";
        std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
        std::cout << std::endl;
        }
      }
    }
  return LineContent;
}

//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
std::string GoDBImport::FindFieldName(const std::string & iLine)
{
  size_t      BegName = iLine.find("<", 0) + 1;
  size_t      EndName = iLine.find(">", 0);
  size_t      NameLength = EndName - BegName;
  std::string oName = iLine.substr(BegName, NameLength);

  if ( oName.find("/", 0) == 0 )
    {
    oName = oName.substr(1);
    }
  return oName;
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
std::string GoDBImport::GetValueForTheLine(const std::string & iLine)
{
  size_t BegValue = iLine.find(">", 0) + 1;
  size_t EndValue = iLine.find("<", BegValue);

  if ( EndValue != iLine.npos )
    {
    size_t ValueLength = EndValue - BegValue;
    return iLine.substr(BegValue, ValueLength);
    }
  return "NoValueOnTheLine";
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
bool GoDBImport::IsLineForNumberOfEntities(const std::string & iLine)
{
  size_t BegValue = iLine.find("NumberOf", 0);

  return ( BegValue != iLine.npos );
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBImport::OpenDBConnection()
{
  this->m_DatabaseConnector = OpenDatabaseConnection(m_ServerName,
                                                     m_Login, m_Password, "gofiguredatabase");
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBImport::CloseDBConnection()
{
  CloseDatabaseConnection(m_DatabaseConnector);
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBImport::SaveIntensityForMesh(std::string & ioLineContent,
                                      const IntMapType & iMapMeshIDs,
                                      const IntMapType & iMapColorIDs)
{
  IntMapType MapChannelIDs;
  while ( this->FindFieldName(ioLineContent) != "NumberOfchannel" )
    {
    getline (this->m_InFile, ioLineContent);
    }

  int EntitiesNumber = atoi( this->GetValueForTheLine(ioLineContent).c_str() );
  getline(this->m_InFile, ioLineContent);
  if ( EntitiesNumber != 0 )
    {
    while ( this->FindFieldName(ioLineContent) != "NumberOfintensity" )
      {
      if ( this->GetValueForTheLine(ioLineContent) != "NoValueOnTheLine" )
        {
        std::cout << "There was supposed to be only the name of the entity to save,the entity will not be saved";
        std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
        std::cout << std::endl;
        }
      std::string NameEntity = this->FindFieldName(ioLineContent);
      if ( NameEntity != "channel" )
        {
        std::cout << "The name of the entity should be channel but is actually different";
        std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
        std::cout << std::endl;
        }
      else
        {
        GoDBChannelRow NewChannel;
        ioLineContent = this->GetValuesFromInfile< GoDBChannelRow >(NewChannel);
        this->ReplaceTheFieldWithNewIDs< GoDBChannelRow >(
          iMapColorIDs, "ColorID", NewChannel);
        int OldID =  NewChannel.GetMapValue<int>("ChannelID");
        NewChannel.SetField("ChannelID", "0");
        NewChannel.SetField("ImagingSessionID", this->m_ImagingSessionID);
        MapChannelIDs[OldID] = NewChannel.SaveInDB(this->m_DatabaseConnector);
        }
      }
    }
  EntitiesNumber = atoi( this->GetValueForTheLine(ioLineContent).c_str() );
  getline(this->m_InFile, ioLineContent);
  if ( EntitiesNumber != 0 )
    {
    while ( this->FindFieldName(ioLineContent) != "ExportTraces" )
      {
      if ( this->GetValueForTheLine(ioLineContent) != "NoValueOnTheLine" )
        {
        std::cout << "There was supposed to be only the name of the entity to save,the entity will not be saved";
        std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
        std::cout << std::endl;
        }
      std::string NameEntity = this->FindFieldName(ioLineContent);
      if ( NameEntity != "intensity" )
        {
        std::cout << "The name of the entity should be channel but is actually different";
        std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
        std::cout << std::endl;
        }
      else
        {
        GoDBIntensityRow NewIntensity;
        ioLineContent = this->GetValuesFromInfile< GoDBIntensityRow >(NewIntensity);
        this->ReplaceTheFieldWithNewIDs< GoDBIntensityRow >(
          iMapMeshIDs, "meshID", NewIntensity);
        this->ReplaceTheFieldWithNewIDs< GoDBIntensityRow >(
          MapChannelIDs, "ChannelID", NewIntensity);
        NewIntensity.SetField("IntensityID", "0");
        NewIntensity.SaveInDB(this->m_DatabaseConnector);
        }
      }
    }
  }
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
template<>
void
GoDBImport::SaveTraces< GoDBMeshRow >(
  const IntMapType &  iMapColorIDs,
  const IntMapType &  iMapCoordIDs,
  const IntMapType &  iMapCollectionIDs,
  std::string & ioLineContent,
  std::vector< int > & ioNewTracesIDs,
  IntMapType & ioMapTraceIDs,
  const IntMapType &  iMapIDsSpecificOne,
  const IntMapType &  iMapIDsSpecificTwo
  )
{
  GoDBMeshRow TraceToSave;
  int NumberOfTraces = atoi( this->GetValueForTheLine(ioLineContent).c_str() );

  getline(this->m_InFile, ioLineContent);

  for ( int i = 0; i < NumberOfTraces; i++ )
    {
    ioLineContent = this->GetValuesFromInfile< GoDBMeshRow >(TraceToSave);

    if ( !iMapIDsSpecificOne.empty() )
      {
      this->ReplaceTheFieldWithNewIDs< GoDBMeshRow >(iMapIDsSpecificOne,
                                                     "CellTypeID", TraceToSave);
      }
    if ( !iMapIDsSpecificTwo.empty() )
      {
      this->ReplaceTheFieldWithNewIDs< GoDBMeshRow >(iMapIDsSpecificTwo,
                                                     "SubCellularID", TraceToSave);
      }

    this->ReplaceCommonFieldsForTraces(TraceToSave, iMapColorIDs,
                                       iMapCoordIDs, iMapCollectionIDs);

    int OldTraceID = atoi( TraceToSave.GetMapValue( TraceToSave.GetTableIDName() ).c_str() );

    /*in order the query works, the TraceID to be saved has to be set to 0 otherwise
    if the TraceID already exists,the query will return the error
    "Duplicate entry TraceID for key primary":*/
    TraceToSave.SetField(TraceToSave.GetTableIDName(), "0");
    int NewTraceID = TraceToSave.DoesThisBoundingBoxExist(this->m_DatabaseConnector);
    if ( NewTraceID == -1 )
      {
      NewTraceID = TraceToSave.SaveInDB(this->m_DatabaseConnector);
      // this->m_NewTraceIDs.push_back(NewTraceID);
      }
    else
      {
      std::cout << "The trace" << OldTraceID << " has the same bounding box as ";
      std::cout << "the existing trace " << NewTraceID;
      std::cout << "so the imported contours belonging to the mesh " << OldTraceID;
      std::cout << " will belong to the existing mesh " << NewTraceID << std::endl;
      }
    ioNewTracesIDs.push_back(NewTraceID);
    ioMapTraceIDs[OldTraceID] = NewTraceID;
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
template<>
void
GoDBImport::SaveTraces< GoDBLineageRow >(
  const IntMapType &  iMapColorIDs,
  const IntMapType &  iMapCoordIDs,
  const IntMapType &  iMapCollectionIDs,
  std::string & ioLineContent,
  std::vector< int > & ioNewTracesIDs,
  IntMapType & ioMapTraceIDs,
  const IntMapType &  iMapIDsSpecificOne,
  const IntMapType &  iMapIDsSpecificTwo
  )
{
  GoDBLineageRow TraceToSave;
  int NumberOfTraces = atoi( this->GetValueForTheLine(ioLineContent).c_str() );

  getline(this->m_InFile, ioLineContent);

  for ( int i = 0; i < NumberOfTraces; i++ )
    {
    ioLineContent = this->GetValuesFromInfile< GoDBLineageRow >(TraceToSave);

    this->ReplaceCommonFieldsForTraces(TraceToSave, iMapColorIDs,
                                       iMapCoordIDs, iMapCollectionIDs);

    this->ReplaceTheFieldWithNewIDs< GoDBLineageRow >(
      iMapIDsSpecificOne, "TrackIDRoot", TraceToSave);

    int OldTraceID = atoi( TraceToSave.GetMapValue( TraceToSave.GetTableIDName() ).c_str() );

    /*in order the query works, the TraceID to be saved has to be set to 0 otherwise
    if the TraceID already exists,the query will return the error
    "Duplicate entry TraceID for key primary":*/
    TraceToSave.SetField(TraceToSave.GetTableIDName(), "0");
    int NewTraceID = TraceToSave.DoesThisBoundingBoxExist(this->m_DatabaseConnector);
    if ( NewTraceID == -1 )
      {
      NewTraceID = TraceToSave.SaveInDB(this->m_DatabaseConnector);
      // this->m_NewTraceIDs.push_back(NewTraceID);
      }
    else
      {
      std::cout << "The trace" << OldTraceID << " has the same bounding box as ";
      std::cout << "the existing trace " << NewTraceID;
      std::cout << "so the imported contours belonging to the mesh " << OldTraceID;
      std::cout << " will belong to the existing mesh " << NewTraceID << std::endl;
      }
    ioNewTracesIDs.push_back(NewTraceID);
    ioMapTraceIDs[OldTraceID] = NewTraceID;
    }
}
//--------------------------------------------------------------------------

void
GoDBImport::SaveTrackFamilyEntities(const IntMapType & iMapTrackIDs,
                                    IntMapType & ioMapTrackFamilyIDs,
                                    std::string & ioLineContent)
{
  std::string temp = this->GetValueForTheLine(ioLineContent);

  int NumberOfTrackFamilies = atoi( temp.c_str() );

  getline(this->m_InFile, ioLineContent);

  for ( int i = 0; i < NumberOfTrackFamilies; i++ )
    {
    GoDBTrackFamilyRow EntityToSave;
    ioLineContent = this->GetValuesFromInfile< GoDBTrackFamilyRow >(EntityToSave);

    this->ReplaceTheFieldWithNewIDs< GoDBTrackFamilyRow >(
      iMapTrackIDs, "TrackIDMother", EntityToSave);

    this->ReplaceTheFieldWithNewIDs< GoDBTrackFamilyRow >(
      iMapTrackIDs, "TrackIDDaughter1", EntityToSave);

    unsigned int TrackIDDaughter1 = EntityToSave.GetMapValue<unsigned int>( "TrackIDDaughter1" );

    this->ReplaceTheFieldWithNewIDs< GoDBTrackFamilyRow >(
      iMapTrackIDs, "TrackIDDaughter2", EntityToSave);

    unsigned int TrackIDDaughter2 = EntityToSave.GetMapValue<unsigned int>( "TrackIDDaughter2" );

    int OldID =  EntityToSave.GetMapValue<int>("TrackFamilyID");
    EntityToSave.SetField("TrackFamilyID", "0");

    int NewID = EntityToSave.SaveInDB(this->m_DatabaseConnector);

    ioMapTrackFamilyIDs[OldID] = NewID;

    GoDBTrackRow Daughter1;
    Daughter1.SetValuesForSpecificID(TrackIDDaughter1, this->m_DatabaseConnector);
    Daughter1.SetField<unsigned int>("TrackFamilyID", NewID);
    Daughter1.SaveInDB(this->m_DatabaseConnector);

    GoDBTrackRow Daughter2;
    Daughter2.SetValuesForSpecificID(TrackIDDaughter2, this->m_DatabaseConnector);
    Daughter2.SetField<unsigned int>("TrackFamilyID", NewID);
    Daughter2.SaveInDB(this->m_DatabaseConnector);
    }
}

//--------------------------------------------------------------------------
void
GoDBImport::SaveTracesEntities(const IntMapType  & iMapColorIDs,
                               const IntMapType  & iMapCoordIDs,
                               const std::string & iLineContent,
                               const IntMapType  & iMapCellTypeIDs,
                               const IntMapType  & iMapSubCellTypeIDs,
                               bool SaveIntensities)
{
  IntMapType MapContourIDs;
  IntMapType MapMeshIDs;
  IntMapType MapTrackIDs;
  IntMapType MapLineageIDs;
  IntMapType MapTrackFamilyIDs;

  std::string LineContent = iLineContent;

    {
    IntMapType MapIDsSpecificOne;
    IntMapType MapIDsSpecificTwo;

    this->SaveTraces< GoDBTrackRow >(iMapColorIDs, iMapCoordIDs, MapLineageIDs,
                                     LineContent, this->m_NewTracksIDs, MapTrackIDs,
                                     MapIDsSpecificOne, MapIDsSpecificTwo );
    }

    {
    IntMapType MapIDsSpecificTwo;
    IntMapType MapCollectionIDs;


    this->SaveTraces< GoDBLineageRow >(iMapColorIDs, iMapCoordIDs,
                                       MapCollectionIDs, LineContent,
                                       this->m_NewLineageIDs, MapLineageIDs,
                                       MapTrackIDs, MapIDsSpecificTwo );

    std::vector< int >::iterator track_it = this->m_NewTracksIDs.begin();

    while( track_it != this->m_NewTracksIDs.end() )
      {
      GoDBTrackRow track;
      track.SetValuesForSpecificID( *track_it, this->m_DatabaseConnector );

      unsigned int OldID = track.GetMapValue< unsigned int >( "lineageID" );

      IntMapType::iterator lIt = MapLineageIDs.find( OldID );

      track.SetField< unsigned int >( "lineageID", lIt->second );
      track.SaveInDB(this->m_DatabaseConnector);

      ++track_it;
      }
    }

    {
    this->SaveTrackFamilyEntities(MapTrackIDs, MapTrackFamilyIDs, LineContent);
    }

    {
    this->SaveTraces< GoDBMeshRow >(iMapColorIDs, iMapCoordIDs, MapTrackIDs,
                                    LineContent, this->m_NewMeshIDs, MapMeshIDs,
                                    iMapCellTypeIDs, iMapSubCellTypeIDs);
    }

    if ( SaveIntensities )
      {
      this->SaveIntensityForMesh(LineContent, MapMeshIDs, iMapColorIDs);
      }

    {
    IntMapType MapIDsSpecificOne;
    IntMapType MapIDsSpecificTwo;

    this->SaveTraces< GoDBContourRow >(iMapColorIDs, iMapCoordIDs, MapMeshIDs,
                                       LineContent, this->m_NewContourIDs, MapContourIDs,
                                       MapIDsSpecificOne, MapIDsSpecificTwo );
    }
}

//--------------------------------------------------------------------------

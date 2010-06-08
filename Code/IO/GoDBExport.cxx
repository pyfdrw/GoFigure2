/*=========================================================================
  Author: $Author$  // Author of last commit
  Version: $Revision$  // Revision of last commit
  Date: $Date$  // Date of last commit
=========================================================================*/

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

#include "GoDBExport.h"
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
#include "GoDBLineageRow.h"


//--------------------------------------------------------------------------
GoDBExport::GoDBExport(std::string iServerName,std::string iLogin,
  std::string iPassword, int iImagingSessionID,std::string iFilename)
{
  this->m_ServerName = iServerName;
  this->m_Login = iLogin;
  this->m_Password = iPassword;
  this->m_ImagingSessionID = iImagingSessionID;
  this->m_outfile.open ( iFilename.c_str(), std::ios::out );
}

//--------------------------------------------------------------------------
GoDBExport::~GoDBExport()
{
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBExport::ExportContours( )
{
  std::string NameDocXml = "ExportTraces";
  int VersionNumber = 1;
  this->OpenDBConnection();
  this->m_outfile << "<?xml version=\"1.0\" ?>" << std::endl;
  this->m_outfile << "<";
  this->m_outfile << NameDocXml;
  this->m_outfile << " version=\"";
  this->m_outfile << VersionNumber;
  this->m_outfile << "\">"<<std::endl;

  this->WriteOnTheOutputFile("imagingsession",this->GetImagingSessionInfoFromDB());
  this->UpdateAllVectorForTracesIDs();
  this->WriteTheColorsInfoFromDatabase();
  this->WriteCellTypeAndSubCellTypeInfoFromDatabase();
  this->WriteCoordinatesInfoFromDatabase();
  this->WriteLineagesInfoFromDatabase();
  this->WriteTracksInfoFromDatabase();
  this->WriteMeshesInfoFromDatabase();
  this->WriteContoursInfoFromDatabase();
  this->CloseDBConnection();
  this->m_outfile << this->GetNameWithSlashBrackets(NameDocXml);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
std::vector<std::pair<std::string,std::string> > 
  GoDBExport::GetImagingSessionInfoFromDB()
{
  std::vector<std::pair<std::string,std::string> > infoImgSession;
  infoImgSession.push_back(this->GetOneInfoFromDBForImgSession("Name"));
  infoImgSession.push_back(this->GetOneInfoFromDBForImgSession("CreationDate"));
  infoImgSession.push_back(this->GetOneInfoFromDBForImgSession("MicroscopeName"));

  return infoImgSession;  
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
std::pair<std::string,std::string> GoDBExport::GetOneInfoFromDBForImgSession(
  std::string iNameInfo)
{
  std::pair<std::string,std::string> OneInfo;
  OneInfo.first = iNameInfo;
  OneInfo.second = ListSpecificValuesForOneColumn(
    this->m_DatabaseConnector,"imagingsession", iNameInfo,"ImagingSessionID",
    ConvertToString<int>(this->m_ImagingSessionID)).at(0);
  return OneInfo;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBExport::UpdateVectorContourIDs()
{
  this->m_VectorContourIDs = ListSpecificValuesForOneColumn(
    this->m_DatabaseConnector,"contour", "ContourID","imagingsessionID",
    ConvertToString<int>(this->m_ImagingSessionID));
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBExport::UpdateVectorMeshIDsWithContours()
{
  this->m_VectorMeshIDs = ListSpecificValuesForOneColumn(
    this->m_DatabaseConnector,"contour", "meshID","ContourID",
    this->m_VectorContourIDs,true,true);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBExport::UpdateVectorTrackIDsToExportInfo()
{
  if(!this->m_VectorMeshIDs.empty())
   {
   this->m_VectorTrackIDs = ListSpecificValuesForOneColumn(
     this->m_DatabaseConnector,"mesh", "TrackID","MeshID",
     this->m_VectorMeshIDs,true,true);
   }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBExport::UpdateVectorLineageIDsToExportInfo()
{
  if(!this->m_VectorTrackIDs.empty())
   {
   this->m_VectorLineageIDs = ListSpecificValuesForOneColumn(
     this->m_DatabaseConnector,"track", "LineageID","TrackID",
     this->m_VectorTrackIDs,true,true);
   }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBExport::UpdateAllVectorForTracesIDs()
{
  this->UpdateVectorContourIDs();
  this->UpdateVectorMeshIDsWithContours();
  this->UpdateVectorTrackIDsToExportInfo();
  this->UpdateVectorLineageIDsToExportInfo();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBExport::WriteTheColorsInfoFromDatabase()
{
  /*std::vector<std::string> ListColorIDs = GetSamefieldFromTwoTables(
    this->m_DatabaseConnector,"contour","mesh","ColorID",
    "ImagingSessionID",ConvertToString<int>(this->m_ImagingSessionID),
    "MeshID",iListMeshIDsWithContours);
  std::vector<std::string> TablesNames(4);
  TablesNames[0] = "contour";
  TablesNames[1] = "mesh";
  TablesNames[2] = "track";
  TablesNames[3] = "lineage";
  std::vector<std::string> FieldNames(4);
  FieldNames[0] = "ContourID";
  FieldNames[1] = "MeshID";
  FieldNames[2] = "TrackID";
  FieldNames[3] = "LineageID";
  std::vector<std::vector<std::string> > VectorTracesIDs(4);
  VectorTracesIDs[0] = this->m_VectorContourIDs;
  VectorTracesIDs[1] = this->m_VectorMeshIDs;
  VectorTracesIDs[2] = this->m_VectorTrackIDs;
  VectorTracesIDs[4] = this->m_VectorLineageIDs;*/
  std::vector<std::string> TablesNames;
  std::vector<std::string> FieldNames;
  std::vector<std::vector<std::string> > VectorTracesIDs;
  this->GetVectorsTableNamesTracesIDsAndFields(
    TablesNames,VectorTracesIDs,FieldNames);
  std::vector<std::string> ColumnNames(1);
  ColumnNames[0] = "ColorID";
  std::vector<std::string> ListColorIDs = GetSameFieldsFromSeveralTables(
    this->m_DatabaseConnector,ColumnNames,TablesNames,FieldNames,VectorTracesIDs);
  this->WriteTableInfoFromDB<GoDBColorRow>(ListColorIDs);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBExport::WriteCellTypeAndSubCellTypeInfoFromDatabase()
{
  std::vector<std::string> ListCellTypeIDs = ListSpecificValuesForOneColumn(
    this->m_DatabaseConnector,"mesh", "CellTypeID","MeshID",
    this->m_VectorMeshIDs,true,true);
  this->WriteTableInfoFromDB<GoDBCellTypeRow>(ListCellTypeIDs);
  std::vector<std::string> ListSubCellTypeIDs = 
    ListSpecificValuesForOneColumn(this->m_DatabaseConnector,
    "mesh", "SubCellularID","MeshID",this->m_VectorMeshIDs,true,true);
  this->WriteTableInfoFromDB<GoDBSubCellTypeRow>(ListSubCellTypeIDs);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBExport::WriteCoordinatesInfoFromDatabase()
{
  std::vector<std::string> TablesNames;
  std::vector<std::string> FieldNames;
  std::vector<std::vector<std::string> > VectorTracesIDs;
  this->GetVectorsTableNamesTracesIDsAndFields(TablesNames,
    VectorTracesIDs,FieldNames);
  std::vector<std::string> ColumnNames(2);
  ColumnNames[0] = "CoordIDMax";
  ColumnNames[1] = "CoordIDMin";
  std::vector<std::string> ListCoordIDs = GetSameFieldsFromSeveralTables(
    this->m_DatabaseConnector,ColumnNames,TablesNames,FieldNames,VectorTracesIDs);
  /*std::vector<std::string> ListCoordIDs = GetSamefieldsFromTwoTables(
    this->m_DatabaseConnector,"contour","mesh","CoordIDMax","CoordIDMin",
    "ImagingSessionID",ConvertToString<int>(this->m_ImagingSessionID),
    "MeshID",this->m_VectorMeshIDs);*/

  this->WriteTableInfoFromDB<GoDBCoordinateRow>(ListCoordIDs);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBExport::WriteLineagesInfoFromDatabase()
{
  if(!this->m_VectorLineageIDs.empty())
    {
    this->WriteTableInfoFromDB<GoDBLineageRow>(this->m_VectorLineageIDs);
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBExport::WriteTracksInfoFromDatabase()
{
  if(!this->m_VectorTrackIDs.empty())
    {
    this->WriteTableInfoFromDB<GoDBTrackRow>(this->m_VectorTrackIDs);
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBExport::WriteMeshesInfoFromDatabase()
{
  if(!this->m_VectorMeshIDs.empty())
    {
    this->WriteTableInfoFromDB<GoDBMeshRow>(this->m_VectorMeshIDs);
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------

void GoDBExport::WriteContoursInfoFromDatabase()
{
  std::vector<std::string> ListContoursIDs = ListSpecificValuesForOneColumn(
    this->m_DatabaseConnector,"contour", "ContourID","ImagingSessionID",
    ConvertToString<int>(this->m_ImagingSessionID),true);
  this->WriteTableInfoFromDB<GoDBContourRow>(ListContoursIDs);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBExport::WriteOnTheOutputFile(std::string iNameOfEntity,
  std::vector<std::pair<std::string,std::string> > iInfoToWrite)
{  
  this->AddTabulation();
  this->m_outfile << GetNameWithBrackets(iNameOfEntity)<<std::endl;
  std::vector<std::pair<std::string,std::string> >::iterator iter =
    iInfoToWrite.begin();
  while(iter!=iInfoToWrite.end())
    {
    this->AddTabulation();
    this->AddTabulation();
    this->m_outfile << this->GetNameWithBrackets(iter->first);
    this->m_outfile << iter->second;
    this->m_outfile << this->GetNameWithSlashBrackets(iter->first)<<std::endl;
    iter++;
    }
  this->AddTabulation();
  this->m_outfile << GetNameWithSlashBrackets(iNameOfEntity)<<std::endl;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBExport::WriteNumberOfEntities(std::string iNameOfEntity,int iNumber)
{
  this->AddTabulation();
  std::string NameToWrite = "NumberOf";
  NameToWrite += iNameOfEntity;
  this->m_outfile << this->GetNameWithBrackets(NameToWrite);
  this->m_outfile << iNumber;
  this->m_outfile << this->GetNameWithSlashBrackets(NameToWrite)<<std::endl;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
std::string GoDBExport::GetNameWithBrackets(std::string iName)
{
  std::stringstream NameWithBrackets;
  NameWithBrackets << "<";
  NameWithBrackets << iName;
  NameWithBrackets << ">";
  return NameWithBrackets.str();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
std::string GoDBExport::GetNameWithSlashBrackets(std::string iName)
{
  std::stringstream NameWithBrackets;
  NameWithBrackets << "</";
  NameWithBrackets << iName;
  NameWithBrackets << ">";
  return NameWithBrackets.str();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBExport::AddTabulation()
{
  this->m_outfile << "  ";
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBExport::OpenDBConnection()
{
  this->m_DatabaseConnector = OpenDatabaseConnection(m_ServerName,
    m_Login,m_Password,"gofiguredatabase");
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBExport::CloseDBConnection()
{
  CloseDatabaseConnection(m_DatabaseConnector);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBExport::GetVectorsTableNamesTracesIDsAndFields( 
  std::vector<std::string> & ioVectorTableNames,
  std::vector<std::vector<std::string> > & ioVectorTracesIDs,
  std::vector<std::string> & ioVectorFields)
{
  if (!this->m_VectorContourIDs.empty())
    {
    ioVectorTableNames.push_back("contour");
    ioVectorFields.push_back("ContourID");
    ioVectorTracesIDs.push_back(this->m_VectorContourIDs);
    }
  if (!this->m_VectorMeshIDs.empty())
    {
    ioVectorTableNames.push_back("mesh");
    ioVectorFields.push_back("MeshID");
    ioVectorTracesIDs.push_back(this->m_VectorMeshIDs);
    }
  if (!this->m_VectorTrackIDs.empty())
    {
    ioVectorTableNames.push_back("track");
    ioVectorFields.push_back("TrackID");
    ioVectorTracesIDs.push_back(this->m_VectorTrackIDs);
    }
  if (!this->m_VectorLineageIDs.empty())
    {
    ioVectorTableNames.push_back("lineage");
    ioVectorFields.push_back("LineageID");
    ioVectorTracesIDs.push_back(this->m_VectorLineageIDs);
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
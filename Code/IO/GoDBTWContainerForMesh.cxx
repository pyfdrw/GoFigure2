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
#include "GoDBTWContainerForMesh.h"

GoDBTWContainerForMesh::GoDBTWContainerForMesh(int iImgSessionID) :
  GoDBTWContainerForContourMesh("mesh", "track", iImgSessionID),
  m_MeshAttributes(NULL)
{
  this->SetSpecificInfoForMeshTable();
  //checker the columnsinfo
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
GoDBTWContainerForMesh::~GoDBTWContainerForMesh()
{
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBTWContainerForMesh::SetColumnsInfoBasedOnChannelsInfo()
{
  //Get the info for the total intensities per channel:
  GoDBTraceInfoForTableWidget temp;

  std::pair< GoDBTraceInfoForTableWidget,
             std::vector< std::string > > PairTemp;
  size_t                                  NumberOfChannels = this->m_ChannelsInfo.size();

  if ( this->m_ChannelsInfo.empty() )
    {
    std::cout << "No info for the channels" << std::endl;
    std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
    std::cout << std::endl;
    return;
    }
  for ( size_t i = 0; i < NumberOfChannels; i++ )
    {
    std::string InfoName = "TotalIntensityForChannelID";
    InfoName += this->m_ChannelsInfo.at(i).at(1);
    temp.InfoName = InfoName;
    temp.ColumnNameDatabase = "Value";
    std::string ColumnNameTableWidget = "T.I.";
    ColumnNameTableWidget += this->m_ChannelsInfo.at(i).at(0);
    temp.ColumnNameTableWidget = ColumnNameTableWidget;
    temp.ToolTip = "Total Intensity For the Channel";
    temp.TableNameDatabase = "intensity";
    temp.TableForeignKeyDatabase = "MeshID";
    temp.TableKeyDatabase = "MeshID";
    m_ColumnsInfos.push_back(temp);
    PairTemp.first = temp;
    m_RowContainer.push_back(PairTemp);
    temp.Clear();
    }
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBTWContainerForMesh::SetSpecificInfoForMeshTable()
{
  GoDBTraceInfoForTableWidget temp;

  std::pair< GoDBTraceInfoForTableWidget,
             std::vector< std::string > > PairTemp;

  //Get the info for the Volume:
  temp.InfoName = "Volume";
  temp.ColumnNameTableWidget = "Volume";
  m_ColumnsInfos.push_back(temp);
  PairTemp.first = temp;
  m_RowContainer.push_back(PairTemp);
  temp.Clear();

  //Get the info for the Surface Area:
  temp.InfoName = "SurfaceArea";
  temp.ColumnNameTableWidget = "SurfaceArea";
  m_ColumnsInfos.push_back(temp);
  PairTemp.first = temp;
  m_RowContainer.push_back(PairTemp);
  temp.Clear();

  //Get the info for CelltypeID:
  temp.InfoName = "CellTypeID";
  temp.ColumnNameDatabase = "CellTypeID";
  temp.TableNameDatabase = this->m_TracesName;
  temp.TableForeignKeyDatabase = "CellTypeID";
  temp.TableKeyDatabase = "CellTypeID";
  m_ColumnsInfos.push_back(temp);
  PairTemp.first = temp;
  m_RowContainer.push_back(PairTemp);
  temp.Clear();

  //Get the info for the celltype name:
  temp.ColumnNameTableWidget = "CellType";
  temp.ColumnNameDatabase = "Name";
  temp.TypeName = "string";
  temp.TableNameDatabase = "celltype";
  temp.InfoName = "CellTypeName";
  temp.TableForeignKeyDatabase = "CellTypeID";
  temp.TableKeyDatabase = "CellTypeID";
  m_ColumnsInfos.push_back(temp);
  PairTemp.first = temp;
  m_RowContainer.push_back(PairTemp);
  temp.Clear();

  //Get the info for SubCelltypeID:
  temp.InfoName = "SubCellTypeID";
  temp.ColumnNameDatabase = "SubCellularID";
  temp.TableNameDatabase = this->m_TracesName;
  temp.TableForeignKeyDatabase = "SubCellularID";
  temp.TableKeyDatabase = "SubCellularID";
  m_ColumnsInfos.push_back(temp);
  PairTemp.first = temp;
  m_RowContainer.push_back(PairTemp);
  temp.Clear();

  //Get the info for the SubCellType name:
  temp.ColumnNameTableWidget = "SubCellType";
  temp.ColumnNameDatabase = "Name";
  temp.TypeName = "string";
  temp.TableNameDatabase = "subcellulartype";
  temp.InfoName = "SubCellTypeName";
  temp.TableForeignKeyDatabase = "SubCellularID";
  temp.TableKeyDatabase = "SubCellularID";
  m_ColumnsInfos.push_back(temp);
  PairTemp.first = temp;
  m_RowContainer.push_back(PairTemp);
  temp.Clear();
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBTWContainerForMesh::FillRowContainerForMeshValues(
  vtkMySQLDatabase *iDatabaseConnector, std::vector< std::string > iVectMeshIDs)
{
  std::vector< std::vector< std::string > > ValuesToFill;
  std::vector< std::string >                SelectFields;
  this->GetValuesForIntensities(iDatabaseConnector, iVectMeshIDs, ValuesToFill,
                                SelectFields);
  //if the mesh was just created from the visu (so there is only one):
  if ( this->m_MeshAttributes != 0 && iVectMeshIDs.size() == 1 )
    {
    this->GetValuesForSurfaceVolume(ValuesToFill,
                                    SelectFields);
    }

  this->FillRowContainer(
    ValuesToFill, SelectFields, "ColumnNameTableWidget");
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBTWContainerForMesh::GetValuesForIntensities(
  vtkMySQLDatabase *iDatabaseConnector,
  std::vector< std::string > iVectMeshIDs,
  std::vector< std::vector< std::string > > & ioValuesToFill,
  std::vector< std::string >               & ioSelectFields)
{
  if ( this->m_ChannelsInfo.empty() )
    {
    this->SetChannelsInfo(iDatabaseConnector);
    }
  //fill the columns names to be filled in the row container:
  for ( unsigned int i = 0; i < this->m_ChannelsInfo.size(); i++ )
    {
    std::string NameTableWidgetColumn = "T.I.";
    NameTableWidgetColumn += this->m_ChannelsInfo.at(i).at(0);
    ioSelectFields.push_back(NameTableWidgetColumn);
    }

  if ( iVectMeshIDs.empty() )
    {
    return;
    }
  //get the needed data from the database:
  std::vector< std::string > SelectedFields(3);
  SelectedFields.at(0) = "mesh.meshid";
  SelectedFields.at(1) = "left(points,10) as points";
  SelectedFields.at(2) = "value";
  std::vector< std::string > ResultQuery;

  if ( iVectMeshIDs.size() == 1 )
    {
    this->GetIntensityValuesForOneMesh(iVectMeshIDs.front(),
                                       ioValuesToFill, iDatabaseConnector);
    }
  else
    {
    std::vector< FieldWithValue > Condition(1);
    FieldWithValue                ImgSession = { "ImagingSessionID", ConvertToString< int >(this->m_ImgSessionID), "=" };
    Condition[0] = ImgSession;
    FieldWithValue JoinCondition = { this->m_TracesIDName, this->m_TracesIDName, "=" };
    ResultQuery = GetAllSelectedValuesFromTwoTables(
        iDatabaseConnector, this->m_TracesName, "intensity", SelectedFields,
        JoinCondition, Condition);
    this->GetValuesToFillForIntensityFromQueryResults(
      ResultQuery, iVectMeshIDs, ioValuesToFill);
    }
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBTWContainerForMesh::GetIntensityValuesForOneMesh(std::string iMeshID,
                                                          std::vector< std::vector< std::string > > & ioValuesToFill,
                                                          vtkMySQLDatabase *iDatabaseConnector)
{
  std::vector< std::string > temp;
  for ( unsigned int i = 0; i < this->m_ChannelsInfo.size(); i++ )
    {
    std::string                   ChannelIDValue = this->m_ChannelsInfo.at(i).at(1);
    std::vector< FieldWithValue > Conditions(2);
    FieldWithValue                MeshID = { "MeshID", iMeshID, "=" };
    FieldWithValue                ChannelID = { "ChannelID", ChannelIDValue, "=" };
    Conditions[0] = MeshID;
    Conditions[1] = ChannelID;

    int ValueIntensity = FindOneID(iDatabaseConnector, "intensity",
                                   "Value", Conditions);
    if ( ValueIntensity == -1 )
      {
      temp.push_back("");
      }
    else
      {
      temp.push_back( ConvertToString< int >(ValueIntensity) );
      }
    }
  ioValuesToFill.push_back(temp);
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBTWContainerForMesh::GetValuesToFillForIntensityFromQueryResults
  (std::vector< std::string > iResultQuery, std::vector< std::string > iVectMeshIDs,
  std::vector< std::vector< std::string > > & ioValuesToFill)
{
  std::vector< std::string >::iterator iterResult = iResultQuery.begin();

  //iResultQuery has meshid, points, value for channel0, meshid,points,
  //value for channel1....
  std::vector< std::string >::iterator iterMeshID = iVectMeshIDs.begin();

  size_t i;

  while ( ( iterMeshID != iVectMeshIDs.end() )
          && ( iterResult != iResultQuery.end() ) )
    {
    ++iterResult;
    std::vector< std::string > temp;
    std::string                Points = *iterResult;
    std::string                IntensityValue;
    if ( ( Points == "0" ) || ( Points == "" ) ) //if the mesh has no points, he
                                                 // has no intensity
      {
      IntensityValue = "";

      for ( i = 0; i < this->m_ChannelsInfo.size(); i++ )
        {
        temp.push_back(IntensityValue);
        }

      // iterResult = iterResult + 2;
      if( iterResult != iResultQuery.end() )
        {
        ++iterResult;
        }
      else
        {
        itkGenericExceptionMacro( << "iterResult == iResultQuery" );
        }
      if( iterResult != iResultQuery.end() )
        {
        ++iterResult;
        }
      else
        {
        itkGenericExceptionMacro( << "iterResult == iResultQuery" );
        }
      }
    else
      {
      if( iterResult != iResultQuery.end() )
        {
        ++iterResult;
        }
      else
        {
        itkGenericExceptionMacro( << "iterResult == iResultQuery" );
        }

      for ( i = 0; i < this->m_ChannelsInfo.size() - 1; i++ )
        {
        if ( iterResult != iResultQuery.end() )
          {
          IntensityValue = *iterResult;
          temp.push_back(IntensityValue);
          }

        // iterResult = iterResult + 3;
        if( iterResult != iResultQuery.end() )
          {
          ++iterResult;
          }
        else
          {
          itkGenericExceptionMacro( << "iterResult == iResultQuery" );
          }
        if( iterResult != iResultQuery.end() )
          {
          ++iterResult;
          }
        else
          {
          itkGenericExceptionMacro( << "iterResult == iResultQuery" );
          }
        if( iterResult != iResultQuery.end() )
          {
          ++iterResult;
          }
        else
          {
          itkGenericExceptionMacro( << "iterResult == iResultQuery" );
          }
        }
      IntensityValue = *iterResult;
      temp.push_back(IntensityValue);

      if( iterResult != iResultQuery.end() )
        {
        ++iterResult;
        }
      else
        {
        itkGenericExceptionMacro( << "iterResult == iResultQuery" );
        }
      }
    ioValuesToFill.push_back(temp);
    ++iterMeshID;
    }
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBTWContainerForMesh::FillRowContainerForMeshValues(
  vtkMySQLDatabase *iDatabaseConnector, int iMeshID)
{
  std::vector< std::string > MeshIDs;
  MeshIDs.push_back( ConvertToString< int >(iMeshID) );
  this->FillRowContainerForMeshValues(iDatabaseConnector, MeshIDs);
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBTWContainerForMesh::GetValuesForSurfaceVolume(
  std::vector< std::vector< std::string > > & ioValuesToFill,
  std::vector< std::string >               & ioSelectFields)
{
  if ( this->m_MeshAttributes != 0 )
    {
    if ( ioValuesToFill.size() != 1 )
      {
      std::cout << "more than 1 mesh for volume and surface values" << std::endl;
      std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
      std::cout << std::endl;
      return;
      }
    else
      {
      std::vector< std::string > temp = ioValuesToFill.at(0);
      ioSelectFields.push_back("Volume");
      temp.push_back( ConvertToString< double >(this->m_MeshAttributes->m_Volume) );
      ioSelectFields.push_back("SurfaceArea");
      temp.push_back( ConvertToString< double >(this->m_MeshAttributes->m_Area) );
      ioValuesToFill.clear();
      ioValuesToFill.push_back(temp);
      }
    }
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
GoDBTableWidgetContainer::TWContainerType
GoDBTWContainerForMesh::GetContainerLoadedWithAllFromDB(
  vtkMySQLDatabase *iDatabaseConnector, std::list<unsigned int> iListTPs)
{
  GoDBTableWidgetContainer::GetContainerLoadedWithAllFromDB(iDatabaseConnector, iListTPs);
  std::vector< std::string > VectMeshIDs;
  if (iListTPs.empty() )
    {
    VectMeshIDs = ListSpecificValuesForOneColumn(
        iDatabaseConnector, "mesh", "MeshID", "ImagingSessionID",
        ConvertToString< unsigned int >(this->m_ImgSessionID) );
    }
  else
    {
    FieldWithValue joinCondition = {"CoordIDMin", "CoordID", "="};
    FieldWithValue andCondition = {"ImagingSessionID", ConvertToString< unsigned int >(this->m_ImgSessionID), "="};
    std::vector< std::string > VectorValues = ListUnsgIntToVectorString(iListTPs);
      std::list<unsigned int> MeshIDs = GetListValuesFromTwoTablesAndCondition(
      iDatabaseConnector, "mesh", "coordinate","MeshID", joinCondition,
      "coordinate.TCoord", VectorValues, andCondition);
    VectMeshIDs = ListUnsgIntToVectorString(MeshIDs);
    }

  this->FillRowContainerForMeshValues(iDatabaseConnector, VectMeshIDs);
  return this->m_RowContainer;
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBTWContainerForMesh::SetChannelsInfo(
  vtkMySQLDatabase *iDatabaseConnector)
{
  if ( this->m_ChannelsInfo.empty() )
    {
    std::vector< std::string > SelectFields;
    SelectFields.push_back("Name");
    SelectFields.push_back("channel.ChannelID");
    std::vector< std::string > JoinTablesOnTraceTable;
    JoinTablesOnTraceTable.push_back("channel");
    JoinTablesOnTraceTable.push_back("image.ChannelID = channel.ChannelID");

    this->m_ChannelsInfo = GetValuesFromSeveralTables(
        iDatabaseConnector, "image", SelectFields, "ImagingSessionID",
        ConvertToString< unsigned int >(this->m_ImgSessionID), JoinTablesOnTraceTable, true);
    //this->SetSpecificColumnsInfoForMesh();
    this->SetColumnsInfoBasedOnChannelsInfo();
    }
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
GoDBTableWidgetContainer::TWContainerType
GoDBTWContainerForMesh::GetContainerForOneSpecificTrace(
  vtkMySQLDatabase *iDatabaseConnector, int iTraceID)
{
  GoDBTableWidgetContainer::GetContainerForOneSpecificTrace(iDatabaseConnector,
                                                            iTraceID);
  this->FillRowContainerForMeshValues(iDatabaseConnector, iTraceID);
  this->m_MeshAttributes = 0;
  return this->m_RowContainer;
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void GoDBTWContainerForMesh::SetMeshAttributes(
  GoFigureMeshAttributes *iMeshAttributes)
{
  this->m_MeshAttributes = iMeshAttributes;
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------

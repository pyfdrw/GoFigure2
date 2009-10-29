/*=========================================================================
  Author: $Author: lydiesouhait $  // Author of last commit
  Version: $Rev: 542 $  // Revision of last commit
  Date: $Date: 2009-08-06 16:08:10 -0400 (Thu, 06 Aug 2009) $  // Date of last commit
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
#include "SelectQueryDataBaseHelper.h"
#include "vtkMySQLDatabase.h"
#include "vtkSQLQuery.h"
#include "vtkStdString.h"
#include "vtkVariant.h"
#include <sstream>
#include <string>


std::vector<std::string> ListAllValuesForOneColumn(vtkMySQLDatabase* DatabaseConnector,
  std::string ColumnName, std::string TableName)
{
  std::vector< std::string > result;

  vtkSQLQuery* query = DatabaseConnector->GetQueryInstance();
  std::stringstream querystream;
  querystream << "SELECT ";
  querystream << ColumnName;
  querystream << " FROM ";
  querystream << TableName;
  querystream << ";";

  query->SetQuery( querystream.str().c_str() );
  if ( !query->Execute() )
    {
    itkGenericExceptionMacro(
      << "List of all fields query failed"
      << query->GetLastErrorText() );
    query->Delete();
    return result;
    }
  while( query->NextRow() )
    {
    result.push_back( query->DataValue( 0 ).ToString() );
    }
  query->Delete();

  return result;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
std::map<std::string,std::string> MapTwoColumnsFromTable(
  vtkMySQLDatabase* DatabaseConnector,std::string ColumnNameOne,
  std::string ColumnNameTwo,std::string TableName)
{
  std::map<std::string,std::string> Result;
  vtkSQLQuery* query = DatabaseConnector->GetQueryInstance();
  std::stringstream querystream;
  querystream << "SELECT ";
  querystream << ColumnNameOne;
  querystream << " , ";
  querystream << ColumnNameTwo;
  querystream << " FROM ";
  querystream << TableName;
  querystream << ";";

  query->SetQuery( querystream.str().c_str() );
  if ( !query->Execute() )
    {
    itkGenericExceptionMacro(
      << "List of all fields query failed"
      << query->GetLastErrorText() );
    query->Delete();
    return Result;
    }
  while( query->NextRow() )
    {
    Result[query->DataValue(0).ToString()]= query->DataValue(1).ToString();
    }
  query->Delete();
  return Result;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
std::map<std::string,std::string> MapTwoColumnsFromTable(
  vtkMySQLDatabase* DatabaseConnector,std::string ColumnNameOne,
  std::string ColumnNameTwo,std::string TableName,
  std::string field, std::string value)
{
  std::map<std::string,std::string> Result;
  vtkSQLQuery* query = DatabaseConnector->GetQueryInstance();
  std::stringstream querystream;
  querystream << "SELECT ";
  querystream << ColumnNameOne;
  querystream << " , ";
  querystream << ColumnNameTwo;
  querystream << " FROM ";
  querystream << TableName;
  querystream << " WHERE ";
  querystream << field;
  querystream << " = '";
  querystream << value;
  querystream << "';";

  query->SetQuery( querystream.str().c_str() );
  if ( !query->Execute() )
    {
    itkGenericExceptionMacro(
      << "List of all fields query failed"
      << query->GetLastErrorText() );
    query->Delete();
    return Result;
    }
  while( query->NextRow() )
    {
    Result[query->DataValue(0).ToString()]= query->DataValue(1).ToString();
    }
  query->Delete();
  return Result;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
std::vector<std::string> ListSpecificValuesForRow(
  vtkMySQLDatabase* DatabaseConnector,std::string TableName, std::string field,
  std::string value)
{
  std::vector< std::string > result;
  vtkSQLQuery* query = DatabaseConnector->GetQueryInstance();
  std::stringstream querystream;
  querystream << "SELECT * FROM ";
  querystream << TableName;
  querystream << " WHERE ";
  querystream << field;
  querystream << " = '";
  querystream << value;
  querystream << "';";

  query->SetQuery( querystream.str().c_str() );
  if ( !query->Execute() )
    {
    itkGenericExceptionMacro(
      << "List of all values of ExpID query failed"
      << query->GetLastErrorText() );
    query->Delete();
    return result;
    }

  while (query->NextRow())
    {
    for(  int i = 0; i < query->GetNumberOfFields(); i++)
      {
      result.push_back( query->DataValue( i ).ToString() );
      }
    }
  query->Delete();

  return result;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//query: "SELECT ColumnName FROM TableName WHERE (field1 = value1
//AND field2 = value2 AND field3 = value3);
int FindOneID(vtkMySQLDatabase* DatabaseConnector,
  std::string TableName, std::string ColumnName,
  std::string ColumnNameOne,std::string valueOne,
  std::string ColumnNameTwo,std::string valueTwo,
  std::string ColumnNameThree,std::string valueThree)
{
  int ID = -1;

  vtkSQLQuery* query = DatabaseConnector->GetQueryInstance();
  std::stringstream querystream;
  querystream << "SELECT ";
  querystream << ColumnName;
  querystream << " FROM ";
  querystream << TableName;
  querystream << " WHERE (";
  querystream << ColumnNameOne;
  querystream << " = '";
  querystream << valueOne;
  querystream << "' AND ";
  querystream << ColumnNameTwo;
  querystream << " = '";
  querystream << valueTwo;
  querystream << "' AND ";
  querystream << ColumnNameThree;
  querystream << " = '";
  querystream << valueThree;
  querystream << "');";

  query->SetQuery( querystream.str().c_str() );
  if ( !query->Execute() )
    {
    itkGenericExceptionMacro(
      << "The FindOneID query failed"
      << query->GetLastErrorText() );
    DatabaseConnector->Close();
    DatabaseConnector->Delete();
    query->Delete();
    return ID;
    }

  if (query->NextRow())
    {
    ID = query->DataValue(0).ToInt();
    }
  query->Delete();

  return ID;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int FindOneID(vtkMySQLDatabase* DatabaseConnector,
  std::string TableName, std::string ColumnName,
  std::string ColumnNameOne,std::string valueOne,
  std::string ColumnNameTwo,std::string valueTwo)
{
  int ID = -1;
  vtkSQLQuery* query = DatabaseConnector->GetQueryInstance();
  std::stringstream querystream;
  querystream << "SELECT ";
  querystream << ColumnName;
  querystream << " FROM ";
  querystream << TableName;
  querystream << " WHERE (";
  querystream << ColumnNameOne;
  querystream << " = '";
  querystream << valueOne;
  querystream << "' AND ";
  querystream << ColumnNameTwo;
  querystream << " = '";
  querystream << valueTwo;
  querystream << "');";

  query->SetQuery( querystream.str().c_str() );
  if ( !query->Execute() )
    {
    itkGenericExceptionMacro(
      << "The FindOneID query failed"
      << query->GetLastErrorText() );
    DatabaseConnector->Close();
    DatabaseConnector->Delete();
    query->Delete();
    return ID;
    }

  if (query->NextRow())
    {
    ID = query->DataValue(0).ToInt();
    }
  query->Delete();

  return ID;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//query: "SELECT ColumnName FROM TableName WHERE (field1 = value1
//AND field2 = value2 AND field3 = value3 AND field4 = value4);
int FindOneID(vtkMySQLDatabase* DatabaseConnector,
  std::string TableName, std::string ColumnName,
  std::string ColumnNameOne,std::string valueOne,
  std::string ColumnNameTwo,std::string valueTwo,
  std::string ColumnNameThree,std::string valueThree,
  std::string ColumnNameFour,std::string valueFour)
{
  int ID = -1;

  vtkSQLQuery* query = DatabaseConnector->GetQueryInstance();
  std::stringstream querystream;
  querystream << "SELECT ";
  querystream << ColumnName;
  querystream << " FROM ";
  querystream << TableName;
  querystream << " WHERE (";
  querystream << ColumnNameOne;
  querystream << " = '";
  querystream << valueOne;
  querystream << "' AND ";
  querystream << ColumnNameTwo;
  querystream << " = '";
  querystream << valueTwo;
  querystream << "' AND ";
  querystream << ColumnNameThree;
  querystream << " = '";
  querystream << valueThree;
  querystream << "' AND ";
  querystream << ColumnNameFour;
  querystream << " = '";
  querystream << valueFour;
  querystream << "');";

  query->SetQuery( querystream.str().c_str() );
  if ( !query->Execute() )
    {
    itkGenericExceptionMacro(
      << "The FindOneID query failed"
      << query->GetLastErrorText() );
    DatabaseConnector->Close();
    DatabaseConnector->Delete();
    query->Delete();
    return ID;
    }

  if (query->NextRow())
    {
    ID = query->DataValue(0).ToInt();
    }
  query->Delete();

  return ID;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
std::vector<std::string> ListSpecificValuesForOneColumn(
  vtkMySQLDatabase* DatabaseConnector,
  std::string TableName, std::string ColumnName,
  std::string field,std::string value)

{ std::vector< std::string > result;

  vtkSQLQuery* query = DatabaseConnector->GetQueryInstance();
  std::stringstream querystream;
  querystream << "SELECT ";
  querystream << ColumnName;
  querystream << " FROM ";
  querystream << TableName;
  querystream << " WHERE ";
  querystream << field;
  querystream << " = '";
  querystream << value;
  querystream << "';";

  query->SetQuery( querystream.str().c_str() );
  if ( !query->Execute() )
    {
    itkGenericExceptionMacro(
      << "List of all values of ExpID query failed"
      << query->GetLastErrorText() );
    DatabaseConnector->Close();
    DatabaseConnector->Delete();
    query->Delete();
    return result;
    }

  while (query->NextRow())
    {
    for( int i = 0; i < query->GetNumberOfFields(); i++)
      {
      result.push_back( query->DataValue( i ).ToString() );
      }
    }

  query->Delete();

  return result;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
std::string ReturnOnlyOneValue(vtkMySQLDatabase* DatabaseConnector,
  std::string TableName, std::string ColumnName,std::string field,
  std::string value)
{
  std::string result;

  vtkSQLQuery* query = DatabaseConnector->GetQueryInstance();
  std::stringstream querystream;
  querystream << "SELECT ";
  querystream << ColumnName;
  querystream << " FROM ";
  querystream << TableName;
  querystream << " WHERE ";
  querystream << field;
  querystream << " = '";
  querystream << value;
  querystream << "' LIMIT 1;";

  query->SetQuery( querystream.str().c_str() );
  if ( !query->Execute() )
    {
    itkGenericExceptionMacro(
      << "List of all values of ExpID query failed"
      << query->GetLastErrorText() );
    DatabaseConnector->Close();
    DatabaseConnector->Delete();
    query->Delete();
    return result;
    }

  while (query->NextRow())
    {
    for( int i = 0; i < query->GetNumberOfFields(); i++)
      {
      result = query->DataValue( i ).ToString();
      }
    }

  query->Delete();

  return result;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int MaxValueForOneColumnInTable(
  vtkMySQLDatabase* DatabaseConnector,
  std::string ColumnName,std::string TableName )
{
  int MaxValue = -1;

  vtkSQLQuery* query = DatabaseConnector->GetQueryInstance();
  std::stringstream querystream;
  querystream << "SELECT MAX(";
  querystream << ColumnName;
  querystream << ") FROM ";
  querystream << TableName;
  querystream << ";";

  query->SetQuery( querystream.str().c_str() );
  if ( !query->Execute() )
    {
    itkGenericExceptionMacro(
      << "find max value query failed"
      << query->GetLastErrorText() );
    DatabaseConnector->Close();
    DatabaseConnector->Delete();
    query->Delete();
    return MaxValue;
    }

  if (query->NextRow())
    {
    MaxValue = query->DataValue(0).ToInt();
    }
  query->Delete();

  return MaxValue;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int LastInsertID(std::string ServerName, std::string login,
  std::string Password, std::string DBName)
{
  int ID = -1;
  vtkMySQLDatabase * DatabaseConnector = vtkMySQLDatabase::New();
  DatabaseConnector->SetHostName( ServerName.c_str() );
  DatabaseConnector->SetUser( login.c_str() );
  DatabaseConnector->SetPassword( Password.c_str() );
  DatabaseConnector->SetDatabaseName( DBName.c_str() );
  if( !DatabaseConnector->Open() )
    {
    std::cerr << "Could not open database." << std::endl;
    std::cerr << "Find last insert ID can not be done."<< std::endl;
    DatabaseConnector->Close();
    DatabaseConnector->Delete();
    return ID;
    }

  vtkSQLQuery* query = DatabaseConnector->GetQueryInstance();
  query->SetQuery( "SELECT LAST_INSERT_ID();" );

  if ( !query->Execute() )
    {
    itkGenericExceptionMacro(
      << "replace value in DB query failed"
      << query->GetLastErrorText() );
    DatabaseConnector->Close();
    DatabaseConnector->Delete();
    query->Delete();
    return ID;
    }

  ID = query->DataValue(0).ToInt();
  DatabaseConnector->Close();
  DatabaseConnector->Delete();
  query->Delete();

  return ID;
}
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
#include "QueryBuilderHelper.h"

#include <sstream>


std::string GetFirstPartQueryForTracesInfo(std::string iTraceName,std::string iCollectionName)
{
  std::stringstream Querystream;
  Querystream << "SELECT ";
  Querystream << iTraceName;
  Querystream << ".";
  Querystream << iTraceName;
  Querystream << "ID, ";
  Querystream << iTraceName;
  Querystream << ".";
  Querystream << iCollectionName;
  Querystream << "ID, ";
  Querystream << iTraceName;
  Querystream << ".Points, coordinate.TCoord, color.Red,\
                 color.Green, color.Blue, color.Alpha from (";
  Querystream << iTraceName;
  Querystream << " left join coordinate on coordinate.CoordID = ";
  Querystream << iTraceName;
  Querystream << ".coordIDMax) left join color on ";
  Querystream << iTraceName;

  Querystream << ".colorID = color.colorID  where ";
  return Querystream.str().c_str();
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
std::string GetSecondPartQueryForTracesInfo(std::string TraceName,
                                            std::vector<int> iVectIDs)
{
  std::stringstream Querystream;
  unsigned int i;
  if (iVectIDs.size()>1)
    {
    Querystream << "(";
    }
  for ( i = 0; i < iVectIDs.size() - 1; i++ )
    {
    Querystream << TraceName;
    Querystream << "ID = '";
    Querystream << iVectIDs[i];
    Querystream << "' OR ";
    }
  Querystream << TraceName;
  Querystream << "ID = '";
  Querystream << iVectIDs[i];
  if (iVectIDs.size()>1)
    {
    Querystream << "')";
    }
  else
    Querystream << "'";
return Querystream.str().c_str();
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
std::string SelectQueryStream(std::string iTable, std::string iColumn, std::string iField,
                              std::string iValue)
{
  std::stringstream querystream;

  querystream << "SELECT ";
  querystream << iColumn;
  querystream << " FROM ";
  querystream << iTable;
  querystream << " WHERE ";
  querystream << iField;
  querystream << " = '";
  querystream << iValue;
  querystream << "'";
  return querystream.str();
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
std::string SelectQueryStreamListConditions(std::string iTable,
                                            std::string iColumn, std::string iField,
                                            std::vector< std::string > iListValues, bool Distinct)
{
  std::stringstream querystream;

  if( !iListValues.empty() )
    {
    querystream << "SELECT ";
    if ( Distinct )
      {
      querystream << "DISTINCT ";
      }
    querystream << iColumn;
    querystream << " FROM ";
    querystream << iTable;
    querystream << " WHERE (";
    unsigned int i;
    for ( i = 0; i < iListValues.size() - 1; i++ )
      {
      querystream << iField;
      querystream << " = '";
      querystream << iListValues[i];
      querystream << "' OR ";
      }
    querystream << iField;
    querystream << " = '";
    querystream << iListValues[i];
    querystream << "')";
    }

  return querystream.str();
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
std::string SelectQueryStreamListConditions(std::string iTable,
                                            std::vector< std::string > iListColumn, std::string iField,
                                            std::vector< std::string > iListValues, bool Distinct)
{
  std::stringstream querystream;

  querystream << "SELECT ";
  if ( Distinct )
    {
    querystream << "DISTINCT ";
    }
  unsigned int j;
  for ( j = 0; j < iListColumn.size() - 1; j++ )
    {
    querystream << iListColumn[j];
    querystream << ", ";
    }
  querystream << iListColumn[j];
  querystream << " FROM ";
  querystream << iTable;
  querystream << " WHERE (";
  unsigned int i;
  for ( i = 0; i < iListValues.size() - 1; i++ )
    {
    querystream << iField;
    querystream << " = '";
    querystream << iListValues[i];
    querystream << "' OR ";
    }
  querystream << iField;
  querystream << " = '";
  querystream << iListValues[i];
  querystream << "')";
  return querystream.str();
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
std::string SelectWithJoinNullIncluded(std::string iSelectQuery,
                                       std::string iJoinOn,
                                       bool doublon)
{
  std::stringstream QueryStream;

  QueryStream << iSelectQuery;
  QueryStream << " UNION ";
  if (doublon)
    {
    QueryStream << " ALL ";
    }
  //QueryStream << iSelectQuery;
  size_t            posWhere = iSelectQuery.find("WHERE", 0);
  std::stringstream SecondPart;
  if ( posWhere != std::string::npos )
    {
    size_t posCloseBracket = iSelectQuery.find(")", iSelectQuery.size() - 1);
      {
      //if an end bracket is found, remove it:
      if ( posCloseBracket != std::string::npos )
        {
        SecondPart << iSelectQuery.substr(0, posCloseBracket - 1);
        }
      //if an end bracket is not found, add an opening bracket after where:
      else
        {
        SecondPart << iSelectQuery.substr(0, posWhere + 6);
        SecondPart << "(";
        size_t BegEndQuery = posWhere + 6;
        size_t Length = iSelectQuery.size() - BegEndQuery;
        SecondPart << iSelectQuery.substr(posWhere + 6, Length);
        }
      }
    }
  else
    {
    SecondPart << iSelectQuery;
    SecondPart << "WHERE ";
    }
  QueryStream << SecondPart.str();
  QueryStream << " AND ";
  QueryStream << iJoinOn;
  QueryStream << " IS NULL";
  if ( posWhere != std::string::npos )
    {
    QueryStream << ");";
    }
  else
    {
    QueryStream << ";";
    }

  return QueryStream.str();
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
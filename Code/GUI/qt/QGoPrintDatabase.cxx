/*=========================================================================
  Author: $Author: lsouhait $  // Author of last commit
  Version: $Revision: 270 $  // Revision of last commit
  Date: $Date: 2009-06-11 15:11:56 -0400 (Thu, 11 Jun 2009) $  // Date of last commit
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


#include <qstringlist.h>
#include <qwidget.h>
#include <QTableWidgetItem>
#include "QGoPrintDatabase.h"
#include "vtkMySQLDatabase.h"
#include "vtkSQLQuery.h"
#include "vtkStringArray.h"
#include "vtkStdString.h"

QGoPrintDatabase::QGoPrintDatabase()
{
  this->setupUi( this );
  DBTabWidget->setTabPosition(QTabWidget::West);
  DBTabWidget->setTabShape(QTabWidget::Triangular);
  DBTabWidget->removeTab(0);

}

QGoPrintDatabase::~QGoPrintDatabase()
{
}


QStringList QGoPrintDatabase::GetTableContentFromDB(QString TableName)
{
  QStringList List;
  QString SQLquery;

  // CONFIGURE CONNECTOR
  vtkMySQLDatabase * DataBaseConnector = vtkMySQLDatabase::New();
  //DataBaseConnector->SetHostName("localhost");
  DataBaseConnector->SetHostName(m_Server.toAscii().data());
  DataBaseConnector->SetUser(m_User.toAscii().data());
  DataBaseConnector->SetPassword(m_Password.toAscii().data());
  DataBaseConnector->SetDatabaseName(m_NameDB.toStdString().c_str());


  // CONNECT TO SERVER
  DataBaseConnector->Open("");
  if( ! DataBaseConnector->IsOpen() )
    {
    DataBaseConnector->Delete();
    exit(0);
    }

  vtkSQLQuery* query = DataBaseConnector->GetQueryInstance();
  SQLquery=QString("select * from %1;").arg(TableName);
  vtkStdString createQuery(SQLquery.toStdString());

  query->SetQuery( createQuery.c_str() );

  if( !query->Execute() )
    {
    std::cerr << "Create query failed" << std::endl;
    DataBaseConnector->Delete();
    query->Delete();
    exit(0);
    }

  for( int col = 0; col < query->GetNumberOfFields(); ++col )
    {
    List <<query->GetFieldName( col );
    }

  while ( query->NextRow() )
    {
    for ( int field = 0; field < query->GetNumberOfFields(); ++ field )
      {
      if ( field > 0 ) std::cout << ", ";
      List << query->DataValue( field ).ToString().c_str();
      }
    std::cout << endl;
    }

  DataBaseConnector->Close();
  DataBaseConnector->Delete();
  query->Delete();

  return List;
}

void QGoPrintDatabase::QPrintColumnNames (QString TableName)
{
  QStringList listColumnNames;
  listColumnNames=GetTableContentFromDB(TableName);
  int numberCol=listColumnNames.size();
  QTableWidget* QTabName = new QTableWidget;
  this->DBTabWidget->addTab(QTabName,TableName);
  QTabName->setRowCount(15);
  QTabName->setColumnCount(numberCol);

  for ( int i = 0; i < numberCol; i++ )
    {
    QTableWidgetItem* HeaderCol=new QTableWidgetItem;
    QString NameHeader;
    NameHeader=listColumnNames[i];
    HeaderCol->setText(NameHeader);
    QFont serifFont("Arial", 10, QFont::Bold);
    HeaderCol->setFont(serifFont);
    QTabName->setHorizontalHeaderItem(i,HeaderCol);
    }
}

void QGoPrintDatabase::Fill_Database(QString ServerName,QString login,
    QString Password, QString DBName, int ExpID, QString ExpName)
{
  m_Server = ServerName;
  m_User = login;
  m_Password = Password;
  m_NameDB=DBName;
  this->setWindowTitle(QString("DB: %1 - Exp: %2").arg(DBName).arg(ExpName));

  QPrintColumnNames ("figure");
  QPrintColumnNames ("mesh");
  QPrintColumnNames ("track");
  QPrintColumnNames ("lineage");

}
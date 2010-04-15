/*=========================================================================
  Author: $Author: lydiesouhait $  // Author of last commit
  Version: $Rev: 1150 $  // Revision of last commit
  Date: $Date: 2010-03-30 15:00:47 -0400 (Tue, 30 Mar 2010) $  // Date of last commit
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
#include "QGoDBInitCreateUserPage.h"
#include "CreateDataBaseHelper.h"
#include "QGoDBInitializationWizard.h"
#include "vtkSQLQuery.h"
#include <QFormLayout>
#include <QMessageBox>
#include <QVariant>
#include <QInputDialog>
#include <QDir>


QGoDBInitCreateUserPage::QGoDBInitCreateUserPage( QWidget *iParent)
  : QWizardPage( iParent )
{
  QFont tfont;
  tfont.setBold(false);
  this->setFont(tfont);

  m_ServerName = "localhost";
  m_DBName     = "gofiguredatabase";

  setSubTitle( tr("Create a user for MySQL Database local Server:"));

  QFormLayout* formLayout = new QFormLayout;
  lineUserName = new QLineEdit;
  linePassword = new QLineEdit;
  linePassword->setEchoMode(QLineEdit::Password);
  linePassword->displayText();

  formLayout->addRow( tr("Choose a User name:"),  lineUserName );
  formLayout->addRow( tr("Choose a Password:"),   linePassword );
  setLayout( formLayout );

  setLayout( formLayout );

  registerField( "User",       lineUserName );
  registerField( "Password",   linePassword );
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
bool QGoDBInitCreateUserPage::validatePage()
{
  if(field("User").toString() == "" ||
       field("Password").toString() == "")
    {
    QMessageBox msgBox;
    msgBox.setText(
      tr("Please fill all the fields.") );
    msgBox.exec();
    return false;
    }
   if(!this->CreateUser())
    {
    QMessageBox msgBox;
    msgBox.setText(
      tr("There is a problem with the creation of your user.") );
    msgBox.exec();
    return false;
    }
   if(
    !CreateGoFigureDataBase(this->m_ServerName,field("User").toString().toStdString(),
    field("Password").toString().toStdString(),this->m_DBName))
     {
     QMessageBox msgBox;
     msgBox.setText(
        tr("There is a problem with the creation of your database.") );
     msgBox.exec();
     return false;
     }
    return true;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
bool QGoDBInitCreateUserPage::CreateUser()
{
  std::string Login = field("User").toString().toStdString();
  std::string Password = field("Password").toString().toStdString();

  vtkMySQLDatabase * DataBaseConnector = vtkMySQLDatabase::New();
  DataBaseConnector->SetHostName( this->m_ServerName.c_str() );
  DataBaseConnector->SetUser( "root" );
  DataBaseConnector->SetPassword("");
  if( !DataBaseConnector->Open() )
    {
    DataBaseConnector->Close();
    DataBaseConnector->Delete();
    bool ok;
    QString text = QInputDialog::getText(0,"Enter your root password for MySQL:",
                                          "Root Password:", QLineEdit::Password,
                                          QDir::home().dirName(), &ok);
    if (ok && !text.isEmpty())
      {
      vtkMySQLDatabase * DataBaseConnectorBis = vtkMySQLDatabase::New();
      DataBaseConnectorBis->SetHostName( this->m_ServerName.c_str() );
      DataBaseConnectorBis->SetUser( "root" );
      DataBaseConnectorBis->SetPassword(text.toStdString().c_str());
      if (!DataBaseConnectorBis->Open())
        {
        QMessageBox msgBox;
        msgBox.setText(
          tr("There is a problem with the connection to your root.") );
        msgBox.exec();
        return false;
        }
      return this->CreateGofigureUserWithDatabaseConnector(DataBaseConnectorBis,Login,
        this->m_ServerName,Password);
      }
    }
  else
    {
    return this->CreateGofigureUserWithDatabaseConnector(DataBaseConnector,Login,
      this->m_ServerName,Password);
    }
  return false;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
bool QGoDBInitCreateUserPage::CreateGofigureUserWithDatabaseConnector (
  vtkMySQLDatabase* DatabaseConnector,std::string iLogin, 
  std::string iServerName, std::string iPassword)
{
  vtkSQLQuery* queryUserExist = DatabaseConnector->GetQueryInstance();
  std::stringstream UserExistScript;
  UserExistScript << "SELECT USER FROM mysql.user WHERE user = '";
  UserExistScript <<  iLogin;
  UserExistScript << "';";
  queryUserExist->SetQuery(UserExistScript.str().c_str());
  if (!queryUserExist->Execute())
    {
   QMessageBox msgBox;
    msgBox.setText(
      tr("There is a problem to check your existing users.") );
    msgBox.exec();
    queryUserExist->Delete();
    DatabaseConnector->Close();
    DatabaseConnector->Delete();
    return false;
    }
  if( queryUserExist->NextRow() )
    {
    queryUserExist->Delete();
    DatabaseConnector->Close();
    DatabaseConnector->Delete();
    QMessageBox msgBox;
    msgBox.setText(
      tr("The user you are trying to create already exits.") );
    msgBox.exec();
    return false;
    }
  queryUserExist->Delete();

  vtkSQLQuery* query = DatabaseConnector->GetQueryInstance();
  std::stringstream queryScript;
  queryScript << "CREATE USER '";
  queryScript <<  iLogin;
  queryScript << "'@'";
  queryScript << iServerName;
  queryScript << "' IDENTIFIED BY '";
  queryScript << iPassword;
  queryScript << "';";
  query->SetQuery(queryScript.str().c_str());
  query->Execute();
  query->Delete();

  vtkSQLQuery* queryPrivileges = DatabaseConnector->GetQueryInstance();
  std::stringstream PrivilegesScript;
  PrivilegesScript << "GRANT ALL PRIVILEGES ON *.* TO '";
  PrivilegesScript << iLogin;
  PrivilegesScript << "'@'";
  PrivilegesScript << iServerName;
  PrivilegesScript << "';";
  queryPrivileges->SetQuery(PrivilegesScript.str().c_str());
  if(!queryPrivileges->Execute())
    {
    queryUserExist->Delete();
    DatabaseConnector->Close();
    DatabaseConnector->Delete();
    QMessageBox msgBox;
    msgBox.setText(
      tr("Sorry, there is a problem with the creation of your user.") );
    msgBox.exec();
    return false;
    }
  queryPrivileges->Delete();
  DatabaseConnector->Close();
  DatabaseConnector->Delete();
  return true;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
/*int QGoDBInitCreateUserPage::nextId() const
{

  std::list<std::string> ListGoDB = ListGofigureDatabases();

  if (ListGoDB.empty())
    {
    return QGoWizardDB::CreateDataBasePageID;
    }
  if (ListGoDB.size() > 1)
    {
    std::cout<<"There is more than one Gofigure DataBase"<<std::endl;
    std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
    std::cout << std::endl;
    }
  else
    {
    std::list<std::string>::iterator i = ListGoDB.begin();
    DBName = *i;
    this->wizard()->setField( "DBName", DBName.c_str() );
    std::cout<<"the db name to open is: "<<field("DBName").toString().toStdString().c_str()<<std::endl;
    }
  if(!this->m_IsAnOpenRecentFile)
    {
    return QGoWizardDB::OpenOrCreateProjectPageID;
    }
  else
    {
    return -1;
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoConnectServerPage::CloseServerConnection()const
{
  m_ConnectionServer.second->Close();
  m_ConnectionServer.second->Delete();
  m_ConnectionServer.second = 0;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoConnectServerPage::OpenConnectionToServer()const
{
  if (this->m_ConnectionServer.second == 0)
    {
    m_ConnectionServer = ConnectToServer(
    field("ServerName").toString().toStdString(),
    field("User").toString().toStdString(),
    field("Password").toString().toStdString() );
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoConnectServerPage::SetImgSessionName(std::string iImgSessionName)
{
  this->m_ImgSessionName = iImgSessionName;
  this->m_IsAnOpenRecentFile = true;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoConnectServerPage::SetIsAnOpenRecentFile(bool iIsAnOpenRecentFile)
{
  this->m_IsAnOpenRecentFile = iIsAnOpenRecentFile;
}*/
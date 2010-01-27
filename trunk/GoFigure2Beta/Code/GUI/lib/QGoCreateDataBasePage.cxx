/*=========================================================================
  Author: $Author: lsouhait $  // Author of last commit
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
#include "QGoCreateDataBasePage.h"
#include "QueryDataBaseHelper.h"
#include "CreateDataBaseHelper.h"
#include <QFormLayout>
#include <QMessageBox>
#include <QLineEdit>
#include <QVariant>
#include <iostream>


QGoCreateDataBasePage::QGoCreateDataBasePage(QWidget *iParent)
:QWizardPage(iParent)
{
  QFont tfont;
  tfont.setBold(false);
  this->setFont(tfont);

  QFormLayout* formLayout = new QFormLayout;
  lineNewDBName = new QLineEdit;
  formLayout->addRow(tr("Name of the new DB to create:"), lineNewDBName);

  setLayout(formLayout);
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoCreateDataBasePage::initializePage()
{
  this->setSubTitle(tr("There is no existing GoFigure Database, please create one:"));
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
bool QGoCreateDataBasePage::validatePage()
{
  QString DBName =lineNewDBName->text();
  if( DBName.isEmpty() )
    {
    QMessageBox msgBox;
    msgBox.setText( 
      tr( "Please enter a name for your new DataBase :" ) );
    msgBox.exec();
    return false;
    }
  this->wizard()->setField( "DBName", DBName );
  std::cout<<"the DBName to create is: "<<field("DBName").toString().toStdString().c_str()<<std::endl;
 
  CreateGoFigureDataBase(field("ServerName").toString().toStdString(),
      field("User").toString().toStdString(),
      field("Password").toString().toStdString(),
      field("DBName").toString().toStdString());
 
  return true;
}
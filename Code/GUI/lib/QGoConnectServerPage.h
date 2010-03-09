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
#ifndef __QGoConnectServerPage_h
#define __QGoConnectServerPage_h

#include <QWizard>
#include <QWizardPage>
#include <QLineEdit>
#include "vtkMySQLDatabase.h"


class QGoConnectServerPage : public QWizardPage
{
  Q_OBJECT

public:
  QGoConnectServerPage(QWidget *parent = 0);
  bool validatePage();
  int nextId() const;
  mutable std::pair<bool,vtkMySQLDatabase*> m_ConnectionServer;

private:
  void OpenConnectionToServer()const;
  void CloseServerConnection()const;

  QLineEdit* lineServerName;
  QLineEdit* lineUserName;
  QLineEdit* linePassword;
  QLineEdit* lineDBName;
 
  /**\brief return the list of the names of the databases of gofigure type:*/
  std::list<std::string> ListGofigureDatabases() const;
  /* the number of times nextID() is called has to be counted in order to make 
  it do some things only at the validation part, not during the initialization
  part during which nextID() is called 2 times*/
  mutable int NbNextIDCalled;
  

};
#endif

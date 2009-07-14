/*=========================================================================
  Author: $Author: arnaudgelas $  // Author of last commit
  Version: $Rev: 194 $  // Revision of last commit
  Date: $Date: 2009-05-19 12:18:20 -0400 (Tue, 19 May 2009) $  // Date of last commit
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

#include "QTableWidgetChild.h"
#include <iostream>

QTableWidgetChild::QTableWidgetChild( QWidget* parent ): QTableWidget( parent )
{
  PrevCol = -1;
  PrevOrder = -1;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QTableWidgetChild::QTableWidgetChild ( int rows, int columns, 
                  QWidget * parent ):QTableWidget(rows,columns,parent)
{
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QTableWidgetChild::~QTableWidgetChild()
{
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void QTableWidgetChild::sortItems(int column, Qt::SortOrder order)
{
  std::cout<<"column"<<column<<std::endl;
  std::cout<<"order"<<order<<std::endl;  
  if ( column != PrevCol && order !=PrevOrder)
    {
    PrevCol = column;
    PrevOrder = order;
    QTableWidget::sortItems(column,order);
    }
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int QTableWidgetChild::findFigureID(int FigureID)
{
  QStringList ColumnsHeader = recordHeaderNamesOrder();
  if (ColumnsHeader.isEmpty())
    {
    std::cout<<"The QStringlist ColumnsHeader is empty"<<std::endl;
    return -1;
    }

  int ColumnIndex = findColumnName("figureID",ColumnsHeader);
  if (ColumnIndex == -1)
    {
    std::cout<<"The column figureID has not been found"<<std::endl;
    return -1;
    }
  else
    {
    for (int i=0;i<rowCount();i++)
      {
      if (this->item(i,ColumnIndex)->text().toInt() == FigureID)
        {
        return i;
        }
      }
    }
  return -1;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int QTableWidgetChild::findColumnName(QString ColumnName, QStringList ColumnsHeader)
{
  return ColumnsHeader.indexOf(ColumnName,0);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QStringList QTableWidgetChild::recordHeaderNamesOrder()
{
  QStringList ColumnNamesOrder;
  for (int i=0; i<this->columnCount();i++)
    {
    ColumnNamesOrder.append(this->horizontalHeaderItem(i)->text());
    std::cout<<"Column Name is: "<<this->horizontalHeaderItem(i)->text().toAscii().data()<<std::endl;
    }

  return ColumnNamesOrder;
}
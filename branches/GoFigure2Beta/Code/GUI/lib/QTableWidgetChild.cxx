/*=========================================================================
  Author: $Author: lsouhait $  // Author of last commit
  Version: $Rev: 577 $  // Revision of last commit
  Date: $Date: 2009-08-18 10:45:50 -0400 (Tue, 18 Aug 2009) $  // Date of last commit
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
#include <sstream>
#include <QTableWidgetSelectionRange>
#include <QHeaderView>
#include <QSettings>
#include "QTableWidgetNumericalItem.h"


QTableWidgetChild::QTableWidgetChild( QWidget* iParent ): QTableWidget( iParent )
{
  PrevCol = -1;
  PrevOrder = -1;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
QTableWidgetChild::QTableWidgetChild ( int rows, int columns,
                  QWidget * iParent ):QTableWidget(rows,columns,iParent)
{
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
QTableWidgetChild::~QTableWidgetChild()
{
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QTableWidgetChild::sortItems( int iColumn, Qt::SortOrder iOrder )
{
  if ( ( iColumn != PrevCol ) && ( iOrder != PrevOrder ) )
    {
    PrevCol = iColumn;
    PrevOrder = iOrder;
    QTableWidget::sortItems( iColumn, iOrder );
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
int QTableWidgetChild::findValueGivenColumn(int Value, QString Column)
{
  QStringList ColumnsHeader = recordHeaderNamesOrder();
  if (ColumnsHeader.isEmpty())
    {
    std::cout<<"The QStringlist ColumnsHeader is empty";
    std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
    std::cout << std::endl;
    return -1;
    }

  int ColumnIndex = findColumnName(Column,ColumnsHeader);
  if (ColumnIndex == -1)
    {
    std::cout<<"The column figureID has not been found";
    std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
    std::cout << std::endl;
    return -1;
    }
  else
    {
    for (int i=0;i<rowCount();i++)
      {
      if (this->item(i,ColumnIndex)->text().toInt() == Value)
        {
        return i;
        }
      }
    }
  return -1;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
int QTableWidgetChild::findColumnName(QString ColumnName, QStringList ColumnsHeader)
{
  return ColumnsHeader.indexOf(ColumnName,0);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
QStringList QTableWidgetChild::recordHeaderNamesOrder()
{
  QStringList ColumnNamesOrder;
  for (int i=0; i<this->columnCount();i++)
    {
    ColumnNamesOrder.append(this->horizontalHeaderItem(i)->text());
    }

  return ColumnNamesOrder;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QTableWidgetChild::SetSelectRowTraceID (std::string TraceName, 
  int TraceID, bool IsSelected)
{
  std::stringstream TraceIDName;
  TraceIDName << TraceName;
  TraceIDName <<"ID";

  int RowIndex = this->findValueGivenColumn(TraceID, TraceIDName.str().c_str());
  if (RowIndex == -1)
    {
    std::cout<<"The contour "<<TraceID<<"has not been found ";
    std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
    std::cout << std::endl;
    }
  else
    {
    QTableWidgetSelectionRange RangeToSelect(RowIndex,0,RowIndex,columnCount()-1);
    this->setRangeSelected(RangeToSelect,IsSelected);
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
 void QTableWidgetChild::TracesToHighlight(
  std::string TraceName,std::vector<ContourMeshStructure> & ioTracesInfo)
{
  //first set all the hightlighted traces to false:
  for( unsigned int i=0; i < ioTracesInfo.size(); i++ )
    {
    ioTracesInfo[i].Highlighted = false;
    }
  
  //get the selected TraceID:
  std::stringstream TraceID;
  TraceID <<TraceName;
  TraceID << "ID" ;
  TraceID.str();
  QStringList ListTraceIDToHighLight = ValuesForSelectedRows(TraceID.str().c_str());
  //then, set to IsHighlight the selected ones:
  for (int i = 0; i<ListTraceIDToHighLight.size();i++)
    {
    int j = ListTraceIDToHighLight.at(i).toInt()-1;
    ioTracesInfo[j].Highlighted = true;
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
QStringList QTableWidgetChild::ValuesForSelectedRows(QString ColumnName)
{
  QList<QTableWidgetSelectionRange> Selection;
  //Selection = this->selectedRanges();
  

  QStringList ColumnsHeader = this->recordHeaderNamesOrder();
  int ColumnIndex = findColumnName(ColumnName,ColumnsHeader);

  QList<QString> Values;
  for( int i=0; i< Selection.size(); i++)
    {
    int TopRowSelected = Selection[i].topRow();
    int BottomRowSelected = Selection[i].bottomRow();

    for (int j = TopRowSelected; j<BottomRowSelected+1;j++)
      {
      Values.append(this->item(j,ColumnIndex)->text());
      }
    }
  return Values;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QTableWidgetChild::DisplayColumnNames( QString TableName,
 std::list<std::string> ColumnNames)
{
  int numberCol=ColumnNames.size();
  this->setColumnCount(numberCol);
  
  int i = 0;
  for ( std::list<std::string>::iterator iter = ColumnNames.begin(); 
    iter!= ColumnNames.end();iter++)
    {
    QTableWidgetItem* HeaderCol=new QTableWidgetItem;
    std::string NameHeader;
    NameHeader =*iter;

    HeaderCol->setText(NameHeader.c_str());
    QFont serifFont("Arial", 10, QFont::Bold);
    HeaderCol->setFont(serifFont);
    this->setHorizontalHeaderItem(i,HeaderCol);
    this->resizeColumnToContents(i);
    i++;
    }
  
  this->horizontalHeader()->setSortIndicatorShown(true);
  /*Need to disabled the Sorting while printing the values from the database in
  the table widget as the sorting is making trouble*/
  this->setSortingEnabled(false);
  this->horizontalHeader()->setMovable(true);

  QObject::connect( this->horizontalHeader(),
    SIGNAL( sortIndicatorChanged(int,Qt::SortOrder) ),
    this,SLOT( sortItems(int,Qt::SortOrder)) );

  QSettings settings( "MegasonLab", "Gofigure2" );
  QByteArray stateTableWidget = settings.value("StateTableWidget").toByteArray();
  //QTabTableName->horizontalHeader()->restoreState(stateTableWidget);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QTableWidgetChild::DisplayContent(GoDBTableWidgetContainer* iLinkToRowContainer,
  std::string TraceName, std::string CollectionName)
{ 
  DBTableWidgetContainerType RowContainer = iLinkToRowContainer->GetRowContainer();
  if (RowContainer.empty())
    {
    std::cout<<"The Row Container is totally empty ";
    std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
    std::cout << std::endl;
    }
  else
    {
    unsigned int NbofRows = 0;
    for (unsigned int i = 0; i < RowContainer.size(); i++)
      {    
      bool RowsCountSet = false;
      //check that the column has to be displayed in the table widget and that there are
      //some values to be displayed:
      if (RowContainer[i].first.ColumnNameTableWidget != "None" && !RowContainer[i].second.empty())
        {
        if (NbofRows == 0)
          {
          NbofRows = RowContainer[i].second.size();
          this->setRowCount(NbofRows);
          RowsCountSet = true;
          }
        for (int j = 0; j < this->columnCount();j++)
          {
          std::string HeaderCol = this->horizontalHeaderItem(j)->text().toStdString();
          if (HeaderCol == RowContainer[i].first.ColumnNameTableWidget)
            {
            std::vector<std::string>::iterator iter = RowContainer[i].second.begin();
            int k=0;
            while(iter != RowContainer[i].second.end())
              {
              QTableWidgetNumericalItem* CellTable = new QTableWidgetNumericalItem;
              std::string Value = *iter;
              CellTable->setText(Value.c_str());
              CellTable->setTextAlignment(Qt::AlignCenter);
              this->setItem(k,j,CellTable);
              iter++;
              k++;
              }//ENDWHILE
            }//ENDIF
          }//ENDFOR
        }//ENDIF
      }//ENDFOR
      SetSelectedColumn(NbofRows,0);
      this->SetColorForAllTable(iLinkToRowContainer,TraceName);
      this->SetColorForAllTable(iLinkToRowContainer,CollectionName);
    }//ENDELSE
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QTableWidgetChild::SetSelectedColumn(unsigned int iNbOfRows, 
  unsigned int StartedRow)
{
  //int indexCol = findColumnName( "Selected", recordHeaderNamesOrder());
   int indexCol = findColumnName( "", recordHeaderNamesOrder());
  for (unsigned int i = StartedRow ; i < iNbOfRows+StartedRow ; i++)
    {
    QTableWidgetNumericalItem* Checkbox = new QTableWidgetNumericalItem;
    Checkbox->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled |
      Qt::ItemIsSelectable);
    Checkbox->setCheckState(Qt::Unchecked);
    this->setItem(i,indexCol,Checkbox);
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QTableWidgetChild::SetColorForAllTable (GoDBTableWidgetContainer* iLinkToRowContainer,
  std::string NameGroupColor )
{
  DBTableWidgetContainerType RowContainer = iLinkToRowContainer->GetRowContainer();

  std::string RedInfoName = "RedFor";
  RedInfoName += NameGroupColor;
  int indexRedColumn = iLinkToRowContainer->GetIndexInsideRowContainer(RedInfoName);

  std::string GreenInfoName = "GreenFor";
  GreenInfoName += NameGroupColor;
  int indexGreenColumn = iLinkToRowContainer->GetIndexInsideRowContainer(GreenInfoName);

  std::string BlueInfoName = "BlueFor";
  BlueInfoName += NameGroupColor;
  int indexBlueColumn = iLinkToRowContainer->GetIndexInsideRowContainer(BlueInfoName);

  //for the collection color:
  std::string ColumnNameID = NameGroupColor;
  ColumnNameID += "ID";
  int indexGroupIDInTableWidget = findColumnName(ColumnNameID.c_str(),
    this->recordHeaderNamesOrder());
  for (unsigned int i=0; i < this->rowCount();i++)
    {
    QColor Color;
    QColor TextColor;
    if (RowContainer[indexGroupIDInTableWidget].second[i] == "0")
      {
      Color.setRgb(255,255,255,255);
      int rgb = 255 - (255*3)/3;
      TextColor.setRgb(rgb,rgb,rgb,255);
      }
    else
      {
      int Red   = atoi(RowContainer[indexRedColumn].second[i].c_str());
      int Green = atoi(RowContainer[indexGreenColumn].second[i].c_str());
      int Blue  = atoi(RowContainer[indexBlueColumn].second[i].c_str());
      Color.setRgb(Red,Green,Blue,255);
      int rgb = 255 - (Red+Green+Blue)/3;
      TextColor.setRgb(rgb,rgb,rgb,255);
      }
    this->item(i,indexGroupIDInTableWidget)->setBackgroundColor(Color);
    this->item(i,indexGroupIDInTableWidget)->setTextColor(TextColor);
    }
  //QColor Color (Red,Green,Blue,Alpha); 
  //int Row = 0;
  //int Column = 0;
  //this->item(Row,Column)->setBackgroundColor(Color);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
int IndexForRowContainer(QTableWidgetChild::DBTableWidgetContainerType iRowContainer)
{
  return 0;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QTableWidgetChild::InsertNewRow(GoDBTableWidgetContainer* iLinkToRowContainer,
    std::string TraceName, std::string CollectionName)
{
   DBTableWidgetContainerType NewTraceRowContainer = iLinkToRowContainer->GetRowContainer();
   if (NewTraceRowContainer.size() == 0 || NewTraceRowContainer[1].second.size() != 1)
    {
    std::cout<<"The New Trace Row Container is totally empty or there is more than 1 trace in it";
    std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
    std::cout << std::endl;
    }
  else
    {
    int NewRow = this->rowCount()+ 1;
    int NbRow = NewRow;
    this->setRowCount(NbRow);
    for (unsigned int i = 0; i < NewTraceRowContainer.size(); i++)
      {    
      if (NewTraceRowContainer[i].first.ColumnNameTableWidget != "None" && !NewTraceRowContainer[i].second.empty())
        {
        for (int j = 0; j < this->columnCount();j++)
          {
          std::string HeaderCol = this->horizontalHeaderItem(j)->text().toStdString();
          if (HeaderCol == NewTraceRowContainer[i].first.ColumnNameTableWidget)
            {
            QTableWidgetNumericalItem* CellTable = new QTableWidgetNumericalItem;
            std::string Value = NewTraceRowContainer[i].second[0];
            CellTable->setText(Value.c_str());
            CellTable->setTextAlignment(Qt::AlignCenter);
            this->setItem(NewRow-1,j,CellTable);
            }//ENDIF
          }//ENDFOR
        }//ENDIF
      }//ENDFOR
      SetSelectedColumn(1,NewRow-1);
      this->SetColorForAllTable(iLinkToRowContainer,TraceName);
      this->SetColorForAllTable(iLinkToRowContainer,CollectionName);
    }//ENDELSE
}
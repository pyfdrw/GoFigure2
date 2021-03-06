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

#include "QGoTableWidget.h"
#include <iostream>
#include <sstream>
#include <QTableWidgetSelectionRange>
#include <QHeaderView>
#include <QSettings>
#include <QApplication>
#include <QClipboard>
#include <QToolButton>
#include <QMessageBox>

QGoTableWidget::QGoTableWidget(QWidget *iParent) : QTableWidget(iParent)
{
  PrevCol = -1;
  PrevOrder = -1;
  QObject::connect( this,
                    SIGNAL( cellClicked(int, int) ),
                    this, SLOT( UpdateColumnsWithCheckBoxes(int, int) ) );

  this->setContextMenuPolicy(Qt::CustomContextMenu);
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
QGoTableWidget::QGoTableWidget(int rows, int columns, QWidget *iParent) :
  QTableWidget(rows, columns, iParent), PrevCol(-1), PrevOrder(-1)
{
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
QGoTableWidget::~QGoTableWidget()
{
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::sortItems(int iColumn, Qt::SortOrder iOrder)
{
  if ( ( iColumn != PrevCol ) && ( iOrder != PrevOrder ) )
    {
    PrevCol = iColumn;
    PrevOrder = iOrder;
    QTableWidget::sortItems(iColumn, iOrder);
    }
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
int QGoTableWidget::findValueGivenColumn(int iValue, const QString & iColumn)
{
  int ColumnIndex = findColumnName(iColumn);

  if ( ColumnIndex == -1 )
    {
    std::cout << "The column figureID has not been found";
    std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
    std::cout << std::endl;
    std::cout << std::endl;
    return -1;
    }
  else
    {
    for ( int i = 0; i < rowCount(); i++ )
      {
      if ( this->item(i, ColumnIndex)->text().toInt() == iValue )
        {
        return i;
        }
      }
    }
  return -1;
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
int QGoTableWidget::findColumnName(const QString & iColumnName)
{
  QStringList ColumnsHeader = recordHeaderNamesOrder();

  if ( ColumnsHeader.isEmpty() )
    {
    std::cout << "The QStringlist ColumnsHeader is empty";
    std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
    std::cout << std::endl;
    return -1;
    }
  return ColumnsHeader.indexOf(iColumnName, 0);
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
QStringList QGoTableWidget::recordHeaderNamesOrder()
{
  QStringList ColumnNamesOrder;

  for ( int i = 0; i < this->columnCount(); i++ )
    {
    ColumnNamesOrder.append( this->horizontalHeaderItem(i)->text() );
    }

  return ColumnNamesOrder;
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::SetVisibleStateForTraceID(unsigned int iTraceID,
                                               const std::string & iTraceName,
                                               Qt::CheckState iState,
                                               bool EmitSignal)
{
  unsigned int ColumnIndex = this->findColumnName("Show");
  int          RowIndex = this->GetRowForTraceID(iTraceID, iTraceName);

  QTableWidgetItem* temp = this->item(RowIndex, ColumnIndex);

  if( temp )
    {
    this->setVisibleStateCheckBox( temp, iState, EmitSignal);
    }
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::SetVisibleStateForListTraceIDs(
  const std::list< unsigned int > & iListTraceIDs,
  Qt::CheckState iState,
  const std::string & iTraceName)
{
  std::list< unsigned int >::const_iterator iter = iListTraceIDs.begin();

  while ( iter != iListTraceIDs.end() )
    {
    this->SetVisibleStateForTraceID(*iter, iTraceName, iState, false);
    ++iter;
    }
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::SetCheckStateForTraceID(unsigned int iTraceID,
                                             const std::string & iTraceName,
                                             Qt::CheckState iState,
                                             bool EmitSignal)
{

std::cout << "iTraceID: " << iTraceID << std::endl;
std::cout << "iTraceName: " << iTraceName << std::endl;
std::cout << "EmitSignal: " << EmitSignal << std::endl;

unsigned int ColumnIndex = this->findColumnName("");
int          RowIndex = this->GetRowForTraceID(iTraceID, iTraceName);

std::cout << "ColumnIndex: " << ColumnIndex << std::endl;
std::cout << "RowIndex: " << RowIndex << std::endl;
std::cout << "this->item(RowIndex, ColumnIndex): " << this->item(RowIndex, ColumnIndex) << std::endl;

  this->setCheckedUncheckedStateCheckBox(
    this->item(RowIndex, ColumnIndex), iState, EmitSignal);
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
int QGoTableWidget::GetRowForTraceID(unsigned int iTraceID,
                                     const std::string & iTraceName)
{
  //get the row index based on iTraceName:
  std::stringstream TraceIDName;

  TraceIDName << iTraceName;
  TraceIDName << "ID";
  int oRowIndex = this->findValueGivenColumn( iTraceID, TraceIDName.str().c_str() );
  if ( oRowIndex == -1 )
    {
    //std::cerr << "The trace " << TraceID << "has not been found in TW";
    std::cerr << "Debug: In " << __FILE__ << ", line " << __LINE__;
    std::cerr << std::endl;
    return oRowIndex;
    }
  return oRowIndex;
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
QStringList QGoTableWidget::ValuesForSelectedRows(const QString & iColumnName)
{
  QList< QTableWidgetSelectionRange > Selection = this->selectedRanges();
  int                                 ColumnIndex = findColumnName(iColumnName);

  QList< QString > Values;
  for ( int i = 0; i < Selection.size(); i++ )
    {
    int TopRowSelected = Selection[i].topRow();
    int BottomRowSelected = Selection[i].bottomRow();

    for ( int j = TopRowSelected; j < BottomRowSelected + 1; j++ )
      {
      Values.append( this->item(j, ColumnIndex)->text() );
      }
    }
  return Values;
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::DisplayColumnNames(
  const std::list< std::pair< std::string, std::string > >& iColumnNamesAndToolTip)
{
  size_t numberCol = iColumnNamesAndToolTip.size();

  this->setColumnCount( static_cast< int >( numberCol ) );

  int i = 0;

  for ( std::list< std::pair< std::string, std::string > >::const_iterator
          iter = iColumnNamesAndToolTip.begin();
        iter != iColumnNamesAndToolTip.end();
        ++iter, ++i )
    {
    QTableWidgetItem *HeaderCol = new QTableWidgetItem;
    std::string       NameHeader, ToolTip;
    NameHeader = iter->first;
    ToolTip = iter->second;

    HeaderCol->setText( NameHeader.c_str() );
    if ( ToolTip != "None" )
      {
      HeaderCol->setToolTip( ToolTip.c_str() );
      }

    QFont serifFont("Arial", 10, QFont::Bold);
    HeaderCol->setFont(serifFont);
    this->setHorizontalHeaderItem(i, HeaderCol);
    }

  this->horizontalHeader()->setSortIndicatorShown(true);
  //Need to disabled the Sorting while printing the values from the database in
  // the table widget as the sorting is making trouble
  this->setSortingEnabled(false);
  this->horizontalHeader()->setMovable(true);

  QObject::connect( this->horizontalHeader(),
                    SIGNAL( sortIndicatorChanged(int, Qt::SortOrder) ),
                    this, SLOT( sortItems(int, Qt::SortOrder) ) );

  // QSettings settings( "MegasonLab", "Gofigure2" );
  QSettings  settings;
  QByteArray stateTableWidget = settings.value("StateTableWidget").toByteArray();
  //QTabTableName->horizontalHeader()->restoreState(stateTableWidget);
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget
::DisplayInitialContent(const TWContainerType & iTWRowContainer,
                        const std::vector< int > & iIndexColorTraceRowContainer,
                        const std::vector< int > & iIndexColorCollectionRowContainer,
                        const std::string & iTraceName,
                        const std::string & iCollectionName,
                        const std::list< std::pair< std::string, std::string > > & iColumnNamesAndToolTip,
                        Qt::CheckState iState,
                        int iIndexShowColumn)
{
  this->DisplayColumnNames(iColumnNamesAndToolTip);
  this->InsertNewRows(iTWRowContainer,
                    iIndexColorTraceRowContainer,
                    iIndexColorCollectionRowContainer,
                    iTraceName, iCollectionName,
                    iState);
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::SetSelectedColumn(unsigned int iIndexRow)
{
  int indexCol = findColumnName("");

  QTableWidgetItem *Checkbox = new QTableWidgetItem;
    //Checkbox->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled |
    //  Qt::ItemIsSelectable );
    Checkbox->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    // Checkbox->setFlags( Qt::ItemIsUserCheckable);
    QColor WhiteColor(Qt::white);
    Checkbox->setTextColor(WhiteColor);
    this->setCheckedUncheckedStateCheckBox(Checkbox, Qt::Unchecked, false);
    this->setItem(iIndexRow, indexCol, Checkbox);
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::SetVisibleColumn(unsigned int iIndexRow,
                                      Qt::CheckState iState)
{
  int indexCol = findColumnName("Show");

  QTableWidgetItem *Checkbox = new QTableWidgetItem;
    //Checkbox->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable |
    // Qt::ItemIsUserCheckable);
    Checkbox->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    QColor WhiteColor(Qt::white);
    Checkbox->setTextColor(WhiteColor);
    this->setItem(iIndexRow, indexCol, Checkbox);
    this->setVisibleStateCheckBox(Checkbox, iState, false);
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::SetColorForTable(const TWContainerType & iTWRowContainer,
                                      unsigned int iIndexTWRowContainer,
                                      const std::vector< int > & iIndexColorRowContainer,
                                      const std::string & iNameGroupColor,
                                      unsigned int iIndexRowTW)
{
  //for the collection color:
  std::string ColumnNameID = iNameGroupColor;
  ColumnNameID += "ID";
  int indexGroupIDInTableWidget = findColumnName( ColumnNameID.c_str() );
  QColor Color;
  QColor TextColor;
  int  Alpha = atoi( iTWRowContainer[iIndexColorRowContainer[3]].second[iIndexTWRowContainer].c_str() );
  if ( iTWRowContainer[indexGroupIDInTableWidget].second[iIndexTWRowContainer] == "0" || Alpha == 0 )
      {
      Color.setRgb(255, 255, 255, 255);
      int rgb = 255 - ( 255 * 3 ) / 3;
      TextColor.setRgb(rgb, rgb, rgb, 255);
      }
    else
      {
      int Red   = atoi( iTWRowContainer[iIndexColorRowContainer[0]].second[iIndexTWRowContainer].c_str() );
      int Green = atoi( iTWRowContainer[iIndexColorRowContainer[1]].second[iIndexTWRowContainer].c_str() );
      int Blue  = atoi( iTWRowContainer[iIndexColorRowContainer[2]].second[iIndexTWRowContainer].c_str() );
      Color.setRgb(Red, Green, Blue, Alpha);
      int rgb = 255 - ( Red + Green + Blue ) / 3;
      TextColor.setRgb(rgb, rgb, rgb, 255);
      }

    this->item(iIndexRowTW, indexGroupIDInTableWidget)->setBackgroundColor(Color);
    this->item(iIndexRowTW, indexGroupIDInTableWidget)->setTextColor(TextColor);
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::DisplayDataForOneRow(const TWContainerType & iTWRowContainer,
                                          unsigned int iIndexTWRowContainer,
                                          int iIndexTWRow,
                                          const std::vector< int > & iIndexColorTraceRowContainer,
                                          const std::vector< int > & iIndexColorCollectionRowContainer,
                                          const std::string & iTraceName,
                                          const std::string & iCollectionName)
{
  if ( iIndexTWRow != -1 )
    {
    QTableWidgetItem *t_item = NULL;
    unsigned int NumberOfRows = iTWRowContainer.size();
    int NumberOfColumns = this->columnCount();

    for ( unsigned int i = 0; i < NumberOfRows; i++ )
      {
      if ( iTWRowContainer[i].first.ColumnNameTableWidget != "None"
           && !iTWRowContainer[i].second.empty() )
        {
        for ( int j = 0; j < NumberOfColumns; j++ )
          {
          std::string HeaderCol = this->horizontalHeaderItem(j)->text().toStdString();
          if ( HeaderCol == iTWRowContainer[i].first.ColumnNameTableWidget )
            {
            std::string Value = iTWRowContainer[i].second[iIndexTWRowContainer];
            t_item = this->item(iIndexTWRow, j);
            if (!t_item && this->CheckValueToDisplayData(Value, HeaderCol))
              {
              t_item = new QTableWidgetItem;
              t_item->setTextAlignment(Qt::AlignCenter);
              this->setItem(iIndexTWRow, j, t_item);
              }
            if ( t_item )
              {
              if ( iTWRowContainer[i].first.TypeName == "string" )
                {
                t_item->setData( 0, QString::fromStdString(Value) );
                }
              else
                {
                t_item->setData( 0, QString::fromStdString(Value).toDouble() );
                }
               }
             } //ENDIF
           }   //ENDFOR
         }     //ENDIF
        }       //ENDFOR
        this->SetColorForTable( iTWRowContainer, iIndexTWRowContainer,
                                iIndexColorTraceRowContainer, iTraceName, iIndexTWRow);
        if (iCollectionName != "None") //no collection for lineages
          {
          this->SetColorForTable( iTWRowContainer, iIndexTWRowContainer,
                                  iIndexColorCollectionRowContainer,
                                  iCollectionName, iIndexTWRow);
          }
        }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::InsertNewRow(const TWContainerType & iTWRowContainer,
                                  unsigned int iIndexTWRowContainer,
                                  const std::vector< int > & iIndexColorTraceRowContainer,
                                  const std::vector< int > & iIndexColorCollectionRowContainer,
                                  const std::string & iTraceName,
                                  const std::string & iCollectionName,
                                  Qt::CheckState iVisible)
{
  int IndexNewRow = this->rowCount();
  this->setRowCount(IndexNewRow + 1);
  this->DisplayDataForOneRow(iTWRowContainer, iIndexTWRowContainer,
                             IndexNewRow, iIndexColorTraceRowContainer,
                             iIndexColorCollectionRowContainer,
                             iTraceName, iCollectionName);
  this->SetSelectedColumn(IndexNewRow);
  this->SetVisibleColumn(IndexNewRow, iVisible);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::InsertNewRows(const TWContainerType & iTWRowContainer,
                                   const std::vector< int > & iIndexColorTraceRowContainer,
                                   const std::vector< int > & iIndexColorCollectionRowContainer,
                                   const std::string & iTraceName,
                                   const std::string & iCollectionName,
                                   Qt::CheckState iVisible)
{
  if ( !iTWRowContainer[1].second.empty() )
    {
    this->setSortingEnabled(false);
    for ( unsigned int i = 0; i < iTWRowContainer[1].second.size(); i++ )
      {
        this->InsertNewRow(iTWRowContainer, i, iIndexColorTraceRowContainer,
          iIndexColorCollectionRowContainer, iTraceName, iCollectionName,
          iVisible);
      }
    this->setSortingEnabled(true);
    this->resizeColumnsToContents();
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::InsertOnlyOneNewRow(const TWContainerType & iTWRowContainer,
                                         const std::vector< int > & iIndexColorTraceRowContainer,
                                         const std::vector< int > & iIndexColorCollectionRowContainer,
                                         const std::string & TraceName,
                                         const std::string & CollectionName,
                                         Qt::CheckState iVisible)
{
// no sorting inside for performance issue while import
//  this->setSortingEnabled(false);
  if ( iTWRowContainer.size() == 0 || iTWRowContainer[1].second.size() != 1 )
    {
    std::cout << "The New Trace Row Container is totally empty or there is more than 1 trace in it";
    std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
    std::cout << std::endl;
    }
  else
    {
    this->InsertNewRow(iTWRowContainer, 0,
                       iIndexColorTraceRowContainer,
                       iIndexColorCollectionRowContainer,
                       TraceName, CollectionName, iVisible);
    }
//  this->setSortingEnabled(true);
//  this->resizeColumnsToContents();
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
bool QGoTableWidget::CheckValueToDisplayData(const std::string & iValue,
                                             const std::string & iHeaderCol)
{
  if ( iValue == "" )
    {
    return false;
    }
  if ( iValue == "-1" && iHeaderCol.compare("T.I.Channel") > 0 )
    {
    return false;
    }
  return true;
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::UpdateRow(const TWContainerType & iTWRowContainer,
                               const std::vector< int > & iIndexColorTraceRowContainer,
                               const std::vector< int > & iIndexColorCollectionRowContainer,
                               const std::string & iTraceName,
                               const std::string & iCollectionName,
                               int iTraceID)
{
  if ( iTraceID != 0 )
    {
    this->setSortingEnabled(false);
    if ( iTWRowContainer.size() == 0 || iTWRowContainer[1].second.size() != 1 )
      {
      std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
      std::cout << std::endl;
      std::cout << "The Update Trace Row Container is totally empty or there is more than 1 trace in it";
      std::cout << std::endl;
      }
    else
      {
      QString TraceNameID = QString("%1ID").arg( iTraceName.c_str() );
      int     IndexUpdateRow = this->findValueGivenColumn(iTraceID, TraceNameID);
      if ( IndexUpdateRow != -1 )
        {
        this->DisplayDataForOneRow(iTWRowContainer,0,
                                   IndexUpdateRow,
                                   iIndexColorTraceRowContainer,
                                   iIndexColorCollectionRowContainer,
                                   iTraceName, iCollectionName);
        }
      else
        {
        std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
        std::cout << std::endl;
        std::cout << "the row doesn't exist";
        std::cout << std::endl;
        }
      }
    this->setSortingEnabled(true);
    }
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::DeleteCheckedRows(const std::string & iTraceNameID,
                                       const std::list< unsigned int > & iTraceIDs)
{
  this->setSortingEnabled(false);
  std::list< unsigned int >::const_iterator iter = iTraceIDs.begin();
  while ( iter != iTraceIDs.end() )
    {
    int RowToDelete = this->findValueGivenColumn( *iter, iTraceNameID.c_str() );
    removeRow(RowToDelete);
    ++iter;
    }
  this->setSortingEnabled(true);
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::UpdateColumnsWithCheckBoxes(int iRow, int iColumn)
{
  if ( this->horizontalHeaderItem(iColumn)->text() == "" )
    {
    //force the checkbox to change state, even if the user didn't click
    //directly in the checkbox but in the cell containing it:
    if ( this->item(iRow, iColumn)->checkState() == 0 )
      {
      this->setCheckedUncheckedStateCheckBox(this->item(iRow, iColumn), Qt::Checked, true);
      }
    else
      {
      this->setCheckedUncheckedStateCheckBox(this->item(iRow, iColumn), Qt::Unchecked, true);
      }
    }
  if ( this->horizontalHeaderItem(iColumn)->text() == "Show" )
    {
    if ( this->item(iRow, iColumn)->checkState() == 0 )
      {
      this->setVisibleStateCheckBox(this->item(iRow, iColumn), Qt::Checked, true);
      }
    else
      {
      this->setVisibleStateCheckBox(this->item(iRow, iColumn), Qt::Unchecked, true);
      }
    }
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::CopySelection()
{
  //Get only the first selected Range:
  QList< QTableWidgetSelectionRange > SelectedRanges = this->selectedRanges();
  //QTableWidgetSelectionRange range = SelectedRanges.first();
  QString str;
  for ( int k = 0; k < SelectedRanges.size(); k++ )
    {
    QTableWidgetSelectionRange range = SelectedRanges.at(k);
    this->PrepareRangeToCopy(range, str);
    str += "\n";
    }
  QApplication::clipboard()->setText(str);
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::CopyTable()
{
  //first, copy the column names:
  QStringList ColumnsNames = this->recordHeaderNamesOrder();
  QString     str;

  str += "IsSelected";
  str += "\t";
  for ( int i = 1; i < ColumnsNames.size() - 1; i++ )
    {
    str += ColumnsNames.at(i);
    str += "\t";
    }
  str += ColumnsNames.at(ColumnsNames.size() - 1);
  str += "\n";
  //second, get the range for the total selection:
  QTableWidgetSelectionRange RangeForAllTableSelected(
    0, 0, this->rowCount() - 1, this->columnCount() - 1);
  //third, copy the range:
  this->PrepareRangeToCopy(RangeForAllTableSelected, str);
  QApplication::clipboard()->setText(str);
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::PrepareRangeToCopy(const QTableWidgetSelectionRange & iRange,
                                        QString & istr)
{
  QTableWidgetItem *t_item = NULL;

  for ( int i = 0; i < iRange.rowCount(); ++i )
    {
    if ( i > 0 )
      {
      istr += "\n";
      }
    for ( int j = 0; j < iRange.columnCount() - 1; ++j )
      {
      int k = iRange.leftColumn() + j;
      if ( k == 0 )
        {
        //for the selected column:
        if ( item(i, 0)->checkState() == 0 )
          {
          istr += "No \t";
          }
        else
          {
          istr += "Yes \t";
          }
        }
      else
        {
        t_item = this->item(iRange.topRow() + i, iRange.leftColumn() + j);
        if ( t_item )
          {
          istr += t_item->text();
          }
        else
          {
          istr += "*";
          }
        istr += "\t";
        }
      }
    t_item =
      this->item(iRange.topRow() + i, iRange.leftColumn() + iRange.columnCount() - 1);

    if ( t_item )
      {
      istr += t_item->text();
      }
    else
      {
      istr += "*";
      }
    }
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::ChangeCheckStateSelectedRows(std::string iTraceName,
                                                  std::string iTraceNameID, Qt::CheckState iState)
{
  QStringList ListSelectedTracesID = this->ValuesForSelectedRows(
      iTraceNameID.c_str() );

  if ( !ListSelectedTracesID.empty() )
    {
    for ( int i = 0; i < ListSelectedTracesID.size(); i++ )
      {
      this->SetCheckStateForTraceID(ListSelectedTracesID.at(i).toUInt(),
                                    iTraceName, iState, false);
      }
    emit ModifyHighlightListTraces(ListSelectedTracesID, iState);
    }
  else
    {
    std::cout << "The list of selected Traces ID is empty";
    std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
    std::cout << std::endl;
    }
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::ChangeVisibilityStateSelectedRows(std::string iTraceName,
                                                       std::string iTraceNameID,
                                                       Qt::CheckState iState)
{
  QStringList ListSelectedTracesID = this->ValuesForSelectedRows(
      iTraceNameID.c_str() );

  if ( !ListSelectedTracesID.empty() )
    {
    for ( int i = 0; i < ListSelectedTracesID.size(); i++ )
      {
      this->SetVisibleStateForTraceID(ListSelectedTracesID.at(i).toUInt(), iTraceName,
                                      iState, false);
      }
    emit ModifyVisibilityListTraces(ListSelectedTracesID, iState);
    }
  else
    {
    std::cout << "The list of selected Traces ID is empty";
    std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
    std::cout << std::endl;
    }
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::AddValuesForID(const std::vector< std::string > & iColumnsNames,
                                    const std::vector< std::string > & iValues,
                                    unsigned int iID,
                                    const std::string & iColumnNameForTraceID)
{
  int RowIndex = this->findValueGivenColumn( iID, iColumnNameForTraceID.c_str() );

  if ( RowIndex != -1 )
    {
    for ( unsigned int i = 0; i < iColumnsNames.size(); i++ )
      {
      int ColumnIndex = this->findColumnName( iColumnsNames.at(i).c_str() );
      if ( ColumnIndex != -1 )
        {
        QTableWidgetItem *CellTable = new QTableWidgetItem;
        CellTable->setData( 0, QString::fromStdString( iValues.at(i) ).toDouble() );
//QString::fromStdString( iValues.at(i)));
        CellTable->setTextAlignment(Qt::AlignCenter);
        this->setItem(RowIndex, ColumnIndex, CellTable);
        }
      }
    }
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
GoDBCoordinateRow QGoTableWidget::GetCoordinateCenterBoundingBox(
  unsigned int iTraceID, const std::string & iTraceName)
{
  GoDBCoordinateRow CenterCoord;
  int               RowIndex = this->GetRowForTraceID(iTraceID, iTraceName);

  if ( RowIndex != -1 )
    {
    CenterCoord.SetField( "TCoord",
                          this->GetValueForItem("TimePoint", RowIndex) );
    CenterCoord.SetField( "XCoord",
                          this->GetMeanValue("XMax", "XMin", RowIndex) );
    CenterCoord.SetField( "YCoord",
                          this->GetMeanValue("YMax", "YMin", RowIndex) );
    CenterCoord.SetField( "ZCoord",
                          this->GetMeanValue("ZMax", "ZMin", RowIndex) );
    }
  return CenterCoord;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
QString QGoTableWidget::GetValue(unsigned int iTraceID,
                                 const std::string & iTraceName,
                                 const std::string & iColumn)
{
  int RowIndex = this->GetRowForTraceID(iTraceID, iTraceName);
  return this->item(
          RowIndex, this->findColumnName( iColumn.c_str() ) )->text();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
int QGoTableWidget::GetValueForItem(const std::string & iColumnName, int iRowIndex)
{
  return
    this->item(
      iRowIndex, this->findColumnName( iColumnName.c_str() ) )->text().toInt();
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
std::string QGoTableWidget::GetMeanValue(const std::string & iColumnNameOne,
                                         const std::string & iColumnNameTwo,
                                         unsigned int iRowIndex)
{
  int ValueOne = this->GetValueForItem(iColumnNameOne, iRowIndex);
  int ValueTwo = this->GetValueForItem(iColumnNameTwo, iRowIndex);
  int meanValue = ( ValueOne + ValueTwo ) / 2;

  return ConvertToString< int >(meanValue);
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::setCheckedUncheckedStateCheckBox(QTableWidgetItem *iItem,
                                                      Qt::CheckState iState,
                                                      bool EmitSignal)
{
  if( iItem )
    {
    if ( this->setCheckStateCheckBox(iItem, iState) && EmitSignal )
      {
      int  Row = iItem->row();
      emit CheckedRowsChanged( this->item(Row, 1)->text().toInt() );
      }
    }
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::setVisibleStateCheckBox(QTableWidgetItem *iItem,
                                             Qt::CheckState iState,
                                             bool EmitSignal)
{
  if( iItem )
    {
    if ( this->setCheckStateCheckBox(iItem, iState) )
      {
      QIcon Icon;
      if ( iState == Qt::Checked )
        {
        Icon.addPixmap(QPixmap( QString::fromUtf8(":/fig/EyeIcon.png") ),
                       QIcon::Normal, QIcon::Off);
        }
      else
        {
        Icon.addPixmap(QPixmap( QString::fromUtf8(":/fig/BlankIcon.png") ),
                       QIcon::Normal, QIcon::Off);
        }
      if ( EmitSignal )
        {
        int Row = iItem->row();
        VisibleRowsChanged( this->item(Row, 1)->text().toInt() );
        }
      iItem->setIcon(Icon);
      }
    }
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
bool QGoTableWidget::setCheckStateCheckBox(QTableWidgetItem *iItem,
                                           Qt::CheckState iState)
{
  bool oModification = false;

  if( iItem )
    {
    if ( iState == Qt::Checked )
      {
      //if the row is already checked, no need to do anything:
      if ( iItem->checkState() != 2 )
        {
        iItem->setCheckState(Qt::Checked);
        iItem->setText("1");
        oModification = true;
        }
      }
    else
      {
      //if the row is already unchecked, no need to do anything:
      if ( iItem->checkState() != Qt::Unchecked )
        {
        iItem->setCheckState(Qt::Unchecked);
        iItem->setText("0");
        oModification = true;
        }
      }
    }
  return oModification;
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
std::map< unsigned int, std::string >
QGoTableWidget::GetTraceIDAndColumnsValues(const std::string & iTraceIDName,
                                           std::string & ioColumnName)
{
  std::map< unsigned int, std::string > oMapValues =
    std::map< unsigned int, std::string >();
  QList< QTableWidgetSelectionRange > Ranges = this->selectedRanges();
  if ( Ranges.size() > 1 || Ranges[0].columnCount() > 1 )
    {
    QMessageBox msgBox;
    msgBox.setText(
      tr("Please choose only one column to color code") );
    msgBox.exec();
    return oMapValues;
    }
  unsigned int ColumnIndex = Ranges[0].leftColumn();
  ioColumnName = this->horizontalHeaderItem(ColumnIndex)->text().toStdString();
  int          NbOfRows = this->rowCount();
  unsigned int IndexTraceID = this->findColumnName( iTraceIDName.c_str() );
  for ( int i = 0; i < NbOfRows; i++ )
    {
    if ( this->item(i, ColumnIndex) )
      {
      std::string Text = this->item(i, ColumnIndex)->text().toStdString();   //for
                                                                             //
                                                                             //
                                                                             //
                                                                             //
                                                                             //
                                                                             // test
                                                                             //
                                                                             //
                                                                             //
                                                                             //
                                                                             //
                                                                             // purpose
      oMapValues[this->item(i, IndexTraceID)->text().toUInt()] =
        this->item(i, ColumnIndex)->text().toStdString();
      }
    else
      {
      oMapValues[this->item(i, IndexTraceID)->text().toUInt()] = "";
      }
    }
  return oMapValues;
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::ShowOnlyRowsForTimePoint(unsigned int iTimePoint)
{
  int ColumnIndex = this->findColumnName("TimePoint");

  if ( ColumnIndex == -1 )
    {
    std::cout << "column TimePoint not found in the table widget";
    std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
    std::cout << std::endl;
    return;
    }
  for ( int i = 0; i < this->rowCount(); i++ )
    {
    if ( this->item(i, ColumnIndex)->text().toUInt() != iTimePoint )
      {
      this->hideRow(i);
      }
    else
      {
      this->showRow(i);
      }
    }
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::ShowAllRows()
{
  for ( int i = 0; i < this->rowCount(); i++ )
    {
    this->showRow(i);
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoTableWidget::DeleteRowsWithSpecificTimePoints( const QStringList & iListTPs)
{
  int IndexColumnTime = this->findColumnName("TimePoint");

  if (IndexColumnTime != -1)
    {
    this->setSortingEnabled(false);
    for (int i = rowCount()-1; i>=0; --i)
      {
        if (iListTPs.contains(this->item(i, IndexColumnTime)->text() ) )
        {
        this->removeRow(i);
        }
      }
    this->setSortingEnabled(true);
    }

}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
QGoTableWidget::
DeleteRowsAndColumns()
{
  for(int i=rowCount()-1; i>=0; --i)
    {
    this->removeRow(i);
    }

  for(int i=columnCount()-1; i>=0; --i)
    {
    this->removeColumn(i);
    }
}
//--------------------------------------------------------------------------

/*=========================================================================
  Author: $Author$  // Author of last commit
  Version: $Rev$  // Revision of last commit
  Date: $Date$  // Date of last commit
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
#ifndef __QGoTabImageView3DwT_h
#define __QGoTabImageView3DwT_h

#include "QGoTabElementBase.h"

#include "GoFigureFileInfoMultiIndexContainerHelper.h"
#include "itkMegaCaptureReader.h"
#include "ContourMeshStructureHelper.h"
#include "QGoPrintDatabase.h"
#include "QGoTraceManualEditingDockWidget.h"
#include <QHBoxLayout>
#include <string>

#include "vtkSmartPointer.h"

class QGoImageView3D;
class QGoVisualizationDockWidget;
class QGoManualSegmentationDockWidget;
class QGoPrintDatabase;
class QGoOneClickSegmentationDockWidget;
class QGoManualSegmentationSettingsDialog;
class QGoTraceManualEditingWidget;

#if defined ENABLEFFMPEG || defined ENABLEAVI
  class QGoVideoRecorder;
#endif /* ENABLEFFMPEG */

class vtkLSMReader;
class vtkImageData;
class vtkContourWidget;
class vtkOrientedGlyphContourRepresentation;
class vtkMySQLDatabase;
class vtkProperty;
// class vtkQuadricLODActor;
class vtkActor;
class vtkViewImage2D;

#include "QGoGUILibConfigure.h"

/**
\class QGoTabImageView3DwT
\brief
\example GUI/lib/qgotabimageview3dwt.cxx
*/
class QGOGUILIB_EXPORT QGoTabImageView3DwT : public QGoTabElementBase
{
  Q_OBJECT
public:
  /**
   * \brief Default Constructor
   * @param parent
   */
  QGoTabImageView3DwT( QWidget* parent = 0 );


  /**
   * \brief Destructor
   */
  virtual ~QGoTabImageView3DwT();

  typedef QGoTabElementBase::QGoDockWidgetStatusPair QGoDockWidgetStatusPair;
  typedef QGoPrintDatabase::NamesDescrContainerType   NamesDescrContainerType;

  /**
   * \brief
   * @return
   */
  GoFigure::TabDimensionType GetTabDimensionType( ) const;

  /**
   *
   * @param iReader
   * @param iTimePoint
   */
  void SetLSMReader( vtkLSMReader* iReader, const int& iTimePoint );

  /**
   *
   * @param iContainer
   * @param iFileType
   * @param iHeader
   * @param iTimePoint
   */
  void SetMegaCaptureFile(
    const GoFigureFileInfoHelperMultiIndexContainer& iContainer,
    const GoFigure::FileType& iFileType,
    const std::string& iHeader,
    const unsigned int& iTimePoint  );

  /** \todo must modify container */
  //void AddPolyData( vtkPolyData* iMesh );

  /**
   *
   */
  virtual void Update();

  /**
   *
   * @param parent
   */
  void setupUi( QWidget* parent );
  /**
   *
   * @param parent
   */
  void retranslateUi( QWidget *parent );

  /**
   *
   * @return
   */
  virtual std::list< QWidget* > AdditionalWidget()
    {return std::list< QWidget* >(); }

  /**
   *
   */
  virtual void WriteSettings() {}
  /**
   *
   */
  virtual void ReadSettings() {}

  /**
   *
   * @param[in] iNodes Nodes to be used by
   * @param[in] iRgba[]
   * @param[in] iHighlighted
   */
  void AddContourFromNodes( const unsigned int& iContourID, vtkPolyData* iNodes, double iRgba[4],
    const bool& iHighlighted, const unsigned int& iTCoord, const bool& iSaveInDataBase );
  void AddContourFromNodes( const unsigned int& iContourID, vtkPolyData* iNodes,
    const double& iR, const double& iG, const double& iB, const double& iA,
    const bool& iHighlighted, const unsigned int& iTCoord, const bool& iSaveInDataBase );

  void AddMeshFromNodes( const unsigned int& iContourID, vtkPolyData* iNodes, double iRgba[4],
    const bool& iHighlighted, const unsigned int& iTCoord, const bool& iSaveInDataBase );
  void AddMeshFromNodes( const unsigned int& iContourID, vtkPolyData* iNodes,
    const double& iR, const double& iG, const double& iB, const double& iA,
    const bool& iHighlighted, const unsigned int& iTCoord, const bool& iSaveInDataBase );

  void LoadAllContoursForGivenTimePoint( const unsigned int& iT );
  void RemoveAllContoursForGivenTimePoint( const unsigned int& iT );

  void LoadAllMeshesForGivenTimePoint( const unsigned int& iT );
  void RemoveAllMeshesForGivenTimePoint( const unsigned int& iT );

  int GetSliceViewXY() const;
  int GetSliceViewXZ() const;
  int GetSliceViewYZ() const;
  int GetTimePoint() const;
  QGoManualSegmentationDockWidget* GetManualSegmentationWidget();
  QGoTraceManualEditingWidget*     GetTraceManualEditingWidget();
  QGoPrintDatabase* m_DataBaseTables;

signals:
  void TimePointChanged( int TimePoint );
  void SliceViewXYChanged( int Slice );
  void SliceViewXZChanged( int Slice );
  void SliceViewYZChanged( int Slice );
  void FullScreenViewChanged( int FullScreen );
  void UpdateBookmarkOpenActions(std::vector<QAction*>);
  void ContourRepresentationPropertiesChanged();

public slots:
  void SetTimePoint( const int& );
  void DisplayAnnotations();
  void DisplaySplinePlanes();
  void DisplayCube();
  void TakeSnapshot();
  void SetSliceView();

  void DefaultMode();
  void ZoomMode();
  void PanMode();
  void OneClickMode();

  void GenerateContourRepresentationProperties();

#if defined ( ENABLEFFMPEG ) || defined ( ENABLEAVI )
  void SetRendererWindow( int );
#endif /* ENABLEVIDEORECORD */

  QString SnapshotViewXY( const GoFigure::FileType& iType,
    const QString& iBaseName = QString( "snapshot" ) );
  QString SnapshotView2( const GoFigure::FileType& iType,
    const QString& iBaseName = QString( "snapshot" ) );
  QString SnapshotView3( const GoFigure::FileType& iType,
    const QString& iBaseName = QString( "snapshot" ) );
  QString SnapshotViewXYZ( const GoFigure::FileType& iType,
    const QString& iBaseName = QString( "snapshot" ) );

  void SetSliceViewXY( const int& );
  void SetSliceViewXZ( const int& );
  void SetSliceViewYZ( const int& );

  void SetFullScreenView( const int& iS );
  void Quadview();
  void FullScreenViewXY();
  void FullScreenViewXZ();
  void FullScreenViewYZ();
  void FullScreenViewXYZ();

  void ChangeLookupTable();
  void ShowScalarBar( const bool& );
  void ChangeBackgroundColor();

  void ShowAllChannels( bool iChecked );
  void ShowOneChannel( int iChannel );

  void LoadAllContoursForCurrentTimePoint();
  void RemoveAllContoursForPresentTimePoint();

  void LoadAllMeshesForCurrentTimePoint();
  void RemoveAllMeshesForPresentTimePoint();

  void ActivateManualSegmentationEditor( const bool& iActivate );
  void ValidateContour();
  int  SavePolyDataAsContourInDB( vtkPolyData* iView );
  void SavePolyDataAsVolumeInDB( vtkPolyData* iView );
  void ReinitializeContour();
  void ReEditContour( const unsigned int& iId );

  void HighLightContours();
  void HighLightContoursFromTable();
  void SelectContoursInTable();
  void DeleteContoursFromTable( const std::list< int >& iList );

  void HighLightMeshesFromTable();

  void ChangeContourRepresentationProperty();

  /** \brief Get the info for the new created collection from the collectionIDcombobox,
  pass them to the database to be saved in and update the list of collection IDs
  in the visu dock widget*/
  void UpdateDBAndCollectionIDComboBoxForANewCreatedCollection();
  //void PassInfoForDBFromCollectionIDComboBox();

  /** \brief Get the info for the current selected color from the visu widget and pass it
  to the database*/
  void PassInfoForDBForCurrentSelectedColor();
  /** \brief Get the current selected collectionid from the visu dock widget and update the
  currentCollectionID in the table widget*/
  void PassInfoForCurrentCollectionID();

  void Change3DPerspectiveToAxial();
  void Change3DPerspectiveToCoronal();
  void Change3DPerspectiveToSagittal();

  void ApplyOneClickSegmentationFilter();

protected:
  QHBoxLayout*          m_LayOut;
  QGoImageView3D*       m_ImageView;
  std::vector< vtkSmartPointer< vtkLSMReader > > m_LSMReader;
  std::vector< vtkSmartPointer< vtkImageData > > m_InternalImages;
  vtkSmartPointer< vtkImageData > m_Image;

  double m_LinesWidth;
  QColor m_LinesColor;
  QColor m_NodesColor;
  QColor m_ActiveNodesColor;

  itk::MegaCaptureReader::Pointer           m_MegaCaptureReader;
  GoFigureFileInfoHelperMultiIndexContainer m_FileList;
  GoFigure::FileType    m_FileType;
  QColor                m_BackgroundColor;
  QAction*              m_BackgroundColorAction;
  QAction*              m_TakeSnapshotAction;
  int                   m_TimePoint;
  unsigned int          m_ContourId;
  unsigned int          m_MeshId;
  bool                  m_ReEditContourMode;

  /// \todo rename as QGoVisualizationDockWidget
  QGoVisualizationDockWidget*         m_NavigationDockWidget;
  QGoManualSegmentationDockWidget*    m_ManualSegmentationDockWidget;
  QGoOneClickSegmentationDockWidget*  m_OneClickSegmentationDockWidget;

  /// \bug Creates one leak
  /// \todo why here?
  vtkSmartPointer<vtkPoints>        m_SeedsWorldPosition;

  QGoTraceManualEditingDockWidget*     m_TraceManualEditingDockWidget;
  //QDockWidget*                 m_test;

    

   /// \todo remove m_FFMPEGWriter and m_AVIWriter from this class
  #if defined ENABLEFFMPEG || defined ENABLEAVI
    QGoVideoRecorder*                         m_VideoRecorderWidget;
  #endif /* ENABLEFFMPEG || ENABLEAVI */

  /// \todo move the three following instance into the visualization element.
  std::vector< vtkSmartPointer< vtkContourWidget > >                      m_ContourWidget;
  std::vector< vtkSmartPointer< vtkOrientedGlyphContourRepresentation > > m_ContourRepresentation;
  ContourMeshStructureMultiIndexContainer                   m_ContourContainer;
  ContourMeshStructureMultiIndexContainer                   m_MeshContainer;

  /**
   * @param[in] iContourID
   * @param[in] iDir
   * @param[in] iHighlighted
   * @param[in] iR red component in [0,1]
   * @param[in] iG green component in [0,1]
   * @param[in] iB blue component in [0,1]
   * @param[in] iA alpha component in [0,1]
   * @param[in] iSaveInDataBase save in data base if true
   * \todo Alpha component is not used at all, it is assumed to be opaque
   */
  virtual void ValidateContour( const int& iContourID, const int& iDir,
    const double& iR, const double& iG, const double& iB, const double& iA,
    const bool& iHighlighted, const unsigned int& iTCoord, const bool& iSaveInDataBase );

  /**
   * @param[in] iContourID
   * @param[in] iDir
   * @param[in] iHighlighted
   * @param[in] iR red component in [0,1]
   * @param[in] iG green component in [0,1]
   * @param[in] iB blue component in [0,1]
   * @param[in] iA alpha component in [0,1]
   * @param[in] iSaveInDataBase save in data base if true
   * \todo Alpha component is not used at all, it is assumed to be opaque
   */
  virtual int SavePolyDataAsContourInDB( vtkPolyData* iView, const int& iContourID, const int& iDir,
    const double& iR, const double& iG, const double& iB, const double& iA,
    const bool& iHighlighted, const unsigned int& iTCoord, const bool& iSaveInDataBase );

  /**
   * @param[in] iContourID
   * @param[in] iDir
   * @param[in] iHighlighted
   * @param[in] iR red component in [0,1]
   * @param[in] iG green component in [0,1]
   * @param[in] iB blue component in [0,1]
   * @param[in] iA alpha component in [0,1]
   * @param[in] iSaveInDataBase save in data base if true
   * \todo Alpha component is not used at all, it is assumed to be opaque
   * \todo Change method name. Volume is not coherent with DB design.
   */
  virtual void SavePolyDataAsVolumeInDB( vtkPolyData* iView, const int& iContourID, const int& iDir,
    const double& iR, const double& iG, const double& iB, const double& iA,
    const bool& iHighlighted, const unsigned int& iTCoord, const bool& iSaveInDataBase );

  void GetBackgroundColorFromImageViewer( );
  void SetBackgroundColorToImageViewer( );
  void CreateAllViewActions();
  void CreateToolsActions();
  void CreateBookmarkActions();
  void CreateModeActions();
  void CreateVisuDockWidget();
  void CreateSettingAndDialogSegmentationWidgets();
  void CreateManualSegmentationdockWidget();
  void CreateOneClickSegmentationDockWidget();
  void CreateDataBaseTablesConnection();

  void OneClickSphereContours();
  void OneClickSphereVolumes();
  void LevelSetSegmentation2D();
  void LevelSetSegmentation3D();

#if defined ( ENABLEFFMPEG ) || defined ( ENABLEAVI )
  void CreateVideoRecorderWidget();
#endif /* ENABLEVIDEORECORD */

  void RemoveActorFromViewer( const int& iId, vtkActor* iActor );
  void DisplayActorInViewer( const int& iId, vtkActor* iActor );

  int* GetImageCoordinatesFromWorldCoordinates( double pos[3] );

//   std::vector< vtkQuadricLODActor* >
  std::vector< vtkActor* > AddContour( const int& iId,
      vtkPolyData* dataset,
      vtkProperty* property = NULL );

  void SetTimePointWithLSMReaders( const int& iTimePoint );
  void SetTimePointWithMegaCapture( const int& iTimePoint );

  std::vector< vtkSmartPointer<vtkPolyData> > CreateSphereContours(
      vtkViewImage2D& iView, double iCenter[3], double iRadius );
  vtkSmartPointer<vtkPolyData> GenerateCircleFromGivenSphereAndGivenZ( double iC[3],
      const double& iRadius, double iZ, const int& iN );

  vtkSmartPointer<vtkPolyData> CreateSphereVolume( vtkViewImage2D* iView,
      double iCenter[3], double iRadius );

protected slots:
  void AddBookmark();
  void GetTheOpenBookmarksActions();
  void OpenExistingBookmark();
  void SetTheCurrentCellType();
  void SetTheCurrentSubCellType();
  void ShowTraceDockWidgetForContour(bool ManualSegVisible);
  void ShowTraceDockWidgetForMesh(bool OneClickVisible);
  void GoToDefaultMenu(std::string iTracename,std::string iCollectionName);

};

#endif

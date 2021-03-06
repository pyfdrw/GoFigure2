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
#ifndef __QGoTabImageView3DwT_h
#define __QGoTabImageView3DwT_h

#include "QGoTabElementBase.h"

#include "GoFigureFileInfoMultiIndexContainerHelper.h"
#include "itkMegaCaptureReader.h"

#include "ContourContainer.h"
#include "MeshContainer.h"
#include "TrackContainer.h"
#include "TraceContainerBase.h"

#include "QGoPrintDatabase.h"

#include "GoFigureMeshAttributes.h"

#include <QHBoxLayout>
#include <QSplitter>
#include <string>

#include "vtkSmartPointer.h"

#include "GoImageProcessor.h"

// base segmentation dock widget
class QGoContourEditingWidgetManager;
class QGoMeshEditingWidgetManager;
//class QGoContourSegmentationBaseDockWidget;
//class QGoMeshSegmentationBaseDockWidget;

//track dockwidget
class QGoTrackViewDockWidget;

//lineage dock widget
class QGoLineageViewDockWidget;

// lineage viewer
class QGoLineageViewerWidget;

class QGoImageView3D;
class QGoNavigationDockWidget;
class QGoTransferFunctionDockWidget;
class GoTransferFunctionEditorWidget;
class QGoPrintDatabase;

#if defined ENABLEFFMPEG || defined ENABLEAVI
class QGoVideoRecorder;
#endif /* ENABLEFFMPEG */

class vtkLSMReader;
class vtkImageData;

class vtkMySQLDatabase;
class vtkProperty;
// class vtkQuadricLODActor;
class vtkActor;
class vtkViewImage2D;
class vtkProp3D;

class QGoSeedsSegmentation;

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
   * \param parent
   */
  explicit QGoTabImageView3DwT(QWidget *parent = 0);

  /**
   * \brief Destructor
   */
  virtual ~QGoTabImageView3DwT();

  typedef QGoTabElementBase::QGoDockWidgetStatusPair QGoDockWidgetStatusPair;
  typedef QGoPrintDatabase::NamesDescrContainerType  NamesDescrContainerType;
  typedef QGoPrintDatabase::IDWithColorData          IDWithColorData;

  /**
   * \brief
   * \return
   */
  GoFigure::TabDimensionType GetTabDimensionType() const;

  /**
   *
   * \param[in] iReader
   * \param[in] iTimePoint
   */
  void SetLSMReader(vtkLSMReader *iReader, const int & iTimePoint);

  /**
   *
   * \param[in] iContainer  MegaCapture file container
   * \param[in] iFileType   file type (PNG, JPEG...)
   * \param[in] iHeader     path to MegaCapture header
   * \param[in] iTimePoint  Time point
   */
  void SetMegaCaptureFile(
    const GoFigureFileInfoHelperMultiIndexContainer & iContainer,
    const GoFigure::FileType & iFileType,
    const std::string & iHeader,
    const unsigned int & iTimePoint);

  /**
   * \brief
   * \param parent
   */
  void setupUi(QWidget *parent);

  void CreateModeToolBar(QMenu* iMenu, QToolBar* iToolBar);

  /**
   * \brief
   * \param parent
   */
  void retranslateUi(QWidget *parent);

  /**
   *
   */
  virtual void WriteSettings() {}
  /**
   *
   */
  virtual void ReadSettings() {}

  ContourContainer * GetContourContainer()
  {
    return m_ContourContainer;
  }

  MeshContainer * GetMeshContainer()
  {
    return m_MeshContainer;
  }

  TrackContainer * GetTrackContainer()
  {
    return m_TrackContainer;
  }

  template< class TIndex >
  void AddMeshFromNodes(
    typename ContourMeshContainer::MultiIndexContainerType::index< TIndex >::type::iterator
      iIt )
  {
    VisualizeMesh< TIndex >(iIt);
  }

  template< class TIndex >
  void AddTrackFromNodes(
    typename TrackContainer::MultiIndexContainerType::index< TIndex >::type::iterator
      iIt )
  {
    VisualizeTrack< TIndex >(iIt);
  }

  //-------------------------------------------------------------------------
  template< class TIndex >
  void
  AddContourFromNodes(
    typename ContourContainer::MultiIndexContainerType::index< TIndex >::type::iterator
      iIt )
  {
    vtkPolyData *nodes = iIt->Nodes;

    if ( nodes->GetNumberOfPoints() > 2 )
      {
      int dir = ContourMeshContainer::ComputeDirectionFromContour(nodes);

      if ( dir != -1 )
        {
        m_ImageView->EnableContourWidget(true);
        m_ImageView->InitializeContourWidgetNodes(dir, nodes);
        vtkPolyData *contour = m_ImageView->GetContourRepresentationAsPolydata(dir);

        VisualizeContour< TIndex >(iIt, contour);

        m_ImageView->InitializeContourWidgetNodes(dir, NULL);
        m_ImageView->EnableContourWidget(false);
        }
      }
  }

  int GetSliceViewXY() const;

  int GetSliceViewXZ() const;

  int GetSliceViewYZ() const;

  int GetTimePoint() const;

  int GetTimeInterval() const;

  //QGoTraceManualEditingWidget *     GetTraceManualEditingWidget();

  QGoPrintDatabase *m_DataBaseTables;

  GoFigureMeshAttributes ComputeMeshAttributes(vtkPolyData *iMesh,
                                               const bool& iIntensity,
                                               const unsigned int& iTCoord );

  void InitializeToolsForTracesToolBar(QMenu* iMenu, QToolBar* iToolBar);
  void InitializeTraceSettingsToolBar(QToolBar* iToolBar);

  /**
    \brief Creates actors for the contours which are at the given time points
    in the container. The actors are not visible by default. (see ShowTraces)
    \param[in] iTPointToLoad Time points to be used to create actors.
    */
  void CreateContoursActorsFromVisuContainer(
          std::list<unsigned int> iTPointToLoad);

  void CreateContoursActorsFromVisuContainer();

  /**
    \brief Creates actors for the meshes which are at the given time points
    in the container. The actors are not visible by default. (see ShowTraces)
    \param[in] iTPointToLoad Time points to be used to create actors.
    */
  void CreateMeshesActorsFromVisuContainer(
          std::list<unsigned int> iTPointToLoad);

  void CreateMeshesActorsFromVisuContainer();

  /**
    \brief Show traces from container which are at the given time point.
    Only updates contours and meshes since tracks and lineages go over several
    time points.
    \param[in] iTimePoint Visible time point.
    */
  void ShowTraces(const unsigned int& iTimePoint);

  void UpdateTFEditor();

signals:
  void TimePointChanged(int TimePoint);

  void SliceViewXYChanged(int Slice);

  void SliceViewXZChanged(int Slice);

  void SliceViewYZChanged(int Slice);

  void FullScreenViewChanged(int FullScreen);

  void UpdateBookmarkOpenActions(std::vector< QAction * > );

  void ContourRepresentationPropertiesChanged();

  void StartMeshSegmentation(vtkPoints *iPoints);

  void StartContourSegmentation(vtkPoints *iPoints);

  void RequestedPolydatas(std::list< vtkPolyData* >);

public slots:

  void AdjustWindowLevel(double iMin, double iMax);

  void SetTimePoint(const int &);

  void TakeSnapshot();

  void SetSliceView();

  void GoToDefaultMenu(bool iEnable = false);

  void updatePoints(QString,
                    std::map< unsigned int, unsigned int >,
                    QColor,
                    int,
                    int,
                    int);

#if defined ( ENABLEFFMPEG ) || defined ( ENABLEAVI )
  void SetRendererWindow(int);
#endif /* ENABLEVIDEORECORD */

  QString SnapshotViewXY( GoFigure::FileType iType,
                          QString iBaseName = QString("snapshot-xy-") );

  QString SnapshotViewXZ( GoFigure::FileType iType,
                          QString BaseName = QString("snapshot-xz-") );

  QString SnapshotViewYZ( GoFigure::FileType iType,
                          QString iBaseName = QString("snapshot-yz-") );

  QString SnapshotViewXYZ( GoFigure::FileType iType,
                           QString iBaseName = QString("snapshot-xyz-") );

  void SetSliceViewXY(int );

  void SetSliceViewXZ(int );

  void SetSliceViewYZ(int );

  void SetFullScreenView(int iS);

  void Quadview();

  void FullScreenViewXY();

  void FullScreenViewXZ();

  void FullScreenViewYZ();

  void FullScreenViewXYZ();

  //void ChangeLookupTable();

  void ChangeBackgroundColor();

  void ModeChanged(int iChannel);

  void StepChanged(int iStep);

  void DopplerSizeChanged( int iSize);

  void ValidateContour(int iTCoord);

  int SaveAndVisuContour(int iTCoord, vtkPolyData *iView = NULL);

  //void SaveAndVisuContoursList(std::vector<vtkPolyData* >* iContours);

  /** \brief Save a mesh in the database and render the mesh
   * at the given time point.
   * \param[in] iCollection Collection ID we want the mesh to belong to.
   * if -1, we get the collection ID from the trace editing widget.
  \todo to be renamed */
  void  SaveAndVisuMesh(vtkPolyData *iView,
                        unsigned int iTCoord,
                        int iCollection = -1);

  /** \brief Save a mesh in the database and render the mesh.
   * at the current time point
  */
  void SaveInDBAndRenderMeshForVisu(
    std::vector<vtkPolyData *> iVectPolydata, int iTCoord);

  void SplitInDBAndRenderMeshForVisu(
    std::vector<vtkPolyData *> iVectPolydata);

  void MergeInDBAndRenderMeshForVisu( vtkPolyData * iVectPolydata);

  void SaveInDBAndRenderSetOfContoursForVisu(
    std::vector<std::vector<vtkPolyData*> >, int);

  void SaveInDBAndRenderContourForVisu(
    std::vector<vtkPolyData *> iVectPolydata, int iTCoord);

  void ReEditContour(const unsigned int & iId);

  void HighlightPickedActor();
  void VisibilityPickedActor();

  void Change3DPerspectiveToAxial();

  void Change3DPerspectiveToCoronal();

  void Change3DPerspectiveToSagittal();

  void CreateMeshFromSelectedContours(std::list< unsigned int > ListContourIDs, int iMeshID);

  void AddContourForMeshToContours(vtkPolyData *);

  void visibilityChanged(QString iName, bool iVisibility);

  void updateSlot();

  void PolydatasRequested();

protected:
  QGoImageView3D *                               m_ImageView;

  vtkProperty *m_HighlightedContoursProperty;
  vtkProperty *m_HighlightedMeshesProperty;

  GoImageProcessor*                         m_ImageProcessor;
  // TODO Nicolas don't need it...
  itk::MegaCaptureReader::Pointer           m_MegaCaptureReader;
  GoFigureFileInfoHelperMultiIndexContainer m_FileList;
  GoFigure::FileType                        m_FileType;
  QColor                                    m_BackgroundColor;
  QAction *                                 m_BackgroundColorAction;
  QAction *                                 m_TakeSnapshotAction;
  QToolBar*                                 m_TraceSettingsToolBar;

  float m_IntersectionLineWidth;

  int m_PCoord;
  int m_RCoord;
  int m_CCoord;
  int m_XTileCoord;
  int m_YTileCoord;
  int m_ZTileCoord;
  int m_TCoord;

  QGoNavigationDockWidget *m_NavigationDockWidget;
  QGoTransferFunctionDockWidget *m_TransferFunctionDockWidget;

  // base segmentation dockwidget for contours
  //QGoContourSegmentationBaseDockWidget *m_ContourSegmentationDockWidget;

  // base segmentation dockwidget for meshes
  //QGoMeshSegmentationBaseDockWidget *m_MeshSegmentationDockWidget;
  QGoMeshEditingWidgetManager*       m_MeshEditingWidget;
  QGoContourEditingWidgetManager*    m_ContourEditingWidget;

  QGoTrackViewDockWidget*   m_TrackViewDockWidget;

  QGoLineageViewDockWidget* m_LineageViewDockWidget;

  QGoLineageViewerWidget*            m_QGoLineageViewerWidget;

  QGoTraceSettingsWidget*   m_TraceSettingsWidget;

  std::vector< vtkPoints* > m_Seeds;
  //std::vector<vtkPoints*> m_OrderedSeeds;

  ContourContainer *m_ContourContainer;
  MeshContainer    *m_MeshContainer;
  TrackContainer   *m_TrackContainer;
  LineageContainer *m_LineageContainer;

  //bool m_TraceWidgetRequiered;

  /// \todo remove m_FFMPEGWriter and m_AVIWriter from this class

  #if defined ENABLEFFMPEG || defined ENABLEAVI
  QGoVideoRecorder *m_VideoRecorderWidget;
  #endif /* ENABLEFFMPEG || ENABLEAVI */

  void SaveContour(vtkPolyData *contour, vtkPolyData *contour_nodes, int iTCoord);

  std::vector< vtkActor * > VisualizeTrace(vtkPolyData *iTrace, double* iRGBA);

  //int VisualizeContour(const int& iContourID,
  //    const unsigned int& iTCoord, vtkPolyData* contour,
  //    vtkPolyData* contour_nodes, const double iRGBA[4]);

  template< class TIndex >
  void VisualizeContour(
    typename ContourContainer::MultiIndexContainerType::template index< TIndex >::type::iterator iIt,
    vtkPolyData *iContour)
  {
    if ( ( iContour->GetNumberOfPoints() > 2 ) && ( m_TCoord >= 0 ) )
      {
      bool visibility =
        ( static_cast< unsigned int >( m_TCoord ) == iIt->TCoord );
      bool highlighted = false;

      vtkPolyData *contour_copy = vtkPolyData::New();
      contour_copy->DeepCopy(iContour);

      AddTraceIDIntoPolydata(contour_copy, iIt->TraceID, "CONTOUR");

      VisualizeTraceBase< ContourContainer, TIndex >( m_ContourContainer, iIt,
                                                      highlighted, visibility,
                                                      contour_copy );

      contour_copy->Delete();
      }
  }

  template< class TIndex >
  void
  VisualizeMesh(
    typename MeshContainer::MultiIndexContainerType::template index< TIndex >::type::iterator iIt)
  {
    if ( iIt->Nodes )
      {
      bool highlighted = false;
      bool visibility = false;

      AddTraceIDIntoPolydata(iIt->Nodes, iIt->TraceID, "MESH");

      VisualizeTraceBase< MeshContainer, TIndex >( m_MeshContainer, iIt,
                                                   highlighted, visibility );
      }
  }

  template< class TIndex >
  void
  VisualizeTrack(
    typename TrackContainer::MultiIndexContainerType::template index< TIndex >::type::iterator iIt)
  {
    if ( iIt->Nodes )
      {
      bool highlighted = false;
      bool visibility = false;

      AddTraceIDIntoPolydata(iIt->Nodes, iIt->TraceID, "TRACK");

      VisualizeTraceBase< TrackContainer, TIndex >( m_TrackContainer, iIt,
                                                   highlighted, visibility );
      }
  }

  template< class TContainer, class TIndex >
  void
  VisualizeTraceBase(
    TContainer* iContainer,
    typename TContainer::MultiIndexContainerType::template index< TIndex >::type::iterator iIt,
    const double & iHighlighted,
    const double & iVisible,
    vtkPolyData* iContour = NULL )
    {
    const double *iRgba = iIt->rgba;

    vtkProperty *mesh_property = vtkProperty::New();
    mesh_property->SetColor(iRgba[0], iRgba[1], iRgba[2]);
    mesh_property->SetOpacity(iRgba[3]);

    vtkPolyData* temp = iIt->Nodes;

    if( iContour )
      {
      temp = iContour;
      }

    std::vector< vtkActor * > mesh_actor = this->AddContour( temp, mesh_property );

    mesh_property->Delete();

    iContainer->template UpdateVisualizationForGivenElement< TIndex >( iIt,
                                                              mesh_actor,
                                                              iHighlighted,
                                                              iVisible );
    }

  template<typename T>
  void CreateConnectionsTraceEditingWidget(int iTimeMin, int iTimeMax, T* iTraceWidget)
    {
    m_DockWidgetList.push_back(
    std::pair< QGoDockWidgetStatus *, QDockWidget * >(
      new QGoDockWidgetStatus(
        iTraceWidget->GetDockWidget(), Qt::LeftDockWidgetArea, false, true),
        iTraceWidget->GetDockWidget()) );

    QObject::connect(iTraceWidget,
                   SIGNAL(UpdateSeeds() ),
                   this,
                   SLOT(UpdateSeeds() ) );

    QObject::connect(iTraceWidget,
                   SIGNAL(ClearAllSeeds() ),
                   this->m_ImageView,
                   SLOT(ClearAllSeeds() ) );

    QObject::connect( iTraceWidget,
                    SIGNAL( SetSeedInteractorBehaviour(bool) ),
                    this,
                    SLOT( SeedInteractorBehavior(bool) ) );
    }


  std::vector< int > GetBoundingBox(vtkPolyData *contour);

  void CreateContour(vtkPolyData *contour_nodes, vtkPolyData *iView);

  /**
   * \brief Save mesh in Database
   * \param[in] iMesh mesh to be saved
   * \param[in] iTCoord
   */
  void SaveMesh(vtkPolyData *iMesh, int iTCoord, int iCollectionID = -1);

  void GetBackgroundColorFromImageViewer();

  void SetBackgroundColorToImageViewer();

  void CreateAllViewActions();

  void CreateTracesActions();

  void CreateToolsActions();

  void CreateBookmarkActions();

  //void CreateModeActions();

  void CreateVisuDockWidget();

  // segmentation dockwidgets
  //void CreateContourSegmentationDockWidget();
  void CreateContourEditingDockWidget(int iTimeMin, int iTimeMax);
  //void CreateMeshSegmentationDockWidget();
  void CreateMeshEditingDockWidget(int iTimeMin, int iTimeMax);

  void CreateDataBaseTablesConnection();

#if defined ( ENABLEFFMPEG ) || defined ( ENABLEAVI )
  void CreateVideoRecorderWidget();

#endif /* ENABLEVIDEORECORD */

  int *GetImageCoordinatesFromWorldCoordinates(double pos[3]);

//   std::vector< vtkQuadricLODActor* >
  std::vector< vtkActor * > AddContour(vtkPolyData *dataset,
                                       vtkProperty *property = NULL);

  // TODO remove megacapture
  void UpdateImage();

    // TODO remove megacaptureZz
  void BuildDopplerWidget();

  /**
  \brief give the adress for the contours, meshes and tracks container to the
  QGoPrintDatabase
  */
  void SetTheContainersForDB();

protected slots:
  void AddBookmark();

  void GetTheRelatedToDBActions();

  void GetTheOpenBookmarksActions();

  void OpenExistingBookmark();

  /**
  \brief slot connected to the toggleaction of the TW, the contour and mesh editing,
  update the trace and collection name of the trace settings toolbar and widget and
  check if the trace settings toolbar needs to be shown/hidden
  */
  void SetTraceSettingsToolBarVisible(bool IsVisible);

  void UpdateSeeds();

  void GoToLocation(int iX, int iY, int iZ, int iT);
  void GoToRealLocation(double iX, double iY, double iZ, int iT);

  /**
   * \brief Mouse interaction style allows contours segmentation, according to
   * the selected type of segmentation
   */
  void ManualInteractorBehavior(bool);

  /**
   * \brief Mouse interaction style allows meshes segmentation, according to
   * the selected type of segmentation
   */
  void SeedInteractorBehavior(bool);

  /**
   * \brief Mouse interaction style set as default
   */
  virtual void DefaultInteractorBehavior(bool);

  /**
   * \brief Mouse interaction style allows user to zoom in/out volume with all
   * buttons
   */
  virtual void ZoomInteractorBehavior(bool);

  /**
   * \brief Mouse interaction style allows user to Translate volume with all buttons
   */
  virtual void TranslateInteractorBehavior(bool);

  /**
   * \brief Mouse interaction style allows user to pick contours
   */
  // void ContourPickingInteractorBehavior(bool);

  /**
   * \brief Mouse interaction style allows user to pick contours
   */
  void ActorPickingInteractorBehavior(bool);

  void DistanceWidgetInteractorBehavior(bool);

  void AngleWidgetInteractorBehavior(bool);

  void Box3DPicking(bool);

  void PlaneWidgetInteractorBehavior(bool);

  void ImportContours();

  void ImportMeshes();

  void ImportTracks();

  void ImportVTKMesh();

  /**
  \brief access to the megacapture reader to get the entity of interest images through time.
  updates the navigation widget.
  */
  void StartDopplerView();

  /**
  \brief give the adress for the contours, meshes and tracks container to the
  QGoPrintDatabase, and make the connection for the status bar once the database
  variables have been set for the QGoPrintDatabase
  */
  void SetDatabaseContainersAndDelayedConnections();

  void AddTraceIDIntoPolydata( vtkPolyData* iPolydata, unsigned int iTraceID, const char* iTrace);

  /**
  \brief depending on the doppler/classic mode, update the TimePoints and channels
  of the mesh and contour widget
  */
  void UpdateTracesEditingWidget();

  void EnableVolumeRendering(bool iEnable);

  void MoveToNextTimePoint();
  void MoveToPreviousTimePoint();

private:
  void InitializeImageRelatedWidget();
  void SetUpShortcuts();
  void CreateDopplerTFEditor();

  void createTransferFunctionEditor(QString iName);

  std::vector< QString > GetChannelNames();

  bool m_VolumeRenderingEnabled;

  Q_DISABLE_COPY(QGoTabImageView3DwT);
};

#endif

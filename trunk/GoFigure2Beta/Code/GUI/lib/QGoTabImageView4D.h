#ifndef __QGoTabImageView4D_h
#define __QGoTabImageView4D_h

#include "QGoTabElementBase.h"

class QGoPrintDatabase;

class QGoImageView3D;
class QGoVisualizationDockWidget;
class QGoManualSegmentationDockWidget;

#ifdef   ENABLEVIDEORECORD
class QGoVideoRecorder;
#endif

class vtkContourWidget;
class vtkOrientedGlyphContourRepresentation;
class vtkActor;
class vtkProperty;
class vtkPolyData;

#include "vtkSmartPointer.h"

#include "itkMegaCaptureReader.h"
#include "GoFigureFileInfoMultiIndexContainerHelper.h"

#include <QResizeEvent>
#include <QSplitter>

/**
\class QGoTabImageView4D
\brief
\example GUI/lib/qgotabimageview4d.cxx
*/
class QGoTabImageView4D : public QGoTabElementBase
{
  Q_OBJECT
  public:
    QGoTabImageView4D( QWidget* parent = 0 );
    virtual ~QGoTabImageView4D();

    GoFigure::TabDimensionType GetTabDimensionType( ) const;

    void SetMegaCaptureFile(
      const GoFigureFileInfoHelperMultiIndexContainer& iContainer,
      const GoFigure::FileType& iFileType,
      const std::string& iHeader );

    virtual void Update();

    void setupUi( QWidget* parent );
    void retranslateUi( QWidget *parent );

    virtual std::vector< QAction* > ViewActions();

    virtual std::list< QDockWidget* > DockWidget();

    virtual std::list< QWidget* > AdditionalWidget()
      {return std::list< QWidget* >(); }

    virtual void WriteSettings() {}
    virtual void ReadSettings() {}

    virtual void ValidateContour( const int& iId );

  QGoPrintDatabase* m_DataBaseTables;

signals:
  void TimePointChanged( int TimePoint );
  void ZSliceChanged( int ZSlice );
  void YSliceChanged( int YSlice );
  void XSliceChanged( int XSlice );
  void FullScreenViewChanged( int FullScreen );

  public slots:
    void SetTimePoint( const int& );
    void SetZSlice( const int& );
    void SetYSlice( const int& );
    void SetXSlice( const int& );

    QString SnapshotViewXY( const GoFigure::FileType& iType,
      const QString& iBaseName = QString( "snapshot" ) );
    QString SnapshotView2( const GoFigure::FileType& iType,
      const QString& iBaseName = QString( "snapshot" ) );
    QString SnapshotView3( const GoFigure::FileType& iType,
      const QString& iBaseName = QString( "snapshot" ) );
    QString SnapshotViewXYZ( const GoFigure::FileType& iType,
      const QString& iBaseName = QString( "snapshot" ) );
    QString SnapshotViewXT( const GoFigure::FileType& iType,
      const QString& iBaseName = QString( "snapshot" ) );
    QString SnapshotViewYT( const GoFigure::FileType& iType,
      const QString& iBaseName = QString( "snapshot" ) );
    QString SnapshotViewXYT( const GoFigure::FileType& iType,
      const QString& iBaseName = QString( "snapshot" ) );

    void SetFullScreenView( const int& iS );

    void Octview();
    void QuadviewXYZ();
    void QuadviewXYT();
    void FullScreenViewXY();
    void FullScreenViewXZ();
    void FullScreenViewYZ();
    void FullScreenViewXYZ();
    void FullScreenViewXT();
    void FullScreenViewYT();
    void FullScreenViewXYT();

    void ChangeLookupTable();
    void ShowScalarBar( const bool& );
    void ChangeBackgroundColor();

    void ShowAllChannels( bool iChecked );
    void ShowOneChannel( int iChannel );

    void ActivateManualSegmentationEditor( const bool& iActivate );
    void ValidateContour();
    void ChangeContourRepresentationProperty();

  protected:
    QSplitter*            m_Splitter;
    QGoImageView3D*       m_XYZImageView;
    QGoImageView3D*       m_XYTImageView;
    vtkSmartPointer< vtkImageData > m_XYZImage;
    vtkSmartPointer< vtkImageData > m_XYTImage;
    std::vector< vtkSmartPointer< vtkImageData > > m_XYZInternalImages;
    std::vector< vtkSmartPointer< vtkImageData > > m_XYTInternalImages;

    QColor                m_BackgroundColor;
    int                   m_TimePoint;
    int                   m_ZSlice;
    unsigned int          m_ContourId;

    itk::MegaCaptureReader::Pointer m_Reader1;
    itk::MegaCaptureReader::Pointer m_Reader2;

    GoFigureFileInfoHelperMultiIndexContainer m_FileList;
    GoFigure::FileType                        m_FileType;
    bool                                      m_FirstUpdate;

    std::vector< QAction* > m_ViewActions;
    QGoVisualizationDockWidget*       m_VisuDockWidget;
    QGoManualSegmentationDockWidget*  m_ManualSegmentationDockWidget;

    #ifdef   ENABLEVIDEORECORD
    QGoVideoRecorder*                 m_VideoRecorderWidget;
#endif

    std::vector< vtkSmartPointer< vtkContourWidget > >                      m_ContourWidget;
    std::vector< vtkSmartPointer< vtkOrientedGlyphContourRepresentation > > m_ContourRepresentation;

    void CreateAllViewActions();
    void CreateVisuDockWidget();
    void CreateManualSegmentationdockWidget();

    void GetBackgroundColorFromImageViewer( );
    void SetBackgroundColorToImageViewer( );

    std::vector< vtkActor* > AddContour( const int& iId,
      vtkPolyData* dataset,
      vtkProperty* property = NULL );

    int* GetImageCoordinatesFromWorldCoordinates( double pos[3] );

    virtual void resizeEvent( QResizeEvent* event );
};
#endif
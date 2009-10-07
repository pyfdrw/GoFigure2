#ifndef __QGoTabImageView3DwT_h
#define __QGoTabImageView3DwT_h

#include "QGoTabElementBase.h"
#include <QHBoxLayout>

#include "QGoPlugin.h"
#include "QGoImageView3D.h"

#include "vtkLSMReader.h"
#include "itkMultiFileReader.h"

#include "GoFigureFileInfoHelper.h"
#include "SnapshotHelper.h"

class QGoTabImageView3DwT : public QGoTabElementBase
{
  Q_OBJECT
public:
    QGoTabImageView3DwT( QWidget* parent = 0 );
    virtual ~QGoTabImageView3DwT();

    GoFigure::TabDimensionType GetTabDimensionType( ) const;

    void SetLSMReader( vtkLSMReader* iReader, const int& iTimePoint );
    void SetMultiFiles( FileListType& iFileList,
      const int& iSerieType,
      const int& iTimePoint );

    virtual void Update();

    void setupUi( QWidget* parent );
    void retranslateUi( QWidget *parent );

    virtual std::vector< QAction* > ViewActions();

    virtual std::list< QDockWidget* > DockWidget();

    virtual std::list< QWidget* > AdditionalWidget()
      {return std::list< QWidget* >(); }

    virtual void WriteSettings() {}
    virtual void ReadSettings() {}

public slots:
    void SetTimePoint( const int& );

    QString SnapshotViewXY( const GoFigure::SnapshotImageType& iType,
      const QString& iBaseName = QString( "snapshot" ) );
    QString SnapshotView2( const GoFigure::SnapshotImageType& iType,
      const QString& iBaseName = QString( "snapshot" ) );
    QString SnapshotView3( const GoFigure::SnapshotImageType& iType,
      const QString& iBaseName = QString( "snapshot" ) );
    QString SnapshotViewXYZ( const GoFigure::SnapshotImageType& iType,
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

protected:
  QHBoxLayout*          m_LayOut;
  QGoImageView3D*       m_ImageView;
  vtkLSMReader*         m_LSMReader;
  vtkImageData*         m_Image;
  itk::MultiFileReader* m_MultiFileReader;
  FileListType          m_FileList;
  QColor                m_BackgroundColor;
  int                   m_TimePoint;

  std::vector< QAction* > m_ViewActions;
  QDockWidget* m_DockWidget;

  void GetBackgroundColorFromImageViewer( );
  void SetBackgroundColorToImageViewer( );
};

#endif
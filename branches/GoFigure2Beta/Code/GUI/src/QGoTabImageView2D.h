#ifndef __QGoTabImageView2D_h
#define __QGoTabImageView2D_h

class vtkImageData;

class QAction;
class QToolBar;
class QDockWidget;

#include "QGoTabImageViewElementBase.h"

#include "QGoImageView2D.h"

/**
\class QGoTabImageView2D
\brief Element of the QTabWidget to be used to visualized 2D images.
*/
class QGoTabImageView2D : public QGoTabImageViewElementBase
{
  Q_OBJECT
public:
  QGoTabImageView2D( QWidget* parent = 0 );
  virtual ~QGoTabImageView2D();

  GoFigure::TabDimensionType GetTabDimensionType( ) const;

  void SetImage( vtkImageData* iImage );
  void Update();

  void setupUi( QWidget* parent );
  void retranslateUi( QWidget *parent );

  std::vector< QAction* > ViewActions();
  std::vector< QToolBar* > ToolBar();
  std::list< QDockWidget* > DockWidget();
  std::list< QWidget* > AdditionalWidget();

  void WriteSettings();
  void ReadSettings();

public slots:
  void ChangeBackgroundColor();
  void ChangeLookupTable();
  void ShowScalarBar( const bool& );

protected:
  QHBoxLayout*      m_LayOut;
  QGoImageView2D*   m_ImageView;
  vtkImageData*     m_Image;

  std::vector< QAction* > m_ViewActions;
  QToolBar* m_ViewToolBar;
private:
  QGoTabImageView2D( const QGoTabImageView2D& );
  void operator = ( const QGoTabImageView2D& );
};
#endif

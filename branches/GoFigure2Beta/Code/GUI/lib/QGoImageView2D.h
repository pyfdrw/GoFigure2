#ifndef __QGoImageView2D_h
#define __QGoImageView2D_h

#include <QWidget>

class vtkImageData;
class QVTKWidget;
class vtkEventQtSlotConnect;
class vtkViewImage2DWithContourWidget;


/** 
\class QGoImageView2D 
\brief Widget to visualize a 2D image (represented as vtkImageData).
*/
class QGoImageView2D : public QWidget
{
  public:
    QGoImageView2D( QWidget* parent = 0 );
    ~QGoImageView2D();

    void SetImage( vtkImageData* iImage );
    void Update();

  protected:
    QHBoxLayout*                      m_LayOut;
    QVTKWidget*                       m_QVTKWidgetXY;
    vtkViewImage2DWithContourWidget*  m_View;
    vtkImageData*                     m_Image;

  private:
    QGoImageView2D( const QGoImageView2D& );
    void operator = ( const QGoImageView2D& );
};
#endif


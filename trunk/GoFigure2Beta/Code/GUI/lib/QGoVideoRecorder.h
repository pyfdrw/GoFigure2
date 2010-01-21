#ifndef __QGOVIDEORECORDER_H
#define __QGOVIDEORECORDER_H

#include <QtGui>
#include "ui_VideoRecorderDockWidget.h"

#include <QProgressDialog>

#include <iostream>
#include <string>

class vtkRenderWindow;
class vtkFFMPEGRenderWindowRecorder;

class QGoVideoRecorder : public QDockWidget, private Ui::DockWidgetVideoRecorder
{
    Q_OBJECT

    public:
        explicit QGoVideoRecorder( QWidget* parent = 0);
        ~QGoVideoRecorder();

        void SetXMinAndMax( int XMin, int XMax );
        void SetXSlice( int X );

        void SetYMinAndMax( int YMin, int YMax );
        void SetYSlice( int Y );

        void SetZMinAndMax( int ZMin, int ZMax );
        void SetZSlice( int Z );

        void SetTMinAndMax( int TMin, int TMax );
        void SetTSlice( int T );

    private:

        unsigned int m_XMin;
        unsigned int m_XFixed;
        unsigned int m_XMax;
        unsigned int m_YMin;
        unsigned int m_YFixed;
        unsigned int m_YMax;
        unsigned int m_ZMin;
        unsigned int m_ZFixed;
        unsigned int m_ZMax;
        unsigned int m_TMin;
        unsigned int m_TFixed;
        unsigned int m_TMax;

        unsigned int m_RecordX;
        unsigned int m_RecordY;
        unsigned int m_RecordZ;
        unsigned int m_RecordTX;
        unsigned int m_RecordTY;
        unsigned int m_RecordTZ;

        QProgressDialog* m_ProgressDialog;

    // in tab "record video"
        unsigned int m_WindowSelected;

        QString m_VideoName2;

        unsigned int m_FrameRate2;
        unsigned int m_VideoQuality2;

        vtkFFMPEGRenderWindowRecorder *m_VideoRecorder2;

        QTimer *m_InternalTimer;
        unsigned int m_FrameCounter;

        int m_RendererWindow;
        vtkRenderWindow* iRenderingWindowTEST;

        bool m_RenderWindowSelected;

    public slots:
      void SetRendererWindow( int iSlice );
      void SetRenderingWindow2( vtkRenderWindow* iRenderingWindow );

           signals:
      void XSliceChanged( int Slice );
      void YSliceChanged( int Slice );
      void ZSliceChanged( int Slice );
      void TSliceChanged( int Slice );

      void FullScreenViewXY();
      void FullScreenViewYZ();
      void FullScreenViewXZ();


    private slots:

    // in tab "create video"
        //Create video pushbutton
        void on_startVideo_clicked();

        //Video selection checkBoxes
        void on_xSliceCheckBox_stateChanged( int state );
        void on_ySliceCheckBox_stateChanged( int state );
        void on_zSliceCheckBox_stateChanged( int state );
        void on_xtSliceCheckBox_stateChanged( int state );
        void on_ytSliceCheckBox_stateChanged( int state );
        void on_ztSliceCheckBox_stateChanged( int state );

        //First slice for recording
        void on_xSpinMin_valueChanged(int value);
        void on_ySpinMin_valueChanged(int value);
        void on_zSpinMin_valueChanged(int value);
        void on_tSpinMin_valueChanged(int value);

        //Last slice for recording
        void on_xSpinMax_valueChanged(int value);
        void on_ySpinMax_valueChanged(int value);
        void on_zSpinMax_valueChanged(int value);
        void on_tSpinMax_valueChanged(int value);

        //Time range
        void on_xSpinFixed_valueChanged( int value );
        void on_ySpinFixed_valueChanged( int value );
        void on_zSpinFixed_valueChanged( int value );
        void on_tSpinFixed_valueChanged( int value );

        //Video parameters
        void on_createFile_clicked();
        void on_frameRate_valueChanged(int value);
        void on_videoQuality_valueChanged(int value);

        //Video parameters
        void on_createFile_2_clicked();
        void on_frameRate_2_valueChanged(int value);
        void on_videoQuality_2_valueChanged(int value);

        void on_startRecord_clicked();
        void on_endRecord_clicked();

        void timeout();

};

#endif

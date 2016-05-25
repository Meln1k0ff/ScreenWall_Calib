#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define _USER32_

//qt
#include <QMainWindow>
#include <QMessageBox>

#include <QSet>
#include <QString>
#include <QDebug>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QDir>
#include <QDesktopWidget>
#include <QByteArray>
#include <QBuffer>

#include <QList>
#include <QBuffer>
#include <QByteArray>
#include <QCamera>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QSize>
#include <QtGlobal>
#include <QScreen>
#include <QErrorMessage>
#include <QStringList>

#include <QtGlobal>
#include <QProcess>
#include <QIODevice>

//openCV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <math.h>

//WinAPI DirectShow
//#include <qwindowdefs_win.h>
#include <qt_windows.h>
#include <dshow.h>
#include <strmif.h>
#include <objbase.h>
#include <oleauto.h>
#include "Setupapi.h"
#include <devguid.h>
#include <initguid.h>
//#include <strsafe.h>
#include <Windows.h>
#include <WinUser.h>
#include <WinGDI.h>
//#include <Mfidl.h>

//#include <mfapi.h>
//#include <mfplay.h>


//проектные файлы
//#include "camera.h"
#include "gridwidget.h"
#include "saveimagedialog.h"
#include "aboutdialog.h"
#include "streamwidget.h"
#include "opencv2qt.h"
#include "graphicsscene.h"
#include "graphicsview.h"
#include "streamthread.h"
#include "capturethread.h"
#include "sharedimagebuffer.h"
#include "buffer.h"
#include "viewport_defines.h"

class AboutDialog;

class StreamWidget;
class GridWidget;
class SaveImageDialog;

class GraphicsScene;
class GraphicsView;

class CaptureThread;
class StreamThread;

class ViewPort;

namespace Ui {
    class MainWindow;
}

using namespace std;
using namespace cv;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QList<QString> getMonitorsList();

    static const int VGA_RATIO = 0;
    static const int HD_RATIO = 1;
    static const int SXGA_RATIO = 2;
    static const int SPEC_RATIO = 3;
    static const int QHD_RATIO = 4;
    static const int UNKNOWN_RATIO = -1;


    void setupCamera();


    void setCameraViewportSize(int width,int height);

    QList<QString> getCamerasNames();
    void getCameraResolutions();


    HRESULT EnumerateDevices();

    void searchCameras();
    void searchDisplays();

//    int getCamerasList();

    int getMonitorWidth();
    int getMonitorHeight();
    int getMonitorBitrate();

    void startProcessing(int cameraIndex);
    void stopProcessing();






    std::vector<cv::Point2f>  getFramePoints();

    void mouseDoubleClickEvent( QMouseEvent * e );

    void closeEvent(QCloseEvent *);

public slots:
    void showAllMonResolutions(int displayIndex);
    void showAllCamResolutions(int cameraIndex);

    void selectCamera(int cameraIndex);

    void updateConnectedCameras();
    void updateConnectedDisplays();

    void updateCameraSettings(int resIndex);

    void saveSnapshots(const QPixmap &cameraShot, const QPixmap &gridShot);
    void resizeDisplayResolution();
    void processCamera();



private slots:

    void on_startButton_clicked();

    void updateFrame(QImage  image);

    void on_actionAbout_triggered();

    void on_actionQuit_triggered();

protected slots :


private:
    Ui::MainWindow *ui;
//    Camera *camera_;

    cv::Mat capMonitor_ ;

    int id_;

    GridWidget *gridWidget;
    SaveImageDialog *saveDialog;
    AboutDialog *aboutDialog;
    GraphicsScene* graphicsScene;


    StreamThread *streamThread_;
    CaptureThread *captureThread_;
    SharedImageBuffer *sharedImageBuffer;


    /*Функции извлечения названий и разрешнений камер через DirectShow*/
    void _FreeMediaType(AM_MEDIA_TYPE& mt);
    HRESULT CamCaps(IBaseFilter *pBaseFilter);
    void process_filter(IBaseFilter *pBaseFilter);
    int enum_devices();
    QList<QString> enum_cameras();

    QTimer *waitTimer;


/*Флаги внутри приложения */
    bool startPressed; //= false;
    bool stopPressed; //= false;
   /* bool spacePressed;*/ //= false;
    bool gridActive; //= false;
    bool gridVisible;// = false;
    bool monitorsFound;


    QList<QString> monitorsList_;
    QList<QString> camerasList_;

    QHash<int,QList<QString> > cameraResolutionsList_;
    QList<QString> cameraResolutions_;
    QList<QScreen*> screensList;
    QList<int> camIDsList_;

    std::vector<cv::Point2f> framePoints_;

    int displayIndex_;


    int dispWidth;
    int dispHeight;
    int dispBpp;

    int defaultDisplayWidth_;
    int defaultDisplayHeight_;
    int defaultDisplayBpp_;

    int camWidth_;
    int camHeight_;
    int camFps_;

    double prevRatio_;

    double ratio_;

    double widthCoef_;
    double heightCoef_;


    bool initialStart_;

    int computeRatio(int &width, int &height);




};

#endif // MAINWINDOW_H

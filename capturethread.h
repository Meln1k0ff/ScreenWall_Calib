#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

#include <QThread>

#include <opencv2/highgui/highgui.hpp>

#include "sharedimagebuffer.h"

/**
 * @brief The CaptureThread class       Поток видеозахвата
 */

class CaptureThread : public QThread
{
    Q_OBJECT

public:
    CaptureThread(SharedImageBuffer *sharedImageBuffer,int deviceNumber,int camWidth, int camHeight);

    volatile bool doStop_;
    volatile bool widthChanged_;
    volatile bool heightChanged_;
    volatile bool idChanged_;


    bool connectToCamera();
    bool disconnectCamera();
    bool isCameraConnected();

    void stop();

    int camWidth() const;
    void setCamWidth(int camWidth);

    int camHeight() const;
    void setCamHeight(int camHeight);

    int deviceNumber() const;
    void setDeviceNumber(int deviceNumber);

    void resume();
    void suspend();
    int camFps() const;
    void setCamFps(int camFps);

private:
    cv::VideoCapture cap_;
    cv::Mat grabbedFrame_;

    SharedImageBuffer *sharedImageBuffer_;
    QMutex doStopMutex_;

    int camWidth_;
    int camHeight_;
    int camFps_;

    int deviceNumber_;

    bool dropFrameIfBufferFull_;
    bool isStreaming_;
    bool suspended_;

protected:
    void run();

signals:

public slots:


};

#endif // CAPTURETHREAD_H

#ifndef STREAMTHREAD_H
#define STREAMTHREAD_H

#include <QThread>
#include <QMutex>

#include <QTime>
#include <QQueue>
// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "opencv2qt.h"
#include "sharedimagebuffer.h"

class SharedImageBuffer;
class StreamThread : public QThread
{
     Q_OBJECT
public:
    StreamThread(SharedImageBuffer *sharedImageBuffer, int deviceNumber);
    void stop();
    void restart();

    int deviceNumber() const;
    void setDeviceNumber(int deviceNumber);

protected:
    void run();
private:
    int camWidth_;
    int camHeight_;
    int deviceNumber_;

    volatile bool doStop;
    bool isStreaming;

//    Camera camera_;

    QMutex doStopMutex;
    QMutex processingMutex;

    cv::Mat currentFrame_;
    QImage frame_;

    SharedImageBuffer *sharedImageBuffer_;


signals:
    void newFrame(const QImage &frame); //sends signal to update GUI Thread

};

#endif // STREAMTHREAD_H

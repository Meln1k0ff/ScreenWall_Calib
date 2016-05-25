#include "streamthread.h"

StreamThread::StreamThread(SharedImageBuffer *sharedImageBuffer,int deviceNumber) :  QThread(),
    deviceNumber_(deviceNumber),
    sharedImageBuffer_(sharedImageBuffer)
{
    isStreaming = true;
    doStop=false;

}

void StreamThread::stop()
{
    QMutexLocker locker(&doStopMutex);
    doStop=true;
    isStreaming = false;
}

/**
 * @brief StreamThread::restart             Перезапуск видеопотока
 */
void StreamThread::restart()
{
    doStop=false;
    isStreaming = true;
}

void StreamThread::run()
{
    isStreaming = true;
    while (isStreaming){
        doStopMutex.lock();
        if(doStop)
        {
            doStop=false;
            doStopMutex.unlock();
            break;
        }
          doStopMutex.unlock();

          processingMutex.lock();
          currentFrame_=cv::Mat(sharedImageBuffer_->getByDeviceNumber(deviceNumber_)->get().clone());
          frame_ = cvtCvMat2QImage(currentFrame_);
          processingMutex.unlock();
          emit newFrame(frame_);
    }
}

int StreamThread::deviceNumber() const
{
    return deviceNumber_;
}

void StreamThread::setDeviceNumber(int deviceNumber)
{
    deviceNumber_ = deviceNumber;
}




#include "capturethread.h"

CaptureThread::CaptureThread(SharedImageBuffer *sharedImageBuffer, int deviceNumber, int camWidth, int camHeight): QThread(),
    sharedImageBuffer_(sharedImageBuffer)
{
    this->deviceNumber_ = deviceNumber;
    camWidth_ = camWidth;
    camHeight_ = camHeight;
    dropFrameIfBufferFull_ = true;
    doStop_ = false;
    widthChanged_= false;
    heightChanged_ = false;
    idChanged_ = false;
    isStreaming_ = true;
    sharedImageBuffer_->setSyncEnabled(true);
}

/**
 * @brief CaptureThread::run                Запускает поток видеозахвата
 */

void CaptureThread::run()
{
    while(isStreaming_)
    {
        ////////////////////////////////
        // Stop thread if doStop=TRUE //
        ////////////////////////////////
        doStopMutex_.lock();
        if(doStop_)
        {
            isStreaming_ = false;
            doStop_=false;
            doStopMutex_.unlock();
            break;
        }

        doStopMutex_.unlock();
        /////////////////////////////////
        /////////////////////////////////


        // Synchronize with other streams (if enabled for this stream)
        sharedImageBuffer_->sync(deviceNumber_);

        // Capture frame (if available)
        if (!cap_.grab())
            continue;
        // Retrieve frame
        cap_.retrieve(grabbedFrame_);
        // Add frame to buffer
        sharedImageBuffer_->getByDeviceNumber(deviceNumber_)->add(grabbedFrame_, dropFrameIfBufferFull_);
    }
    qDebug() << "Stopping capture thread...";
}

/**
 * @brief CaptureThread::connectToCamera            Подключение к камере
 * @return
 */
bool CaptureThread::connectToCamera()
{
    // Open camera
    bool camOpenResult = cap_.open(deviceNumber_);
    qDebug() << "camOpenResult=" << camOpenResult;
    // Set resolution
    if(camWidth_ != -1) {
         qDebug() << "camWidth_" << camWidth_;
        cap_.set(CV_CAP_PROP_FRAME_WIDTH, camWidth_);
    }
    if(camHeight_ != -1){
        qDebug() << "camHeight_" << camHeight_;
        cap_.set(CV_CAP_PROP_FRAME_HEIGHT, camHeight_);
    }
    if (camFps_ != -1){
        cap_.set(CV_CAP_PROP_FRAME_COUNT,camFps_);
    }
    isStreaming_ = true;
    // Return result
    return camOpenResult;
}

/**
  * @brief CaptureThread::disconnectCamera          Отключение от камеры
  * @return
  */

 bool CaptureThread::disconnectCamera(){


     if(cap_.isOpened())
     {

         cap_.release();
         isStreaming_  = false;
         return true;
     }
     // Камера не подключена
     else
         return false;
 }

 bool CaptureThread::isCameraConnected(){
     return cap_.isOpened();
 }

 /**
  * @brief CaptureThread::stop                  Останавливает поток
  */
 void CaptureThread::stop()
 {
     QMutexLocker locker(&doStopMutex_);
     doStop_=true;
     isStreaming_ = false;
 }

 int CaptureThread::camWidth() const
 {
     return camWidth_;
 }

 void CaptureThread::setCamWidth(int camWidth)
 {
     camWidth_ = camWidth;
 }

 int CaptureThread::camHeight() const
 {
     return camHeight_;
 }

 void CaptureThread::setCamHeight(int camHeight)
 {
     camHeight_ = camHeight;
 }

 int CaptureThread::deviceNumber() const
 {
     return deviceNumber_;
 }

 void CaptureThread::setDeviceNumber(int deviceNumber)
 {
     deviceNumber_ = deviceNumber;
 }
 void CaptureThread::suspend()
 {
//     suspendManager_.wait(&suspendMutex_);
     suspended_ = true;
 }

 int CaptureThread::camFps() const
 {
     return camFps_;
 }

 void CaptureThread::setCamFps(int camFps)
 {
     camFps_ = camFps;
 }

 void CaptureThread::resume()
 {

     suspended_ = false;
 }

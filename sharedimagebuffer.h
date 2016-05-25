#ifndef SHAREDIMAGEBUFFER_H
#define SHAREDIMAGEBUFFER_H



// Qt
#include <QHash>
#include <QSet>
#include <QWaitCondition>
#include <QMutex>
// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "buffer.h"
/**
 * @brief The SharedImageBuffer class       Видеобуфер
 */
class SharedImageBuffer
{
public:
    SharedImageBuffer();
    void add(int deviceNumber, Buffer<cv::Mat> *imageBuffer, bool sync=false);
    Buffer<cv::Mat>* getByDeviceNumber(int deviceNumber);
    void removeByDeviceNumber(int deviceNumber);
    void sync(int deviceNumber);    
    void wakeAll();
    void setSyncEnabled(bool enable);
    bool isSyncEnabledForDeviceNumber(int deviceNumber);
    bool getSyncEnabled();
    bool containsImageBufferForDeviceNumber(int deviceNumber);

private:
    QHash<int, Buffer<cv::Mat>*> imageBufferMap;
    QSet<int> syncSet;
    QWaitCondition wc;
    QMutex mutex;
    int nArrived;
    bool doSync;

signals:

public slots:
};

#endif // SHAREDIMAGEBUFFER_H

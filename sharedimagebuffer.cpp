#include "sharedimagebuffer.h"

SharedImageBuffer::SharedImageBuffer()
{
    nArrived=0;
    doSync=true;
    imageBufferMap.clear();
}

/**
 * @brief SharedImageBuffer::add            Добавить новый буфер кадров к видеоустройству захвата
 * @param deviceNumber                      Номер видеокамеры в системе
 * @param imageBuffer                       Буфер кадров (объявляется в MainWindow перед запуском камеры)
 * @param sync                              Требуется ли синхронизация
 */

void SharedImageBuffer::add(int deviceNumber, Buffer<cv::Mat>* imageBuffer, bool sync)
{

    if(sync)
    {
        mutex.lock();
        syncSet.insert(deviceNumber);
        mutex.unlock();
    }
    // Add image buffer to map
    imageBufferMap[deviceNumber]=imageBuffer;
}

/**
 * @brief SharedImageBuffer::getByDeviceNumber      Получить буфер кадров по номеру видеоустройства захвата
 * @param deviceNumber
 * @return
 */

Buffer<cv::Mat>* SharedImageBuffer::getByDeviceNumber(int deviceNumber)
{
    return imageBufferMap[deviceNumber];
}


/**
 * @brief SharedImageBuffer::removeByDeviceNumber   Удалить буфер кадров по номеру видеоустройства захвата
 * @param deviceNumber
 */

void SharedImageBuffer::removeByDeviceNumber(int deviceNumber)
{
    qDebug() << "buffer from devNumber " << deviceNumber << " is removed";

    imageBufferMap.remove(deviceNumber);


    mutex.lock();
    if(syncSet.contains(deviceNumber))
    {
        syncSet.remove(deviceNumber);
        wc.wakeAll();
    }
    mutex.unlock();
    imageBufferMap.clear();
}

/**
 * @brief SharedImageBuffer::sync                       Выполнить синхронизацию потоков
 * @param deviceNumber
 */
void SharedImageBuffer::sync(int deviceNumber)
{

    mutex.lock();
    if(syncSet.contains(deviceNumber))
    {
        // Increment arrived count
        nArrived++;
        qDebug() << "nArrived="  << nArrived;
        // We are the last to arrive: wake all waiting threads
        if(doSync && (nArrived==syncSet.size()))
            wc.wakeAll();
        // Still waiting for other streams to arrive: wait
        else
            wc.wait(&mutex);
        // Decrement arrived count
        nArrived--;
    }
    mutex.unlock();
}


/**
 * @brief SharedImageBuffer::wakeAll                       Разбудить все потоки
 */
void SharedImageBuffer::wakeAll()
{
    QMutexLocker locker(&mutex);
    wc.wakeAll();
}

/**
 * @brief SharedImageBuffer::setSyncEnabled                 Включить синхронизацию потоков
 * @param enable
 */

void SharedImageBuffer::setSyncEnabled(bool enable)
{
    doSync=enable;
}
/**
 * @brief SharedImageBuffer::isSyncEnabledForDeviceNumber       Включена ли синхронизация для определенного устройства
 * @param deviceNumber                                          Номер устройства
 * @return
 */
bool SharedImageBuffer::isSyncEnabledForDeviceNumber(int deviceNumber)
{
    return syncSet.contains(deviceNumber);
}
/**
 * @brief SharedImageBuffer::getSyncEnabled                     Включена ли синхронизация потоков
 * @return
 */
bool SharedImageBuffer::getSyncEnabled()
{
    return doSync;
}
/**
 * @brief SharedImageBuffer::containsImageBufferForDeviceNumber     Включена ли поддержка буферизации для
 *                                                                  каждого кадра
 * @param deviceNumber                                              Номер устройства
 * @return
 */
bool SharedImageBuffer::containsImageBufferForDeviceNumber(int deviceNumber)
{
    return imageBufferMap.contains(deviceNumber);
}

#include "opencv2qt.h"
#include <opencv2/core/core_c.h>
#include <stdio.h>

/**
 * @brief cvtCvMat2QImage
 * @param image
 * @return
 */
QImage cvtCvMat2QImage(const cv::Mat & image)
{

    qDebug() << "image.channels=" << image.channels();
    QImage qtemp;

    if(!image.empty() && image.depth() == CV_8UC1)
    {
        const unsigned char * data = image.data;
        qtemp = QImage(image.cols, image.rows, QImage::Format_RGB32);
        for(int y = 0; y < image.rows; ++y, data += image.cols*image.elemSize())
        {
            for(int x = 0; x < image.cols; ++x)
            {
                QRgb * p = ((QRgb*)qtemp.scanLine (y)) + x;
                *p = qRgb(data[x * image.channels()+2], data[x * image.channels()+1], data[x * image.channels()]);
            }
        }
    }
    else if(!image.empty() && image.depth() != CV_8U)
    {
        qDebug() << "Wrong image format, must be 8_bits\n";
    }

    return qtemp;

}

/**
 * @brief cvtQPoint2CvPoint
 * @param point
 * @return
 */
cv::Point2f cvtQPoint2CvPoint(QPoint &point)
{
    int x =point.x();
    qDebug() << "x="<<x;
    int y = point.y();
    qDebug() << "y="<<y;
    cv::Point2f cvp(x,y);

    return cvp;
}

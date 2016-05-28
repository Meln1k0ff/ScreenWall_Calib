#include "qt2opencv.h"

/**
 * @brief cvtQImage2CvMat
 * @param image
 * @return
 */
cv::Mat cvtQImage2CvMat(const QImage & image)
{
    qDebug() << "image.height==" << image.height();
    qDebug() << "image.width==" << image.width();
    qDebug() << "image.depth()=" << image.depth();

    cv::Mat cvImage;
    if(!image.isNull() && image.depth() == 32 && image.format() == QImage::Format_RGB32)
    {
        // assume RGB (3 channels)
        int channels = 3;
        cvImage = cv::Mat(image.height(), image.width(), CV_8UC3);
        unsigned char * data = cvImage.data;
        const IplImage test = cvImage;
        printf("%d vs %d\n", cvImage.cols*int(cvImage.elemSize()), test.widthStep);
        for(int y = 0; y < image.height(); ++y, data+=cvImage.cols*cvImage.elemSize())
        {
            for(int x = 0; x < image.width(); ++x)
            {
                QRgb rgb = image.pixel(x, y);
                data[x * channels+2] = qRed(rgb); //r
                data[x * channels+1] = qGreen(rgb); //g
                data[x * channels] = qBlue(rgb); //b
            }
        }
    }
    else
    {

    }
    return cvImage;
}
/**
 * @brief cvtQImage2CvMatObj
 * @param image
 * @param format
 * @return
 */
cv::Mat cvtQImage2CvMatObj(QImage &image,int format)
{
    return cv::Mat(image.height(), image.width(), format, image.bits(), image.bytesPerLine()).clone();
}

#ifndef QT2OPENCV
#define QT2OPENCV

#include <QtGui/QImage>
#include <QDebug>

#include <opencv2/core/core.hpp>

cv::Mat cvtQImage2CvMat(const QImage & image);
cv::Mat cvtQImage2CvMatObj(QImage & image,int format);


#endif // QT2OPENCV


#ifndef OPENCV2QT_H
#define OPENCV2QT_H
#include <QImage>
#include <QPoint>
#include <QDebug>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;

QImage cvtCvMat2QImage(const cv::Mat &image);
cv::Point2f cvtQPoint2CvPoint(QPoint &point);
#endif // OPENCV2QT_H

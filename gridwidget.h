#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H


#include <QWidget>
#include <QKeyEvent>

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QRgb>
#include <QPaintEvent>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>



#include "streamwidget.h"
#include "saveimagedialog.h"

#include "qt2opencv.h"
#include "opencv2qt.h"




class StreamWidget;
class SaveImageDialog;

namespace Ui {
class GridWidget;
}

class GridWidget : public QWidget
{
    Q_OBJECT

public:

    GridWidget(StreamWidget *stream, QWidget *parent = 0);
    ~GridWidget();

    void initGrid(int dispWidth, int dispHeight, int cameraWidth, int cameraHeight, std::vector<Point2f> &framePoints);
    void displayGrid(cv::Mat mask);

    void keyPressEvent(QKeyEvent *keyevent);

    const QPixmap & pixmap() const {return pixmap_;}

    bool getGridShown() const;

    QPixmap getPixmapTransformed() const;

protected :


signals:
    void spacePressed(const QPixmap &cameraShot, const QPixmap &gridShot);
    void gridDestroyed();

private:
    Ui::GridWidget *ui;    

    QPixmap pixmap_;
    QPixmap pixmapTransformed_;

    int gridHeight;

    StreamWidget *streamWidget_;
    SaveImageDialog *saveImageDialog_;

    int dispWidth_;
    int dispHeight_;
    int cameraWidth_;
    int cameraHeight_;

    bool gridShown;

    std::vector<Point2f> framePoints_;
    std::vector<cv::Point2f> screenPts_;




};

#endif // GRIDWIDGET_H

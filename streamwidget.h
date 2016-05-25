#ifndef STREAMWIDGET_H
#define STREAMWIDGET_H

#include <QWidget>
#include <QString>

#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QVBoxLayout>

#include <QContextMenuEvent>
#include <QPaintEvent>
#include <QMouseEvent>

#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <math.h>

#include "draggableitem.h"
#include "graphicsscene.h"
#include "graphicsview.h"
#include "opencv2qt.h"

using namespace std;
using namespace cv;


class StreamWidget : public QWidget
{
    Q_OBJECT
public:
    StreamWidget(QWidget * parent);   
    ~StreamWidget();
    void setData(QImage image);
    void computeScaleOffsets(float & scale, float & offsetX, float & offsetY);   
    void setPixmapSize(int width,int height);
    void clearViewport();    
    QPixmap pixmap() const;
signals:

protected:
    void mouseMoveEvent(QMouseEvent *e);




public slots:
private:
    QLabel *streamLabel_;
    QPixmap pixmap_;
    QImage img_;




};

#endif // STREAMWIDGET_H

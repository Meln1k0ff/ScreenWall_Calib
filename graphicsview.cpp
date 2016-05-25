#include "graphicsview.h"

GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
{

}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    qDebug()  << "mouse pressed";
    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
     qDebug()  << "view released x=" << event->pos();
    QGraphicsView::mouseReleaseEvent(event);

}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    qDebug()  << "mouse moved";
    QGraphicsView::mouseMoveEvent(event);
    qDebug()  << "view x=" << event->pos();
}



void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape){

//        scene->restartFrame();
        QGraphicsView::keyPressEvent(event);
    }
}


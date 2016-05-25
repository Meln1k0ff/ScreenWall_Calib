#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QDebug>

#include "graphicsscene.h"

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

signals:

public slots:
};

#endif // GRAPHICSVIEW_H

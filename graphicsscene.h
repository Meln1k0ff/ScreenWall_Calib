#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <qgraphicsscene.h>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPixmap>
#include <QKeyEvent>

#include "draggableitem.h"
class GraphicsScene : public QGraphicsScene
{
public:
    GraphicsScene(QObject *parent = 0);
    virtual ~GraphicsScene();

    QGraphicsLineItem *topLine_;
    QGraphicsLineItem *leftLine_;
    QGraphicsLineItem *bottomLine_;
    QGraphicsLineItem *rightLine_;

    void resetFrame();
    void restartFrame(double ratiow, double ratioh);
    void clearFrame();
    void recalculateFrame(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal x4, int y4, qreal ratiow,qreal ratioh);


    QPointF *getDp0() const;

    QPointF *getDp1() const;

    QPointF *getDp2() const;

    QPointF *getDp3() const;

    bool checkBounds(qreal curX, qreal curY, int maxWidth, int maxHeight, int corner);    
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

     void keyPressEvent(QKeyEvent *event);
signals:

private:
     QPixmap pixmap_;
     DraggableItem * dragItems_[4];

     QPointF *dp0_;
     QPointF *dp1_;
     QPointF *dp2_;
     QPointF *dp3_;



public slots:
};

#endif // GRAPHICSSCENE_H

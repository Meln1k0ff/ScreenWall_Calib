#ifndef DRAGGABLEITEM_H
#define DRAGGABLEITEM_H

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QBrush>
#include <cmath>
#include <QPainter>
#include <QStyleOptionGraphicsItem>


class DraggableItem : public QGraphicsEllipseItem
{
public:
    DraggableItem(qreal x, qreal y, qreal rw,qreal rh, const QColor &color, int num, QGraphicsEllipseItem *parent);
    ~DraggableItem();

    qreal x_;
    qreal y_;
    bool isSelected() const;
    void setIsSelected(bool isSelected);   

protected:
     void mousePressEvent(QGraphicsSceneMouseEvent *event);
     void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
     void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


private:

     qreal rw_;
     qreal rh_;
     QPointF center_;
     bool isResizing_;
     bool isSelected_;
     int number_;
     QGraphicsTextItem numberItem_;


signals:

public slots:
};

#endif // DRAGGABLEITEM_H

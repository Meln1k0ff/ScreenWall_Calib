#include "draggableitem.h"


/**
 * @brief DraggableItem::DraggableItem          Уголок рамки для выделения области экрана
 * @param x
 * @param y
 * @param r
 * @param color
 * @param num
 * @param parent
 */
DraggableItem::DraggableItem(qreal x, qreal y, qreal rw,qreal rh, const QColor &color, int num, QGraphicsEllipseItem *parent)
    : QGraphicsEllipseItem(x,y,rw,rh,parent),
      x_(x),
      y_(y),
      rw_(rw),
      rh_(rh),
      number_(num)
{
    this->setBrush(QBrush(color));
    this->setFlag(QGraphicsItem::ItemIsMovable,true);
    isSelected_ = false;
    this->setZValue(2.0);

    this->mapToScene(1,1);

}

DraggableItem::~DraggableItem()
{

}

/**
 * @brief DraggableItem::mousePressEvent
 * @param event
 */
void DraggableItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

     if(event->button() == Qt::LeftButton) {
         qDebug() << "Custom item left clicked.";
         this->setSelected(true);
         this->isSelected_ = true;
         QGraphicsItem::mousePressEvent(event);
         event->accept();
     }

     else if(event->button() == Qt::RightButton) {
                 qDebug() << "Custom item right clicked.";
                 event->ignore();
             }

}


/**
 * @brief DraggableItem::mouseMoveEvent
 * @param event
 */
void DraggableItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->modifiers() != Qt::AltModifier) {

                qDebug() << "Custom item moved.";

                //int x = event->scenePos().x();
                //int y = event->scenePos().y();

//                if (x() < 0)
//                   {
//                       setPos(1, y());
//                   }
//                   else if (x() + boundingRect().right() > scene()->width())
//                   {
//                       setPos(scene()->width() - boundingRect().width(), y());
//                   }

//                   if (y() < 0)
//                   {
//                       setPos(x(), 1);
//                   }
//                   else if ( y()+ boundingRect().bottom() > scene()->height())
//                   {
//                       setPos(x(), scene()->height() - boundingRect().height());
//                   }

                qDebug()<<"moved"<<event->scenePos();
                QGraphicsItem::mouseMoveEvent(event);

            }
}

/**
 * @brief DraggableItem::mouseReleaseEvent
 * @param event
 */
void DraggableItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
                qDebug() << "released" ;


//                if (x <= 0) x = 1;
//                if (y<= 0) y=1;


                 QGraphicsItem::mouseReleaseEvent(event);
                 int x = event->scenePos().x();
                 int y = event->scenePos().y();
                   qDebug() << "event->scenePos().x()="<<event->scenePos();
                   qDebug() << "event->Pos().x()="<<event->pos();
                if (x < 0 ) x = event->pos().x();
                if (y < 0) y = event->pos().y();

                 x_ = x;
                 y_ = y;
                qDebug() << "xx="<<x_;
                qDebug() << "yy="<<y_;

                this->isSelected_ = false;
}

/**
 * @brief DraggableItem::paint
 * @param painter
 * @param option
 * @param widget
 */
void DraggableItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

         painter->save();
         painter->setPen(QColor(0,255,0));
         painter->setBrush(QColor(0,255,0));
           QGraphicsEllipseItem::paint(painter, option, widget);
}

/**
 * @brief DraggableItem::isSelected
 * @return
 */
bool DraggableItem::isSelected() const
{
    return isSelected_;
}
/**
 * @brief DraggableItem::setIsSelected
 * @param isSelected
 */
void DraggableItem::setIsSelected(bool isSelected)
{
    isSelected_ = isSelected;
}







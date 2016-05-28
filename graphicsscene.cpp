#include "graphicsscene.h"

/**
 * @brief GraphicsScene::GraphicsScene
 * @param parent
 */
GraphicsScene::GraphicsScene(QObject *parent) : QGraphicsScene(parent)
{
    qDebug() << "scene inited";    
}
/**
 * @brief GraphicsScene::~GraphicsScene
 */
GraphicsScene::~GraphicsScene(){
    delete dp0_,dp1_,dp2_,dp3_;
}
/**
 * @brief GraphicsScene::mousePressEvent
 * @param event
 */
void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
}
/**
 * @brief GraphicsScene::mouseMoveEvent
 * @param event
 */
void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{   

    if ((dragItems_[0]->isSelected()) &&  (!checkBounds(dragItems_[0]->x_,dragItems_[0]->y_,this->width(),this->height(),0))){
        qDebug() << "p1 out of bounds";
             resetFrame();
      //
    }
    else if ((dragItems_[1]->isSelected()) && (!checkBounds(dragItems_[1]->x_,dragItems_[1]->y_,this->width(),this->height(),1))){
        qDebug() << "p2 out of bounds";
        resetFrame();

    }
    else if ((dragItems_[2]->isSelected()) && (!checkBounds(dragItems_[2]->x_,dragItems_[2]->y_,this->width(),this->height(),2))){
        qDebug() << "p3 out of bounds";
        resetFrame();

    }
    else if ( (dragItems_[3]->isSelected()) && (!checkBounds(dragItems_[3]->x_,dragItems_[3]->y_,this->width(),this->height(),3))){
        qDebug() << "p4 out of bounds";
        resetFrame();

    }

    QGraphicsScene::mouseMoveEvent(event);
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
     QGraphicsScene::mouseReleaseEvent(event);
      qDebug()  << "scene released x=" << event->pos();
      resetFrame();
 }

/**
 * @brief GraphicsScene::keyPressEvent
 * @param event
 */
void GraphicsScene::keyPressEvent(QKeyEvent *event)
{

}

/**
 * @brief GraphicsScene::restartFrame       Метод рисует точки рамки на начальных позициях
 */
void GraphicsScene::restartFrame(double ratiow,double ratioh)
{
    qDebug() << "ratiow=" << (double)ratiow;
    qDebug() << "ratioh=" << (double)ratioh;
    dragItems_[0] = new DraggableItem(1,1,ratiow*5,ratioh*5,QColor(255,0,0),1,0);
    dragItems_[0]->setFlag(QGraphicsItem::ItemIsMovable);
    dragItems_[1] = new DraggableItem(50,1,ratiow*5,ratioh*5,QColor(255,0,0),2,0);
    dragItems_[1]->setFlag(QGraphicsItem::ItemIsMovable);
    dragItems_[2] = new DraggableItem(50,50,ratiow*5,ratioh*5,QColor(255,0,0),3,0);
    dragItems_[2]->setFlag(QGraphicsItem::ItemIsMovable);
    dragItems_[3] = new DraggableItem(1,50,ratiow*5,ratioh*5,QColor(255,0,0),4,0);
    dragItems_[3]->setFlag(QGraphicsItem::ItemIsMovable);

    dp0_ = new QPointF(dragItems_[0]->x_,dragItems_[0]->y_);
    dp1_ = new QPointF(dragItems_[1]->x_,dragItems_[1]->y_);
    dp2_ = new QPointF(dragItems_[2]->x_,dragItems_[2]->y_);
    dp3_ = new QPointF(dragItems_[3]->x_,dragItems_[3]->y_);

    topLine_ = new QGraphicsLineItem(dp0_->x(),dp0_->y(),dp1_->x(),dp1_->y());
    leftLine_ = new QGraphicsLineItem(dp0_->x(),dp0_->y(),dp3_->x(),dp3_->y());
    bottomLine_ = new QGraphicsLineItem(dp2_->x(),dp2_->y(),dp3_->x(),dp3_->y());
    rightLine_ = new QGraphicsLineItem(dp1_->x(),dp1_->y(),dp2_->x(),dp2_->y());
}

void GraphicsScene::clearFrame()
{
    this->removeItem(dragItems_[0]);
    this->removeItem(dragItems_[1]);
    this->removeItem(dragItems_[2]);
    this->removeItem(dragItems_[3]);


    this->removeItem(topLine_);
    this->removeItem(leftLine_);
    this->removeItem(bottomLine_);
    this->removeItem(rightLine_);
}

void GraphicsScene::recalculateFrame(qreal x1,qreal y1,qreal x2,qreal y2,qreal x3,qreal y3,qreal x4,int y4,qreal ratiow,qreal ratioh)
{
    dragItems_[0] = new DraggableItem(x1,y1,ratiow,ratioh,QColor(255,0,0),1,0);
    dragItems_[0]->setFlag(QGraphicsItem::ItemIsMovable);
    dragItems_[1] = new DraggableItem(x2,y2,ratiow,ratioh,QColor(255,0,0),2,0);
    dragItems_[1]->setFlag(QGraphicsItem::ItemIsMovable);
    dragItems_[2] = new DraggableItem(x3,y3,ratiow,ratioh,QColor(255,0,0),3,0);
    dragItems_[2]->setFlag(QGraphicsItem::ItemIsMovable);
    dragItems_[3] = new DraggableItem(x4,y4,ratiow,ratioh,QColor(255,0,0),4,0);
    dragItems_[3]->setFlag(QGraphicsItem::ItemIsMovable);

    dp0_ = new QPointF(dragItems_[0]->x_,dragItems_[0]->y_);
    dp1_ = new QPointF(dragItems_[1]->x_,dragItems_[1]->y_);
    dp2_ = new QPointF(dragItems_[2]->x_,dragItems_[2]->y_);
    dp3_ = new QPointF(dragItems_[3]->x_,dragItems_[3]->y_);

    topLine_ = new QGraphicsLineItem(dp0_->x(),dp0_->y(),dp1_->x(),dp1_->y());
    leftLine_ = new QGraphicsLineItem(dp0_->x(),dp0_->y(),dp3_->x(),dp3_->y());
    bottomLine_ = new QGraphicsLineItem(dp2_->x(),dp2_->y(),dp3_->x(),dp3_->y());
    rightLine_ = new QGraphicsLineItem(dp1_->x(),dp1_->y(),dp2_->x(),dp2_->y());
}
/**
 * @brief GraphicsScene::getDp3
 * @return
 */
QPointF * GraphicsScene::getDp3() const
{
    return dp3_;
}
/**
 * @brief GraphicsScene::getDp2
 * @return
 */
QPointF * GraphicsScene::getDp2() const
{
    return dp2_;
}
/**
 * @brief GraphicsScene::getDp1
 * @return
 */
QPointF * GraphicsScene::getDp1() const
{
    return dp1_;
}
/**
 * @brief GraphicsScene::getDp0
 * @return
 */
QPointF * GraphicsScene::getDp0() const
{
    return dp0_;
}

/**/
/**
 * @brief GraphicsScene::checkBounds        Метод проверяет, не находится ли точка за пределами окна видеопотока
 * @param curX                              Текущее положение точки по X
 * @param curY                              Текущее положение точки по Y
 * @param maxWidth                          Ширина окна видео-стрима
 * @param maxHeight                         Высота окна видео-стрима
 * @param corner                            Номер точки
 * @return
 */
bool GraphicsScene::checkBounds(qreal curX, qreal curY, int maxWidth, int maxHeight,int corner)
{

    if (curX < 0) { qDebug() << "x out of bounds" ;curX = 0; dragItems_[corner]->x_ = curX; this->addItem(dragItems_[corner]); dragItems_[corner]->update(curX,curY,8,8);  return false;}
    else if (curY < 0) { qDebug() << "y out of bounds" ;curY = 0; dragItems_[corner]->y_ = curY;  this->addItem(dragItems_[corner]); dragItems_[corner]->update(curX,curY,8,8); return false;}
    if (curX > maxWidth) { qDebug() << "x out of bounds" ;curX = maxWidth; dragItems_[corner]->x_ = curX;   this->addItem(dragItems_[corner]); dragItems_[corner]->update(curX,curY,8,8); return false;}
    else if (curY > maxHeight) { qDebug() << "y out of bounds" ;curY = maxHeight; dragItems_[corner]->y_ = curY;  this->addItem(dragItems_[corner]); dragItems_[corner]->update(curX,curY,8,8); return false;}
    return true;
}

/**
 * @brief GraphicsScene::resetFrame         Перерисовка рамки на окне видеопотока.
 */
void GraphicsScene::resetFrame()
{
        dp0_->setX(dragItems_[0]->x_); dp0_->setY(dragItems_[0]->y_);
        dp1_->setX(dragItems_[1]->x_); dp1_->setY(dragItems_[1]->y_);
        dp2_->setX(dragItems_[2]->x_); dp2_->setY(dragItems_[2]->y_);
        dp3_->setX(dragItems_[3]->x_); dp3_->setY(dragItems_[3]->y_);

        topLine_->setLine(dp0_->x(),dp0_->y(),dp1_->x(),dp1_->y());// = topLine;

        leftLine_->setLine(dp0_->x(),dp0_->y(),dp3_->x(),dp3_->y());

        bottomLine_->setLine(dp2_->x(),dp2_->y(),dp3_->x(),dp3_->y());

        rightLine_->setLine(dp1_->x(),dp1_->y(),dp2_->x(),dp2_->y());

        if (!checkBounds(dragItems_[0]->x_,dragItems_[0]->y_,this->width(),this->height(),0)){
            qDebug() << "p1 out of bounds";
          //
        }
        else if (!checkBounds(dragItems_[1]->x_,dragItems_[1]->y_,this->width(),this->height(),1)){
            qDebug() << "p2 out of bounds";

        }
        else if (!checkBounds(dragItems_[2]->x_,dragItems_[2]->y_,this->width(),this->height(),2)){
            qDebug() << "p3 out of bounds";

        }
        else if (!checkBounds(dragItems_[3]->x_,dragItems_[3]->y_,this->width(),this->height(),3)){
            qDebug() << "p4 out of bounds";

        }

        this->addItem(dragItems_[0]);
        this->addItem(dragItems_[1]);
        this->addItem(dragItems_[2]);
        this->addItem(dragItems_[3]);




        this->addItem(topLine_);
        this->addItem(leftLine_);
        this->addItem(bottomLine_);
        this->addItem(rightLine_);
}



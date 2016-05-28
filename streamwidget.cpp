#include "streamwidget.h"

/**
 * @brief StreamWidget::StreamWidget
 * @param parent
 */
StreamWidget::StreamWidget(QWidget * parent) : QWidget(parent)
{

    streamLabel_ = new QLabel(parent);
    streamLabel_->setScaledContents(true);

    this->setLayout(new QVBoxLayout(this));
    this->layout()->addWidget(streamLabel_);
    this->layout()->setContentsMargins(0,0,0,0);
}


/**
 * @brief StreamWidget::~StreamWidget
 */
StreamWidget::~StreamWidget()
{
    delete streamLabel_;
}

void StreamWidget::setData(QImage image)
{
//    emit clearViewport();

    img_ = image;
    pixmap_ = QPixmap::fromImage(img_);
    streamLabel_->setPixmap(pixmap_);
}

void StreamWidget::computeScaleOffsets(float &scale, float &offsetX, float &offsetY)
{
    scale = 1.0f;
        offsetX = 0.0f;
        offsetY = 0.0f;

        if(!pixmap_.isNull())
        {
            float w = pixmap_.width(); //real
            float h = pixmap_.height(); //real
            float widthRatio = float(this->rect().width()) / w;
            float heightRatio = float(this->rect().height()) / h;

            //printf("w=%f, h=%f, wR=%f, hR=%f, sW=%d, sH=%d\n", w, h, widthRatio, heightRatio, this->rect().width(), this->rect().height());
            if(widthRatio < heightRatio)
            {
                scale = widthRatio;
            }
            else
            {
                scale = heightRatio;
            }

            //printf("ratio=%f\n",ratio);

            w *= scale;
            h *= scale;

            if(w < this->rect().width())
            {
                offsetX = (this->rect().width() - w)/2.0f;
            }
            if(h < this->rect().height())
            {
                offsetY = (this->rect().height() - h)/2.0f;
            }
            //printf("offsetX=%f, offsetY=%f\n",offsetX, offsetY);
        }
}



void StreamWidget::clearViewport()
{
      streamLabel_->clear();
}

void StreamWidget::mouseMoveEvent(QMouseEvent *e)
{
    QWidget::mouseMoveEvent(e);
    qDebug() << "mouse move" << e->pos();
    float scale, offsetX, offsetY;
    computeScaleOffsets(scale, offsetX, offsetY);
    qDebug() << "mouse move offset x" << (e->pos().x()-offsetX)/scale;
    qDebug() << "mouse move offset y" << (e->pos().y()-offsetY)/scale;

}

QPixmap StreamWidget::pixmap() const
{
    return pixmap_;
}







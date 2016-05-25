#include "gridwidget.h"
#include "ui_gridwidget.h"


/**
 * @brief GridWidget::GridWidget
 * @param camera
 * @param stream
 * @param parent
 */
GridWidget::GridWidget(StreamWidget *stream, QWidget *parent):
    streamWidget_(stream),
    QWidget(parent),
    ui(new Ui::GridWidget)
{
    ui->setupUi(this);
    this->setCursor(Qt::BlankCursor);

    screenPts_.clear();
}

GridWidget::~GridWidget()
{
    delete ui;
}

/*
 *
*/
/**
 * @brief GridWidget::initGrid              Рисует шахматку на экране с установленными параметрами
 * @param dispWidth                         Реальная ширина монитора
 * @param dispHeight                        Реальная высота монитора
 * @param cameraWidth
 * @param cameraHeight
 */
void GridWidget::initGrid(int dispWidth,int dispHeight,int cameraWidth,int cameraHeight,std::vector<cv::Point2f> &framePoints){

    dispWidth_ = dispWidth;
    dispHeight_ = dispHeight;
    cameraWidth_ = cameraWidth; //camera width
    cameraHeight_ = cameraHeight; //camera height

    ui->gridLabel->setFixedSize(dispWidth_,dispHeight_);
    ui->gridLabel->move(0,0);
    this->setFixedSize(dispWidth_,dispHeight_);

    screenPts_.push_back(cv::Point2f(0, 0));
    screenPts_.push_back(cv::Point2f((float)dispWidth_-1, 0));
    screenPts_.push_back(cv::Point2f((float)dispWidth_-1,(float) dispHeight_-1));
    screenPts_.push_back(cv::Point2f(0, (float)dispHeight_-1));



    cv::Mat gridBW;
    cv::Mat gridLaplace; // Преобразование Лапласа для более контрастных границ клеток
    cv::Mat bilateralMat;
    cv::Mat gaussianMat;

        cv::Mat grid(cameraHeight,cameraWidth,CV_8UC1);
        cv::Mat gridScreen(cameraHeight,cameraWidth,CV_8UC1);

             for (int y=0; y<cameraHeight; y++) {

                   for (int x=0; x<cameraWidth; x++) {
                       if (( x + y ) % 2){

                            cv::rectangle(grid,cv::Rect(x,y,1,1),cv::Scalar(0,0,0),1);

                       }
                       else {

                             cv::rectangle(grid,cv::Rect(x,y,1,1),cv::Scalar(255,255,255),1);
                       }

                   }
             }

//        /* Немного увеличить контрастность */
        Mat kernel = (Mat_<float>(3,3) <<
                 -1, -1, -1,
                 -1, 8,-1,
                 -1,  -1,-1 );
//         cvtColor(grid,grid,CV_BGR2GRAY);
         cv::GaussianBlur(grid,gaussianMat,cv::Size(21,21),99);

//         cv::addWeighted(grid, 9.5, gaussianMat, -0.5, 0, gaussianMat);
          grid = gaussianMat - grid;

        cv::Mat transmtx = cv::getPerspectiveTransform(framePoints, screenPts_);

        cv::warpPerspective(grid,gridScreen, transmtx,Size(dispWidth_,dispHeight_));
        cv::equalizeHist(gridScreen,gridScreen);
//        cv::normalize(gridScreen,gridScreen,1,0,NORM_L2,-1);

       QImage img = cvtCvMat2QImage(gridScreen);

    pixmap_ = QPixmap::fromImage(img);


    ui->gridLabel->setPixmap(pixmap_);
    ui->gridLabel->show();

    gridShown = true;

}

/**
 * @brief GridWidget::keyPressEvent
 * @param keyevent
 */
void GridWidget::keyPressEvent(QKeyEvent *keyevent){
    if ((keyevent->key() == Qt::Key_Space) && (!keyevent->isAutoRepeat())){

        emit spacePressed(streamWidget_->pixmap(),pixmap());

    }

    if ((keyevent->key() == Qt::Key_Escape) && (!keyevent->isAutoRepeat())){

        screenPts_.clear();
        framePoints_.clear();
        this->setVisible(false);
        emit gridDestroyed();

    }

}

/**
 * @brief GridWidget::getPixmapTransformed
 * @return
 */
QPixmap GridWidget::getPixmapTransformed() const
{
    return pixmapTransformed_;
}
/**
 * @brief GridWidget::getGridShown
 * @return
 */
bool GridWidget::getGridShown() const
{
    return gridShown;
}








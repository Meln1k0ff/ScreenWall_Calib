#ifndef SAVEIMAGEDIALOG_H
#define SAVEIMAGEDIALOG_H

#include <QDialog>
#include <QFile>
#include <QKeyEvent>
#include <QFileDialog>
#include <QDebug>
#include <QBuffer>

#include <opencv2/core/core.hpp>
#include "streamwidget.h"

namespace Ui {
class SaveImageDialog;
}

class StreamWidget;
class SaveImageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveImageDialog(const QPixmap &photo,const QPixmap &grid, QWidget *parent = 0);
    ~SaveImageDialog();


    void saveMonitorImage(cv::Mat screen);


    bool spacePressed;

private slots:
    void on_saveCamSnapshotBtn_clicked();

    void on_saveGridSnapshotBtn_clicked();

private:
    Ui::SaveImageDialog *ui;
    void saveImage();

    void showPixmaps(QPixmap &photo, QPixmap &grid);

    QImage imgScreen_;
    QImage imgGrid_;

    QPixmap displayPixmap_;
    QPixmap gridPixmap_;





};

#endif // SAVEIMAGEDIALOG_H

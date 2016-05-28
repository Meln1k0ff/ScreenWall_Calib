#include "saveimagedialog.h"
#include "ui_saveimagedialog.h"

/**
 * @brief SaveImageDialog::SaveImageDialog      Диалоговое окно снимка экрана
 * @param photo
 * @param grid
 * @param parent
 */
SaveImageDialog::SaveImageDialog(const QPixmap &photo, const QPixmap &grid, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveImageDialog)
{
    this->setFixedSize(551,300);

    displayPixmap_ = photo;
    gridPixmap_ = grid;


    ui->setupUi(this);

    showPixmaps(displayPixmap_,gridPixmap_);

}
/**
 * @brief SaveImageDialog::~SaveImageDialog
 */
SaveImageDialog::~SaveImageDialog()
{
    delete ui;
}

/**
 * @brief SaveImageDialog::showPixmaps          Демонстрирует снимки экрана и шахматки
 * @param photo
 * @param grid
 */
void SaveImageDialog::showPixmaps(QPixmap &photo, QPixmap &grid)
{
        ui->camSnapshot->setScaledContents(true);
        ui->gridSnapshot->setScaledContents(true);
        ui->camSnapshot->setPixmap(photo);

        ui->gridSnapshot->setPixmap(grid);
}
/**
 * @brief SaveImageDialog::on_saveCamSnapshotBtn_clicked
 */
void SaveImageDialog::on_saveCamSnapshotBtn_clicked()
{

    QString dirPath = QFileDialog::getSaveFileName(
                                this,
                                tr("Save File"),
                                "",
                                tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" )
                                );

    if (!dirPath.isEmpty()){
        qDebug()<<dirPath;
        QFile file(dirPath);
        file.open(QIODevice::WriteOnly);
        displayPixmap_.save(&file, "PNG");

    }
}

/**
 * @brief SaveImageDialog::on_saveGridSnapshotBtn_clicked
 */
void SaveImageDialog::on_saveGridSnapshotBtn_clicked()
{
    QString dirPath = QFileDialog::getSaveFileName(
                                this,
                                tr("Save File"),
                                "",
                                tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" )
                                );

    if (!dirPath.isEmpty()){
        qDebug()<<dirPath;
        QFile file(dirPath);
        file.open(QIODevice::WriteOnly);

        gridPixmap_.save(&file, "PNG");

    }
}

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}
/**
 * @brief The AboutDialog class                     Диалоговое окно с описанием приложения
 */

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H

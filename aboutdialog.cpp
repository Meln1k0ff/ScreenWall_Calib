#include "aboutdialog.h"
#include "ui_aboutdialog.h"

/**
 * @brief AboutDialog::AboutDialog
 * @param parent
 */
AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
/**
 * @brief AboutDialog::on_buttonBox_accepted
 */
void AboutDialog::on_buttonBox_accepted()
{
    delete this;
}

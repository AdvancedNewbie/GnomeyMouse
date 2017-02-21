#include "aboutdialog.h"
#include "ui_aboutdialog.h"

aboutDialog::aboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutDialog)
{
    ui->setupUi(this);

    QPixmap image(":/images/rubberducky.jpg");
    ui->duckLabel->setScaledContents(true);
    ui->duckLabel->setPixmap(image);

}

aboutDialog::~aboutDialog()
{
    delete ui;
}


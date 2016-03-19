#include "ccdialog.h"
#include "ui_ccdialog.h"

CCDialog::CCDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CCDialog)
{
    ui->setupUi(this);
}

CCDialog::~CCDialog()
{
    delete ui;
}
void CCDialog::on_pushButton_clicked()
{
    CCDialog::close();
}

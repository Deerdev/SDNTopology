#include "showsportdialog.h"
#include "ui_showsportdialog.h"

ShowSPortDialog::ShowSPortDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowSPortDialog)
{
    ui->setupUi(this);
}

ShowSPortDialog::~ShowSPortDialog()
{
    delete ui;
}

#include "parahostdlg.h"
#include "ui_parahostdlg.h"

ParaHostDlg::ParaHostDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParaHostDlg)
{
    ui->setupUi(this);
}

ParaHostDlg::~ParaHostDlg()
{
    delete ui;
}

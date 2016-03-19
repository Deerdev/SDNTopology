#include "weightinfo.h"
#include "ui_weightinfo.h"

WeightInfo::WeightInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WeightInfo)
{
    ui->setupUi(this);
}

WeightInfo::~WeightInfo()
{
    delete ui;
}

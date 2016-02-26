#include "addflowdialog.h"
#include "ui_addflowdialog.h"

AddFlowDialog::AddFlowDialog(NetworkSimulationPlatform *_platform,QWidget *parent) :
    QDialog(parent),m_platform(_platform),
    ui(new Ui::AddFlowDialog)
{
    ui->setupUi(this);
}

AddFlowDialog::~AddFlowDialog()
{
    delete ui;
}

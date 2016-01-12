#include "CPartitionNumDialog.h"
#include "ui_CPartitionNumDialog.h"

CPartitionNumDialog::CPartitionNumDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPartitionNumDialog)
{
    ui->setupUi(this);
    m_cancel = false;
}

CPartitionNumDialog::~CPartitionNumDialog()
{
    delete ui;
}

int CPartitionNumDialog::getPartitonNum()
{
    return ui->partitionSpinBox->text().toInt();
}

void CPartitionNumDialog::on_okButton_clicked()
{
    accept();
}

void CPartitionNumDialog::on_cancelButton_clicked()
{
    m_cancel = true;
    close();
}

bool CPartitionNumDialog::getCancel()
{
    return m_cancel;
}

#include "showflowsdialog.h"
#include "ui_showflowsdialog.h"
#include <QMessageBox>
#include "addflowdialog.h"

ShowFlowsDialog::ShowFlowsDialog(NetworkSimulationPlatform *_platform,QWidget *parent) :
    QDialog(parent),m_platform(_platform),
    ui(new Ui::ShowFlowsDialog)
{
    ui->setupUi(this);
    getFlowsFromController();
    ui->flowShowWidget->verticalHeader()->setHidden(true);

    ui->flowShowWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//����ѡ��
    ui->flowShowWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//���ܱ༭
    ui->flowShowWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);//��ѡ

    ui->flowShowWidget->setSpan(0,0,3,1);
    ui->flowShowWidget->setSpan(3,0,3,1);
}

ShowFlowsDialog::~ShowFlowsDialog()
{
    delete ui;
}

//add flow
void ShowFlowsDialog::on_addFlowButton_clicked()
{
    AddFlowDialog *addflowLog = new AddFlowDialog(m_platform);
    if (addflowLog->exec() == QDialog::Accepted)
    {
        QMessageBox::information(this, tr("��ʾ"),tr("��ӳɹ���"),QMessageBox::Ok);
    }
}

//get flow
void ShowFlowsDialog::on_getFlowButton_clicked()
{
    QMessageBox::information(this, tr("��ʾ"),tr("������ɣ�"),QMessageBox::Ok);
}

//delete flow
void ShowFlowsDialog::on_delFlowButton_clicked()
{
    QMessageBox::information(this, tr("��ʾ"),tr("ɾ���ɹ���"),QMessageBox::Ok);
}

//clear flow
void ShowFlowsDialog::on_clearFlowButton_clicked()
{
    QMessageBox::information(this, tr("��ʾ"),tr("����ɹ���"),QMessageBox::Ok);
}

void ShowFlowsDialog::getFlowsFromController()
{

}

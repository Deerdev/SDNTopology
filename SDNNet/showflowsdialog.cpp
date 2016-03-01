#include "showflowsdialog.h"
#include "ui_showflowsdialog.h"
#include <QMessageBox>
#include "addflowdialog.h"
//************QJson*****
#include "paraser.h"

ShowFlowsDialog::ShowFlowsDialog(NetworkSimulationPlatform *_platform,QWidget *parent) :
    QDialog(parent),m_platform(_platform),
    ui(new Ui::ShowFlowsDialog)
{
    ui->setupUi(this);
    m_curl = new SDNCurl();

    getFlowsFromController();
    ui->flowShowWidget->verticalHeader()->setHidden(true);

    ui->flowShowWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//按行选中
    ui->flowShowWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不能编辑
    ui->flowShowWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);//多选

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
        QMessageBox::information(this, tr("提示"),tr("添加成功！"),QMessageBox::Ok);
    }
}

//get flow
void ShowFlowsDialog::on_getFlowButton_clicked()
{

    QMessageBox::information(this, tr("提示"),tr("更新完成！"),QMessageBox::Ok);
}

//delete flow
void ShowFlowsDialog::on_delFlowButton_clicked()
{
    int row = ui->flowShowWidget->currentRow();
    QTableWidgetItem *t_nameItem = ui->flowShowWidget->item(row,2);
    QString t_name = t_nameItem->text();
    if(m_curl->deleteFlow(t_name))
        QMessageBox::information(this, tr("提示"),tr("删除成功！"),QMessageBox::Ok);
}

//clear flow
void ShowFlowsDialog::on_clearFlowButton_clicked()
{
    QMessageBox::information(this, tr("提示"),tr("清除成功！"),QMessageBox::Ok);
}

void ShowFlowsDialog::getFlowsFromController()
{
    QString t_flowsStr;
    m_curl->getFlows(t_flowsStr);
    if(t_flowsStr.isEmpty())
        return;

    QJson::Parser parser;
    bool ok;
    QVariantMap t_flowsMap = parser.parse(t_flowsStr.toUtf8(), &ok).toMap();
    if (!ok) {
        //qFatal("An error occurred during parsing");
        return;
    }

    foreach (QVariant flow, t_flowsMap[m_switchNames.at(0)].toList()) {
        qDebug() << flow.toString();
    }

}

void ShowFlowsDialog::getDeviceName()
{
    QString t_switchName;
    m_curl->getFlows(t_switchName);
    if(t_switchName.isEmpty())
        return;

    QJson::Parser parser;
    bool ok;
    QVariantMap t_flowsMap = parser.parse(t_switchName.toUtf8(), &ok).toMap();
    if (!ok) {
        //qFatal("An error occurred during parsing");
        return;
    }
}






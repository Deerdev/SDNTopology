#include "showflowsdialog.h"
#include "ui_showflowsdialog.h"
#include <QMessageBox>
#include <QDebug>
#include "addflowdialog.h"
//************QJson*****
#include "parser.h"
#include "serializer.h"

ShowFlowsDialog::ShowFlowsDialog(NetworkSimulationPlatform *_platform,QWidget *parent) :
    QDialog(parent),m_platform(_platform),
    ui(new Ui::ShowFlowsDialog)
{
    ui->setupUi(this);
    m_curl = new SDNCurl();

    //ui->flowShowWidget->verticalHeader()->setHidden(true);

    ui->flowShowWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//按行选中
    ui->flowShowWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不能编辑
    ui->flowShowWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);//多选

    ui->flowShowWidget->setRowCount(12);
    ui->flowShowWidget->setColumnCount(3);

    QFont serifFont("Ubuntu", 13);
    QTableWidgetItem *HeaderItem0 = new QTableWidgetItem(QString(tr("交换机")));
    HeaderItem0->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
    HeaderItem0->setFont(serifFont);
    ui->flowShowWidget->setHorizontalHeaderItem(0,HeaderItem0);

    QTableWidgetItem *HeaderItem1 = new QTableWidgetItem(QString(tr("流表名字")));
    HeaderItem1->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
    HeaderItem1->setFont(serifFont);
    ui->flowShowWidget->setHorizontalHeaderItem(1,HeaderItem1);

    QTableWidgetItem *HeaderItem2 = new QTableWidgetItem(QString(tr("流表内容")));
    HeaderItem2->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
    HeaderItem2->setFont(serifFont);
    ui->flowShowWidget->setHorizontalHeaderItem(2,HeaderItem2);
//    ui->flowShowWidget->setSpan(0,0,3,1);
//    ui->flowShowWidget->setSpan(3,0,3,1);

    getDeviceName();
    getFlowsFromController();
}

ShowFlowsDialog::~ShowFlowsDialog()
{
    delete ui;
    if(m_curl)
        delete m_curl;
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

    getFlowsFromController();
    QMessageBox::information(this, tr("提示"),tr("更新完成！"),QMessageBox::Ok);
}

//delete flow
void ShowFlowsDialog::on_delFlowButton_clicked()
{
    int row = ui->flowShowWidget->currentRow();
    QTableWidgetItem *t_nameItem = ui->flowShowWidget->item(row,1);
    QString t_name = t_nameItem->text();
    if(t_name.isEmpty())
        return;
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
    //清空表格
    if(ui->flowShowWidget->item(0,0))
    {
        ui->flowShowWidget->clear();
    }

    int rowSpanCount = 0, columnSpanCount =1;
    int rowbegin = 0;


    QString t_flowsStr;
    m_curl->getFlows(t_flowsStr);
    if(t_flowsStr.isEmpty())
        return;

    qDebug()<<t_flowsStr;
    QJson::Parser parser;
    bool ok;
    QVariantMap t_flowsMap = parser.parse(t_flowsStr.toUtf8(), &ok).toMap();
    if (!ok) {
        //qFatal("An error occurred during parsing");
        return;
    }
    qDebug()<< "get flows success!";

    //通过交换机的名字获取交换机的流表
    for(int i=0; i < m_switchNames.size()/*交换机的数量*/; ++i)
    {
        qDebug()<< m_switchNames.at(i);
        QVariant t_switchFlows  = t_flowsMap[m_switchNames.at(i)];
        qDebug()<< t_switchFlows;
        QVariantList t_switchFlowList = t_switchFlows.toList();
        qDebug()<< t_switchFlowList.size();

        if(t_switchFlowList.size() <= 0)
            continue;
        rowSpanCount = t_switchFlowList.size();
        QTableWidgetItem *newItem11 = new QTableWidgetItem(m_switchNames.at(i));
        newItem11->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
        ui->flowShowWidget->setItem(rowbegin, 0, newItem11);


        int flowRow = rowbegin;
        for(int j=0; j<t_switchFlowList.size()/*交换机中流表数量*/; ++j)
        {
           QVariantMap t_sFlowMap= t_switchFlowList.at(j).toMap();
           //qDebug()<< ttt["switchDPID"].toString();

           QJson::Serializer serializer;
           bool ok;
           QByteArray t_flow = serializer.serialize(t_sFlowMap, &ok);
           QString t_flowStr = t_flow;//交换机的一个流表
           if (ok) {
               qDebug() << t_flowStr;
           } else {
               qCritical() << "Something went wrong:" << serializer.errorMessage();
           }

           //获取name
           QStringList t_flowStrList = t_flowStr.split(":");
           qDebug() << t_flowStrList;
           QString t_name;//交换机流表中的名字
           if (t_flowStrList.size()>=1)
           {
               QString t_first = t_flowStrList.at(0);
               for (int i = 1; i < t_first.size(); ++i)
               {
                   t_name.append(t_first.at(i));
               }
           }
           qDebug()<<t_name;

           QTableWidgetItem *newItem12 = new QTableWidgetItem(t_name);
           newItem12->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
           ui->flowShowWidget->setItem(flowRow, 1, newItem12);
           QTableWidgetItem *newItem13 = new QTableWidgetItem(t_flowStr);
           newItem13->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
           ui->flowShowWidget->setItem(flowRow, 2, newItem13);
           flowRow++;
        }
        ui->flowShowWidget->setSpan(rowbegin,0,rowSpanCount,1);
        rowbegin += rowSpanCount;
    }
}

void ShowFlowsDialog::getDeviceName()
{
    QString t_switchName;
    m_curl->getSwitchesInf(t_switchName);
    if(t_switchName.isEmpty())
        return;

    QJson::Parser parser;
    bool ok;
    QVariantList t_devicesList = parser.parse(t_switchName.toUtf8(), &ok).toList();
    if (!ok) {
        qFatal("An error occurred during parsing");
        exit (1);
    }
    for(int i=0; i<t_devicesList.size(); ++i)
    {
       QVariantMap t_deviceMap= t_devicesList.at(i).toMap();
       QString t_sName =  t_deviceMap["switchDPID"].toString();
       qDebug()<<t_sName;
       m_switchNames.append(t_sName);
    }
}






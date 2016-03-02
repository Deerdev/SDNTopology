#include "addflowdialog.h"
#include <QString>
#include "ui_addflowdialog.h"
#include "SDNCurl/sdncurl.h"

#include <QDebug>
#include <iostream>

AddFlowDialog::AddFlowDialog(NetworkSimulationPlatform *_platform,QWidget *parent) :
    QDialog(parent),m_platform(_platform),
    ui(new Ui::AddFlowDialog)
{
    ui->setupUi(this);
    m_jsonVec.clear();
    m_curl = new SDNCurl();
}

AddFlowDialog::~AddFlowDialog()
{
    m_jsonVec.clear();
    delete ui;
    if(m_curl)
        delete m_curl;
}

void AddFlowDialog::on_submitButton_clicked()//提交流表
{
    QString t_s1 = ui->DPIDLineEdit->text();
    QString t_s2 = ui->flowNameLineEdit->text();
    QString t_s3 = ui->inPortLineEdit->text();
    QString t_s4 = ui->srcMACLineEdit->text();
    QString t_s5 = ui->desMACLineEdit->text();
    QString t_s6 = ui->srcIPLineEdit->text();
    QString t_s7 = ui->desIPLineEdit->text();
    QString t_s8 = ui->priorityLineEdit->text();
    QString t_s9 = ui->activeComboBox->currentText();
    QString t_s10 = ui->actionLineEdit->text();
    //QString t_s9 = ui->activeComboBox()->currentText();

    qDebug()  <<t_s1<<" "<<t_s8 << " "<<t_s9;

    QString strJson;
    strJson = "{";

    if(!t_s1.isEmpty())
    {
        strJson = strJson + "\"switch\" :" + t_s1 + ",";
        qDebug()<< strJson;
    }
    if(!t_s2.isEmpty())
    {
        strJson = strJson + "\"name\" :" + t_s2 + ",";
    }
    if(!t_s3.isEmpty())
    {
        strJson = strJson + "\"in_port\" :" + t_s3 + ",";
    }
    if(!t_s4.isEmpty())
    {
        strJson = strJson + "\"eth_src\" :" + t_s4 + ",";
    }
    if(!t_s5.isEmpty())
    {
        strJson = strJson + "\"eth_dst\" :" + t_s5 + ",";
    }
    if(!t_s6.isEmpty())
    {
        strJson = strJson + "\"ipv4_src\" :" + t_s6 + ",";
    }
    if(!t_s7.isEmpty())
    {
        strJson = strJson + "\"ipv4_dst\" :" + t_s7 + ",";
    }
    if(!t_s8.isEmpty())
    {
        strJson = strJson + "\"priority\" :" + t_s8 + ",";
    }
    if(!t_s9.isEmpty())
    {
        strJson = strJson + "\"active\" :" + "\"" + t_s9 + "\"" + ",";
    }
    if(!t_s10.isEmpty())
    {
        strJson = strJson + "\"actions\" :" + t_s10;
    }
    strJson = strJson +"}";
    qDebug()<< strJson;

    m_jsonVec.append(strJson);
    m_curl->addFlows(m_jsonVec);

//    strJson += "\"name\" : \"flow-mod-1\",";
//    strJson += "\"cookie\" : \"0\",";
//    strJson += "\"priority\" : \"32768\",";
//    strJson += "\"in_port\" : \"1\",";
//    strJson += "\"active\" : \"true\",";
//    strJson += "\"actions\" : \"output=flood\"";
//    strJson += "}";
    //t_sjsonVec.append(strJson);

    m_jsonVec.clear();
    accept();
}

void AddFlowDialog::on_addAgainButton_clicked()//继续添加流表
{
    QString t_s1 = ui->DPIDLineEdit->text();
    QString t_s2 = ui->flowNameLineEdit->text();
    QString t_s3 = ui->inPortLineEdit->text();
    QString t_s4 = ui->srcMACLineEdit->text();
    QString t_s5 = ui->desMACLineEdit->text();
    QString t_s6 = ui->srcIPLineEdit->text();
    QString t_s7 = ui->desIPLineEdit->text();
    QString t_s8 = ui->priorityLineEdit->text();
    QString t_s9 = ui->activeComboBox->currentText();
    QString t_s10 = ui->actionLineEdit->text();
    //QString t_s9 = ui->activeComboBox()->currentText();

    QString strJson;
    strJson = "{";

    if(!t_s1.isEmpty())
    {
        strJson = strJson + "\"switch\" :" + t_s1 + ",";
        qDebug()<< strJson;
    }
    if(!t_s2.isEmpty())
    {
        strJson = strJson + "\"name\" :" + t_s2 + ",";
    }
    if(!t_s3.isEmpty())
    {
        strJson = strJson + "\"in_port\" :" + t_s3 + ",";
    }
    if(!t_s4.isEmpty())
    {
        strJson = strJson + "\"eth_src\" :" + t_s4 + ",";
    }
    if(!t_s5.isEmpty())
    {
        strJson = strJson + "\"eth_dst\" :" + t_s5 + ",";
    }
    if(!t_s6.isEmpty())
    {
        strJson = strJson + "\"ipv4_src\" :" + t_s6 + ",";
    }
    if(!t_s7.isEmpty())
    {
        strJson = strJson + "\"ipv4_dst\" :" + t_s7 + ",";
    }
    if(!t_s8.isEmpty())
    {
        strJson = strJson + "\"priority\" :" + t_s8 + ",";
    }
    if(!t_s9.isEmpty())
    {
        strJson = strJson + "\"active\" :" + "\"" + t_s9 + "\"" + ",";
    }
    if(!t_s10.isEmpty())
    {
        strJson = strJson + "\"actions\" :" + t_s10;
    }
    strJson = strJson +"}";
    qDebug()<< strJson;


    m_jsonVec.append(strJson);
}

void AddFlowDialog::on_cancelButton_clicked()//取消流表
{
    close();
}

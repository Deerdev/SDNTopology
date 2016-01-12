#include "CPortDialog.h"
#include "ui_CPortDialog.h"

QString EthernetBandwidth[] = {"1000","100"};
QString POSBandwidth[] = {"115","622"};

CPortDialog::CPortDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPortDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("新建链接窗口"));

    ui->mask1->setValue(255);
    ui->mask2->setValue(255);
    ui->mask3->setValue(255);
    ui->mask4->setValue(252);
    ui->delay->setValue(0.5);

    ui->linktype->addItem(tr("POS"));
    ui->linktype->addItem(tr("Ethernet"));
    for ( size_t i = 0; i < sizeof(POSBandwidth)/sizeof(QString); ++i )
    {
        ui->bandWidth->addItem(POSBandwidth[i]);
    }

    ui->OkButton->setFocus(); //设置默认焦点
    ui->OkButton->setDefault(true);

    connect(ui->linktype, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLink(int)));
}

CPortDialog::~CPortDialog()
{
    delete ui;
}

void CPortDialog::setPortName(QString _networkLocation, QString _portName, QString _linkPortName, int s_parentID, int e_parentID)
{
    m_PortInfo.networkLocation = _networkLocation;
    ui->networkLocation->setText(_networkLocation);
    ui->networkLocation->setDisabled(true);
    m_PortInfo.name = _portName;
    m_PortInfo.linkPort_name = _linkPortName;
    m_LinkPortInfo.name = _linkPortName;
    m_LinkPortInfo.linkPort_name = _portName;
    ui->srcDevice->setText(_portName);
    ui->srcDevice->setDisabled(true);
    ui->desDevice->setText(_linkPortName);
    ui->desDevice->setDisabled(true);
    m_PortInfo.Device_ID = s_parentID;
    m_PortInfo.linkPort_Device_ID = e_parentID;
    m_LinkPortInfo.Device_ID = e_parentID;
    m_LinkPortInfo.linkPort_Device_ID = s_parentID;
}

void CPortDialog::setPortIDandType(int _portID, int _linkPortID)
{
    m_PortInfo.Port_ID = _portID;
    m_PortInfo.linkPort_ID = _linkPortID;
    m_LinkPortInfo.Port_ID = _linkPortID;
    m_LinkPortInfo.linkPort_ID = _portID;
    ui->srcInterface->setText("Interface" + QVariant(_portID).toString());
    ui->desInterface->setText("Interface" + QVariant(_linkPortID).toString());
}

void CPortDialog::on_CancelButton_clicked()
{
    close();
}

void CPortDialog::on_OkButton_clicked()
{
    if (ui->srcInterface->text() == "")
    {
        QMessageBox::information(this, tr("提示"), tr("请输入本端端口！"));
        ui->srcInterface->setFocus();
        return;
    }
    else if (ui->desInterface->text() == "")
    {
        QMessageBox::information(this, tr("提示"), tr("请输入对端端口！"));
        ui->desInterface->setFocus();
        return;
    }
    else
    {
        for(int i = 0; i != v_ports.size(); ++i)
        {
            if(ui->srcInterface->text() == v_ports[i].interfaceName)
            {
                QMessageBox::information(this, tr("提示"), tr("端口") + ui->srcInterface->text() + tr("已被使用，请重新输入！"));
                ui->srcInterface->setText("Interface" + QVariant(m_PortInfo.Port_ID).toString());
                ui->srcInterface->setFocus();
                return;
            }
        }
        for(int j = 0; j != link_v_ports.size(); ++j)
        {
            if(ui->desInterface->text() == link_v_ports[j].interfaceName)
            {
                QMessageBox::information(this, tr("提示"), tr("对端端口已被使用，请重新输入！"));
                ui->desInterface->setText("Interface" + QVariant(m_PortInfo.linkPort_ID).toString());
                ui->desInterface->setFocus();
                return;
            }
        }
    }

    m_PortInfo.interfaceName = ui->srcInterface->text();
    m_PortInfo.linkPort_interfaceName = ui->desInterface->text();

    m_PortInfo.area = ui->srcArea->text();
    m_PortInfo.linkPort_area = ui->desArea->text();

    m_PortInfo.IP = ui->ip1->text()+"."+ui->ip2->text()+"."+ui->ip3->text()+"."+ui->ip4->text();
    m_PortInfo.linkPort_IP = ui->ip21->text()+"."+ui->ip22->text()+"."+ui->ip23->text()+"."+ui->ip24->text();

    m_PortInfo.subnetMask = ui->mask1->text()+"."+ui->mask2->text()+"."+ui->mask3->text()+"."+ui->mask4->text();
    m_PortInfo.linkPort_subnetMask = ui->mask1->text()+"."+ui->mask2->text()+"."+ui->mask3->text()+"."+ui->mask4->text();

    m_PortInfo.describe = ui->srcDescribe->text();
    m_PortInfo.linkPort_describe = ui->desDescribe->text();

    m_PortInfo.costValue = ui->srcCost->text();
    m_PortInfo.linkPort_costValue = ui->desCost->text();

    m_PortInfo.remark = ui->remark->text();
    m_PortInfo.linkPort_remark = ui->remark->text();

    m_PortInfo.bandWidth = ui->bandWidth->currentText().toInt();
    m_PortInfo.delay = ui->delay->text().toFloat();
    m_PortInfo.linkType = ui->linktype->currentIndex();

    m_LinkPortInfo.interfaceName = ui->desInterface->text();
    m_LinkPortInfo.linkPort_interfaceName = ui->srcInterface->text();

    m_LinkPortInfo.area = ui->desArea->text();
    m_LinkPortInfo.linkPort_area = ui->srcArea->text();

    m_LinkPortInfo.IP = ui->ip21->text()+"."+ui->ip22->text()+"."+ui->ip23->text()+"."+ui->ip24->text();
    m_LinkPortInfo.linkPort_IP = ui->ip1->text()+"."+ui->ip2->text()+"."+ui->ip3->text()+"."+ui->ip4->text();

    m_LinkPortInfo.subnetMask = ui->mask1->text()+"."+ui->mask2->text()+"."+ui->mask3->text()+"."+ui->mask4->text();
    m_LinkPortInfo.linkPort_subnetMask = ui->mask1->text()+"."+ui->mask2->text()+"."+ui->mask3->text()+"."+ui->mask4->text();

    m_LinkPortInfo.describe = ui->desDescribe->text();
    m_LinkPortInfo.linkPort_describe = ui->srcDescribe->text();

    m_LinkPortInfo.costValue = ui->desCost->text();
    m_LinkPortInfo.linkPort_costValue = ui->srcCost->text();

    m_LinkPortInfo.remark = ui->remark->text();
    m_LinkPortInfo.linkPort_remark = ui->remark->text();

    m_LinkPortInfo.bandWidth = ui->bandWidth->currentText().toInt();
    m_LinkPortInfo.delay = ui->delay->text();
    m_LinkPortInfo.linkType = ui->linktype->currentIndex();

    m_linkInfo.bandWidth = ui->bandWidth->currentText();
    m_linkInfo.linkType = ui->linktype->currentIndex();
    m_linkInfo.delay = ui->delay->text();

    if(m_ipPond->count(m_PortInfo.IP))
    {
        QMessageBox::information(this, tr("提示"), tr("本端IP已被使用，请重新输入！"),QMessageBox::Ok);
        return;
    }
    else if(m_ipPond->count(m_PortInfo.linkPort_IP) || m_PortInfo.IP == m_PortInfo.linkPort_IP)
    {
        QMessageBox::information(this, tr("提示"), tr("对端IP已被使用，请重新输入！"),QMessageBox::Ok);
        return;
    }
    else
    {
        QString srcNetworkID = calculateNetworkID(m_PortInfo.IP, m_PortInfo.subnetMask);
        QString desNetworkID = calculateNetworkID(m_PortInfo.linkPort_IP, m_PortInfo.linkPort_subnetMask);

        QString networkBroadcast = calculateNetworkBroadcast(m_PortInfo.IP, m_PortInfo.subnetMask);

        if (m_ipPool->count(srcNetworkID))
        {
            QMessageBox::information(this, tr("提示"), tr("IP所在网络号已被使用，请重新输入！"),QMessageBox::Ok);
            return;
        }
        if ( srcNetworkID != desNetworkID )
        {
            QMessageBox::information(this, tr("提示"), tr("源端IP与终端IP不在同一网络，请重新输入！"),QMessageBox::Ok);
            return;
        }
        if (m_PortInfo.IP == srcNetworkID )
        {
            QMessageBox::information(this, tr("提示"), tr("源IP地址不能为所在网号，请重新输入！"),QMessageBox::Ok);
            return;
        }
        if (m_PortInfo.linkPort_IP == srcNetworkID )
        {
            QMessageBox::information(this, tr("提示"), tr("对端IP地址不能为所在网号，请重新输入！"),QMessageBox::Ok);
            return;
        }

        if (m_PortInfo.IP == networkBroadcast )
        {
            QMessageBox::information(this, tr("提示"), tr("源IP地址不能为所在网络广播地址，请重新输入！"),QMessageBox::Ok);
            return;
        }

        if (m_PortInfo.linkPort_IP == networkBroadcast )
        {
            QMessageBox::information(this, tr("提示"), tr("对端IP地址不能为所在网络广播地址，请重新输入！"),QMessageBox::Ok);
            return;
        }
        m_ipPond->insert(m_PortInfo.IP);
        m_ipPond->insert(m_PortInfo.linkPort_IP);
        (*m_ipPool)[srcNetworkID] = 2;
    }

    emit accept();
}

void CPortDialog::clear(void)
{
    ui->networkLocation->clear();
    ui->remark->clear();
    ui->srcDevice->clear();
    ui->desDevice->clear();
    ui->srcInterface->clear();
    ui->desInterface->clear();
    ui->srcDescribe->clear();
    ui->desDescribe->clear();
    ui->ip1->setValue(0);
    ui->ip2->setValue(0);
    ui->ip3->setValue(0);
    ui->ip4->setValue(0);
    ui->ip21->setValue(0);
    ui->ip22->setValue(0);
    ui->ip23->setValue(0);
    ui->ip24->setValue(0);
//    ui->mask1->clear();
//    ui->mask2->clear();
//    ui->mask3->clear();
//    ui->mask4->clear();
//    ui->mask21->clear();
//    ui->mask22->clear();
//    ui->mask23->clear();
//    ui->mask24->clear();
//    ui->srcArea->clear();
//    ui->desArea->clear();
//    ui->srcCost->clear();
//    ui->desCost->clear();
}

void CPortDialog::changeLink(int _index)
{
    ui->bandWidth->clear();
    if (POS == _index)
    {
        int POSBandwidthSize = sizeof(POSBandwidth)/sizeof(QString);
        for ( int i = 0; i < POSBandwidthSize; ++i )
        {
            ui->bandWidth->addItem(POSBandwidth[i]);
        }
    }
    else if(Ethernet == _index)
    {
        int EthernetBandwidthSize = sizeof(EthernetBandwidth)/sizeof(QString);
        for ( int i = 0; i < EthernetBandwidthSize; ++i )
        {
            ui->bandWidth->addItem(EthernetBandwidth[i]);
        }
    }
}

QString CPortDialog::calculateNetworkID(QString _IP, QString _mask)
{
    QList<QString> ipList = _IP.split('.');
    QList<QString> maskList = _mask.split('.');
    if(ipList.size() != 4 || maskList.size() != 4)
    {
        QMessageBox::critical(this, tr("错误"),tr("IP地址或子网掩码错误!"),QMessageBox::Ok);
        return NULL;
    }
    QString networkID1 = QString::number(ipList[0].toInt() & maskList[0].toInt());
    QString networkID2 = QString::number(ipList[1].toInt() & maskList[1].toInt());
    QString networkID3 = QString::number(ipList[2].toInt() & maskList[2].toInt());
    QString networkID4 = QString::number(ipList[3].toInt() & maskList[3].toInt());
    return networkID1 + "." + networkID2 + "." + networkID3 + "." + networkID4;
}

QString CPortDialog::calculateNetworkBroadcast(QString _IP, QString _mask)
{
    QList<QString> ipList = _IP.split('.');
    QList<QString> maskList = _mask.split('.');
    if(ipList.size() != 4 || maskList.size() != 4)
    {
        QMessageBox::critical(this, tr("错误"),tr("IP地址或子网掩码错误!"),QMessageBox::Ok);
        return NULL;
    }

    QString BroadcastID1 = QString::number((ipList[0].toInt() & maskList[0].toInt()) + 255 - maskList[0].toInt());
    QString BroadcastID2 = QString::number((ipList[1].toInt() & maskList[1].toInt()) + 255 - maskList[1].toInt());
    QString BroadcastID3 = QString::number((ipList[2].toInt() & maskList[2].toInt()) + 255 - maskList[2].toInt());
    QString BroadcastID4 = QString::number((ipList[3].toInt() & maskList[3].toInt()) + 255 - maskList[3].toInt());
    return BroadcastID1 + "." + BroadcastID2 + "." + BroadcastID3 + "." + BroadcastID4;
}

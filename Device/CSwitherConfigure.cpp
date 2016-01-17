#include "CSwitherConfigure.h"
#include <QMessageBox>

CSwitherConfigure::CSwitherConfigure(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

    ui.portNumComboBox->addItem("32");
    ui.portNumComboBox->addItem("48");
    ui.deviceLocation->addItem(tr("�Ǹɲ�"));
    ui.deviceLocation->addItem(tr("��۲�"));
    ui.deviceLocation->addItem(tr("�����"));
    ui.protocolComboBox->addItem("Default");
    ui.protocolComboBox->addItem("OSPF");
    ui.okButton->setFocus();
    ui.tableWidget->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    ui.tableWidget->setSelectionBehavior ( QAbstractItemView::SelectRows); //����ѡ����Ϊ������Ϊ��λ
    //ui.tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    //ui.tableWidget->verticalHeader()->setHidden(true);
    //ui.tableWidget->setColumnCount(6);
    //ui.tableWidget->setSelectionMode ( QAbstractItemView::SingleSelection); //����ѡ��ģʽ��ѡ����
    //ui.tableWidget->setFocusPolicy(Qt::NoFocus);
}

CSwitherConfigure::~CSwitherConfigure()
{

}

void CSwitherConfigure::SetSwitcherInfo(CSwitcherInfo &_switcherInfo)
{
	m_switcherInfo = _switcherInfo;//���ý���������
    ui.deviceNameLineEdit->setText(m_switcherInfo.name);
//    ui.deviceLocation->setText(m_switcherInfo.networkLocation);
    ui.id->setText( QVariant(m_switcherInfo.ID).toString());
    ui.id->setDisabled(true);
    ui.portNumComboBox->setDisabled(true);

	if (m_switcherInfo.portNum == 32)
	{
        ui.portNumComboBox->setCurrentIndex(0);
	}
	else if (m_switcherInfo.portNum == 48)
	{
        ui.portNumComboBox->setCurrentIndex(1);
	}
    if(!(QString::compare(m_switcherInfo.networkLocation, tr("�Ǹɲ�"), Qt::CaseInsensitive)))
    {
        ui.deviceLocation->setCurrentIndex(0);
    }
    else if(!(QString::compare(m_switcherInfo.networkLocation, tr("��۲�"), Qt::CaseInsensitive)))
    {
        ui.deviceLocation->setCurrentIndex(1);
    }
    else
    {
        ui.deviceLocation->setCurrentIndex(2);
    }
//    if(!(QString::compare(m_switcherInfo.protocol, "OSPF", Qt::CaseInsensitive)))
//    {
//        ui.protocolComboBox->setCurrentIndex(1);
//    }
//    else
//    {
//        ui.protocolComboBox->setCurrentIndex(0);
//    }
	
    int size = m_switcherInfo.LNodes.size();
    ui.tableWidget->setRowCount(size);
    for (int i = 0; i != size; ++i)
    {
        ui.tableWidget->setItem(i,0,new QTableWidgetItem(m_switcherInfo.LNodes[i].interfaceName));
        ui.tableWidget->setItem(i,1,new QTableWidgetItem(m_switcherInfo.LNodes[i].IP));
        ui.tableWidget->setItem(i,2,new QTableWidgetItem(m_switcherInfo.LNodes[i].linkPort_name));
        ui.tableWidget->setItem(i,3,new QTableWidgetItem(m_switcherInfo.LNodes[i].linkPort_IP));
        if(m_switcherInfo.LNodes[i].linkType == 0)
            ui.tableWidget->setItem(i,4,new QTableWidgetItem("POS"));
        else
            ui.tableWidget->setItem(i,4,new QTableWidgetItem("Ethernet"));
        ui.tableWidget->setItem(i,5,new QTableWidgetItem(QVariant(m_switcherInfo.LNodes[i].bandWidth).toString()+"MB/s"));
        ui.tableWidget->item(i, 2)->setFlags(Qt::NoItemFlags);//�Զ��豸�����ûҲ��ɱ༭

//        for(int j = 2; j != 5; ++j)
//        {
//            ui.tableWidget->item(i, j)->setFlags(ui.tableWidget->item(i, j)->flags() & (~Qt::ItemIsEditable));//���û��Ҳ��ɱ༭
//            ui.tableWidget->setItem(i, j, ui.tableWidget->item(i, j));
//        }
    }
}
QString CSwitherConfigure::calculateNetworkID(QString _IP, QString _mask)
{
    QList<QString> ipList = _IP.split('.');
    QList<QString> maskList = _mask.split('.');
    if(ipList.size() != 4 || maskList.size() != 4)
    {
        QMessageBox::critical(this, tr("����"),tr("IP��ַ�������������!"),QMessageBox::Ok);
        return NULL;
    }
    QString networkID1 = QString::number(ipList[0].toInt() & maskList[0].toInt());
    QString networkID2 = QString::number(ipList[1].toInt() & maskList[1].toInt());
    QString networkID3 = QString::number(ipList[2].toInt() & maskList[2].toInt());
    QString networkID4 = QString::number(ipList[3].toInt() & maskList[3].toInt());
    return networkID1 + "." + networkID2 + "." + networkID3 + "." + networkID4;
}

QString CSwitherConfigure::calculateNetworkBroadcast(QString _IP, QString _mask)
{
    QList<QString> ipList = _IP.split('.');
    QList<QString> maskList = _mask.split('.');
    if(ipList.size() != 4 || maskList.size() != 4)
    {
        QMessageBox::critical(this, tr("����"),tr("IP��ַ�������������!"),QMessageBox::Ok);
        return NULL;
    }

    QString BroadcastID1 = QString::number((ipList[0].toInt() & maskList[0].toInt()) + 255 - maskList[0].toInt());
    QString BroadcastID2 = QString::number((ipList[1].toInt() & maskList[1].toInt()) + 255 - maskList[1].toInt());
    QString BroadcastID3 = QString::number((ipList[2].toInt() & maskList[2].toInt()) + 255 - maskList[2].toInt());
    QString BroadcastID4 = QString::number((ipList[3].toInt() & maskList[3].toInt()) + 255 - maskList[3].toInt());
    return BroadcastID1 + "." + BroadcastID2 + "." + BroadcastID3 + "." + BroadcastID4;
}

void CSwitherConfigure::on_okButton_clicked()
{
    if(!m_vDeleteInterfaceName.empty())
    {
        for(int i = 0; i < m_vDeleteInterfaceName.size(); ++i)
        {
            for(int j = 0; j < m_switcherInfo.LNodes.size(); ++j)
            {
                if(m_vDeleteInterfaceName[i] == m_switcherInfo.LNodes[j].interfaceName)
                {
                    SNodeStructInfo t_portInfo = m_switcherInfo.LNodes[j];
                    QString t_sIP = t_portInfo.IP;
                    QString t_eIP = t_portInfo.linkPort_IP;
                    QString srcNetworkID = calculateNetworkID(t_sIP, t_portInfo.subnetMask);
                    if(m_ipPool->count(srcNetworkID))
                    {
                        m_ipPool->erase(srcNetworkID);
                    }
                    if(m_ipPond->count(t_sIP) || m_ipPond->count(t_eIP))
                    {
                        m_ipPond->erase(t_sIP);
                        m_ipPond->erase(t_eIP);
                    }
                    m_deletePortsInfo.push_back(t_portInfo);
                    m_switcherInfo.LNodes.erase(m_switcherInfo.LNodes.begin() + j);
                    break;
                }
            }
       }
       m_vDeleteInterfaceName.clear();
    }
    if(ui.deviceNameLineEdit->text() == "")
    {
        QMessageBox::information(this, tr("��ʾ"), tr("�������豸���ƣ�"));
        ui.deviceNameLineEdit->setFocus();
        return;
    }
    for(int i = 0; i != m_switcherInfo.LNodes.size(); ++i)
    {
        m_ipPond->erase(m_switcherInfo.LNodes[i].IP);
        m_ipPond->erase(m_switcherInfo.LNodes[i].linkPort_IP);
    }
    for(int i = 0; i != ui.tableWidget->rowCount(); ++i)
    {
        //�ж϶˿��Ƿ�Ϊ��
        if (ui.tableWidget->item(i, 0)->text() == "")
        {
            QMessageBox::information(this, tr("��ʾ"), tr("�����뱾�˶˿ڣ�"));
            ui.tableWidget->setFocus();
            return;
        }
    }
    if(ui.tableWidget->rowCount() > 0)
    {
        m_switcherInfo.LNodes[0].interfaceName = ui.tableWidget->item(0, 0)->text();
        for(int i = 1; i != ui.tableWidget->rowCount(); ++i)
        {
            //�жϱ��˶˿��Ƿ��ѱ�ʹ��
            for(int j = 0; j != i; ++j)
            {
                if(ui.tableWidget->item(i, 0)->text() == m_switcherInfo.LNodes[j].interfaceName)
                {
                    QMessageBox::information(this,tr("��ʾ"),tr("Դ�˿�") + ui.tableWidget->item(i, 0)->text() + tr("�ѱ�ʹ��,���������룡"), QMessageBox::Ok);
                    ui.tableWidget->setFocus();
                    return;
                }
                m_switcherInfo.LNodes[i].interfaceName = ui.tableWidget->item(i, 0)->text();
            }
        }
    }
    for(int i = 0; i != ui.tableWidget->rowCount(); ++i)
    {

        //�ж�IP�Ƿ�Ϊ��
        if (ui.tableWidget->item(i, 1)->text() == "")
        {
            QMessageBox::information(this, tr("��ʾ"), tr("�����뱾��IP��"));
            ui.tableWidget->setFocus();
            return;
        }
        if (ui.tableWidget->item(i, 3)->text() == "")
        {
            QMessageBox::information(this, tr("��ʾ"), tr("�������IP��"));
            ui.tableWidget->setFocus();
            return;
        }

        //�ж�IP�Ƿ����
        QString t_sip = ui.tableWidget->item(i, 1)->text();
        QString t_eip = ui.tableWidget->item(i, 3)->text();
        QString srcNetworkID = calculateNetworkID(t_sip, m_switcherInfo.LNodes[i].subnetMask);
        QString desNetworkID = calculateNetworkID(t_eip, m_switcherInfo.LNodes[i].subnetMask);
        QString networkBroadcast = calculateNetworkBroadcast(t_sip, m_switcherInfo.LNodes[i].subnetMask);
        if(m_ipPond->count(t_sip))
        {
            QMessageBox::information(this, tr("��ʾ"), "IP(" + t_sip + tr(")�ѱ�ʹ�ã����������룡"),QMessageBox::Ok);
            ui.tableWidget->item(i, 1)->text().clear();
            ui.tableWidget->setFocus();
            return;
        }
        if ( srcNetworkID != desNetworkID )
        {
            QMessageBox::information(this, tr("��ʾ"),"IP(" + t_sip + tr(")��IP(") + t_eip + tr(")����ͬһ���磬���������룡"),QMessageBox::Ok);
            ui.tableWidget->item(i, 1)->text().clear();
            ui.tableWidget->item(i, 3)->text().clear();
            ui.tableWidget->setFocus();
            return;
        }
        if (t_sip == srcNetworkID )
        {
            QMessageBox::information(this, tr("��ʾ"),"IP(" + t_sip + tr(")Ϊ�������ţ����������룡"),QMessageBox::Ok);
            ui.tableWidget->item(i, 1)->text().clear();
            ui.tableWidget->setFocus();
            return;
        }
        if (t_sip == networkBroadcast )
        {
            QMessageBox::information(this, tr("��ʾ"),"IP(" + t_sip + tr(")Ϊ��������㲥��ַ�����������룡"),QMessageBox::Ok);
            ui.tableWidget->item(i, 1)->text().clear();
            ui.tableWidget->setFocus();
            return;
        }
        m_ipPond->insert(t_sip);
        if(m_ipPond->count(t_eip))
        {
            QMessageBox::information(this, tr("��ʾ"), "IP(" + t_eip + tr(")�ѱ�ʹ�ã����������룡"),QMessageBox::Ok);
            ui.tableWidget->item(i, 3)->text().clear();
            ui.tableWidget->setFocus();
            return;
        }

//        if (m_ipPool->count(srcNetworkID))
//        {
//            QMessageBox::information(this, tr("��ʾ"),"IP(" + t_sip + tr(")����������ѱ�ʹ�ã����������룡"),QMessageBox::Ok);
//            ui.tableWidget->item(i, 1)->text().clear();
//            return;
//        }


        if (t_eip == srcNetworkID )
        {
            QMessageBox::information(this, tr("��ʾ"),"IP(" + t_eip + tr(")Ϊ�������ţ����������룡"),QMessageBox::Ok);
            ui.tableWidget->item(i, 3)->text().clear();
            ui.tableWidget->setFocus();
            return;
        }

        if (t_eip == networkBroadcast )
        {
            QMessageBox::information(this, tr("��ʾ"),"IP(" + t_eip + tr(")Ϊ��������㲥��ַ�����������룡"),QMessageBox::Ok);
            ui.tableWidget->item(i, 3)->text().clear();
            ui.tableWidget->setFocus();
            return;
        }
        m_ipPond->insert(t_eip);
        m_switcherInfo.LNodes[i].IP = t_sip;
        m_switcherInfo.LNodes[i].linkPort_IP = t_eip;
    }

    m_switcherInfo.name = ui.deviceNameLineEdit->text();
    m_switcherInfo.networkLocation = ui.deviceLocation->currentText();
    m_switcherInfo.portNum = ui.portNumComboBox->currentText().toInt();
    //m_switcherInfo.protocol = ui.protocolComboBox->currentText();


    emit accepted();
	
}

void CSwitherConfigure::on_deleteButton_clicked()
{
    QItemSelectionModel *selections = ui.tableWidget->selectionModel();
    QModelIndexList selected = selections->selectedIndexes();
    stack<int> deleteRows;
    foreach (QModelIndex index, selected)
    {
        if(index.column() == 0)
        {
            deleteRows.push(index.row());
            QString interfaceName = ui.tableWidget->item(index.row(), 0)->text();
            m_vDeleteInterfaceName.push_back(interfaceName);
        }
    }
    while(!deleteRows.empty())
    {
        int row = deleteRows.top();
        ui.tableWidget->removeRow(row);
        deleteRows.pop();
    }
}

void CSwitherConfigure::on_cancelButton_clicked()
{
    m_vDeleteInterfaceName.clear();
    close();
}

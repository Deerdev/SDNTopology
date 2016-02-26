#include "CNewSwitcher.h"
#include <QMessageBox>

CNewSwitcher::CNewSwitcher(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
    ui.name->setText("Router");

    ui.swichType->addItem("Cisco 7609");
    ui.swichType->addItem("Cisco 7609-S");
	
    ui.portnum->addItem("32");
    ui.portnum->addItem("48");

    ui.id->addItem("1");

    ui.protocol->addItem("Default");
    ui.protocol->addItem("OSPF");

    ui.address->addItem(tr("�Ǹɲ�"));
    ui.address->addItem(tr("��۲�"));
    ui.address->addItem(tr("�����"));


	connect( ui.Switch_OK, SIGNAL(clicked()), this ,SLOT(okProcess()) );
	connect( ui.Switch_Cancel, SIGNAL(clicked()), this ,SLOT(cancelProcess()) );
}

CNewSwitcher::~CNewSwitcher()
{

}

void CNewSwitcher::setSwitchIDandType(int _switchID)
{
	m_switherInfo.ID = _switchID;
    ui.id->setEditText(QVariant(_switchID).toString());
    ui.id->setDisabled(true);
    m_switherInfo.name = "Router" + QVariant(_switchID).toString();
    ui.name->setText("Router" + QVariant(_switchID).toString());
}

void CNewSwitcher::okProcess( void )
{
   // m_switherInfo.type = (ui.swichType->currentText()).toInt();
    m_switherInfo.ID = ui.id->currentText().toInt();
    m_switherInfo.portNum = ui.portnum->currentText().toInt();
    m_switherInfo.name = ui.name->text();
    //m_switherInfo.protocol = ui.protocol->currentText();
    m_switherInfo.networkLocation = ui.address->currentText();

	QString str = m_switherInfo.name;

    if(str.isEmpty())
    {
        QMessageBox::information(0,tr("��ʾ"),tr("���������뽻�������ƣ�"),QMessageBox::Ok);
        return;
    }
    else if(str.size() > 8)
	{
		QMessageBox::information(0,tr("��ʾ"),tr("���������Ʋ��ܳ���8���ַ������������룡"),QMessageBox::Ok);
        ui.name->clear();
        return;
	}

    if(ui.address->currentText().isEmpty())
    {
        QMessageBox::information(0,tr("��ʾ"),tr("�����������ַ��"),QMessageBox::Ok);
         return;
    }

	emit accepted();
}

void CNewSwitcher::cancelProcess( void )
{
	close();
}

void CNewSwitcher::clear(void)
{
//    ui.name->clear();
//    ui.id->clear();
//    ui.portnum->clear();
//    ui.protocol->clear();
//    ui.address->clear();
}

#include "CEventscheduldialog.h"
#include "ui_CEventscheduldialog.h"

CEventSchedulDialog::CEventSchedulDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CEventSchedulDialog)
{
    ui->setupUi(this);
    ui->routelist->setSelectionMode(QAbstractItemView::SingleSelection);
    m_selectSwitcher = NULL;
    ui->eventParameter->addItem("DataRate");
    ui->eventParameter->addItem("MTU");
    ui->eventParameter->addItem("MaxPackets");
    ui->portTableWidget->setColumnCount(5);
    connect(ui->routelist, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(showPorts()));
}

CEventSchedulDialog::~CEventSchedulDialog()
{
    delete ui;
}

void CEventSchedulDialog::setSwichterInfo(vector<CSwitcherInfo> *v_switcherInfo)
{
    m_VswitcherInfo = v_switcherInfo;
    for(unsigned int i = 0; i < (*m_VswitcherInfo).size(); ++i)
        ui->routelist->addItem((*m_VswitcherInfo)[i].name);
}
//?????¨¬§¥????
void CEventSchedulDialog::showPorts()
{
    ui->portTableWidget->clearContents();
    QList<QListWidgetItem *> selectItem = ui->routelist->selectedItems();
    ui->portTableWidget->setRowCount( (*m_VswitcherInfo).size() );
    for(unsigned int i = 0; i < (*m_VswitcherInfo).size(); ++i)
    {
        if(selectItem[0]->text() == (*m_VswitcherInfo)[i].name)
        {
            m_selectSwitcher = &( (*m_VswitcherInfo)[i] );
            int t_size = (*m_VswitcherInfo)[i].LNodes.size();
            ui->portTableWidget->setRowCount(t_size);
            for(int j = 0; j < t_size; ++j)
            {
                QString interfaceName = (*m_VswitcherInfo)[i].LNodes[j].interfaceName;
                ui->portTableWidget->setItem(j, 0, new QTableWidgetItem(interfaceName));
            }
            showEventParameter();
        }
    }
}

void CEventSchedulDialog::on_eventParameter_currentIndexChanged(int index)
{
    showEventParameter();
}

void CEventSchedulDialog::showEventParameter(void)
{
    int eventIndex = ui->eventParameter->currentIndex();
    if(m_selectSwitcher != NULL)
    {
        for(int i = 0; i < m_selectSwitcher->LNodes.size(); ++i)
        {
            if(eventIndex == 0)
            {
                ui->portTableWidget->horizontalHeaderItem(1)->setText("Old DataRate");
                ui->portTableWidget->horizontalHeaderItem(2)->setText("New DataRate");
                QString bandWidth = QString::number( (*m_selectSwitcher).LNodes[i].bandWidth);
                ui->portTableWidget->setItem(i, 1, new QTableWidgetItem(bandWidth));
            }
            else if(eventIndex == 1)
            {
                ui->portTableWidget->horizontalHeaderItem(1)->setText("Old MTU");
                ui->portTableWidget->horizontalHeaderItem(2)->setText("New MTU");
                QString mtu = "1500";
                ui->portTableWidget->setItem(i, 1, new QTableWidgetItem(mtu));
            }
            else if(eventIndex == 2)
            {
                ui->portTableWidget->horizontalHeaderItem(1)->setText("Old MaxPackets");
                ui->portTableWidget->horizontalHeaderItem(2)->setText("New MaxPackets");
                QString MaxPackets = "100";
                ui->portTableWidget->setItem(i, 1, new QTableWidgetItem(MaxPackets));
            }
        }
    }
}

void CEventSchedulDialog::on_okButton_clicked()
{
    QString m_currentPath = QDir::currentPath();
    QString absolutePath = m_currentPath + "/Topology/event.txt";
//    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"),
//                               absolutePath, tr("Topology (*.event)"));
//    if (filePath == absolutePath)
//    {
//        return;
//    }
//    if (filePath.count(".event") == 0)
//    {
//        filePath +=".event";
//    }
    QFile t_file(absolutePath);
    if (!t_file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&t_file);
    for(unsigned int i = 0; i < (*m_VswitcherInfo).size(); ++i)
    {
            QTextStream out(&t_file);
            for(int j = 0; j < (*m_VswitcherInfo)[i].LNodes.size(); ++j)
            {
                SNodeStructInfo *nodeInfo = &( (*m_VswitcherInfo)[i].LNodes[j] );
                out<<nodeInfo->name<<endl
                     <<nodeInfo->interfaceName<<endl;
            }
    }
}

void CEventSchedulDialog::on_cancelButton_clicked()
{
    close();
}

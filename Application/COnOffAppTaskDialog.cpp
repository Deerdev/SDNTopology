#include "COnOffAppTaskDialog.h"
#include "ui_COnOffAppTaskDialog.h"
#include <QComboBox>

COnOffAppTaskDialog::COnOffAppTaskDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::COnOffAppTaskDialog)
{
    ui->setupUi(this);

    m_close = true;
    m_selectMany = false;
    m_selectAll = false;
    m_preDesNode = -1;

    ui->addTaskButton->setEnabled(false);

    initSelectTableWidget();
}

COnOffAppTaskDialog::~COnOffAppTaskDialog()
{
    delete ui;
}

QString COnOffAppTaskDialog::getPayload()
{
    return ui->payloadSpinBox->text();
}

QString COnOffAppTaskDialog::getClientStartTime()
{
    return ui->clientStartTimeSpinBox->text();
}

QString COnOffAppTaskDialog::getClientEndTime()
{
    return ui->clientEndTimeSpinBox->text();
}

QString COnOffAppTaskDialog::getSeverStartTime()
{
    return ui->severStartTimeSpinBox->text();
}

QString COnOffAppTaskDialog::getSeverEndTime()
{
    return ui->severEndTimeSpinBox->text();
}
QString COnOffAppTaskDialog::getDataRate()
{
    return ui->dataRateSpinBox->text();
}
QString COnOffAppTaskDialog::getPacketSize()
{
    return ui->packetsizeSpinBox->text();
}
void COnOffAppTaskDialog::init(QList<QGraphicsItem*>& _switcherInfo)
{
    m_switcherInfo = _switcherInfo;

    for (int i = 0; i < m_switcherInfo.size() ; i++)
    {
        CSwitcher* t_switch = dynamic_cast<CSwitcher*>(m_switcherInfo[i]);

        if (t_switch != NULL)
        {
            CSwitcherInfo t_switchInfo = t_switch->getSwitcherInfo();
            ui->srcDeviceComboBox->addItem(t_switchInfo.name);
            ui->desDeviceComboBox->addItem(t_switchInfo.name);

            vector<QString> tmpVec(t_switchInfo.LNodes.size());
            for (int j = 0; j < t_switchInfo.LNodes.size(); j ++)
            {
                tmpVec[j] = t_switchInfo.LNodes[j].IP;
            }
            m_IPVec.push_back(tmpVec);
            m_nodeNameVec.push_back(t_switchInfo.name);
        }
    }

    ui->srcDeviceComboBox->setCurrentIndex(-1);
    ui->desDeviceComboBox->setCurrentIndex(-1);
}

void COnOffAppTaskDialog::on_okButton_clicked()
{
    if(!validParameter())
    {
        return;
    }

    if(!m_selectAll && !m_selectMany)
    {
        TaskStruct task;
        task.srcDevice = ui->srcDeviceComboBox->currentText();
        task.srcIP = ui->srcIPComboBox->currentText();
        m_tasks.push_back(task);
    }
    else if(m_selectAll)
    {
        int rowCount = ui->selectTableWidget->rowCount();
        m_tasks.resize(rowCount);
        for ( int i = 0; i < rowCount; ++i )
        {
            TaskStruct task;
            task.srcDevice = ui->selectTableWidget->item(i,SrcDevice)->text();
            QWidget *srcIPWidget = ui->selectTableWidget->cellWidget(i,SrcIP);
            QComboBox* srcIPComboBox = dynamic_cast<QComboBox*>(srcIPWidget);
            task.srcIP = srcIPComboBox->currentText();
            m_tasks[i] = task;
        }
    }
    else
    {
        int rowCount = ui->selectTableWidget->rowCount();
        m_tasks.resize(rowCount);
        for ( int i = 0; i < rowCount; ++i )
        {
            TaskStruct task;
            task.srcDevice = ui->selectTableWidget->item(i,SrcDevice)->text();
            task.srcIP = ui->selectTableWidget->item(i,SrcIP)->text();

            m_tasks[i] = task;
        }
    }

    m_task = "Packet Size: " + ui->payloadSpinBox->text() + "Bytes\n" +
            "OnTime Interval: " + ui->OnTimeSpinBox->text() + "s\n" +
            "OffTime Interval: " + ui->OffTimeSpinBox->text() + "s\n" +
            "Client Start Time: " + ui->clientStartTimeSpinBox->text() + "s\n"+
            "Client End Time: " + ui->clientEndTimeSpinBox->text() + "s\n" +
            "Sever Start Time: " + ui->severStartTimeSpinBox->text() + "s\n"+
            "Sever End Time: " + ui->severEndTimeSpinBox->text() + "s";

    m_close = false;
    accept();
}

void COnOffAppTaskDialog::on_cancelButton_clicked()
{
    close();
}

void COnOffAppTaskDialog::on_srcDeviceComboBox_activated(int _index)
{
    ui->srcIPComboBox->clear();
    for (size_t i = 0; i < m_IPVec[_index].size(); ++i )
    {
        ui->srcIPComboBox->addItem(m_IPVec[_index][i]);
    }
}

void COnOffAppTaskDialog::on_desDeviceComboBox_activated(int _index)
{
    int rowCount = ui->selectTableWidget->rowCount();
    if(m_selectAll || m_selectMany)
    {        
        for ( int i = 0; i < rowCount; ++i )
        {
            ui->selectTableWidget->setItem(i, DesDevice, new QTableWidgetItem(m_nodeNameVec[_index]));
            ui->selectTableWidget->setItem(i, DesIP, new QTableWidgetItem(m_IPVec[_index][0]));
        }
    }

    if(m_selectAll)
    {
        for ( int i = 0; i < rowCount; ++i )
        {
            if(ui->selectTableWidget->item(i,SrcDevice)->text() == ui->desDeviceComboBox->currentText())
            {
                ui->selectTableWidget->removeRow(i);

                if(m_preDesNode != -1)
                {
                    rowCount = rowCount-1;
                    ui->selectTableWidget->insertRow(rowCount);
                    ui->selectTableWidget->setItem(rowCount, SrcDevice, new QTableWidgetItem(m_nodeNameVec[m_preDesNode]));
                    ui->selectTableWidget->setItem(rowCount, DesDevice, new QTableWidgetItem(m_nodeNameVec[_index]));
                    ui->selectTableWidget->setItem(rowCount, DesIP, new QTableWidgetItem(m_IPVec[_index][0]));

                    QComboBox* combo = new QComboBox;
                    combo->setCurrentIndex(0);
                    ui->selectTableWidget->setCellWidget(rowCount,SrcIP,combo);
                    for ( size_t j = 0; j < m_IPVec[m_preDesNode].size(); ++j )
                    {
                        combo->addItem(m_IPVec[m_preDesNode][j]);
                    }
                }
                m_preDesNode = _index;

                break;
            }
        }
    }

    ui->desIPComboBox->clear();
    for (size_t i = 0; i < m_IPVec[_index].size(); ++i )
    {
        ui->desIPComboBox->addItem(m_IPVec[_index][i]);
    }
}

bool COnOffAppTaskDialog::getClose()
{
    return m_close;
}

QString COnOffAppTaskDialog::getOnInterval()
{
    return ui->OnTimeSpinBox->text();
}

QString COnOffAppTaskDialog::getOffInterval()
{
    return ui->OffTimeSpinBox->text();
}

void COnOffAppTaskDialog::on_selectManyCheckBox_stateChanged(int _state)
{
    ui->selectTableWidget->clearContents();
    int rowCount = ui->selectTableWidget->rowCount();
    for(int i = 0; i < rowCount;++i)
    {
        ui->selectTableWidget->removeRow(0);
    }

    if(_state == Qt::Checked)
    {
        m_selectMany = true;
        m_selectAll = false;
        ui->selectAllCheckBox->setCheckState(Qt::Unchecked);
        ui->addTaskButton->setEnabled(true);

    }
    else if(_state == Qt::Unchecked)
    {
        m_selectMany = false;
        ui->addTaskButton->setEnabled(false);
    }
}

void COnOffAppTaskDialog::on_selectAllCheckBox_stateChanged(int _state)
{
    ui->selectTableWidget->clearContents();
    int rowCount = ui->selectTableWidget->rowCount();
    for(int i = 0; i < rowCount;++i)
    {
        ui->selectTableWidget->removeRow(0);
    }

    if(_state == Qt::Checked)
    {
        m_selectMany = false;
        m_selectAll = true;
        ui->selectManyCheckBox->setCheckState(Qt::Unchecked);
        ui->addTaskButton->setEnabled(false);

        int nodeNum = m_IPVec.size();
        for ( int i = 0; i < nodeNum; ++i )
        {
            int row = ui->selectTableWidget->rowCount();
            ui->selectTableWidget->insertRow(row);
            ui->selectTableWidget->setItem(row, SrcDevice, new QTableWidgetItem(m_nodeNameVec[i]));
            ui->selectTableWidget->setItem(row, DesDevice, new QTableWidgetItem(ui->desDeviceComboBox->currentText()));
            ui->selectTableWidget->setItem(row, DesIP, new QTableWidgetItem(ui->desIPComboBox->currentText()));

            QComboBox* combo = new QComboBox;
            combo->setCurrentIndex(0);
            ui->selectTableWidget->setCellWidget(row,SrcIP,combo);
            for ( size_t j = 0; j < m_IPVec[i].size(); ++j )
            {
                combo->addItem(m_IPVec[i][j]);
            }
        }
    }
    else if(_state == Qt::Unchecked)
    {
        m_selectAll = false;
        ui->addTaskButton->setEnabled(false);

    }
}

void COnOffAppTaskDialog::initSelectTableWidget()
{
    ui->selectTableWidget->setColumnCount(4);
    QStringList header;
    header<<tr("源端")<<tr("源端IP")<<tr("终端")<<tr("终端IP");
    ui->selectTableWidget->setHorizontalHeaderLabels(header);
    ui->selectTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->selectTableWidget->setColumnWidth(SrcDevice, 100);
    ui->selectTableWidget->setColumnWidth(SrcIP, 120);
    ui->selectTableWidget->setColumnWidth(DesDevice, 100);
    ui->selectTableWidget->setColumnWidth(DesIP, 120);
}

int COnOffAppTaskDialog::getSelectState()
{
    if(!m_selectAll && !m_selectMany)
        return 0;
    else if(m_selectMany)
        return 1;
    else
        return 2;
}

void COnOffAppTaskDialog::on_addTaskButton_clicked()
{
    if(!validParameter())
    {
        return;
    }

    int row = ui->selectTableWidget->rowCount();

    for ( int i = 0; i < row; ++i )
    {
        if(ui->selectTableWidget->item(i,0)->text() == ui->srcDeviceComboBox->currentText())
        {
            QMessageBox::information(this, tr("提示"), tr("已配置过相同任务!"), QMessageBox::Ok);
            return ;
        }
    }

    ui->selectTableWidget->insertRow(row);
    ui->selectTableWidget->setItem(row, SrcDevice, new QTableWidgetItem(ui->srcDeviceComboBox->currentText()));
    ui->selectTableWidget->setItem(row, SrcIP, new QTableWidgetItem(ui->srcIPComboBox->currentText()));
    ui->selectTableWidget->setItem(row, DesDevice, new QTableWidgetItem(ui->desDeviceComboBox->currentText()));
    ui->selectTableWidget->setItem(row, DesIP, new QTableWidgetItem(ui->desIPComboBox->currentText()));
}

void COnOffAppTaskDialog::on_desIPComboBox_activated(int _index)
{
    if(m_selectAll)
    {
        for ( size_t i = 0; i < m_nodeNameVec.size(); ++i )
        {
            ui->selectTableWidget->setItem(i, 3, new QTableWidgetItem(m_IPVec[ui->desDeviceComboBox->currentIndex()][_index]));
        }
    }

    if(m_selectMany)
    {
        int rowCount = ui->selectTableWidget->rowCount();
        for ( int i = 0; i < rowCount; ++i )
        {
            ui->selectTableWidget->setItem(i, 3, new QTableWidgetItem(m_IPVec[ui->desDeviceComboBox->currentIndex()][_index]));
        }
    }
}

vector<TaskStruct> COnOffAppTaskDialog::getTasks()
{
    return m_tasks;
}

bool COnOffAppTaskDialog::validParameter()
{
    QString srcDevice = ui->srcDeviceComboBox->currentText();
    QString desDevice = ui->desDeviceComboBox->currentText();

    if (srcDevice == desDevice)
    {
        QMessageBox::information(this, tr("提示"), tr("请选择不同设备!"), QMessageBox::Ok);
        return false;
    }

    if (ui->srcIPComboBox->currentText().isEmpty() && !m_selectAll )
    {
        QMessageBox::information(this, tr("提示"), tr("请选择客户端IP!"), QMessageBox::Ok);
        return false;
    }

    if (ui->desIPComboBox->currentText().isEmpty() )
    {
        QMessageBox::information(this, tr("提示"), tr("请选择服务器IP!"), QMessageBox::Ok);
        return false;
    }

    if (ui->clientStartTimeSpinBox->text().toFloat() > ui->clientEndTimeSpinBox->text().toFloat())
    {
        QMessageBox::information(this, tr("提示"), tr("客户端起始时间不能超过终止时间!"), QMessageBox::Ok);
        return false;
    }

    if (ui->severStartTimeSpinBox->text().toFloat() > ui->severEndTimeSpinBox->text().toFloat())
    {
        QMessageBox::information(this, tr("提示"), tr("服务器起始时间不能超过终止时间!"), QMessageBox::Ok);
        return false;
    }

    if (ui->clientStartTimeSpinBox->text().toFloat() == ui->clientEndTimeSpinBox->text().toFloat())
    {
        QMessageBox::information(this, tr("提示"), tr("客户端起始时间不能相等!"), QMessageBox::Ok);
        return false;
    }

    if (ui->severStartTimeSpinBox->text().toFloat() == ui->severEndTimeSpinBox->text().toFloat())
    {
        QMessageBox::information(this, tr("提示"), tr("服务器起始时间不能相等!"), QMessageBox::Ok);
        return false;
    }

    if (ui->OnTimeSpinBox->text().toFloat() == 0.0 && ui->OffTimeSpinBox->text().toFloat() == 0.0)
    {
        QMessageBox::information(this, tr("提示"), tr("On-Off持续时间不能同时为0!"), QMessageBox::Ok);
        return false;
    }
    return true;
}

QString COnOffAppTaskDialog::getTask()
{
    return m_task;
}

QString COnOffAppTaskDialog::getDesDevice()
{
    return ui->desDeviceComboBox->currentText();
}

QString COnOffAppTaskDialog::getDesIP()
{
    return ui->desIPComboBox->currentText();
}

#include "CBulkAppTaskDialog.h"
#include "ui_CBulkAppTaskDialog.h"
#include <QComboBox>

CBulkAppTaskDialog::CBulkAppTaskDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CBulkAppTaskDialog)
{
    ui->setupUi(this);

    m_close = true;
    m_selectMany = false;
    m_selectAll = false;
    m_preDesNode = -1;

    ui->addTaskButton->setEnabled(false);

    initSelectTableWidget();
}

CBulkAppTaskDialog::~CBulkAppTaskDialog()
{
    delete ui;
}

QString CBulkAppTaskDialog::getPayload()
{
    return ui->payloadSpinBox->text();
}

QString CBulkAppTaskDialog::getClientStartTime()
{
    return ui->clientStartTimeSpinBox->text();
}

QString CBulkAppTaskDialog::getClientEndTime()
{
    return ui->clientEndTimeSpinBox->text();
}

QString CBulkAppTaskDialog::getSeverStartTime()
{
    return ui->severStartTimeSpinBox->text();
}

QString CBulkAppTaskDialog::getSeverEndTime()
{
    return ui->severEndTimeSpinBox->text();
}

QString CBulkAppTaskDialog::getPacketSize()
{
    return ui->packetSizeSpinBox->text();
}

void CBulkAppTaskDialog::init(QList<QGraphicsItem*>& _switcherInfo)
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

void CBulkAppTaskDialog::on_okButton_clicked()
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
             "Client Start Time: " + ui->clientStartTimeSpinBox->text() + "s\n"+
             "Client End Time: " + ui->clientEndTimeSpinBox->text() + "s\n" +
             "Sever Start Time: " + ui->severStartTimeSpinBox->text() + "s\n"+
             "Sever End Time: " + ui->severEndTimeSpinBox->text() + "s";

    m_close = false;
    accept();
}

void CBulkAppTaskDialog::on_cancelButton_clicked()
{
    close();
}

void CBulkAppTaskDialog::on_srcDeviceComboBox_activated(int _index)
{
    ui->srcIPComboBox->clear();
    for (size_t i = 0; i < m_IPVec[_index].size(); ++i )
    {
        ui->srcIPComboBox->addItem(m_IPVec[_index][i]);
    }
}

void CBulkAppTaskDialog::on_desDeviceComboBox_activated(int _index)
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

bool CBulkAppTaskDialog::getClose()
{
    return m_close;
}

void CBulkAppTaskDialog::on_selectManyCheckBox_stateChanged(int _state)
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

void CBulkAppTaskDialog::on_selectAllCheckBox_stateChanged(int _state)
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

void CBulkAppTaskDialog::initSelectTableWidget()
{
    ui->selectTableWidget->setColumnCount(4);
    QStringList header;
    header<<tr("Դ��")<<tr("Դ��IP")<<tr("�ն�")<<tr("�ն�IP");
    ui->selectTableWidget->setHorizontalHeaderLabels(header);
    ui->selectTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭
    ui->selectTableWidget->setColumnWidth(SrcDevice, 100);
    ui->selectTableWidget->setColumnWidth(SrcIP, 120);
    ui->selectTableWidget->setColumnWidth(DesDevice, 100);
    ui->selectTableWidget->setColumnWidth(DesIP, 120);
}

int CBulkAppTaskDialog::getSelectState()
{
    if(!m_selectAll && !m_selectMany)
        return 0;
    else if(m_selectMany)
        return 1;
    else
        return 2;
}

void CBulkAppTaskDialog::on_addTaskButton_clicked()
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
            QMessageBox::information(this, tr("��ʾ"), tr("�����ù���ͬ����!"), QMessageBox::Ok);
            return ;
        }
    }

    ui->selectTableWidget->insertRow(row);
    ui->selectTableWidget->setItem(row, SrcDevice, new QTableWidgetItem(ui->srcDeviceComboBox->currentText()));
    ui->selectTableWidget->setItem(row, SrcIP, new QTableWidgetItem(ui->srcIPComboBox->currentText()));
    ui->selectTableWidget->setItem(row, DesDevice, new QTableWidgetItem(ui->desDeviceComboBox->currentText()));
    ui->selectTableWidget->setItem(row, DesIP, new QTableWidgetItem(ui->desIPComboBox->currentText()));
}

void CBulkAppTaskDialog::on_desIPComboBox_activated(int _index)
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

vector<TaskStruct> CBulkAppTaskDialog::getTasks()
{
    return m_tasks;
}

bool CBulkAppTaskDialog::validParameter()
{
    QString srcDevice = ui->srcDeviceComboBox->currentText();
    QString desDevice = ui->desDeviceComboBox->currentText();

    if (srcDevice == desDevice)
    {
        QMessageBox::information(this, tr("��ʾ"), tr("��ѡ��ͬ�豸!"), QMessageBox::Ok);
        return false;
    }

    if (ui->srcIPComboBox->currentText().isEmpty() && !m_selectAll )
    {
        QMessageBox::information(this, tr("��ʾ"), tr("��ѡ��ͻ���IP!"), QMessageBox::Ok);
        return false;
    }

    if (ui->desIPComboBox->currentText().isEmpty() )
    {
        QMessageBox::information(this, tr("��ʾ"), tr("��ѡ�������IP!"), QMessageBox::Ok);
        return false;
    }

    if (ui->clientStartTimeSpinBox->text().toFloat() > ui->clientEndTimeSpinBox->text().toFloat())
    {
        QMessageBox::information(this, tr("��ʾ"), tr("�ͻ�����ʼʱ�䲻�ܳ�����ֹʱ��!"), QMessageBox::Ok);
        return false;
    }

    if (ui->severStartTimeSpinBox->text().toFloat() > ui->severEndTimeSpinBox->text().toFloat())
    {
        QMessageBox::information(this, tr("��ʾ"), tr("��������ʼʱ�䲻�ܳ�����ֹʱ��!"), QMessageBox::Ok);
        return false;
    }

    if (ui->clientStartTimeSpinBox->text().toFloat() == ui->clientEndTimeSpinBox->text().toFloat())
    {
        QMessageBox::information(this, tr("��ʾ"), tr("�ͻ�����ʼʱ�䲻�����!"), QMessageBox::Ok);
        return false;
    }

    if (ui->severStartTimeSpinBox->text().toFloat() == ui->severEndTimeSpinBox->text().toFloat())
    {
        QMessageBox::information(this, tr("��ʾ"), tr("��������ʼʱ�䲻�����!"), QMessageBox::Ok);
        return false;
    }

    return true;
}

QString CBulkAppTaskDialog::getTask()
{
    return m_task;
}

QString CBulkAppTaskDialog::getDesDevice()
{
    return ui->desDeviceComboBox->currentText();
}

QString CBulkAppTaskDialog::getDesIP()
{
    return ui->desIPComboBox->currentText();
}

#include "CTaskConfigDialog.h"
#include "ui_CTaskConfigDialog.h"
#include <QFile>
#include <QTextStream>

CTaskConfigDialog::CTaskConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CTaskConfigDialog)
{
    ui->setupUi(this);

    init();
}

CTaskConfigDialog::~CTaskConfigDialog()
{
    delete ui;
    m_task.clear();
    m_taskStructVec.clear();
}

void CTaskConfigDialog::init()
{
    //add app items
    ui->addTaskComboBox->addItem(tr("带应答UDP业务流量"));
    ui->addTaskComboBox->addItem(tr("普通TCP业务流量"));
    ui->addTaskComboBox->addItem(tr("洪泛流量"));
    ui->addTaskComboBox->addItem(tr("端到端时延探测任务"));

    //init task table
    ui->taskTable->setColumnCount(5);
    QStringList header;
    header<<tr("源端")<<tr("源端IP")<<tr("终端")<<tr("终端IP")<<tr("任务");
    ui->taskTable->setHorizontalHeaderLabels(header);
    ui->taskTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->taskTable->setColumnWidth(4, 200);

    connect(ui->addTaskComboBox, SIGNAL(activated(int)), this, SLOT(addTask(int)));
}

void CTaskConfigDialog::on_saveButton_clicked()
{
    //get NS3 path
    QFile infile("Config");
    if(!infile.exists())
    {
        QMessageBox::critical(this, tr("Error"), "Can not find Config file！",QMessageBox::Ok);
        return;
    }
    if(!infile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, tr("Error"), "Can not open Config file！",QMessageBox::Ok);
        return;
    }

    QTextStream fileStream(&infile);
    m_NS3Path = fileStream.readAll();
    m_NS3Path.replace("\n","");
    infile.close();

    QFile outFile(m_NS3Path + "/data/Task");
    if(outFile.exists())
    {
        outFile.remove();
    }
    if(!outFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QMessageBox::critical(this, tr("Error"), "Can not write to task file！",QMessageBox::Ok);
        return;
    }
    QTextStream outFileStream(&outFile);

    //format:
    //类型\t个数\t源端\t源IP\t终端\t终端IP\t任务
    /*save task*/
    int taskNum = m_task.size();
    for ( int i = 0; i < taskNum; ++i )
    {
        if(m_task[i][0] == "0")//Echo Application
        {
            if(m_task[i][1] == "0")//single
            {
                outFileStream<<m_task[i][0]<<"\t1\t"
                             <<m_taskStructVec[i][0].srcDevice<<"\t"
                             <<m_taskStructVec[i][0].srcIP<<"\t"
                             <<ui->taskTable->item(i,2)->text()<<"\t"
                             <<ui->taskTable->item(i,3)->text()<<"\t";

                outFileStream<<m_task[i][2]<<"\t"
                             <<m_task[i][3]<<"\t"
                             <<m_task[i][4]<<"\t"
                             <<m_task[i][5]<<"\t"
                             <<m_task[i][6]<<"\t"
                             <<m_task[i][7]<<"\t"
                             <<m_task[i][8]<<endl;
            }
            else //many & all
            {
                QString desDevice = ui->taskTable->item(i,2)->text();
                QString desIP = ui->taskTable->item(i,3)->text();
                for ( size_t j = 0; j < m_taskStructVec[i].size(); ++j )
                {
                    outFileStream<<m_task[i][0]<<"\t"<<m_taskStructVec[i].size()<<"\t"
                                 <<m_taskStructVec[i][j].srcDevice<<"\t"
                                 <<m_taskStructVec[i][j].srcIP<<"\t"
                                 <<desDevice<<"\t"
                                 <<desIP<<"\t";

                    outFileStream<<m_task[i][2]<<"\t"
                                 <<m_task[i][3]<<"\t"
                                 <<m_task[i][4]<<"\t"
                                 <<m_task[i][5]<<"\t"
                                 <<m_task[i][6]<<"\t"
                                 <<m_task[i][7]<<"\t"
                                 <<m_task[i][8]<<endl;
                }
            }
        }
        else if(m_task[i][0] == "1")//On-Off Application
        {
            if(m_task[i][1] == "0")//single
            {
                outFileStream<<m_task[i][0]<<"\t1\t"
                             <<m_taskStructVec[i][0].srcDevice<<"\t"
                             <<m_taskStructVec[i][0].srcIP<<"\t"
                             <<ui->taskTable->item(i,2)->text()<<"\t"
                             <<ui->taskTable->item(i,3)->text()<<"\t";

                outFileStream<<m_task[i][2]<<"\t"
                              <<m_task[i][3]<<"\t"
                              <<m_task[i][4]<<"\t"
                              <<m_task[i][5]<<"\t"
                              <<m_task[i][6]<<"\t"
                              <<m_task[i][7]<<"\t"
                              <<m_task[i][8]<<"\t"
                              <<m_task[i][9]<<"\t"
                              <<m_task[i][10]<<endl;

            }
            else //many & all
            {
                QString desDevice = ui->taskTable->item(i,2)->text();
                QString desIP = ui->taskTable->item(i,3)->text();
                for ( size_t j = 0; j < m_taskStructVec[i].size(); ++j )
                {
                    outFileStream<<m_task[i][0]<<"\t"<<m_taskStructVec[i].size()<<"\t"
                                 <<m_taskStructVec[i][j].srcDevice<<"\t"
                                 <<m_taskStructVec[i][j].srcIP<<"\t"
                                 <<desDevice<<"\t"
                                 <<desIP<<"\t";

                    outFileStream<<m_task[i][2]<<"\t"
                                 <<m_task[i][3]<<"\t"
                                 <<m_task[i][4]<<"\t"
                                 <<m_task[i][5]<<"\t"
                                 <<m_task[i][6]<<"\t"
                                 <<m_task[i][7]<<"\t"
                                 <<m_task[i][8]<<"\t"
                                 <<m_task[i][9]<<"\t"
                                 <<m_task[i][10]<<endl;
                }
            }
        }
        else if(m_task[i][0] == "2")//BulkSend Application
        {
            if(m_task[i][1] == "0")//single
            {
                outFileStream<<m_task[i][0]<<"\t1\t"
                             <<m_taskStructVec[i][0].srcDevice<<"\t"
                             <<m_taskStructVec[i][0].srcIP<<"\t"
                             <<ui->taskTable->item(i,2)->text()<<"\t"
                             <<ui->taskTable->item(i,3)->text()<<"\t";

                outFileStream<<m_task[i][2]<<"\t"
                              <<m_task[i][3]<<"\t"
                              <<m_task[i][4]<<"\t"
                              <<m_task[i][5]<<"\t"
                              <<m_task[i][6]<<"\t"
                              <<m_task[i][7]<<endl;

            }
            else //many & all
            {
                QString desDevice = ui->taskTable->item(i,2)->text();
                QString desIP = ui->taskTable->item(i,3)->text();
                for ( size_t j = 0; j < m_taskStructVec[i].size(); ++j )
                {
                    outFileStream<<m_task[i][0]<<"\t"<<m_taskStructVec[i].size()<<"\t"
                                 <<m_taskStructVec[i][j].srcDevice<<"\t"
                                 <<m_taskStructVec[i][j].srcIP<<"\t"
                                 <<desDevice<<"\t"
                                 <<desIP<<"\t";

                    outFileStream<<m_task[i][2]<<"\t"
                                 <<m_task[i][3]<<"\t"
                                 <<m_task[i][4]<<"\t"
                                 <<m_task[i][5]<<"\t"
                                 <<m_task[i][6]<<"\t"
                                 <<m_task[i][7]<<endl;
                }
            }
        }
    }
    outFile.close();

    accept();
}

void CTaskConfigDialog::addTask(int _index)
{
    if (EchoApplication == _index || DelayApplication == _index)
    {
        addEchoTask(_index);
    }
    else if (OnOffApplication == _index)
    {
        addOnOffTask();
    }
    else if(BulkSendApplication == _index)
    {
        addBulkTask();
    }
}

void CTaskConfigDialog::setSwichterInfo(QList<QGraphicsItem*> _switcherInfo)
{
    m_switcherInfo = _switcherInfo;
}

void CTaskConfigDialog::on_cancelButton_clicked()
{
    close();
}

void CTaskConfigDialog::addEchoTask(int index)
{
    QString srcDevice = "";
    QString desDevice = "";
    QString srcIP = "";
    QString desIP = "";
    QString task = "";

    CEchoAppTaskDialog echoApp;
    echoApp.sendTaskFlag(index);
    echoApp.init(m_switcherInfo);
    echoApp.exec();

    if (echoApp.getClose())
    {
        return;
    }

    m_taskStructVec.push_back(echoApp.getTasks());
    size_t num = m_taskStructVec.size()-1;


    desDevice = echoApp.getDesDevice();
    desIP = echoApp.getDesIP();
    task = echoApp.getTask();

    int row = ui->taskTable->rowCount();
    int selectState = echoApp.getSelectState();
    ui->taskTable->insertRow(row);
    if(selectState == 2)
    {
        ui->taskTable->setItem(row, 0, new QTableWidgetItem(tr("所有源端")));
        ui->taskTable->setItem(row, 1, new QTableWidgetItem(""));
    }
    else
    {
        for (size_t i = 0; i < m_taskStructVec[num].size()-1; ++i )
        {
            srcDevice += m_taskStructVec[num][i].srcDevice + "\n";
            srcIP += m_taskStructVec[num][i].srcIP + "\n";
        }
        srcDevice += m_taskStructVec[num][m_taskStructVec[num].size()-1].srcDevice ;
        srcIP += m_taskStructVec[num][m_taskStructVec[num].size()-1].srcIP;

        ui->taskTable->setItem(row, 0, new QTableWidgetItem(srcDevice));
        ui->taskTable->setItem(row, 1, new QTableWidgetItem(srcIP));
    }

    ui->taskTable->setItem(row, 2, new QTableWidgetItem(desDevice));
    ui->taskTable->setItem(row, 3, new QTableWidgetItem(desIP));
    ui->taskTable->setItem(row, 4, new QTableWidgetItem(task));
    ui->taskTable->resizeRowToContents(row);

    vector<QString> echoTask;
    echoTask.push_back("0");//标记类型
    echoTask.push_back(QString::number(selectState));
    echoTask.push_back(echoApp.getMaxPackets());
    echoTask.push_back(echoApp.getInterval());
    echoTask.push_back(echoApp.getPayload());
    echoTask.push_back(echoApp.getClientStartTime());
    echoTask.push_back(echoApp.getClientEndTime());
    echoTask.push_back(echoApp.getSeverStartTime());
    echoTask.push_back(echoApp.getSeverEndTime());
    m_task.push_back(echoTask);
}

void CTaskConfigDialog::addOnOffTask()
{
    QString srcDevice = "";
    QString desDevice = "";
    QString srcIP = "";
    QString desIP = "";
    QString task = "";

    COnOffAppTaskDialog onOffApp;
    onOffApp.init(m_switcherInfo);
    onOffApp.exec();

    if (onOffApp.getClose())
    {
        return;
    }

    m_taskStructVec.push_back(onOffApp.getTasks());
    size_t num = m_taskStructVec.size() - 1;
    desDevice = onOffApp.getDesDevice();
    desIP = onOffApp.getDesIP();
    task = onOffApp.getTask();

    int row = ui->taskTable->rowCount();
    int selectState = onOffApp.getSelectState();
    ui->taskTable->insertRow(row);
    if(selectState == 2)
    {
        ui->taskTable->setItem(row, 0, new QTableWidgetItem(tr("所有源端")));
        ui->taskTable->setItem(row, 1, new QTableWidgetItem(""));
    }
    else
    {
        for (size_t i = 0; i < m_taskStructVec[num].size()-1; ++i )
        {
            srcDevice += m_taskStructVec[num][i].srcDevice + "\n";
            srcIP += m_taskStructVec[num][i].srcIP + "\n";
        }
        srcDevice += m_taskStructVec[num][m_taskStructVec[num].size()-1].srcDevice ;
        srcIP += m_taskStructVec[num][m_taskStructVec[num].size()-1].srcIP;

        ui->taskTable->setItem(row, 0, new QTableWidgetItem(srcDevice));
        ui->taskTable->setItem(row, 1, new QTableWidgetItem(srcIP));
    }

    ui->taskTable->setItem(row, 2, new QTableWidgetItem(desDevice));
    ui->taskTable->setItem(row, 3, new QTableWidgetItem(desIP));
    ui->taskTable->setItem(row, 4, new QTableWidgetItem(task));
    ui->taskTable->resizeRowToContents(row);

    vector<QString> onOffTask;
    onOffTask.push_back("1");//标记类型
    onOffTask.push_back(QString::number(selectState));
    onOffTask.push_back(onOffApp.getOnInterval());
    onOffTask.push_back(onOffApp.getOffInterval());
    onOffTask.push_back(onOffApp.getPayload());
    onOffTask.push_back(onOffApp.getClientStartTime());
    onOffTask.push_back(onOffApp.getClientEndTime());
    onOffTask.push_back(onOffApp.getSeverStartTime());
    onOffTask.push_back(onOffApp.getSeverEndTime());
    onOffTask.push_back(onOffApp.getDataRate());
    onOffTask.push_back(onOffApp.getPacketSize());
    m_task.push_back(onOffTask);
}

void CTaskConfigDialog::addBulkTask()
{
    QString srcDevice = "";
    QString desDevice = "";
    QString srcIP = "";
    QString desIP = "";
    QString task = "";

    CBulkAppTaskDialog bulkApp;
    bulkApp.init(m_switcherInfo);
    bulkApp.exec();

    if (bulkApp.getClose())
    {
        return;
    }

    m_taskStructVec.push_back(bulkApp.getTasks());
    size_t num = m_taskStructVec.size() - 1;
    desDevice = bulkApp.getDesDevice();
    desIP = bulkApp.getDesIP();
    task = bulkApp.getTask();

    int row = ui->taskTable->rowCount();
    int selectState = bulkApp.getSelectState();
    ui->taskTable->insertRow(row);
    if(selectState == 2)
    {
        ui->taskTable->setItem(row, 0, new QTableWidgetItem(tr("所有源端")));
        ui->taskTable->setItem(row, 1, new QTableWidgetItem(""));
    }
    else
    {
        for (size_t i = 0; i < m_taskStructVec[num].size()-1; ++i )
        {
            srcDevice += m_taskStructVec[num][i].srcDevice + "\n";
            srcIP += m_taskStructVec[num][i].srcIP + "\n";
        }
        srcDevice += m_taskStructVec[num][m_taskStructVec[num].size()-1].srcDevice ;
        srcIP += m_taskStructVec[num][m_taskStructVec[num].size()-1].srcIP;

        ui->taskTable->setItem(row, 0, new QTableWidgetItem(srcDevice));
        ui->taskTable->setItem(row, 1, new QTableWidgetItem(srcIP));
    }

    ui->taskTable->setItem(row, 2, new QTableWidgetItem(desDevice));
    ui->taskTable->setItem(row, 3, new QTableWidgetItem(desIP));
    ui->taskTable->setItem(row, 4, new QTableWidgetItem(task));
    ui->taskTable->resizeRowToContents(row);

    vector<QString> bulkTask;
    bulkTask.push_back("2");//标记类型
    bulkTask.push_back(QString::number(selectState));
    bulkTask.push_back(bulkApp.getPayload());
    bulkTask.push_back(bulkApp.getClientStartTime());
    bulkTask.push_back(bulkApp.getClientEndTime());
    bulkTask.push_back(bulkApp.getSeverStartTime());
    bulkTask.push_back(bulkApp.getSeverEndTime());
    bulkTask.push_back(bulkApp.getPacketSize());
    m_task.push_back(bulkTask);
}

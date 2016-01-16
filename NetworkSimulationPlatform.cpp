#include "NetworkSimulationPlatform.h"
#include <QSizePolicy>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include "CPartitionNumDialog.h"

/*************************************************
  Function: NetworkSimulationPlatform
  Description: ���캯��
  Parameter: ��
  Return: ��
*************************************************/
NetworkSimulationPlatform::NetworkSimulationPlatform(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	setupUi(this);
	//��ʼ������
    init();
}

/*************************************************
  Function: ~NetworkSimulationPlatform
  Description: ��������
  Parameter: ��
  Return: ��
*************************************************/
NetworkSimulationPlatform::~NetworkSimulationPlatform()
{

}

/*************************************************
  Function: init
  Description: ��ʼ������
  Parameter: ��
  Return: ��
*************************************************/
void NetworkSimulationPlatform::init( void )
{
    m_saveFlag = false;
    m_partitionNum = 1;

    ui_editWidget = new CTopologyWidget();
    setCentralWidget(ui_editWidget);
    ui_editWidget->translate(1000,1000);

    setWindowTitle(tr("SDN��������"));
    setToolTip(tr("SDN��������"));
    setStatusTip(tr("SDN��������"));

	//��������
	createActions();
	//�����˵�
	createMenu();
	//����������
	createToolBar();
	//����¹�����
	createNewTooBar();

	//����ͣ������
	createDockWindow();

    m_currentPath = QDir::currentPath();

    //create dir
    QDir topologyDir;
    if (!topologyDir.exists(m_currentPath + "/Topology"))
    {
        topologyDir.mkdir("Topology");
    }

    m_saveFile = new CWizard;
    connect(m_saveFile, SIGNAL(accepted()), this, SLOT(slotSaveFile()));

    m_saveTopology = new CWizard;
    m_scene = ui_editWidget->getScene();
}

/*************************************************
  Function: createNewTooBar
  Description: �����¹�����
  Parameter: ��
  Return: ��
*************************************************/
void NetworkSimulationPlatform::createNewTooBar( void )
{

}

/*************************************************
  Function: createActions
  Description: ��������
  Parameter: ��
  Return: ��
*************************************************/
void NetworkSimulationPlatform::createActions( void )
{
	//���ø�������
	/*ע�⣺������triggered()������button��clicked()����*/
	/*���ô�������Ŀ����*/
	newNetworkItemAction = new QAction( tr("��������Ŀ"), this );
    newNetworkItemAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/newItem.png"));
	newNetworkItemAction->setShortcut(tr("Ctrl+N"));
	connect( newNetworkItemAction, SIGNAL(triggered()), this, SLOT(createNewNetworkItem()) );

	/*���ô�������Ŀ����*/
	openNetworkItemAction = new QAction( tr("��������Ŀ"), this );
    openNetworkItemAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/open.png"));
    openNetworkItemAction->setShortcut(tr("Ctrl+O"));
	connect( openNetworkItemAction, SIGNAL(triggered()), this, SLOT(openNetworkItem()) );

	/*���ñ��涯��*/
	saveItemAction = new QAction( tr("����"), this );
    saveItemAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/save.png"));
	saveItemAction->setShortcut(tr("Ctrl+S"));
	connect( saveItemAction, SIGNAL(triggered()), this, SLOT(saveItem()));

	/*�����˳�ϵͳ����*/
	exitAction = new QAction( tr("�˳�"), this );
    exitAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/exit.png"));
	exitAction->setShortcut(tr("Ctrl+Q"));
	connect( exitAction, SIGNAL(triggered()), this, SLOT(close()) );

    /*���ÿ�ʼ����*/
    startSimulateAction = new QAction( tr("��ʼ����"), this );
    startSimulateAction->setShortcut(tr("Ctrl+S"));
    connect(startSimulateAction, SIGNAL(triggered()), this, SLOT(startSimulate()));

    /*��ȡSNMP����*/
    importSNMPTopologyAction = new QAction( tr("����SNMP��������"), this);
    connect(importSNMPTopologyAction, SIGNAL(triggered()), this, SLOT(importSNMPTopology()));

    /*���÷Ŵ���*/
	zoomInAction = new QAction( tr("�Ŵ�"), this );
    zoomInAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/zoomIn.png"));
    connect( zoomInAction, SIGNAL(triggered()), ui_editWidget, SLOT(zoomIn()) );

	/*������С����*/
	zoomOutAction = new QAction( tr("��С"), this );
    zoomOutAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/zoomOut.png"));
    connect( zoomOutAction, SIGNAL(triggered()), ui_editWidget, SLOT(zoomOut()) );

	/*���ò����㷨����*/
	layoutAction = new QAction( tr("����"), this );
    layoutAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/layout.png"));
	connect( layoutAction, SIGNAL(triggered()), this, SLOT(layoutAlgorithm()) );

	/*���õ������˶���*/
	exportTopologyAction = new QAction( tr("������������"), this );
    exportTopologyAction->setIcon(QIcon(""));
    connect( exportTopologyAction, SIGNAL(triggered()), this, SLOT(exportTopology()) );

	/*���õ������˶���*/
	importTopologyAction = new QAction( tr("������������"), this );
    importTopologyAction->setIcon(QIcon(""));
    connect( importTopologyAction, SIGNAL(triggered()), this, SLOT(importTopology()) );

    /*��ջ���*/
    clearseceneAction = new QAction(tr("��ջ���"), this);
    clearseceneAction->setIcon(QIcon(""));
    connect(clearseceneAction, SIGNAL(triggered()), this, SLOT(clearScene()));

	/*������·���ö���*/
	flowSetAction = new QAction( tr("��·����"), this );
	flowSetAction->setIcon(QIcon(""));
	connect( flowSetAction, SIGNAL(triggered()), this, SLOT(setFlow()) );
	
	/*�����������ö���*/
	linkSetAction = new QAction( tr("��������"), this );
	linkSetAction->setIcon(QIcon(""));
	connect( linkSetAction, SIGNAL(triggered()), this, SLOT(setLink()) );

    /**/
    configTaskAction = new QAction(tr("��������"), this);
    configTaskAction->setIcon(QIcon(""));
    connect( configTaskAction, SIGNAL(triggered()), this, SLOT(configTask()) );

    eventSchedulAction = new QAction(tr("�¼�����"), this);
    connect(eventSchedulAction, SIGNAL(triggered() ), this, SLOT(eventSchedul() ) );

    /**/
    partitionNumAction = new QAction(tr("���绮������"),this);
    partitionNumAction->setIcon(QIcon(""));
    connect( partitionNumAction, SIGNAL(triggered()), this, SLOT(setPartitonNum()) );

    networkBandAction = new QAction( tr("�������"), this);

    networkCapacityAction = new QAction( tr("����������"), this);
}

/*************************************************
  Function: createNewNetworkItem
  Description: ��������Ŀ�ۺ���
  Parameter: ��
  Return: ��
*************************************************/
void NetworkSimulationPlatform::createNewNetworkItem( void )
{
    //������������Ŀ�Ի���
    m_newFile = new CWizard;
    connect(m_newFile, SIGNAL(accepted()), this, SLOT(slotNewFile()));
    m_newFile->exec();
}

void NetworkSimulationPlatform::slotNewFile(void)
{

    m_projectName = m_newFile->getFilename();
    QFile file(m_projectName+".txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    ui_editWidget->clearScene();
}

/*************************************************
  Function: openNetworkItem
  Description: ����Ŀ�ۺ���
  Parameter: ��
  Return: ��
*************************************************/
void NetworkSimulationPlatform::openNetworkItem( void )
{
	//��������Ŀ�Ի���

}

/*************************************************
  Function: saveItem
  Description: ������Ŀ�ۺ���
  Parameter: ��
  Return: ��
*************************************************/
void NetworkSimulationPlatform::saveItem(void)
{
    m_saveFlag = true;
	//��ȡ��������
    if(m_projectName.isEmpty())
    {
        m_saveFile->exec();
        slotSaveFile();
    }
    else
        slotSaveFile();
	//����
}

void NetworkSimulationPlatform::slotSaveFile(void)
{
    m_saveTopologyFileName = m_saveFile->getFilename();
    m_saveTopologyFilePath = m_saveFile->GetRoute();

    ui_editWidget->OutputPosition(m_saveFile->GetFilePath()+"/data/pos.txt");

    QFile file(m_saveTopologyFilePath+".txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream output(&file);
    QList<QGraphicsItem*> t_itemlist = ui_editWidget->getScene()->items(QRectF(0,0,ui_editWidget->getScene()->width(),ui_editWidget->getScene()->height()));
    QList<CSwitcher*> t_swithers ;
    for(int i = 0; i < t_itemlist.size(); i++)
    {
        CSwitcher* t_item = dynamic_cast <CSwitcher *>(t_itemlist[i]);
        if(t_item != NULL)t_swithers.append(t_item);
    }
    for(int i = 0; i < t_swithers.size(); i++)
    {
        for(int j = 0; j < t_swithers[i]->getSwitcherInfo().LNodes.size(); j++)
        {
            SNodeStructInfo portInfo = t_swithers[i]->getSwitcherInfo().LNodes[j];
            output<<t_swithers[i]->getSwitcherInfo().networkLocation<<"\t"
                  <<portInfo.name<<"\t"
                  <<portInfo.interfaceName<<"\t"
                  <<portInfo.area<<"\t"
                  <<portInfo.IP<<"\t"
                  <<portInfo.subnetMask<<"\t"
                  <<portInfo.describe<<"\t"
                  <<portInfo.costValue<<"\t"
                  <<portInfo.linkPort_name<<"\t"
                  <<portInfo.linkPort_interfaceName<<"\t"
                  <<portInfo.linkPort_area<<"\t"
                  <<portInfo.linkPort_IP<<"\t"
                  <<portInfo.linkPort_subnetMask<<"\t"
                  <<portInfo.linkPort_describe<<"\t"
                  <<portInfo.linkPort_costValue<<"\t"
                  <<portInfo.remark<<"\t"
                  <<portInfo.bandWidth<<"\t"
                  <<portInfo.delay<<"\t"
                  <<t_swithers[i]->getSwitcherInfo().protocol<<"\n";
        }
    }
}

/*************************************************
  Function: startSimulate
  Description: ��ʼ����ۺ����ۺ���
  Parameter:
  Return:
*************************************************/
void NetworkSimulationPlatform::startSimulate(void)
{
    if (!m_saveFlag)
    {
        QMessageBox::information(this, tr("��ʾ"),tr("���ȱ���������Ϣ!"),QMessageBox::Ok);
        return;
    }

    const int MAXLINE = 10240;

    historyListWidget->clear();

    if(m_saveTopologyFileName.isEmpty())
    {
        m_saveTopologyFileName = m_saveFile->getFilename();
    }

    if(m_saveTopologyFilePath.isEmpty())
    {
        m_saveTopologyFilePath = m_saveFile->GetRoute();
    }

    if(m_NS3Path.isEmpty())
    {
        QFile infile("Config");
        if(!infile.exists())
        {
            QMessageBox::critical(this, tr("Error"), "Can not find Config file��",QMessageBox::Ok);
            return;
        }
        if(!infile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::critical(this, tr("Error"), "Can not open Config file��",QMessageBox::Ok);
            return;
        }

        QTextStream fileStream(&infile);
        m_NS3Path = fileStream.readAll();
        m_NS3Path.replace("\n","");
        infile.close();
    }

    QString filePath = m_currentPath + "/waf";
    ofstream outFile(filePath.toLocal8Bit().data());
    outFile<<"cd "<<string(m_NS3Path.toLocal8Bit().data())<<endl;
    if(m_partitionNum == 1)
    {
        outFile<<"./waf --run \"ConstructNetwork --FilePath=" + string(m_saveTopologyFilePath.toLocal8Bit().data())<<".txt --Partition=1\""<<endl;
        outFile<<"./waf --run \"Simulation --FilePath=" + string(m_saveTopologyFilePath.toLocal8Bit().data())<<".txt\" --vis"<<endl;
    }
    else
    {
        outFile<<"./waf --run \"ConstructNetwork --FilePath=" + string(m_saveTopologyFilePath.toLocal8Bit().data())<<".txt --Partition="<<m_partitionNum<<"\""<<endl;
        outFile<<"mpirun -np "<<m_partitionNum<<" ./waf --run \"ParallelSimulation --FilePath=" + string(m_saveTopologyFilePath.toLocal8Bit().data())<<".txt\""<<endl;
    }

    outFile.close();

    FILE *fp;
    char buf[MAXLINE];
    QString command = "sh " + filePath;

    fp = popen(command.toLocal8Bit().data(), "r");
    while(fgets(buf, MAXLINE, fp) != NULL)
    {
        historyListWidget->insertPlainText(QString(buf));
    }
    pclose(fp);
}

/*************************************************
  Function: zoomIn
  Description: �Ŵ����˲ۺ���
  Parameter: ��
  Return: ��
*************************************************/
void NetworkSimulationPlatform::zoomIn(void)
{
	//ע��Ŵ����ͼԪ

}

/*************************************************
  Function: zoomOut
  Description: ��С���˲ۺ���
  Parameter: ��
  Return: ��
*************************************************/
void NetworkSimulationPlatform::zoomOut(void)
{
}

/*************************************************
  Function: exportTopology
  Description: �����������˲ۺ���
  Parameter: ��
  Return: ��
*************************************************/
void NetworkSimulationPlatform::exportTopology( void )
{
    QString absolutePath = m_currentPath + "/Topology";
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"),
                               absolutePath, tr("Topology (*.top)"));
    if (filePath == absolutePath)
    {
        return;
    }
    if (filePath.count(".top") == 0)
    {
        filePath +=".top";
    }

    ui_editWidget->OutputTopology(filePath);
    ui_editWidget->OutputPosition(m_saveFile->GetFilePath()+"/data/pos.txt");
}

/*************************************************
  Function: importTopology
  Description: �����������˲ۺ���
  Parameter: ��
  Return: ��
*************************************************/
void NetworkSimulationPlatform::importTopology( void )
{
    historyListWidget->clear();
    m_projectName = "";
    QString topologyPath = m_currentPath + "/Topology";
    QString filePath = QFileDialog::getOpenFileName(this,
                            tr("Open File"), topologyPath, tr("Toplogy (*.top)"));

    if (filePath == topologyPath)
    {
        return;
    }
    //���ݽ������������ݻ�����������
    ui_editWidget->InputTopology(filePath);
    m_saveFlag = false;
}
/*************************************************
  Function: clearScene
  Description: ��ջ���
  Parameter: ��
  Return: ��
*************************************************/
void NetworkSimulationPlatform::clearScene(void)
{
   ui_editWidget->clearScene();
   historyListWidget->clear();
}

/*************************************************
  Function: layoutAlgorithm
  Description: �����㷨�ۺ���
  Parameter: ��
  Return: ��
*************************************************/
void NetworkSimulationPlatform::layoutAlgorithm( void )
{
	//���Կ����ṩ��������㷨

}

/*************************************************
  Function: createToolBar
  Description: ����������
  Parameter: ��
  Return: ��
*************************************************/
void NetworkSimulationPlatform::createToolBar( void )
{
	mainToolBar->addAction(newNetworkItemAction);
	mainToolBar->addAction(openNetworkItemAction);
    mainToolBar->addAction(saveItemAction);
	mainToolBar->addSeparator();
	mainToolBar->addAction(zoomInAction);
	mainToolBar->addAction(zoomOutAction);
    //mainToolBar->addAction(layoutAction); //���˲���
    //mainToolBar->addAction(configTaskAction);
}

/*************************************************
  Function: createToolBar
  Description: ����������
  Parameter: ��
  Return: ��
*************************************************/
void NetworkSimulationPlatform::createMenu( void )
{
    menu_startMenu = QMainWindow::menuBar()->addMenu(tr("��ʼ"));
    //menu_startMenu->addAction(startSimulateAction);
    //menu_startMenu->addAction(importSNMPTopologyAction);
    //menu_startMenu->addAction(importBackgroundAction);
    //menu_startMenu->addAction(deleteBackgroundAction);
    menu_startMenu->addAction(exitAction);

    menu_networkEditMenu = QMainWindow::menuBar()->addMenu(tr("�༭"));
    menu_networkEditMenu->addAction(newNetworkItemAction);
    menu_networkEditMenu->addAction(openNetworkItemAction);
    menu_networkEditMenu->addAction(saveItemAction);
    menu_networkEditMenu->addAction(zoomInAction);
    menu_networkEditMenu->addAction(zoomOutAction);

    //menu_networkEditMenu->addAction(exportTopologyAction);
    //menu_networkEditMenu->addAction(importTopologyAction);
    menu_networkEditMenu->addAction(clearseceneAction);

//    menu_configNetworkMenu = menu_networkEditMenu->addMenu(tr("����"));
//    menu_configNetworkMenu->addAction(linkSetAction);
//    menu_configNetworkMenu->addAction(flowSetAction);
//    menu_configNetworkMenu->addAction(partitionNumAction);

    menu_toolMenu = menu_networkEditMenu->addMenu(tr("����"));
//    menu_networkStateMenu = QMainWindow::menuBar()->addMenu(tr("��������״̬"));
//    menu_networkStateMenu->addAction(networkBandAction);
//    menu_networkStateMenu->addAction(networkCapacityAction);

//    menu_configTaskMenu = QMainWindow::menuBar()->addMenu(tr("�������"));
//    menu_configTaskMenu->addAction(configTaskAction);
//    menu_configTaskMenu->addAction(eventSchedulAction);
}

/*************************************************
  Function: setFlow
  Description: ��������
  Parameter: ��
  Return: ��
*************************************************/
void NetworkSimulationPlatform::setFlow( void )
{

}

/*************************************************
  Function: setLink
  Description: ��·����
  Parameter: ��
  Return: ��
*************************************************/
void NetworkSimulationPlatform::setLink( void )
{

}

/*************************************************
  Function: createDockWindow
  Description: ����ͣ������
  Parameter: ��
  Return: ��
*************************************************/
void NetworkSimulationPlatform::createDockWindow( void )
{
//    ui_networkDeviceDock = new QDockWidget(tr("�����豸"), this);
//    ui_networkDeviceWidget = new QWidget(ui_networkDeviceDock);
//    ui_networkToolBox = new QToolBox(ui_networkDeviceWidget);
//    ui_routerDevice = new QWidget;
//    ui_PCDevice = new QWidget;

//    ui_networkToolBox->addItem(ui_routerDevice, tr("·����"));
//    ui_networkToolBox->addItem(ui_PCDevice, tr("�ն�"));

//    //���е��豸��CUnitboxwidget�������
//    ui_unitboxwidget = new CUnitboxWidget(ui_routerDevice);
//    ui_editWidget->setLabelWidget(ui_unitboxwidget);

//	//����դ�񲼾�
//    ui_toolBoxGridLayout = new QGridLayout(ui_networkDeviceWidget);
//    ui_toolBoxGridLayout->setSpacing(6);
//    ui_toolBoxGridLayout->setMargin(11);
//    ui_toolBoxGridLayout->addWidget(ui_networkToolBox, 0, 0, 1, 1);
//    ui_networkDeviceDock->setMinimumWidth(150);//������С���

//	/*�����豸ͣ������ͣ������*/
//    ui_networkDeviceDock->setAllowedAreas(Qt::AllDockWidgetAreas);
//    ui_networkDeviceDock->setWidget(ui_networkDeviceWidget);
//    addDockWidget(Qt::RightDockWidgetArea, ui_networkDeviceDock);
//    menu_toolMenu->addAction(ui_networkDeviceDock->toggleViewAction());

	/*���ò�����¼ͣ������ͣ������*/
    ui_historyDock = new QDockWidget(tr("������¼"), this);
    historyListWidget = new QTextEdit(ui_historyDock);
    ui_historyDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    ui_historyDock->setWidget(historyListWidget);
    addDockWidget(Qt::BottomDockWidgetArea, ui_historyDock);
    menu_toolMenu->addAction(ui_historyDock->toggleViewAction());
}

void NetworkSimulationPlatform::importSNMPTopology(void)
{
    m_projectName = "";
    QString topologyPath = m_currentPath + "/Topology";
    QString filePath = QFileDialog::getOpenFileName(this,
                            tr("Open File"), topologyPath, tr("Toplogy (*.txt)"));

    if (filePath == topologyPath)
    {
        return;
    }

    ui_editWidget->ConstructTopology(filePath);
}

void NetworkSimulationPlatform::configTask()
{
    ui_editWidget->configTask();
}

void NetworkSimulationPlatform::eventSchedul()
{
    ui_editWidget->eventSchedul();
}

void NetworkSimulationPlatform::setPartitonNum()
{
    CPartitionNumDialog partitionNum;
    partitionNum.exec();

    if (!partitionNum.getCancel())
    {
        m_partitionNum = partitionNum.getPartitonNum();
    }
}

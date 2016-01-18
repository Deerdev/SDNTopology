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

QString NetworkSimulationPlatform::getSysTime()
{
    QDateTime time = QDateTime::currentDateTime();//��ȡϵͳ���ڵ�ʱ��
    QString str = time.toString("yyyy-MM-dd hh:mm:ss"); //������ʾ��ʽ
    return str;
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
    ui_editWidget->setPlatformPointer(this);
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

    /*���÷Ŵ���*/
    zoomInAction = new QAction( tr("�Ŵ�"), this );
    zoomInAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/zoomIn.png"));
    connect( zoomInAction, SIGNAL(triggered()), ui_editWidget, SLOT(zoomIn()) );

    /*������С����*/
    zoomOutAction = new QAction( tr("��С"), this );
    zoomOutAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/zoomOut.png"));
    connect( zoomOutAction, SIGNAL(triggered()), ui_editWidget, SLOT(zoomOut()) );

    /*���ô�������Ŀ����*/
    refreshNetworkItemAction = new QAction( tr("��������"), this );
    refreshNetworkItemAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/refresh.png"));
    refreshNetworkItemAction->setShortcut(tr("Ctrl+O"));
    connect( refreshNetworkItemAction, SIGNAL(triggered()), this, SLOT( refreshNetworkItem()) );

    /*�����˳�ϵͳ����*/
    exitAction = new QAction( tr("�˳�"), this );
    exitAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/exit.png"));
    exitAction->setShortcut(tr("Ctrl+Q"));
    connect( exitAction, SIGNAL(triggered()), this, SLOT(closeSimuWindowSlot()) );

    /*��ջ���*/
    clearseceneAction = new QAction(tr("��ջ���"), this);
    clearseceneAction->setIcon(QIcon(""));
    connect(clearseceneAction, SIGNAL(triggered()), this, SLOT(clearScene()));





    //*****************************************************************************************
//	/*���ô�������Ŀ����*/
//	newNetworkItemAction = new QAction( tr("��������Ŀ"), this );
//    newNetworkItemAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/newItem.png"));
//	newNetworkItemAction->setShortcut(tr("Ctrl+N"));
//	connect( newNetworkItemAction, SIGNAL(triggered()), this, SLOT(createNewNetworkItem()) );

//	/*���ñ��涯��*/
//	saveItemAction = new QAction( tr("����"), this );
//    saveItemAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/save.png"));
//	saveItemAction->setShortcut(tr("Ctrl+S"));
//	connect( saveItemAction, SIGNAL(triggered()), this, SLOT(saveItem()));

//    /*���ÿ�ʼ����*/
//    startSimulateAction = new QAction( tr("��ʼ����"), this );
//    startSimulateAction->setShortcut(tr("Ctrl+S"));
//    connect(startSimulateAction, SIGNAL(triggered()), this, SLOT(startSimulate()));

//    /*��ȡSNMP����*/
//    importSNMPTopologyAction = new QAction( tr("����SNMP��������"), this);
//    connect(importSNMPTopologyAction, SIGNAL(triggered()), this, SLOT(importSNMPTopology()));

//	/*���ò����㷨����*/
//	layoutAction = new QAction( tr("����"), this );
//    layoutAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/layout.png"));
//	connect( layoutAction, SIGNAL(triggered()), this, SLOT(layoutAlgorithm()) );

//	/*������·���ö���*/
//	flowSetAction = new QAction( tr("��·����"), this );
//	flowSetAction->setIcon(QIcon(""));
//	connect( flowSetAction, SIGNAL(triggered()), this, SLOT(setFlow()) );
//	/*�����������ö���*/
//	linkSetAction = new QAction( tr("��������"), this );
//	linkSetAction->setIcon(QIcon(""));
//	connect( linkSetAction, SIGNAL(triggered()), this, SLOT(setLink()) );
//    /**/
//    configTaskAction = new QAction(tr("��������"), this);
//    configTaskAction->setIcon(QIcon(""));
//    connect( configTaskAction, SIGNAL(triggered()), this, SLOT(configTask()) );
//    eventSchedulAction = new QAction(tr("�¼�����"), this);
//    connect(eventSchedulAction, SIGNAL(triggered() ), this, SLOT(eventSchedul() ) );
//    /**/
//    partitionNumAction = new QAction(tr("���绮������"),this);
//    partitionNumAction->setIcon(QIcon(""));
//    connect( partitionNumAction, SIGNAL(triggered()), this, SLOT(setPartitonNum()) );
//    networkBandAction = new QAction( tr("�������"), this);
//    networkCapacityAction = new QAction( tr("����������"), this);


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

/*************************************************
  Function: openNetworkItem
  Description: ����Ŀ�ۺ���
  Parameter: ��
  Return: ��
*************************************************/
void NetworkSimulationPlatform::refreshNetworkItem( void )
{
	//��������Ŀ�Ի���
    ui_editWidget->RefreshTopology();

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

void NetworkSimulationPlatform::closeSimuWindowSlot()
{
    int ret = QMessageBox::warning(this, tr("�رճ���"),
                                    tr("ȷ��Ҫ�رճ���"),
                                    QMessageBox::Yes | QMessageBox::Cancel,
                                    QMessageBox::Cancel);
    if (ret == QMessageBox::Yes)
    {
        close();
    }
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

void NetworkSimulationPlatform::closeEvent(QCloseEvent *event)
{
//    int ret = QMessageBox::warning(this, tr("�رճ���"),
//                                    tr("ȷ��Ҫ�رճ���"),
//                                    QMessageBox::Yes | QMessageBox::Cancel,
//                                    QMessageBox::Cancel);
//    if (ret == QMessageBox::Yes)
//    {
//        close();
//    }
    close();
}

/*************************************************
  Function: createToolBar
  Description: ����������
  Parameter: ��
  Return: ��
*************************************************/
void NetworkSimulationPlatform::createToolBar( void )
{

    //mainToolBar->addAction(newNetworkItemAction);
    mainToolBar->addAction(refreshNetworkItemAction);
    mainToolBar->addAction(exitAction);
    //mainToolBar->addAction(saveItemAction);

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

    menu_startMenu->addAction(refreshNetworkItemAction);
    //menu_startMenu->addAction(importSNMPTopologyAction);
    //menu_startMenu->addAction(importBackgroundAction);
    //menu_startMenu->addAction(deleteBackgroundAction);
    menu_startMenu->addAction(exitAction);

    menu_networkEditMenu = QMainWindow::menuBar()->addMenu(tr("�༭����"));
    //menu_networkEditMenu->addAction(saveItemAction);
    menu_networkEditMenu->addAction(zoomInAction);
    menu_networkEditMenu->addAction(zoomOutAction);
    //menu_networkEditMenu->addAction(exportTopologyAction);
    //menu_networkEditMenu->addAction(importTopologyAction);
    menu_networkEditMenu->addAction(clearseceneAction);

    menu_flowEditMenu = QMainWindow::menuBar()->addMenu(tr("����༭"));
    menu_DockShowMenu = QMainWindow::menuBar()->addMenu(tr("������ʾ"));

//    menu_configNetworkMenu = menu_networkEditMenu->addMenu(tr("����"));
//    menu_configNetworkMenu->addAction(linkSetAction);
//    menu_configNetworkMenu->addAction(flowSetAction);
//    menu_configNetworkMenu->addAction(partitionNumAction);

    //menu_toolMenu = menu_networkEditMenu->addMenu(tr("����"));
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
    ui_historyDock->setMinimumHeight(50);
    ui_historyDock->setMaximumHeight(150);

    historyListWidget = new QTextEdit(ui_historyDock);
    historyListWidget->setReadOnly(true);
    ui_historyDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    ui_historyDock->setWidget(historyListWidget);
    addDockWidget(Qt::BottomDockWidgetArea, ui_historyDock);
    menu_DockShowMenu->addAction(ui_historyDock->toggleViewAction());

    ui_DeviceInfDock = new QDockWidget(tr("�豸��Ϣ"), this);
    ui_DeviceInfDock->setMinimumWidth(250);
    ui_DeviceInfDock->setMaximumWidth(500);

    ui_DeviceDockWidget = new QWidget(ui_DeviceInfDock);
    ui_DeviceInfTable = new QTableWidget(ui_DeviceDockWidget);
    QPalette pll = ui_DeviceInfTable->palette();

    pll.setBrush(QPalette::Base,QBrush(QColor(255,255,255,0)));

    ui_DeviceInfTable->setPalette(pll);
    //ui_DeviceInfTable->verticalHeader()->setHidden(true);
    //ui_DeviceInfTable->horizontalHeader()->setHidden(true);
    //ui_DeviceInfTable->horizontalHeader()->setStretchLastSection(true);
    //ui_DeviceInTable->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    //ui_DeviceInfTable->resize(2.5*ui_DeviceInfTable->columnWidth(0), 12*ui_DeviceInfTable->rowHeight(0));
    ui_DeviceInfDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    ui_DeviceInfDock->setWidget(ui_DeviceDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, ui_DeviceInfDock);
    menu_DockShowMenu->addAction(ui_DeviceInfDock->toggleViewAction());
}

//void NetworkSimulationPlatform::importSNMPTopology(void)
//{
//    m_projectName = "";
//    QString topologyPath = m_currentPath + "/Topology";
//    QString filePath = QFileDialog::getOpenFileName(this,
//                            tr("Open File"), topologyPath, tr("Toplogy (*.txt)"));

//    if (filePath == topologyPath)
//    {
//        return;
//    }

//    ui_editWidget->ConstructTopology(filePath);
//}

//void NetworkSimulationPlatform::configTask()
//{
//    ui_editWidget->configTask();
//}

//void NetworkSimulationPlatform::eventSchedul()
//{
//    ui_editWidget->eventSchedul();
//}

//void NetworkSimulationPlatform::setPartitonNum()
//{
//    CPartitionNumDialog partitionNum;
//    partitionNum.exec();

//    if (!partitionNum.getCancel())
//    {
//        m_partitionNum = partitionNum.getPartitonNum();
//    }
//}

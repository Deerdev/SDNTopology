#include "NetworkSimulationPlatform.h"
#include <QSizePolicy>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include "CPartitionNumDialog.h"

/*************************************************
  Function: NetworkSimulationPlatform
  Description: 构造函数
  Parameter: 无
  Return: 无
*************************************************/
NetworkSimulationPlatform::NetworkSimulationPlatform(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	setupUi(this);
	//初始化界面
    init();
}

/*************************************************
  Function: ~NetworkSimulationPlatform
  Description: 析构函数
  Parameter: 无
  Return: 无
*************************************************/
NetworkSimulationPlatform::~NetworkSimulationPlatform()
{

}

QString NetworkSimulationPlatform::getSysTime()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式
    return str;
}

/*************************************************
  Function: init
  Description: 初始化界面
  Parameter: 无
  Return: 无
*************************************************/
void NetworkSimulationPlatform::init( void )
{
    m_saveFlag = false;
    m_partitionNum = 1;

    ui_editWidget = new CTopologyWidget();
    setCentralWidget(ui_editWidget);
    ui_editWidget->translate(1000,1000);

    setWindowTitle(tr("SDN网络拓扑"));
    setToolTip(tr("SDN网络拓扑"));
    setStatusTip(tr("SDN网络拓扑"));

	//创建动作
	createActions();
	//创建菜单
	createMenu();
	//创建工具栏
	createToolBar();

	//创建停靠窗口
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
  Description: 创建动作
  Parameter: 无
  Return: 无
*************************************************/
void NetworkSimulationPlatform::createActions( void )
{
	//设置各个动作
	/*注意：动作用triggered()触发，button用clicked()触发*/

    /*设置放大动作*/
    zoomInAction = new QAction( tr("放大"), this );
    zoomInAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/zoomIn.png"));
    connect( zoomInAction, SIGNAL(triggered()), ui_editWidget, SLOT(zoomIn()) );

    /*设置缩小动作*/
    zoomOutAction = new QAction( tr("缩小"), this );
    zoomOutAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/zoomOut.png"));
    connect( zoomOutAction, SIGNAL(triggered()), ui_editWidget, SLOT(zoomOut()) );

    /*设置打开已有项目动作*/
    refreshNetworkItemAction = new QAction( tr("更新网络"), this );
    refreshNetworkItemAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/refresh.png"));
    refreshNetworkItemAction->setShortcut(tr("Ctrl+O"));
    connect( refreshNetworkItemAction, SIGNAL(triggered()), this, SLOT( refreshNetworkItem()) );

    /*设置退出系统动作*/
    exitAction = new QAction( tr("退出"), this );
    exitAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/exit.png"));
    exitAction->setShortcut(tr("Ctrl+Q"));
    connect( exitAction, SIGNAL(triggered()), this, SLOT(closeSimuWindowSlot()) );

    /*清空画布*/
    clearseceneAction = new QAction(tr("清空画布"), this);
    clearseceneAction->setIcon(QIcon(""));
    connect(clearseceneAction, SIGNAL(triggered()), this, SLOT(clearScene()));





    //*****************************************************************************************
//	/*设置创建新项目动作*/
//	newNetworkItemAction = new QAction( tr("创建新项目"), this );
//    newNetworkItemAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/newItem.png"));
//	newNetworkItemAction->setShortcut(tr("Ctrl+N"));
//	connect( newNetworkItemAction, SIGNAL(triggered()), this, SLOT(createNewNetworkItem()) );

//	/*设置保存动作*/
//	saveItemAction = new QAction( tr("保存"), this );
//    saveItemAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/save.png"));
//	saveItemAction->setShortcut(tr("Ctrl+S"));
//	connect( saveItemAction, SIGNAL(triggered()), this, SLOT(saveItem()));

//    /*设置开始仿真*/
//    startSimulateAction = new QAction( tr("开始仿真"), this );
//    startSimulateAction->setShortcut(tr("Ctrl+S"));
//    connect(startSimulateAction, SIGNAL(triggered()), this, SLOT(startSimulate()));

//    /*抽取SNMP拓扑*/
//    importSNMPTopologyAction = new QAction( tr("解析SNMP导入拓扑"), this);
//    connect(importSNMPTopologyAction, SIGNAL(triggered()), this, SLOT(importSNMPTopology()));

//	/*设置布局算法动作*/
//	layoutAction = new QAction( tr("布局"), this );
//    layoutAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/layout.png"));
//	connect( layoutAction, SIGNAL(triggered()), this, SLOT(layoutAlgorithm()) );

//	/*设置链路配置动作*/
//	flowSetAction = new QAction( tr("链路配置"), this );
//	flowSetAction->setIcon(QIcon(""));
//	connect( flowSetAction, SIGNAL(triggered()), this, SLOT(setFlow()) );
//	/*设置流量配置动作*/
//	linkSetAction = new QAction( tr("流量配置"), this );
//	linkSetAction->setIcon(QIcon(""));
//	connect( linkSetAction, SIGNAL(triggered()), this, SLOT(setLink()) );
//    /**/
//    configTaskAction = new QAction(tr("任务配置"), this);
//    configTaskAction->setIcon(QIcon(""));
//    connect( configTaskAction, SIGNAL(triggered()), this, SLOT(configTask()) );
//    eventSchedulAction = new QAction(tr("事件调度"), this);
//    connect(eventSchedulAction, SIGNAL(triggered() ), this, SLOT(eventSchedul() ) );
//    /**/
//    partitionNumAction = new QAction(tr("网络划分数量"),this);
//    partitionNumAction->setIcon(QIcon(""));
//    connect( partitionNumAction, SIGNAL(triggered()), this, SLOT(setPartitonNum()) );
//    networkBandAction = new QAction( tr("网络带宽"), this);
//    networkCapacityAction = new QAction( tr("网络吞吐量"), this);


}

/*************************************************
  Function: createNewNetworkItem
  Description: 创建新项目槽函数
  Parameter: 无
  Return: 无
*************************************************/
void NetworkSimulationPlatform::createNewNetworkItem( void )
{
    //弹出创建新项目对话框
    m_newFile = new CWizard;
    connect(m_newFile, SIGNAL(accepted()), this, SLOT(slotNewFile()));
    m_newFile->exec();
}

/*************************************************
  Function: openNetworkItem
  Description: 打开项目槽函数
  Parameter: 无
  Return: 无
*************************************************/
void NetworkSimulationPlatform::refreshNetworkItem( void )
{
	//弹出打开项目对话框
    ui_editWidget->RefreshTopology();

}

/*************************************************
  Function: zoomIn
  Description: 放大拓扑槽函数
  Parameter: 无
  Return: 无
*************************************************/
void NetworkSimulationPlatform::zoomIn(void)
{
	//注意放大的是图元

}

/*************************************************
  Function: zoomOut
  Description: 缩小拓扑槽函数
  Parameter: 无
  Return: 无
*************************************************/
void NetworkSimulationPlatform::zoomOut(void)
{
}

void NetworkSimulationPlatform::closeSimuWindowSlot()
{
    int ret = QMessageBox::warning(this, tr("关闭程序"),
                                    tr("确定要关闭程序？"),
                                    QMessageBox::Yes | QMessageBox::Cancel,
                                    QMessageBox::Cancel);
    if (ret == QMessageBox::Yes)
    {
        close();
    }
}

/*************************************************
  Function: clearScene
  Description: 清空画布
  Parameter: 无
  Return: 无
*************************************************/
void NetworkSimulationPlatform::clearScene(void)
{
   ui_editWidget->clearScene();
   historyListWidget->clear();
}

/*************************************************
  Function: layoutAlgorithm
  Description: 布局算法槽函数
  Parameter: 无
  Return: 无
*************************************************/
void NetworkSimulationPlatform::layoutAlgorithm( void )
{
	//可以考虑提供多个布局算法

}

void NetworkSimulationPlatform::closeEvent(QCloseEvent *event)
{
//    int ret = QMessageBox::warning(this, tr("关闭程序"),
//                                    tr("确定要关闭程序？"),
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
  Description: 创建工具栏
  Parameter: 无
  Return: 无
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
    //mainToolBar->addAction(layoutAction); //拓扑布局
    //mainToolBar->addAction(configTaskAction);
}

/*************************************************
  Function: createToolBar
  Description: 创建工具栏
  Parameter: 无
  Return: 无
*************************************************/
void NetworkSimulationPlatform::createMenu( void )
{
    menu_startMenu = QMainWindow::menuBar()->addMenu(tr("开始"));

    menu_startMenu->addAction(refreshNetworkItemAction);
    //menu_startMenu->addAction(importSNMPTopologyAction);
    //menu_startMenu->addAction(importBackgroundAction);
    //menu_startMenu->addAction(deleteBackgroundAction);
    menu_startMenu->addAction(exitAction);

    menu_networkEditMenu = QMainWindow::menuBar()->addMenu(tr("编辑画布"));
    //menu_networkEditMenu->addAction(saveItemAction);
    menu_networkEditMenu->addAction(zoomInAction);
    menu_networkEditMenu->addAction(zoomOutAction);
    //menu_networkEditMenu->addAction(exportTopologyAction);
    //menu_networkEditMenu->addAction(importTopologyAction);
    menu_networkEditMenu->addAction(clearseceneAction);

    menu_flowEditMenu = QMainWindow::menuBar()->addMenu(tr("流表编辑"));
    menu_DockShowMenu = QMainWindow::menuBar()->addMenu(tr("窗口显示"));

//    menu_configNetworkMenu = menu_networkEditMenu->addMenu(tr("配置"));
//    menu_configNetworkMenu->addAction(linkSetAction);
//    menu_configNetworkMenu->addAction(flowSetAction);
//    menu_configNetworkMenu->addAction(partitionNumAction);

    //menu_toolMenu = menu_networkEditMenu->addMenu(tr("工具"));
//    menu_networkStateMenu = QMainWindow::menuBar()->addMenu(tr("网络运行状态"));
//    menu_networkStateMenu->addAction(networkBandAction);
//    menu_networkStateMenu->addAction(networkCapacityAction);

//    menu_configTaskMenu = QMainWindow::menuBar()->addMenu(tr("任务管理"));
//    menu_configTaskMenu->addAction(configTaskAction);
//    menu_configTaskMenu->addAction(eventSchedulAction);
}

/*************************************************
  Function: setFlow
  Description: 流量配置
  Parameter: 无
  Return: 无
*************************************************/
void NetworkSimulationPlatform::setFlow( void )
{

}

/*************************************************
  Function: setLink
  Description: 链路配置
  Parameter: 无
  Return: 无
*************************************************/
void NetworkSimulationPlatform::setLink( void )
{

}

/*************************************************
  Function: createDockWindow
  Description: 创建停靠窗口
  Parameter: 无
  Return: 无
*************************************************/
void NetworkSimulationPlatform::createDockWindow( void )
{
//    ui_networkDeviceDock = new QDockWidget(tr("网络设备"), this);
//    ui_networkDeviceWidget = new QWidget(ui_networkDeviceDock);
//    ui_networkToolBox = new QToolBox(ui_networkDeviceWidget);
//    ui_routerDevice = new QWidget;
//    ui_PCDevice = new QWidget;

//    ui_networkToolBox->addItem(ui_routerDevice, tr("路由器"));
//    ui_networkToolBox->addItem(ui_PCDevice, tr("终端"));

//    //所有的设备在CUnitboxwidget类中添加
//    ui_unitboxwidget = new CUnitboxWidget(ui_routerDevice);
//    ui_editWidget->setLabelWidget(ui_unitboxwidget);

//	//设置栅格布局
//    ui_toolBoxGridLayout = new QGridLayout(ui_networkDeviceWidget);
//    ui_toolBoxGridLayout->setSpacing(6);
//    ui_toolBoxGridLayout->setMargin(11);
//    ui_toolBoxGridLayout->addWidget(ui_networkToolBox, 0, 0, 1, 1);
//    ui_networkDeviceDock->setMinimumWidth(150);//设置最小宽度

//	/*设置设备停靠窗口停靠区域*/
//    ui_networkDeviceDock->setAllowedAreas(Qt::AllDockWidgetAreas);
//    ui_networkDeviceDock->setWidget(ui_networkDeviceWidget);
//    addDockWidget(Qt::RightDockWidgetArea, ui_networkDeviceDock);
//    menu_toolMenu->addAction(ui_networkDeviceDock->toggleViewAction());

	/*设置操作记录停靠窗口停靠区域*/
    ui_historyDock = new QDockWidget(tr("操作记录"), this);
    ui_historyDock->setMinimumHeight(50);
    ui_historyDock->setMaximumHeight(150);

    historyListWidget = new QTextEdit(ui_historyDock);
    historyListWidget->setReadOnly(true);
    ui_historyDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    ui_historyDock->setWidget(historyListWidget);
    addDockWidget(Qt::BottomDockWidgetArea, ui_historyDock);
    menu_DockShowMenu->addAction(ui_historyDock->toggleViewAction());

    ui_DeviceInfDock = new QDockWidget(tr("设备信息"), this);
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

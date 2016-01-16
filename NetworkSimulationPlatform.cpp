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
	//添加新工具栏
	createNewTooBar();

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
}

/*************************************************
  Function: createNewTooBar
  Description: 创建新工具栏
  Parameter: 无
  Return: 无
*************************************************/
void NetworkSimulationPlatform::createNewTooBar( void )
{

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
	/*设置创建新项目动作*/
	newNetworkItemAction = new QAction( tr("创建新项目"), this );
    newNetworkItemAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/newItem.png"));
	newNetworkItemAction->setShortcut(tr("Ctrl+N"));
	connect( newNetworkItemAction, SIGNAL(triggered()), this, SLOT(createNewNetworkItem()) );

	/*设置打开已有项目动作*/
	openNetworkItemAction = new QAction( tr("打开已有项目"), this );
    openNetworkItemAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/open.png"));
    openNetworkItemAction->setShortcut(tr("Ctrl+O"));
	connect( openNetworkItemAction, SIGNAL(triggered()), this, SLOT(openNetworkItem()) );

	/*设置保存动作*/
	saveItemAction = new QAction( tr("保存"), this );
    saveItemAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/save.png"));
	saveItemAction->setShortcut(tr("Ctrl+S"));
	connect( saveItemAction, SIGNAL(triggered()), this, SLOT(saveItem()));

	/*设置退出系统动作*/
	exitAction = new QAction( tr("退出"), this );
    exitAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/exit.png"));
	exitAction->setShortcut(tr("Ctrl+Q"));
	connect( exitAction, SIGNAL(triggered()), this, SLOT(close()) );

    /*设置开始仿真*/
    startSimulateAction = new QAction( tr("开始仿真"), this );
    startSimulateAction->setShortcut(tr("Ctrl+S"));
    connect(startSimulateAction, SIGNAL(triggered()), this, SLOT(startSimulate()));

    /*抽取SNMP拓扑*/
    importSNMPTopologyAction = new QAction( tr("解析SNMP导入拓扑"), this);
    connect(importSNMPTopologyAction, SIGNAL(triggered()), this, SLOT(importSNMPTopology()));

    /*设置放大动作*/
	zoomInAction = new QAction( tr("放大"), this );
    zoomInAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/zoomIn.png"));
    connect( zoomInAction, SIGNAL(triggered()), ui_editWidget, SLOT(zoomIn()) );

	/*设置缩小动作*/
	zoomOutAction = new QAction( tr("缩小"), this );
    zoomOutAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/zoomOut.png"));
    connect( zoomOutAction, SIGNAL(triggered()), ui_editWidget, SLOT(zoomOut()) );

	/*设置布局算法动作*/
	layoutAction = new QAction( tr("布局"), this );
    layoutAction->setIcon(QIcon(":/NetworkSimulationPlatform/Resources/layout.png"));
	connect( layoutAction, SIGNAL(triggered()), this, SLOT(layoutAlgorithm()) );

	/*设置导出拓扑动作*/
	exportTopologyAction = new QAction( tr("导出网络拓扑"), this );
    exportTopologyAction->setIcon(QIcon(""));
    connect( exportTopologyAction, SIGNAL(triggered()), this, SLOT(exportTopology()) );

	/*设置导入拓扑动作*/
	importTopologyAction = new QAction( tr("导入网络拓扑"), this );
    importTopologyAction->setIcon(QIcon(""));
    connect( importTopologyAction, SIGNAL(triggered()), this, SLOT(importTopology()) );

    /*清空画布*/
    clearseceneAction = new QAction(tr("清空画布"), this);
    clearseceneAction->setIcon(QIcon(""));
    connect(clearseceneAction, SIGNAL(triggered()), this, SLOT(clearScene()));

	/*设置链路配置动作*/
	flowSetAction = new QAction( tr("链路配置"), this );
	flowSetAction->setIcon(QIcon(""));
	connect( flowSetAction, SIGNAL(triggered()), this, SLOT(setFlow()) );
	
	/*设置流量配置动作*/
	linkSetAction = new QAction( tr("流量配置"), this );
	linkSetAction->setIcon(QIcon(""));
	connect( linkSetAction, SIGNAL(triggered()), this, SLOT(setLink()) );

    /**/
    configTaskAction = new QAction(tr("任务配置"), this);
    configTaskAction->setIcon(QIcon(""));
    connect( configTaskAction, SIGNAL(triggered()), this, SLOT(configTask()) );

    eventSchedulAction = new QAction(tr("事件调度"), this);
    connect(eventSchedulAction, SIGNAL(triggered() ), this, SLOT(eventSchedul() ) );

    /**/
    partitionNumAction = new QAction(tr("网络划分数量"),this);
    partitionNumAction->setIcon(QIcon(""));
    connect( partitionNumAction, SIGNAL(triggered()), this, SLOT(setPartitonNum()) );

    networkBandAction = new QAction( tr("网络带宽"), this);

    networkCapacityAction = new QAction( tr("网络吞吐量"), this);
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
  Description: 打开项目槽函数
  Parameter: 无
  Return: 无
*************************************************/
void NetworkSimulationPlatform::openNetworkItem( void )
{
	//弹出打开项目对话框

}

/*************************************************
  Function: saveItem
  Description: 保存项目槽函数
  Parameter: 无
  Return: 无
*************************************************/
void NetworkSimulationPlatform::saveItem(void)
{
    m_saveFlag = true;
	//获取保存数据
    if(m_projectName.isEmpty())
    {
        m_saveFile->exec();
        slotSaveFile();
    }
    else
        slotSaveFile();
	//保存
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
  Description: 开始仿真槽函数槽函数
  Parameter:
  Return:
*************************************************/
void NetworkSimulationPlatform::startSimulate(void)
{
    if (!m_saveFlag)
    {
        QMessageBox::information(this, tr("提示"),tr("请先保存拓扑信息!"),QMessageBox::Ok);
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

/*************************************************
  Function: exportTopology
  Description: 导出网络拓扑槽函数
  Parameter: 无
  Return: 无
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
  Description: 导入网络拓扑槽函数
  Parameter: 无
  Return: 无
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
    //根据解析出来的数据画出网络拓扑
    ui_editWidget->InputTopology(filePath);
    m_saveFlag = false;
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

/*************************************************
  Function: createToolBar
  Description: 创建工具栏
  Parameter: 无
  Return: 无
*************************************************/
void NetworkSimulationPlatform::createToolBar( void )
{
	mainToolBar->addAction(newNetworkItemAction);
	mainToolBar->addAction(openNetworkItemAction);
    mainToolBar->addAction(saveItemAction);
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
    //menu_startMenu->addAction(startSimulateAction);
    //menu_startMenu->addAction(importSNMPTopologyAction);
    //menu_startMenu->addAction(importBackgroundAction);
    //menu_startMenu->addAction(deleteBackgroundAction);
    menu_startMenu->addAction(exitAction);

    menu_networkEditMenu = QMainWindow::menuBar()->addMenu(tr("编辑"));
    menu_networkEditMenu->addAction(newNetworkItemAction);
    menu_networkEditMenu->addAction(openNetworkItemAction);
    menu_networkEditMenu->addAction(saveItemAction);
    menu_networkEditMenu->addAction(zoomInAction);
    menu_networkEditMenu->addAction(zoomOutAction);

    //menu_networkEditMenu->addAction(exportTopologyAction);
    //menu_networkEditMenu->addAction(importTopologyAction);
    menu_networkEditMenu->addAction(clearseceneAction);

//    menu_configNetworkMenu = menu_networkEditMenu->addMenu(tr("配置"));
//    menu_configNetworkMenu->addAction(linkSetAction);
//    menu_configNetworkMenu->addAction(flowSetAction);
//    menu_configNetworkMenu->addAction(partitionNumAction);

    menu_toolMenu = menu_networkEditMenu->addMenu(tr("工具"));
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

/************************************************************
  Copyright (C), 2014-2015, 电子科技大学.
  FileName: NetworkSimulationPlatform.h
  Author: 程诚
  Version: 1.0
  Date: 2014.7.14
  Description: 程序主界面   
***********************************************************/

#ifndef NETWORKSIMULATIONPLATFORM_H
#define NETWORKSIMULATIONPLATFORM_H

#include <QtGui/QMainWindow>
#include <QDockWidget>
#include <QListWidget>
#include <QToolBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include "StructFile.h"
#include "CUnitboxWidget.h"
#include "CNetworkEditWidget.h"
#include "CWizard.h"
#include "ui_NetworkSimulationPlatform.h"

using namespace std;

class NetworkSimulationPlatform : public QMainWindow, public Ui_NetworkSimulationPlatform
{
	Q_OBJECT

	public:
		NetworkSimulationPlatform(QWidget *parent = 0, Qt::WFlags flags = 0);
		~NetworkSimulationPlatform();

	private:
        void init(void);
        void createActions(void);
        void createMenu(void);
        void createToolBar(void);
        void createNewTooBar(void);
        void createDockWindow(void);

	private slots:
		//网络编辑模块槽函数
        void createNewNetworkItem(void);
		void openNetworkItem(void);
        void saveItem(void);
        void startSimulate(void);
		void exportTopology(void);
		void importTopology(void);
        void clearScene(void);
		void setFlow(void);
		void setLink(void);
		void zoomIn(void);
		void zoomOut(void);
		void layoutAlgorithm(void);
        void slotNewFile(void);
        void slotSaveFile(void);
        void importSNMPTopology(void);
        void configTask(void);
        void setPartitonNum(void);
        void eventSchedul(void);

	private:
		QAction *newNetworkItemAction;
		QAction *openNetworkItemAction;
		QAction *saveItemAction;
		QAction *exitAction;
		QAction *zoomInAction;
		QAction *zoomOutAction;
		QAction *layoutAction;
		QAction *exportTopologyAction;
		QAction *importTopologyAction;
        QAction *clearseceneAction;
		QAction *flowSetAction;
		QAction *linkSetAction;
        QAction *networkBandAction;
        QAction *networkCapacityAction;
        QAction *startSimulateAction;
        QAction *importSNMPTopologyAction;
        QAction *importBackgroundAction;
        QAction *deleteBackgroundAction;
        QAction *configTaskAction;
        QAction *partitionNumAction;
        QAction *eventSchedulAction;

        QMenu *menu_startMenu;
        QMenu *menu_networkEditMenu;
        QMenu *menu_networkStateMenu;
        QMenu *menu_editTopologyMenu;
        QMenu *menu_configNetworkMenu;
        QMenu *menu_toolMenu;
        QMenu *menu_configTaskMenu;

        QWidget *ui_networkDeviceWidget;
        QWidget *ui_routerDevice;
        QWidget *ui_PCDevice;
        QDockWidget *ui_historyDock;
        QDockWidget *ui_networkDeviceDock;
        QTextEdit *historyListWidget;
        QToolBox *ui_networkToolBox;
        QGridLayout* ui_toolBoxGridLayout;
        CUnitboxWidget* ui_unitboxwidget;

        CTopologyWidget *ui_editWidget;

        QGraphicsScene *m_scene;
        CWizard *m_newFile;
        CWizard *m_saveFile;
        CWizard *m_saveTopology;
        QString m_saveTopologyFileName;
        QString m_saveTopologyFilePath;
        QString m_NS3Path;
        QString m_currentPath;

        QString m_projectName;
        bool m_saveFlag;
        int m_partitionNum;
};

#endif // NETWORKSIMULATIONPLATFORM_H

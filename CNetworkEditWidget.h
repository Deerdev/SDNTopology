#ifndef _NETWORKTOPLOGYWIDGET_H
#define _NETWORKTOPLOGYWIDGET_H

#include <QDockWidget> 
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QtGui>
#include <QMouseEvent>
#include "CNewSwitcher.h"
#include "CPortDialog.h"
#include "CWizard.h"
#include "Device/CSwitherConfigure.h"
#include "Device/CSwitcher.h"
#include "Device/CItemGroup.h"
#include "Device/CNetworkPort.h"
#include "Clink.h"
#include "CUnitboxWidget.h"
#include "BoundingRectItem.h"
#include <set>
#include "DragNetwork.h"
#include "CTaskConfigDialog.h"
#include "CEventscheduldialog.h"

class QDragEnterEvent;
class QDropEvent;

class NetworkSimulationPlatform;

class CTopologyWidget : public QGraphicsView
{
	Q_OBJECT
	private:
		QGraphicsScene *m_scene;
        NetworkSimulationPlatform *m_platform;
	public:
        CTopologyWidget(QWidget *parent=0);
		~CTopologyWidget();
        QGraphicsScene* getScene(){return m_scene;}

		QAction *getActionm_deleteAct() {return m_deleteAct;}
		QAction *getActionm_configuerAct() {return m_configuerAct;}

        //更新拓扑
        void RefreshTopology();
        //清空画布
        void clearScene(void);

        void setPlatformPointer(NetworkSimulationPlatform*_platform);

        //=====================================
		//输入拓扑
        void InputTopology(QString path);
		//输出拓扑
        void OutputTopology(QString path);
        //输出节点位置
        void OutputPosition(QString path);
        //解析SNMP构建网络拓扑
        void ConstructTopology(const QString& _path);
        //配置任务
        void configTask();
        //事件调度
        void eventSchedul();
        //获取连接信息
		vector<int> GetConnectedInfo(CSwitcherInfo switchInfo, QStringList vName);
        //设置放图片标签的窗口
        void setLabelWidget(CUnitboxWidget* _unitW);

	private slots:
        //右键菜单按钮对应的槽函数

        //删除
		void slotDeleteAction();
        //查看属性
        void slotAttributeAction();

        //================================
//        void slotConfigureAction();
        void slotRouteVisible();
        void slotDelayAction();

        void slotCopyAction();
        void slotPasteAction();

        void slotNewRouterDia();
		void slotNewDeviceDia();
        void slotDrawLine();
		void slotSwitchDia();

	signals:
		void SendDeviceName(int devID);
		void SendSwitchID(int);

	private:
        //拖拽事件
        void dragEnterEvent(QDragEnterEvent *event);
        void dragMoveEvent(QDragMoveEvent *event);
        void dropEvent(QDropEvent *event);

        //鼠标事件
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void mouseDoubleClickEvent(QMouseEvent *event);
        void wheelEvent(QWheelEvent *event);

        void contextMenuEvent(QContextMenuEvent * event);

        //键盘事件
        void keyPressEvent( QKeyEvent * event );
        //鼠标变形
        void cursorShapeChange(QPointF _scenePos,CBoundingRectItem* _brItem);
        //鼠标点中边界小矩形并移动时创建可移动的虚框
        void CreateMovableRect(QPointF _scenePos);
        //创建右键菜单
        void creatRightMenu();
        //删除子图元
        void deleteChildItems(QGraphicsItem*);

        //calculate network id
        QString calculateNetworkID(QString _IP, QString _mask);
        QString DecToHex(QString AreaID, QString portID);

        void setSwitchInfTable(const CSwitcherInfo &_switcherInfo);
        void setHostInfTable(const CSwitcherInfo &_switcherInfo);

	private:
        QString g_projectpath;

        CUnitboxWidget* m_unitWidget; //放图片标签的窗口
        QPointF m_pressScenePoint_formove;//鼠标点击的scene位置
        CNetworkPort* m_pressItem;//鼠标点击的图元
        CNetworkPort* m_releaseItem;//鼠标释放时的图元
        Clink* link;
		QPointF m_sceneFormerPoint; //鼠标点击的scene位置  在move中变化
		QString m_name; //拖动的标签名字
		QPointF m_dropPos;

        //交换机长宽
        double m_sWidth;   //交换机宽度
        double m_sHeight;   //交换机高度
        double m_width;       //设备宽度加link宽度
        double m_linkLen;   //link的长度
		double m_dWidth;  //设备宽度

        QGraphicsPixmapItem *m_backItem; //背景图元

		CItemGroup* m_itemGroup;
		bool m_moveitemFlag;  //

		QGraphicsRectItem* m_dragRectItem;  //框选的矩形框
        QGraphicsLineItem* m_tempadge;   //临时虚线

		//菜单和按钮
		QMenu *m_rightMenu;
		QMenu *m_rightMenu0;
		QAction *m_linkAct;
		QAction *m_unlinkAct;

        QAction *m_copyAct;
        QAction *m_pasteAct;

		QAction *m_deleteAct;
		QAction *m_attributeAct;

		QAction *m_configuerAct;
        QAction *m_routeVisible;
        QAction *m_delayAct;
		QAction *m_onlineAct;//上线
		QAction *m_offlineAct;//下线
		QAction *m_deviceStatus;//设备状态

        //添加图元的对话框
		CNewSwitcher* m_newSwitcherDia;
        CPortDialog* m_newPortDia;

 		CSwitcher* m_EditSwitchItem;
        CNetworkPort* m_EditPortItem;

		//已经用过的节点域
		QVector<int> m_usedPortID;

        //右键属性对话框
 		CSwitherConfigure *m_switchDia;

		vector<CSwitcherInfo> m_vSwtich;

		//导入导出使用
 		vector<CSwitcher*> t_vSwitchInOut;
 		vector<CNetworkPort*> t_vPortInOut;

        //IP地址池
        set<QString> t_IPPond;
        map<QString,int> m_IPPool;

		//标识是否显示端口信息
		bool m_vShowFlag;
		vector<QGraphicsTextItem*> t_vTextItems;

		vector <int> m_vDeviceNums;

        QVector<Clink *>  m_vlink;
		bool rect0sele,rect1sele,rect2sele,rect3sele,rect4sele,rect5sele,rect6sele,rect7sele;//选中后的8个小矩形的选中状态
	private:
		int m_zoom;
        bool resize;
        void drawLegend();
	public slots:
		void zoomIn();
		void zoomOut();
};

#endif



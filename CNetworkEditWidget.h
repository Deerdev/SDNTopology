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

        //��������
        void RefreshTopology();
        //��ջ���
        void clearScene(void);

        void setPlatformPointer(NetworkSimulationPlatform*_platform);

        //=====================================
		//��������
        void InputTopology(QString path);
		//�������
        void OutputTopology(QString path);
        //����ڵ�λ��
        void OutputPosition(QString path);
        //����SNMP������������
        void ConstructTopology(const QString& _path);
        //��������
        void configTask();
        //�¼�����
        void eventSchedul();
        //��ȡ������Ϣ
		vector<int> GetConnectedInfo(CSwitcherInfo switchInfo, QStringList vName);
        //���÷�ͼƬ��ǩ�Ĵ���
        void setLabelWidget(CUnitboxWidget* _unitW);

	private slots:
        //�Ҽ��˵���ť��Ӧ�Ĳۺ���

        //ɾ��
		void slotDeleteAction();
        //�鿴����
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
        //��ק�¼�
        void dragEnterEvent(QDragEnterEvent *event);
        void dragMoveEvent(QDragMoveEvent *event);
        void dropEvent(QDropEvent *event);

        //����¼�
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void mouseDoubleClickEvent(QMouseEvent *event);
        void wheelEvent(QWheelEvent *event);

        void contextMenuEvent(QContextMenuEvent * event);

        //�����¼�
        void keyPressEvent( QKeyEvent * event );
        //������
        void cursorShapeChange(QPointF _scenePos,CBoundingRectItem* _brItem);
        //�����б߽�С���β��ƶ�ʱ�������ƶ������
        void CreateMovableRect(QPointF _scenePos);
        //�����Ҽ��˵�
        void creatRightMenu();
        //ɾ����ͼԪ
        void deleteChildItems(QGraphicsItem*);

        //calculate network id
        QString calculateNetworkID(QString _IP, QString _mask);
        QString DecToHex(QString AreaID, QString portID);

        void setSwitchInfTable(const CSwitcherInfo &_switcherInfo);
        void setHostInfTable(const CSwitcherInfo &_switcherInfo);

	private:
        QString g_projectpath;

        CUnitboxWidget* m_unitWidget; //��ͼƬ��ǩ�Ĵ���
        QPointF m_pressScenePoint_formove;//�������sceneλ��
        CNetworkPort* m_pressItem;//�������ͼԪ
        CNetworkPort* m_releaseItem;//����ͷ�ʱ��ͼԪ
        Clink* link;
		QPointF m_sceneFormerPoint; //�������sceneλ��  ��move�б仯
		QString m_name; //�϶��ı�ǩ����
		QPointF m_dropPos;

        //����������
        double m_sWidth;   //���������
        double m_sHeight;   //�������߶�
        double m_width;       //�豸��ȼ�link���
        double m_linkLen;   //link�ĳ���
		double m_dWidth;  //�豸���

        QGraphicsPixmapItem *m_backItem; //����ͼԪ

		CItemGroup* m_itemGroup;
		bool m_moveitemFlag;  //

		QGraphicsRectItem* m_dragRectItem;  //��ѡ�ľ��ο�
        QGraphicsLineItem* m_tempadge;   //��ʱ����

		//�˵��Ͱ�ť
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
		QAction *m_onlineAct;//����
		QAction *m_offlineAct;//����
		QAction *m_deviceStatus;//�豸״̬

        //���ͼԪ�ĶԻ���
		CNewSwitcher* m_newSwitcherDia;
        CPortDialog* m_newPortDia;

 		CSwitcher* m_EditSwitchItem;
        CNetworkPort* m_EditPortItem;

		//�Ѿ��ù��Ľڵ���
		QVector<int> m_usedPortID;

        //�Ҽ����ԶԻ���
 		CSwitherConfigure *m_switchDia;

		vector<CSwitcherInfo> m_vSwtich;

		//���뵼��ʹ��
 		vector<CSwitcher*> t_vSwitchInOut;
 		vector<CNetworkPort*> t_vPortInOut;

        //IP��ַ��
        set<QString> t_IPPond;
        map<QString,int> m_IPPool;

		//��ʶ�Ƿ���ʾ�˿���Ϣ
		bool m_vShowFlag;
		vector<QGraphicsTextItem*> t_vTextItems;

		vector <int> m_vDeviceNums;

        QVector<Clink *>  m_vlink;
		bool rect0sele,rect1sele,rect2sele,rect3sele,rect4sele,rect5sele,rect6sele,rect7sele;//ѡ�к��8��С���ε�ѡ��״̬
	private:
		int m_zoom;
        bool resize;
        void drawLegend();
	public slots:
		void zoomIn();
		void zoomOut();
};

#endif



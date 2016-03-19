#ifndef _SWITCHER_H
#define _SWITCHER_H
/************************************************************************
模块描述:  交换机设备
************************************************************************/
#include "CBaseObject.h"
#include "../StructFile.h"

class CSwitcher: public CBaseObject
{
public:	
	CSwitcher(QGraphicsItem *parent, QGraphicsScene *scene);
	~CSwitcher();
	enum{ Type = QGraphicsItem::UserType +1};
	int type()const{return Type;}
    int *m_Szoom;

public:
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
	QRectF boundingRect() const;
	void mousedoubleclick();
	void setBoundingRect(QRectF _rect){m_boundingRect = _rect;prepareGeometryChange();boundingRect();}

	void setSelectedFlag(bool _flag){m_selectedFlag = _flag;}//选中标志
	bool getSelectedFlag(){return m_selectedFlag;}

    void setSwitcherInfo(const CSwitcherInfo &_switcherInfo){m_switcherInfo = _switcherInfo;}
	CSwitcherInfo &getSwitcherInfo(){return m_switcherInfo;}

	//添加已经连接的节点
    void addLinkedNode(SNodeStructInfo _nodeInfo){m_switcherInfo.LNodes.append(_nodeInfo);}
    void highLightSwitch(CSwitcherInfo &_switchInfo);
//	void hightLightDevice( bool flag = true);


private:
	QGraphicsScene* m_scene;
	QRectF m_boundingRect;
	bool m_selectedFlag;

	CSwitcherInfo   m_switcherInfo;
	QColor m_color;



	//级联端口名称
//	vector<QString *>   m_allCascadePortName;

};

#endif // _SWITCHER_H

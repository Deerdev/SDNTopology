#ifndef _SWITCHER_H
#define _SWITCHER_H
/************************************************************************
ģ������:  �����豸
************************************************************************/
#include "CBaseObject.h"
#include "../StructFile.h"

class CHostItem: public CBaseObject
{
public:
    CHostItem(QGraphicsItem *parent, QGraphicsScene *scene);
    ~CHostItem();
    enum{ Type = QGraphicsItem::UserType +1};
    int type()const{return Type;}
    int *m_Szoom;

public:
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    QRectF boundingRect() const;
    void mousedoubleclick();
    void setBoundingRect(QRectF _rect){m_boundingRect = _rect;prepareGeometryChange();boundingRect();}

    void setSelectedFlag(bool _flag){m_selectedFlag = _flag;}//ѡ�б�־
    bool getSelectedFlag(){return m_selectedFlag;}

    void setSwitcherInfo(const CHostItemInfo &_switcherInfo){m_switcherInfo = _switcherInfo;}
    CHostItemInfo &getSwitcherInfo(){return m_switcherInfo;}

    //����Ѿ����ӵĽڵ�
    void addLinkedNode(SNodeStructInfo _nodeInfo){m_switcherInfo.LNodes.append(_nodeInfo);}
    void highLightSwitch(CHostItemInfo &_switchInfo);
//	void hightLightDevice( bool flag = true);


private:
    QGraphicsScene* m_scene;
    QRectF m_boundingRect;
    bool m_selectedFlag;


    CHostInfo m_hostInfo;
    QColor m_color;


};

#endif // _SWITCHER_H

#ifndef _NETWORKPORT_H
#define _NETWORKPORT_H
/************************************************************************
模块描述:  网口模块
************************************************************************/

#include "CBaseObject.h"
#include <QPainter>
#include <QGraphicsItem>
#include "../StructFile.h"


class CNetworkPort: public CBaseObject
{
public:	
	CNetworkPort(QGraphicsItem *parent, QGraphicsScene *scene);
	~CNetworkPort(); 

     enum{ Type = QGraphicsItem::UserType + 2};
     int type() const {return Type;}
     int *m_Pzoom;
public:
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    QRectF boundingRect() const;
    void setBoundingRect(QRectF _rect){m_boundingRect = _rect;}

    void setSelectedFlag(bool _flag){m_selectedFlag = _flag;}//选中标志
    bool getSelectedFlag(){return m_selectedFlag;}

    void setID(int _swichID){ID = _swichID;}
    int getID(void){return ID;}
private:
     QRectF m_boundingRect;
     QGraphicsScene* m_scene;
     QPointF portPos;
     bool m_selectedFlag;
     int ID;
};

#endif // _BASEOBJECT_H

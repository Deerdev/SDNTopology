#ifndef CLINK_H
#define CLINK_H

#include<QGraphicsItem>
#include "Device/CNetworkPort.h"
#include "StructFile.h"
#include "Device/CSwitcher.h"
#include "math.h"


class Clink:public QGraphicsItem
{
public:
    Clink( QGraphicsScene *scene, CNetworkPort *fromPort, CNetworkPort *toPort);
    ~Clink();
    enum{ Type = QGraphicsItem::UserType +4};
    int type()const{return Type;}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect () const;
    QPainterPath shape() const;

    CNetworkPort* getStartPort(){return myfromPort;}
    CNetworkPort* getEndPort(){return mytoPort;}

    void setStartPort(CNetworkPort* _s){ myfromPort = _s;}
    void setEndPort(CNetworkPort* _e){ mytoPort = _e;}

    void setSelectedFlag(bool _flag){m_selectedFlag = _flag;}//选中标志
    bool getSelectedFlag(){return m_selectedFlag;}

    void setID(int _switchID, int _linkswitchID);
    CLinkInfo& getLinkInfo(){return m_linkInfo;}
    void  setLinkInfo(CLinkInfo _linkInfo);

private:
    QGraphicsScene* m_scene;
    CLinkInfo   m_linkInfo;

    CNetworkPort *myfromPort;
    CNetworkPort *mytoPort;

    QPointF m_sP;
    QPointF m_eP;

    QRectF m_boundingRect;

    bool m_selectedFlag;


};

#endif // CLINK_H

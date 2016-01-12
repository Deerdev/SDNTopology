#include "CNetworkPort.h"
#define  precision 0.00000001
 
CNetworkPort::CNetworkPort(QGraphicsItem *parent, QGraphicsScene *scene):CBaseObject(parent,scene)
{
    m_scene = scene;
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    m_boundingRect = QRectF(-10, -10, 20, 20);
}

CNetworkPort::~CNetworkPort()
{

}
void CNetworkPort::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /* = 0 */)
{

//    if(*m_Pzoom < 0)
//    {
//        painter->translate(500, 500);
//    }
    qreal s_portSize = 10;
    QRadialGradient gradient(0, 0, s_portSize/2);
    gradient.setCenter(0, 0);
    gradient.setFocalPoint(0, 0); //¾Û½¹
//    qreal pS = 0.0001;
//    qreal pE = 0.9999;
//    switch(m_state)
//    {
//        case Normal:
//            gradient.setColorAt(pE, QColor(200, 200, 200).light(120));
//            gradient.setColorAt(pS, QColor(Qt::white).light(120));
//            break;
//        case Selected:
//            gradient.setColorAt(pE, QColor(Qt::yellow).light(150));
//            gradient.setColorAt(pS, QColor(Qt::white).light(120));
//            break;
//        case CanBeConnected:
//            gradient.setColorAt(pE, QColor(Qt::green).light(120));
//            gradient.setColorAt(pS, QColor(Qt::white).light(120));
//            break;
//        default:
//            break;
//    }
    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 1));

    //draw port
    painter->drawEllipse(-s_portSize/2, -s_portSize/2, s_portSize, s_portSize);

}

QRectF CNetworkPort::boundingRect() const
{
    /*QRectF t_rect(m_boundingRect.topLeft().x() - 2,m_boundingRect.topLeft().y() - 2,m_boundingRect.width() + 10,m_boundingRect.height() + 10);
    return t_rect;*/
    return m_boundingRect;
}


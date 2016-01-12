#include "Clink.h"

const int maxBandwidth = 1000;
const int minBandwidth = 0;
const int maxPenWidth = 6;
const int minPenWidth = 2;
const int linkInterval = 6;
QColor PenColor[] = {Qt::transparent, Qt::black, Qt::red, Qt::blue, Qt::magenta, Qt::cyan};

Clink::Clink( QGraphicsScene *scene, CNetworkPort *fromPort, CNetworkPort *toPort)
{
    m_scene = scene;

    myfromPort = fromPort;
    mytoPort = toPort;

    m_sP = myfromPort->parentItem()->mapToScene(myfromPort->mapToParent(QPointF(0,0)));
    m_eP = mytoPort->parentItem()->mapToScene(mytoPort->mapToParent(QPointF(0,0)));

    m_selectedFlag = false;

    setZValue(-1000);

}

Clink::~Clink()
{

}

QPainterPath Clink::shape() const
{
    QPainterPath t_shape;
    t_shape.moveTo(m_sP);
    t_shape.lineTo(m_eP);
    return  t_shape;
}

QRectF Clink::boundingRect () const
{
    return QRectF(0,0,1500 , 1500);
}

void Clink::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //set maxBandwidth = 1000 minBandwidth = 0
    //then (bandwidth-minBandwidth)/(maxBandwidth - minBandwidth)=(penWidth-minPenWidth)/(maxPenWidth-minPenWidth)
    painter->setRenderHint(QPainter::Antialiasing, true);
    QPen t_pen(Qt::black);

    if(m_linkInfo.linkType == POS)
    {
        t_pen.setColor(PenColor[m_linkInfo.pos_link_num]);
        t_pen.setStyle(Qt::DashLine);
    }
    else if(m_linkInfo.linkType == Ethernet)
    {
        t_pen.setColor(PenColor[m_linkInfo.ethernet_link_num]);
        t_pen.setStyle(Qt::SolidLine);
    }
    else
    {
        t_pen.setStyle(Qt::DashDotDotLine);
    }

    int bandwidth = m_linkInfo.bandWidth.toInt();
    int penWidth = (bandwidth-minBandwidth)*(maxPenWidth-minPenWidth)/(maxBandwidth - minBandwidth) + minPenWidth;
    t_pen.setWidth(penWidth);
    painter->setPen(t_pen);

    /*
    if(fabs( m_eP.x() - m_sP.x() ) >= fabs( m_eP.y() - m_sP.y() ) )
    {
        int posY = m_linkInfo.link_num/2 * linkInterval * pow(-1, m_linkInfo.link_num-1);
        m_sP= myfromPort->parentItem()->mapToScene(myfromPort->mapToParent(QPointF(0,posY)));
        m_eP = mytoPort->parentItem()->mapToScene(mytoPort->mapToParent(QPointF(0,posY)));
    }
    else
    {
        int posX = m_linkInfo.link_num/2 * linkInterval * pow(-1, m_linkInfo.link_num);
        m_sP= myfromPort->parentItem()->mapToScene(myfromPort->mapToParent(QPointF(posX,0)));
        m_eP = mytoPort->parentItem()->mapToScene(mytoPort->mapToParent(QPointF(posX,0)));
    }
    */
    if (m_linkInfo.linkType == POS)
    {
        m_sP= myfromPort->parentItem()->mapToScene(myfromPort->mapToParent(QPointF(0,0)));
        CSwitcher* swicher = dynamic_cast<CSwitcher*>(mytoPort->parentItem() );
        CSwitcherInfo swichInfo = swicher->getSwitcherInfo();
        int id = swichInfo.ID;
        qDebug()<<id<<endl;
        m_eP = mytoPort->parentItem()->mapToScene(mytoPort->mapToParent(QPointF(0,0)));

    }
    else if(m_linkInfo.linkType == Ethernet)
    {
        if(fabs( m_eP.x() - m_sP.x() ) >= fabs( m_eP.y() - m_sP.y() ) )
        {
            m_sP= myfromPort->parentItem()->mapToScene(myfromPort->mapToParent(QPointF(0,-10)));
            m_eP = mytoPort->parentItem()->mapToScene(mytoPort->mapToParent(QPointF(0,-10)));
        }
        else
        {
            m_sP= myfromPort->parentItem()->mapToScene(myfromPort->mapToParent(QPointF(10,0)));
            m_eP = mytoPort->parentItem()->mapToScene(mytoPort->mapToParent(QPointF(10,0)));
        }

    }

    painter->drawLine(m_sP,m_eP);
}

void Clink::setID(int _switchID, int _linkswitchID)
{
    m_linkInfo.s_switchID = _switchID;
    m_linkInfo.e_switchID = _linkswitchID;
}

void Clink::setLinkInfo(CLinkInfo _linkInfo)
{
    m_linkInfo = _linkInfo;
}

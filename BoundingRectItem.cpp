#include "BoundingRectItem.h"
#include <QPen>
#include <QGraphicsRectItem>

CBoundingRectItem::CBoundingRectItem(QGraphicsItem *parent, QGraphicsScene *scene):QGraphicsItem(parent,scene)
{
	m_scene = scene;
	m_rectItem0 = NULL;
	m_rectItem1 = NULL;
	m_rectItem2 = NULL;
	m_rectItem3 = NULL;
	m_rectItem4 = NULL;
	m_rectItem5 = NULL;
	m_rectItem6 = NULL;
	m_rectItem7 = NULL;
}


CBoundingRectItem::~CBoundingRectItem()
{
	
}

void CBoundingRectItem::drawBoundingItem()
{
 	if (m_rectItem0 != NULL)
	{
		delete m_rectItem0 ;
		m_rectItem0 = NULL;
	}

	if (m_rectItem1 != NULL)
	{
		delete m_rectItem1;
		m_rectItem1 = NULL;
	}

	if (m_rectItem2 != NULL)
	{
		delete m_rectItem2;
		m_rectItem2 = NULL;
	}

	if (m_rectItem3 != NULL)
	{
		delete m_rectItem3;
		m_rectItem3 = NULL;
	}

	if (m_rectItem4 != NULL)
	{
		delete m_rectItem4;
		m_rectItem4 = NULL;
	}

	if (m_rectItem5 != NULL)
	{
		delete m_rectItem5;
		m_rectItem5 = NULL;
	}

	if (m_rectItem6 != NULL)
	{
		delete m_rectItem6;
		m_rectItem6 = NULL;
	}

	if (m_rectItem6 != NULL)
	{
		delete m_rectItem6;
		m_rectItem6 = NULL;
	}

	if (m_rectItem7 != NULL)
	{
		delete m_rectItem7;
		m_rectItem7 = NULL;
	}

	QRectF t_rect(m_boundingRect.topLeft().x() - 3,m_boundingRect.topLeft().y() - 3,m_boundingRect.width() + 6,m_boundingRect.height() + 6);

	m_rectItem0 = new QGraphicsRectItem(QRectF(t_rect.topLeft().x()-3, t_rect.topLeft().y()-3,6,6) , this); 
	m_rectItem0->setBrush(QBrush(Qt::green));

	m_rectItem1 = new QGraphicsRectItem(QRectF(t_rect.topLeft().x()+t_rect.width()-5, t_rect.topLeft().y()-3, 6,6) ,this); 
	m_rectItem1->setBrush(QBrush(Qt::green));

	m_rectItem2= new QGraphicsRectItem(QRectF(t_rect.topLeft().x()-3, t_rect.topLeft().y()+t_rect.height()-3,6,6) , this); 
	m_rectItem2->setBrush(QBrush(Qt::green));

	m_rectItem3 =  new  QGraphicsRectItem(QRectF(t_rect.topLeft().x()+t_rect.width()-3, t_rect.topLeft().y()+t_rect.height()-3, 6,6) ,this); 
	m_rectItem3->setBrush(QBrush(Qt::green));

	m_rectItem4 =  new  QGraphicsRectItem(QRectF(t_rect.topLeft().x()+t_rect.width()/2.0-3, t_rect.topLeft().y()-3, 6,6) , this); 
	m_rectItem4->setBrush(QBrush(Qt::green));	

	m_rectItem5 =  new QGraphicsRectItem(QRectF(t_rect.topLeft().x()-4, t_rect.topLeft().y()+t_rect.height()/2.0-3,6,6),this); 
	m_rectItem5->setBrush(QBrush(Qt::green));

	m_rectItem6 =  new QGraphicsRectItem(QRectF(t_rect.topLeft().x()+t_rect.width()/2.0-3, t_rect.topLeft().y()+t_rect.height()-2,6,6), this); 
	m_rectItem6->setBrush(QBrush(Qt::green));

	m_rectItem7=  new QGraphicsRectItem(QRectF(t_rect.topLeft().x()+t_rect.width()-2, t_rect.topLeft().y()+t_rect.height()/2.0-3, 6,6) ,this); 
	m_rectItem7->setBrush(QBrush(Qt::green));
}
void CBoundingRectItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget  ) 
{
	//ÐéÏß±ß¿ò
//    if(*m_Bzoom < 0)
//    {
//        painter->translate(50,50);
//    }
	QPen t_pen;
	t_pen.setStyle(Qt::DotLine);
	painter->setPen(t_pen);
	QRectF t_rect(m_boundingRect.topLeft().x() - 4,m_boundingRect.topLeft().y() - 4,m_boundingRect.width() + 8,m_boundingRect.height() + 8);
	painter->drawRect(t_rect);
}
QRectF CBoundingRectItem::boundingRect() const
{
	QRectF t_rect(m_boundingRect.topLeft().x() - 4,m_boundingRect.topLeft().y() - 4,m_boundingRect.width() + 8,m_boundingRect.height() + 8);
	return t_rect;
}

#pragma  once
/************************************************************************
ģ������:  �豸ģ��
************************************************************************/
#include <QPainter>
#include <QGraphicsItemGroup>

class CItemGroup: public QGraphicsItemGroup
{
public:
	CItemGroup(QGraphicsItem *parent = 0,QGraphicsScene* _scene = 0);
	~CItemGroup();

	QPainterPath shape() const;


private:
	QRectF m_boundingRect;
	QGraphicsScene* m_scene;
};
#pragma once
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsRectItem>
class CBoundingRectItem : public QGraphicsItem
{
public:
	CBoundingRectItem(QGraphicsItem *parent, QGraphicsScene *scene);
	~CBoundingRectItem();
	enum{ Type = QGraphicsItem::UserType +3};
	int type()const{return Type;}

    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 ) ;
	QRectF boundingRect() const;
	void setBoundingRect(QRectF _rect){m_boundingRect = _rect;}
	void drawBoundingItem();
    int *m_Bzoom;


private:
	QGraphicsScene* m_scene;
	QRectF m_boundingRect;

	QGraphicsRectItem *m_rectItem0;
	QGraphicsRectItem *m_rectItem1;
	QGraphicsRectItem *m_rectItem2;
	QGraphicsRectItem *m_rectItem3;

	QGraphicsRectItem *m_rectItem4;
	QGraphicsRectItem *m_rectItem5;
	QGraphicsRectItem *m_rectItem6;
	QGraphicsRectItem *m_rectItem7;

};


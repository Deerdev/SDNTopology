#ifndef _BASEOBJECT_H
#define _BASEOBJECT_H

#include <QGraphicsItem>
#include <QString>
#include <vector>
#include <QPointF>
#include <QRectF>
#include <QtGui>

using namespace std;


/************************************************************************
模块描述: 网络节点基类
************************************************************************/

class CBaseObject : public QGraphicsItem
{
private:
	//对象名称
	QString                                          m_name;

	//功能描述
	QString                                          m_description;

	//设备位置
	QPointF                                        m_pos;

	//设备的长度和宽度
	float                                    m_width,m_height;
public:	
	//设置对象名称
	void SetName(QString name){m_name = name;}
	
	//获取对象名称
	QString GetName(){return m_name;} 

	//设置对象名称
	void SetDescription(QString des){m_description = des;}

	//获取对象名称
	QString GetDescription(){return m_description;} 

	//获取设备坐标
	QPointF GetPos() 
	{
		QRectF re = boundingRect();
        QPointF tp(pos().x() + re.width()/2,pos().y()  +re.height()/2);
		return tp;
	}

	//获取宽度和长度
	float GetWid(){m_width = boundingRect().width();  return m_width;}
	float GetHig(){m_height = boundingRect().height();  return m_height;}


public:
	CBaseObject(QGraphicsItem *parent, QGraphicsScene *scene):QGraphicsItem(parent,scene){};
	~CBaseObject(){};

	virtual 	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0){};
	virtual QRectF boundingRect() const{return QRectF(0.0,0.0,1,1);};
};

#endif // _BASEOBJECT_H

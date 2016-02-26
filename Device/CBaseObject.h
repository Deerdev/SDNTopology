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
ģ������: ����ڵ����
************************************************************************/

class CBaseObject : public QGraphicsItem
{
private:
	//��������
	QString                                          m_name;

	//��������
	QString                                          m_description;

	//�豸λ��
	QPointF                                        m_pos;

	//�豸�ĳ��ȺͿ��
	float                                    m_width,m_height;
public:	
	//���ö�������
	void SetName(QString name){m_name = name;}
	
	//��ȡ��������
	QString GetName(){return m_name;} 

	//���ö�������
	void SetDescription(QString des){m_description = des;}

	//��ȡ��������
	QString GetDescription(){return m_description;} 

	//��ȡ�豸����
	QPointF GetPos() 
	{
		QRectF re = boundingRect();
        QPointF tp(pos().x() + re.width()/2,pos().y()  +re.height()/2);
		return tp;
	}

	//��ȡ��Ⱥͳ���
	float GetWid(){m_width = boundingRect().width();  return m_width;}
	float GetHig(){m_height = boundingRect().height();  return m_height;}


public:
	CBaseObject(QGraphicsItem *parent, QGraphicsScene *scene):QGraphicsItem(parent,scene){};
	~CBaseObject(){};

	virtual 	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0){};
	virtual QRectF boundingRect() const{return QRectF(0.0,0.0,1,1);};
};

#endif // _BASEOBJECT_H

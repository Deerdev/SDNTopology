#include "CSwitcher.h"
#include<QFont>
#include <QFontMetrics>
#include <QMessageBox>


CSwitcher::CSwitcher(QGraphicsItem *parent, QGraphicsScene *scene):CBaseObject(parent,scene)
{
	m_scene = scene;
    m_boundingRect = QRectF(0,0, 60,60);
	setFlag(QGraphicsItem::ItemIsSelectable,true);
	m_selectedFlag = false;
//	m_cascadeFlag = false;

    m_switcherInfo.name = "Switch";
	m_color = QColor(205,193,197);
}

CSwitcher::~CSwitcher()
{

}

void CSwitcher::highLightSwitch(CSwitcherInfo &_switchInfo)
{

}

void CSwitcher::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    QPen t_pen;
    painter->setPen(t_pen);
    QBrush t_brush(Qt::white);  //笔刷填充颜色：白色
	painter->setBrush(t_brush);

    painter->save();  //保存当前painter状态
	painter->setRenderHint(QPainter::Antialiasing, true);
    QRectF rectangle(2, 7, m_boundingRect.width() - 4,m_boundingRect.height() - 4);

    if(m_switcherInfo.type == SWITCH)
    {
        painter->drawRoundedRect(rectangle,20,15);
    }
    else {
        painter->drawRoundedRect(rectangle,10,5);
    }
	painter->restore();

    painter->save();
	painter->setRenderHint(QPainter::Antialiasing, true);
	QRadialGradient radialGrad(QPointF(m_boundingRect.width()/2, m_boundingRect.height()/2), m_boundingRect.height());

    radialGrad.setColorAt(0.2, QColor(150,214,241));
    radialGrad.setColorAt(0.5, QColor(83,190,249));
    radialGrad.setColorAt(1, QColor(98,176,255));

    painter->setBrush(QBrush(radialGrad));

    QRectF rectangle1(7, 12, m_boundingRect.width() - 14, m_boundingRect.height() - 14);
    if(m_switcherInfo.type == SWITCH)
    {
        painter->drawRoundedRect(rectangle1,20,15);
    }
    else {
        painter->drawRoundedRect(rectangle1,10,5);
    }

	painter->restore();
	
	if(m_switcherInfo.portNum == 32 )
	{
		painter->save();
	    painter->setRenderHint(QPainter::Antialiasing, true);
	    t_brush.setColor(m_color);
	    painter->setBrush(t_brush);
        //painter->drawEllipse(QRectF(m_boundingRect.width()/2 - 5,m_boundingRect.height() - 2 - 8,10,8)); //M duankou
	    painter->restore();
	}
	if(m_switcherInfo.portNum == 48 )
	{
		painter->save();
		painter->setRenderHint(QPainter::Antialiasing, true);
		t_brush.setColor(m_color);
		painter->setBrush(t_brush);
		painter->drawEllipse(QRectF(m_boundingRect.width()/2 - 18,m_boundingRect.height() - 2 - 8,10,8)); //M duankou 
		painter->drawEllipse(QRectF(m_boundingRect.width()/2 + 15,m_boundingRect.height()- 2 - 8,10,8));
		painter->restore();
	}


    int t_fontSize = 10;
	QFont t_font("Times New Roman");
    t_font.setPointSize(t_fontSize);
	t_font.setBold(true);
	QFontMetrics t_textFontM(t_font);

	qreal t_textHeight = t_textFontM.height();

    QString s_id = QString("%1").arg(m_switcherInfo.ID);
    QString t_str2 = s_id +"-" + m_switcherInfo.name;
	qreal t_textWidth2 = t_textFontM.width(t_str2);

    if(t_textWidth2 > m_boundingRect.width())
    {
        t_textWidth2 = m_boundingRect.width();
    }
    if(t_textHeight > m_boundingRect.height()/2)
    {
        t_textHeight = m_boundingRect.height()/4;
    }

    //限定字体大小
    if (t_textWidth2 < m_boundingRect.width())
    {
        t_fontSize = 10;
    }
    else if (t_textWidth2 > m_boundingRect.width())
    {
        t_textWidth2 = m_boundingRect.width();
        t_fontSize = m_boundingRect.width()/10;
    }
    else if(t_textHeight > m_boundingRect.height()/3)
    {
        t_textHeight = m_boundingRect.height()/4;
        t_fontSize = m_boundingRect.width()/20;
    }

	//文字
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setFont(t_font);
	//QPointF t_text1Pos(m_boundingRect.width()/2 - t_textWidth1/2,m_boundingRect.height()/4 /*- t_textHeight*/);
	//painter->drawText(t_text1Pos.x(),t_text1Pos.y(),t_str1);

//	QPointF t_text2Pos(m_boundingRect.width()/2 - t_textWidth2/2,m_boundingRect.height()*3/4 + t_textHeight/1.5);
//	painter->drawText(t_text2Pos.x(),t_text2Pos.y(),t_str2);
    painter->drawText(0, 5, t_str2);

	//交叉线
	painter->setRenderHint(QPainter::Antialiasing, true);
    t_pen.setWidth(2);
    if(m_switcherInfo.type == SWITCH)
    {
        t_pen.setColor(Qt::white);
        painter->setPen(t_pen);
        QPointF t_pnt1(m_boundingRect.width()/8+8,m_boundingRect.height()*3/8+5);
        QPointF t_pnt2(m_boundingRect.width()*3/8+2,m_boundingRect.height()*3/8+5);
        QPointF t_pnt3(m_boundingRect.width()*5/8-2,m_boundingRect.height()*3/8+5);
        QPointF t_pnt4(m_boundingRect.width()*7/8-8,m_boundingRect.height()*3/8+5);

        QPointF t_pnt5(m_boundingRect.width()/8+8,m_boundingRect.height()*5/8+5);
        QPointF t_pnt6(m_boundingRect.width()*3/8+2,m_boundingRect.height()*5/8+5);
        QPointF t_pnt7(m_boundingRect.width()*5/8-2,m_boundingRect.height()*5/8+5);
        QPointF t_pnt8(m_boundingRect.width()*7/8-8,m_boundingRect.height()*5/8+5);


        //箭头点坐标
        QPointF t_pnt11(m_boundingRect.width()/8+8+5,m_boundingRect.height()*3/8-3+5);
        QPointF t_pnt12(m_boundingRect.width()/8+8+5,m_boundingRect.height()*3/8+3+5);

        QPointF t_pnt41(m_boundingRect.width()*7/8-8-5,m_boundingRect.height()*3/8-3+5);
        QPointF t_pnt42(m_boundingRect.width()*7/8-8-5,m_boundingRect.height()*3/8+3+5);

        QPointF t_pnt51(m_boundingRect.width()/8+8+5,m_boundingRect.height()*5/8-3+5);
        QPointF t_pnt52(m_boundingRect.width()/8+8+5,m_boundingRect.height()*5/8+3+5);

        QPointF t_pnt81(m_boundingRect.width()*7/8-8-5,m_boundingRect.height()*5/8-3+5);
        QPointF t_pnt82(m_boundingRect.width()*7/8-8-5,m_boundingRect.height()*5/8+3+5);

        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->drawLine(t_pnt1,t_pnt2);
        painter->drawLine(t_pnt3,t_pnt4);
        painter->drawLine(t_pnt5,t_pnt6);
        painter->drawLine(t_pnt7,t_pnt8);
        painter->drawLine(t_pnt2,t_pnt7);
        painter->drawLine(t_pnt3,t_pnt6);

        painter->drawLine(t_pnt1,t_pnt11);
        painter->drawLine(t_pnt1,t_pnt12);
        painter->drawLine(t_pnt4,t_pnt41);
        painter->drawLine(t_pnt4,t_pnt42);
        painter->drawLine(t_pnt5,t_pnt51);
        painter->drawLine(t_pnt5,t_pnt52);
        painter->drawLine(t_pnt8,t_pnt81);
        painter->drawLine(t_pnt8,t_pnt82);
    }
    else{
        t_pen.setColor(Qt::black);
    }

	

}
QRectF CSwitcher::boundingRect() const
{
    return m_boundingRect;
}





#ifndef _UNITBOXWIDGET_H
#define _UNITBOXWIDGET_H

#include <QDockWidget> 
#include "label.h"
#include <QFrame>
#include <QtGui>
#include <QToolBar>
class QDragEnterEvent;
class QDropEvent;

class CUnitboxWidget : public QWidget
{
public:
	CUnitboxWidget(QWidget * parent = 0, Qt::WindowFlags f = 0 );
	~CUnitboxWidget();
	QString getActivaName(){return m_activeName;}
	QPointF getDis(){return m_dis;}

protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent(QDropEvent *event);
	void mousePressEvent(QMouseEvent *event);

private:
	CLabel* m_switchLabel;
	CLabel* m_singleDeviceLabel;
	CLabel* m_doubleDeviceLabel;

	QString m_activeName;
	QPointF m_dis;
};
#endif


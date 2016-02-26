#include "CUnitboxWidget.h"

#include <QPainter>
#include <QWidget>

CUnitboxWidget::CUnitboxWidget(QWidget *parent /* = 0*/,Qt::WindowFlags f/* = 0*/)
:QWidget(parent, f)
{	
	setMinimumSize(75,32);
	//setFixedSize(75,45);
	//setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
	setAcceptDrops(true);
	m_switchLabel = new CLabel(this);	
	m_singleDeviceLabel  = new CLabel(this);
	m_doubleDeviceLabel = new CLabel(this);

     QPixmap pm = QPixmap(":/NetworkSimulationPlatform/Resources/switcher.png");
	int yStart = (size().height() - pm.rect().height()) / 2;
	m_switchLabel->setName("switcher");
	m_switchLabel->setToolTip(tr("交换机"));
	m_switchLabel->setPixmap(pm);
	m_switchLabel->move(10, yStart);
	m_switchLabel->setAlignment(Qt::AlignVCenter);
	m_switchLabel->show();
	m_switchLabel->setAttribute(Qt::WA_DeleteOnClose);

	m_singleDeviceLabel->setName("singleFIC");
	m_singleDeviceLabel->setToolTip(tr("单口设备"));
    m_singleDeviceLabel->setPixmap(QPixmap(":/NetworkSimulationPlatform/Resources/single.png"));
	m_singleDeviceLabel->move(50, yStart);
	m_singleDeviceLabel->show();
	m_singleDeviceLabel->setAttribute(Qt::WA_DeleteOnClose);
  
	m_doubleDeviceLabel->setName("doubleFIC");
	m_doubleDeviceLabel->setToolTip(tr("双口设备"));
    m_doubleDeviceLabel->setPixmap(QPixmap(":/NetworkSimulationPlatform/Resources/double.png"));
	m_doubleDeviceLabel->move(90, yStart);
	m_doubleDeviceLabel->show();
	m_doubleDeviceLabel->setAttribute(Qt::WA_DeleteOnClose);
}

CUnitboxWidget::~CUnitboxWidget()
{
}

void CUnitboxWidget::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("application/x-dnditemdata")) 
	{
		if (event->source() == this)
		{
			event->setDropAction(Qt::MoveAction);
			event->accept();
		} 
		else 
		{
			event->acceptProposedAction();
		}
	} 
	else 
	{
		event->ignore();
	}
}

void CUnitboxWidget::dragMoveEvent(QDragMoveEvent *event)
{
	if (event->mimeData()->hasFormat("application/x-dnditemdata"))
	{
		if (event->source() == this)
		{
			event->setDropAction(Qt::CopyAction);
			event->accept();
		}
		else 
		{
			event->acceptProposedAction();
		}
	} 
	else 
	{
		event->ignore();
	}
}

void CUnitboxWidget::dropEvent(QDropEvent *event)
{
	if (event->mimeData()->hasFormat("application/x-dnditemdata"))
	{
		QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
		QDataStream dataStream(&itemData, QIODevice::ReadOnly);

		QPixmap pixmap;
		QPoint offset;
		dataStream >> pixmap >> offset;

		QLabel *newIcon = new QLabel(this);
		newIcon->setPixmap(pixmap);
		newIcon->move(event->pos() - offset);
		newIcon->show();
		newIcon->setAttribute(Qt::WA_DeleteOnClose);

		if (event->source() == this)
		{
			if (newIcon != NULL)
			{
				delete newIcon;
				newIcon = NULL;
			}
			event->ignore();
		}
		else 
		{
			event->acceptProposedAction();
		}
	}
	else 
	{
		event->ignore();
	}
}

void CUnitboxWidget::mousePressEvent(QMouseEvent *event)
{
	CLabel *child = dynamic_cast<CLabel*>(childAt(event->pos()));
	if (child != NULL)
	{
		m_activeName = child->getName();
		QPointF t_lt = child->pos();
		m_dis = event->pos() - t_lt;

		QPixmap pixmap = *child->pixmap();

		QByteArray itemData;
		QDataStream dataStream(&itemData, QIODevice::WriteOnly);
		dataStream << pixmap << QPoint(event->pos() - child->pos());
 
		QMimeData *mimeData = new QMimeData;
		mimeData->setData("application/x-dnditemdata", itemData);

		QDrag *drag = new QDrag(this);
		drag->setMimeData(mimeData);
		drag->setPixmap(pixmap);
		drag->setHotSpot(event->pos() - child->pos());

		QPixmap tempPixmap = pixmap;
		QPainter painter;
		painter.begin(&tempPixmap);
		painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
		painter.end();

		child->setPixmap(tempPixmap);

		if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
		{
			child->close();
		}
		else 
		{
			child->show();
			child->setPixmap(pixmap);
		}
	}
}

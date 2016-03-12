#include "CNetworkEditWidget.h"
#include <algorithm>

#include "NetworkSimulationPlatform.h"
#include "topotest.h"
//************QJson*****
#include "parser.h"
#include "serializer.h"
#include "SDNCurl/sdncurl.h"

const int MINWIDTH = 40; 
int g_currentState = 1;
bool ctrlpressed = false;

CTopologyWidget::CTopologyWidget(QWidget *parent)
{
    //setAlignment(Qt::AlignLeft|Qt::AlignTop);
    setAlignment(Qt::AlignCenter);
    m_scene = new QGraphicsScene(0,0,1500,1500);
    this->setScene(m_scene);
    setDragMode(QGraphicsView::RubberBandDrag);
    creatRightMenu();

    //�Ի���
    m_switchDia = new CSwitherConfigure;
    m_newSwitcherDia = new CNewSwitcher;
    m_newPortDia = new CPortDialog;

    connect(m_newPortDia, SIGNAL(accepted()), this, SLOT(slotDrawLine()));
    connect(m_switchDia,SIGNAL(accepted()),this,SLOT(slotSwitchDia()));
    connect(m_newSwitcherDia,SIGNAL(accepted()),this,SLOT(slotNewRouterDia()));

    m_itemGroup = new CItemGroup(0,m_scene);
    m_itemGroup->setSelected(false);

    m_moveitemFlag = false;
    m_dragRectItem = NULL;
    m_tempadge = NULL;

    m_EditSwitchItem = NULL;
    m_vShowFlag = false;
    m_unitWidget = NULL;
    m_zoom = 0;

    m_backItem=NULL;
    //drawLegend();
    m_curl = new SDNCurl();
}

CTopologyWidget::~CTopologyWidget()
{
    if(m_curl)
        delete m_curl;
}

void CTopologyWidget::setLabelWidget(CUnitboxWidget* _unitW)
{
    //��ͼƬ��ǩ�Ĵ���
	m_unitWidget = _unitW;	
}

//��ջ�������ʼ����Ա����
void CTopologyWidget::clearScene(void)
{
    //��տ�ѡ��ͼԪ
    if (m_itemGroup != NULL)
    {
        QList<QGraphicsItem*> t_childItems = m_itemGroup->childItems();
        for (int i = 0; i < t_childItems.size(); ++i)
        {
            if (dynamic_cast<CSwitcher*>(t_childItems[i]) != NULL)
            {
                dynamic_cast<CSwitcher*>(t_childItems[i])->setSelectedFlag(false);
            }
            else if (dynamic_cast<Clink*>(t_childItems[i]) != NULL)
            {
                dynamic_cast<Clink*>(t_childItems[i])->setSelectedFlag(false);
            }

            m_itemGroup->removeFromGroup(t_childItems[i]);
            deleteChildItems(t_childItems[i]);
        }
    }

    m_moveitemFlag = false;
	m_dragRectItem = NULL;
    m_tempadge = NULL;
    m_pressItem = NULL;
    m_releaseItem = NULL;

    m_EditSwitchItem = NULL;
    m_EditPortItem = NULL;
    m_vShowFlag = false;

    m_newSwitcherDia->clear();
    m_newPortDia->clear();
    m_vlink.clear();
    t_vPortInOut.clear();
    //t_vSwitchInOut.clear();
    m_vSwtich.clear();
    m_vDeviceNums.clear();
    t_IPPond.clear();
    m_IPPool.clear();
    m_scene->clear();

    m_itemGroup = new CItemGroup(0,m_scene);
    drawLegend();
}

void CTopologyWidget::setPlatformPointer(NetworkSimulationPlatform *_platform)
{
    m_platform = _platform;
}

//��ͼ��
void CTopologyWidget::drawLegend()
{
    QFont t_font("Times New Roman");
    t_font.setPointSize(10);
    m_scene->addText("POS", t_font)->setPos(1250, 1410);
    m_scene->addText("Ethernet", t_font)->setPos(1240, 1440);
    m_scene->addText(tr("һ��"), t_font)->setPos(1400, 1350);
    m_scene->addText(tr("����"), t_font)->setPos(1400, 1380);
    m_scene->addText(tr("����"), t_font)->setPos(1400, 1410);
    m_scene->addText(tr("����"), t_font)->setPos(1400, 1440);
    m_scene->addText(tr("����"), t_font)->setPos(1400, 1470);
    QPen t_pen(Qt::black);
    t_pen.setStyle(Qt::DashLine);
    t_pen.setWidth(2);
    m_scene->addLine(1300,1420, 1380,1420, t_pen);
    t_pen.setColor(Qt::black);
    t_pen.setStyle(Qt::SolidLine);
    t_pen.setWidth(8);
    m_scene->addLine(1300,1450, 1380,1450, t_pen);
    m_scene->addRect(1450, 1350, 20, 20, QPen(Qt::black), QBrush(Qt::black));
    m_scene->addRect(1450, 1380, 20, 20, QPen(Qt::red), QBrush(Qt::red));
    m_scene->addRect(1450, 1410, 20, 20, QPen(Qt::blue), QBrush(Qt::blue));
    m_scene->addRect(1450, 1440, 20, 20, QPen(Qt::magenta), QBrush(Qt::magenta));
    m_scene->addRect(1450, 1470, 20, 20, QPen(Qt::cyan), QBrush(Qt::cyan));
}

void CTopologyWidget::dragEnterEvent(QDragEnterEvent *event)
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

void CTopologyWidget::dragMoveEvent(QDragMoveEvent *event)
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

void CTopologyWidget::dropEvent(QDropEvent *event)
{
    QPointF tmpPos = mapToScene(event->pos()) - m_unitWidget->getDis();
    if(tmpPos.x() >= m_scene->width() || tmpPos.y() >= m_scene->height() || tmpPos.x() <=0 || tmpPos.y() <= 0)
    {
        qDebug("dropEvent out of range:x=%f,y=%f",tmpPos.x(),tmpPos.y());
        return;
    }

	if (event->mimeData()->hasFormat("application/x-dnditemdata")) 
	{
		QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
		QDataStream dataStream(&itemData, QIODevice::ReadOnly);

		if (event->source() == this)
		{
			event->setDropAction(Qt::MoveAction);
			event->accept();
		}
		else 
		{
			if (m_unitWidget != NULL)
			{
				m_name = m_unitWidget->getActivaName();
				if (m_name == tr("switcher"))
				{                    
                    m_dropPos = mapToScene(event->pos()) - m_unitWidget->getDis();

					//����id
					int id = 1;
					for (;;id++)
					{
                        size_t i = 0;
                        for ( ; i < m_vSwtich.size(); i++)
						{
							if (id == m_vSwtich[i].ID) break;
                        }

						if (i == m_vSwtich.size()) break;
					}
                    m_newSwitcherDia->setSwitchIDandType(id);
                    //���豸ǰ���ԶԻ���
					m_newSwitcherDia->exec();					
				}				
			}

			event->acceptProposedAction();
		}
	} 
	else
	{
		event->ignore();
	}
}

void CTopologyWidget::slotNewRouterDia()
{
        for (size_t i = 0; i < m_vSwtich.size(); ++i)
		{
			if (m_newSwitcherDia->getSwitcherInfo().ID == m_vSwtich[i].ID)
			{
                QMessageBox::information(m_newSwitcherDia,tr("����"),tr("��ͬ���Ƶ�·�����Ѿ�����"));
				return;
			}
		}

        for (size_t i = 0; i < m_vSwtich.size(); ++i)
		{
			if (m_newSwitcherDia->getSwitcherInfo().ID == m_vSwtich[i].ID)
			{
                QMessageBox::information(m_newSwitcherDia,tr("����"),tr("��ͬ������·�����Ѿ�����"));
				return;
			}
		}

        CSwitcher* t_switch = new CSwitcher(NULL, m_scene);
        CNetworkPort *t_port = new CNetworkPort(t_switch, m_scene);
        QRectF m_boundingRect = t_switch->boundingRect();

        t_switch->setPos(m_dropPos);
        t_switch->setToolTip(tr("����: ") + t_switch->getSwitcherInfo().name + "\n"+
                             tr("�ͺ�:") + t_switch->getSwitcherInfo().type + "\n" +
                             tr("λ��: ") + t_switch->getSwitcherInfo().networkLocation);

        t_port->setPos(m_boundingRect.width()/2, m_boundingRect.height()/2+5);
		t_switch->setSwitcherInfo(m_newSwitcherDia->getSwitcherInfo());
		m_vSwtich.push_back(m_newSwitcherDia->getSwitcherInfo());
		m_newSwitcherDia->close();
        update();
	
}
void CTopologyWidget::slotNewDeviceDia()
{

}

void CTopologyWidget::slotSwitchDia()
{
	if (m_EditSwitchItem == NULL)
	{
		m_switchDia->close();
		return;
	}
    if (m_EditSwitchItem->getSwitcherInfo().name != m_switchDia->GetSwitcherInfo().name)
    {
        for (size_t i = 0; i < m_vSwtich.size(); ++i)
        {
            if (m_switchDia->GetSwitcherInfo().name == m_vSwtich[i].name)
            {
                QMessageBox::information(m_switchDia,tr("����"),tr("��ͬ���ƵĽ������Ѿ�����"));
                return;
            }
        }
    }

    m_EditSwitchItem->setSwitcherInfo(m_switchDia->GetSwitcherInfo());

    QList<QGraphicsItem*> t_itemlist = m_scene->items();
    QList<CSwitcher*> t_switchList ;
    for (int i = 0; i < t_itemlist.count(); i++)
    {
        CSwitcher * t_switch = NULL;

        t_switch = dynamic_cast<CSwitcher*>(t_itemlist[i]);

        if (t_switch != NULL)
        {
            t_switchList.push_back(t_switch);
        }
    }
    if(m_switchDia->GetSwitchDeletePortsInfo().size() != 0)
    {
        vector<SNodeStructInfo> deletePortsInfo = m_switchDia->GetSwitchDeletePortsInfo();
        for(unsigned int i = 0; i < deletePortsInfo.size(); ++i)
        {
            //delete portInfo from swicher
            for(int j = 0; j < t_switchList.size(); ++j)
            {
                if(deletePortsInfo[i].linkPort_Device_ID = t_switchList[j]->getSwitcherInfo().ID)
                {
                    for(int k = 0; k < t_switchList[j]->getSwitcherInfo().LNodes.size(); ++k)
                    {
                        if(t_switchList[j]->getSwitcherInfo().LNodes[k].interfaceName == deletePortsInfo[i].linkPort_interfaceName)
                        {
                            t_switchList[j]->getSwitcherInfo().LNodes.erase(t_switchList[j]->getSwitcherInfo().LNodes.begin() + k);
                        }
                    }
                }
            }
        }

    }
    int t_size = m_EditSwitchItem->getSwitcherInfo().LNodes.size();
    for(int i = 0; i != t_size; ++i)
    {
        for(int j = 0; j != t_switchList.size(); ++j)
        {
            if(m_EditSwitchItem->getSwitcherInfo().LNodes[i].linkPort_Device_ID == t_switchList[j]->getSwitcherInfo().ID)
            {
                for(int k = 0; k != t_switchList[j]->getSwitcherInfo().LNodes.size(); ++k)
                {
                    if(t_switchList[j]->getSwitcherInfo().LNodes[k].interfaceName == m_EditSwitchItem->getSwitcherInfo().LNodes[i].linkPort_interfaceName)
                    {
                        CSwitcherInfo t_switchInfo = t_switchList[j]->getSwitcherInfo();
                        t_switchInfo.LNodes[k].linkPort_name = m_EditSwitchItem->getSwitcherInfo().name;
                        t_switchInfo.LNodes[k].IP = m_EditSwitchItem->getSwitcherInfo().LNodes[i].linkPort_IP;
                        t_switchInfo.LNodes[k].linkPort_IP = m_EditSwitchItem->getSwitcherInfo().LNodes[i].IP;
                        t_switchList[j]->setSwitcherInfo(t_switchInfo);
                    }
                }
            }
        }
    }
	m_switchDia->close();
}

//���ѡ���¼����༭���߱߿�
void CTopologyWidget::mousePressEvent(QMouseEvent *event)
{
    QPointF mouseclicked_pos = mapToScene(event->pos());
    qDebug("%d, %d", event->pos().x(), event->pos().y());
    qDebug("%f, %f", mouseclicked_pos.x(), mouseclicked_pos.y());
    QList<QGraphicsItem*> t_itemlist = m_scene->items(QRectF(0,0,m_scene->width(),m_scene->height()));
    // m_moveitemFlag = false;
    if (event->buttons() == Qt::RightButton)
    {
        return;
    }

    QPointF scenePos = mapToScene(event->pos());
    m_pressScenePoint_formove = scenePos;
    m_sceneFormerPoint = scenePos;

    QGraphicsItem*  t_item = m_scene->itemAt(scenePos);
    CSwitcher* t_switchItem = NULL;
    Clink* t_linkItem = NULL;
    QGraphicsRectItem* t_smallRectItem = dynamic_cast<QGraphicsRectItem*>(t_item);
    QGraphicsPixmapItem* t_backItem = dynamic_cast<QGraphicsPixmapItem*>(t_item);

    //left button pressed
    if(event->button() == Qt::LeftButton)
    { 
        if(t_backItem != NULL || t_item == NULL) //���������հ�  ȡ��ѡ�е�ͼԪ  ɾ����ͼԪ
        {
            QList<QGraphicsItem*> t_childItems = m_itemGroup->childItems();
            for (int i = 0; i < t_childItems.size(); ++i)
            {
                if (dynamic_cast<CSwitcher*>(t_childItems[i]) != NULL)
                {
                    dynamic_cast<CSwitcher*>(t_childItems[i])->setSelectedFlag(false);
                    m_moveitemFlag = false;
                }
                else if (dynamic_cast<Clink*>(t_childItems[i]) != NULL)
                {
                    dynamic_cast<Clink*>(t_childItems[i])->setSelectedFlag(false);
                }

                m_itemGroup->removeFromGroup(t_childItems[i]);
                deleteChildItems(t_childItems[i]);
            }
            m_dragRectItem = new QGraphicsRectItem;
            m_dragRectItem->setPen(QPen(Qt::DashLine));
            m_dragRectItem->setPos(scenePos);
            m_scene->addItem(m_dragRectItem);

            return;
        }
        else
        {
//            //��������б�
//            if(m_platform->ui_DeviceInfTable->item(0,0))
//            {
//                m_platform->ui_DeviceInfTable->clear();
//            }

            switch(t_item->type())
            {
            case QGraphicsItem::UserType + 1://swicher
                t_switchItem = dynamic_cast<CSwitcher*>(t_item);

                if (t_switchItem->getSelectedFlag() == false)  //û��ѡ��
                {
                    if (event->modifiers() != 0x04000000)  //û�а���ctrl��  ������е�ͼԪ
                    {
                        QList<QGraphicsItem*> t_itemingroup = m_itemGroup->childItems();
                        for (int i = 0; i < t_itemingroup.size(); ++i)
                        {
                            if (dynamic_cast<CSwitcher*>(t_itemingroup[i]) != NULL)
                                dynamic_cast<CSwitcher*>(t_itemingroup[i])->setSelectedFlag(false);
                            else if(dynamic_cast<Clink*>(t_itemingroup[i]) != NULL)
                                dynamic_cast<Clink*>(t_itemingroup[i])->setSelectedFlag(false);

                            m_itemGroup->removeFromGroup(t_itemingroup[i]);
                            deleteChildItems(t_itemingroup[i]);
                        }
                    }
                    t_switchItem->setSelectedFlag(true);
                    CSwitcherInfo t_nodeInf = t_switchItem->getSwitcherInfo();
                    if (t_nodeInf.type == SWITCH)
                    {
                        //�������������Խ���
                        qDebug()<<"in table0";
                        setSwitchInfTable(t_nodeInf);
                    }
                    else{
                        //�����ڵ����Խ���
                        setHostInfTable(t_nodeInf);
                    }

                    CBoundingRectItem* t_brItem = new CBoundingRectItem(t_switchItem,m_scene);
                    //t_brItem->setZValue(100);
                    t_brItem->setBoundingRect(t_switchItem->boundingRect());
                    t_brItem->drawBoundingItem();
                    m_itemGroup->addToGroup(t_switchItem);
                }
                if(t_switchItem->getSelectedFlag() == true)
                {
                    m_moveitemFlag = true;
                }

                break;
            case QGraphicsItem::UserType + 2: //port
            {
                m_pressItem = (CNetworkPort *)t_item;
                m_tempadge = new QGraphicsLineItem;
                m_tempadge->setPen(QPen(Qt::DashLine));
                //m_tempadge->setZValue(1000);
                m_scene->addItem(m_tempadge);
                break;
            }
            case QGraphicsItem::UserType + 3:
            // �϶��Ŵ�����С  ֻѡ��һ��ͼԪ��ʱ��ſɷŴ���С
            {
                QList<QGraphicsItem*> t_groupItems = m_itemGroup->childItems();
                if (t_smallRectItem != NULL)
                {
                    if (t_groupItems.size() > 1)
                    {
                        return;
                    }
                    QGraphicsItem *tempItem=t_smallRectItem->parentItem();  //boundingRectItem
                    if (tempItem != NULL)
                    {
                        QList<QGraphicsItem *> _itemL = tempItem->childItems();
                        QList<QGraphicsItem*> itemL;
                        itemL.clear();
                        for(int i = 0; i < _itemL.size(); ++i)
                        {
                            QGraphicsRectItem* t_rectItem = dynamic_cast<QGraphicsRectItem*>(_itemL[i]);
                            if (t_rectItem != NULL)
                            {
                                itemL.append(_itemL[i]);
                            }
                        }
                        for ( int j = 0; j < itemL.size(); j++ )
                        {
                            if (itemL[j]->contains(itemL[j]->mapFromScene(scenePos)))   //����С���ε�ʱ�� ��Ϊ��
                            {
                                switch(j)
                                {
                                case 0:
                                    rect0sele = true;
                                    break;
                                case 1:
                                    rect1sele = true;
                                    break;
                                case 2:
                                    rect2sele = true;
                                    break;
                                case 3:
                                    rect3sele = true;
                                    break;
                                case 4:
                                    rect4sele = true;
                                    break;
                                case 5:
                                    rect5sele = true;
                                    break;
                                case 6:
                                    rect6sele = true;
                                    break;
                                case 7:
                                    rect7sele = true;
                                    break;
                                default:
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            break;
            case QGraphicsItem::UserType + 4://link
            {
                t_linkItem = dynamic_cast<Clink*>(t_item);
                t_linkItem->setSelectedFlag(true);
                break;
            }
            default:
                break;
            }
        }
    }
}

/************************************************************************
���ܣ�����ƶ��¼�
����:   ��
����ֵ����
************************************************************************/
void CTopologyWidget::mouseMoveEvent(QMouseEvent *event)
{
	QSize sz = size();
	if (event->pos().x() < 0 || event->pos().x() >sz.width() || event->pos().y()< 0 || event->pos().y() > sz.height() )
	{
        return;
	}
    QPointF t_Pos = mapToScene(event->pos());
    if(t_Pos.x()<60 || t_Pos.y()<60 || t_Pos.y()>1440 || t_Pos.x()>1440)
    {
        return;
    }
	QPointF nowScenePos = this->mapToScene(event->pos());
	QGraphicsItem*  t_item = m_scene->itemAt(nowScenePos);
	

	QList<QGraphicsItem*> t_groupItems = m_itemGroup->childItems();
	if (t_groupItems.size() == 1)
	{
		QGraphicsRectItem *t_smallRect = dynamic_cast<QGraphicsRectItem*>(t_item);
		if (t_smallRect != NULL && t_smallRect->parentItem() != NULL) //��ʾ���ο�С����ѡ��
		{
			cursorShapeChange(nowScenePos,dynamic_cast<CBoundingRectItem*>(t_smallRect->parentItem()));//�����С����ʱ�ı���״
		}
		else
		{
			setCursor(QCursor(Qt::ArrowCursor));
		}
	}


	if (event->buttons() & Qt::LeftButton)
	{
		//�϶�ѡ�е�ͼԪ
        if (m_moveitemFlag == true)
		{
			if(g_currentState == 1)
			{
                qreal moveX = nowScenePos.x() - m_sceneFormerPoint.x();
				qreal moveY = nowScenePos.y() - m_sceneFormerPoint.y();
				m_sceneFormerPoint = nowScenePos;
				m_itemGroup->moveBy(moveX,moveY);

                for (int k = 0; k < m_vlink.size();++k)
				{
                    if (m_vlink[k] !=NULL)
					{
                        m_vlink[k]->update();
					}
				}
			}

            else if(g_currentState == 3)
			{
				return;
			}			
		}
        //������ʱ����
        else if(m_tempadge != NULL)
        {
            QPointF m_portPos = m_pressItem->parentItem()->mapToScene(m_pressItem->mapToParent(QPointF(0,0)));
            m_tempadge->setLine(m_portPos.x(), m_portPos.y(), nowScenePos.x(), nowScenePos.y());
        }
		//������ѡ���߿�
		else if (m_dragRectItem != NULL)
		{
            if(nowScenePos.x() > m_pressScenePoint_formove.x() && nowScenePos.y() > m_pressScenePoint_formove.y())
                m_dragRectItem->setRect(0,0,nowScenePos.x() - m_pressScenePoint_formove.x(),nowScenePos.y() - m_pressScenePoint_formove.y());
		}
		else if (rect0sele==true||rect1sele==true||rect2sele==true||rect3sele==true||rect4sele==true||rect5sele==true||rect6sele==true||rect7sele==true)//����С�������϶���ʱ��
		{			
			CreateMovableRect(nowScenePos);//������ɫ���ı߽�С���β��ƶ�ʱ�������ƶ������	
            for (int k = 0; k < m_vlink.size();++k)
			{
                if (m_vlink[k] !=NULL)
                {
                    CNetworkPort* t_startPort = m_vlink[k]->getStartPort();
                    CNetworkPort* t_endPort = m_vlink[k]->getEndPort();
                    if (t_startPort != NULL && t_startPort->pos().x() > 20) //�ұ�
                    {
                        CSwitcher* temp_switchItem = dynamic_cast<CSwitcher*>(t_startPort->parentItem());
                        if (temp_switchItem != NULL)
                        {
                            QPointF t_iniPos = t_startPort->pos();
                            t_startPort->setPos(temp_switchItem->boundingRect().bottomRight().x()-9,t_iniPos.y());
                        }
                    }
                    if (t_endPort != NULL && t_endPort->pos().x() > 20) //�ұ�
                    {
                        CSwitcher* temp_switchItem = dynamic_cast<CSwitcher*>(t_endPort->parentItem());
                        if (temp_switchItem != NULL)
                        {
                            QPointF t_iniPos = t_endPort->pos();
                            t_endPort->setPos(temp_switchItem->boundingRect().bottomRight().x()-9,t_iniPos.y());
                        }
                    }
                    m_vlink[k]->update();
				}
			}
		}	
	}
}

//����ͷ��¼�
void CTopologyWidget::mouseReleaseEvent(QMouseEvent *event)
{
    bool bIsMoved = false;
    if(m_pressScenePoint_formove != event->pos())
    {
        bIsMoved = true;
    }
    else
    {
        if(m_dragRectItem != NULL)
            m_scene->removeItem(m_dragRectItem);
    }
    //left button released
    if(event->button() == Qt::LeftButton)
    {
        if(bIsMoved)
        {
            QList<QGraphicsItem*> t_itemlist;
            t_itemlist.clear();

            if(m_tempadge != NULL)
            {
                m_scene->removeItem(m_tempadge);
                m_tempadge = NULL;
            }

            QGraphicsItem *a_item = itemAt(event->pos());

            if(a_item != NULL && a_item->type() == QGraphicsItem::UserType + 2)
            {
                m_releaseItem = dynamic_cast<CNetworkPort *>(a_item);

                CSwitcher *_switcher = (CSwitcher *)(m_pressItem->parentItem());
                CSwitcher *_linkSwitcher = (CSwitcher *)(m_releaseItem->parentItem());
                if(_switcher->getSwitcherInfo().name == _linkSwitcher->getSwitcherInfo().name)
                    return;
                else
                {
                    int count = 0;
                    for(int i = 0; i < _switcher->getSwitcherInfo().LNodes.size(); i++)
                    {
                        if(_linkSwitcher->getSwitcherInfo().ID == _switcher->getSwitcherInfo().LNodes[i].linkPort_Device_ID)
                            count++;//the num of link between two routers
                    }
                    if(count >= 5)
                    {
                        QMessageBox::information(this, tr("��ʾ"), tr("�����豸����·��������5����"));
                        return;
                    }
                    int id_s = 1, id_e = 1;
                    for (;; id_s++)
                    {
                        int i = 0;
                        for ( ; i < _switcher->getSwitcherInfo().LNodes.size(); i++)
                        {
                            if (id_s == _switcher->getSwitcherInfo().LNodes[i].Port_ID)break;
                        }

                        if (i == _switcher->getSwitcherInfo().LNodes.size()) break;
                    }
                    for (;;id_e++)
                    {
                        int i = 0;
                        for ( ; i < _linkSwitcher->getSwitcherInfo().LNodes.size(); i++)
                        {
                            if (id_e == _linkSwitcher->getSwitcherInfo().LNodes[i].Port_ID) break;
                        }

                        if (i == _linkSwitcher->getSwitcherInfo().LNodes.size()) break;
                    }
                    m_newPortDia->setPortIDandType(id_s, id_e);
                    m_newPortDia->setPortName(_switcher->getSwitcherInfo().networkLocation, _switcher->getSwitcherInfo().name, _linkSwitcher->getSwitcherInfo().name, _switcher->getSwitcherInfo().ID, _linkSwitcher->getSwitcherInfo().ID);
                    m_newPortDia->setVports(_switcher->getSwitcherInfo().LNodes);
                    m_newPortDia->setLinkVports(_linkSwitcher->getSwitcherInfo().LNodes);
                    m_newPortDia->setIPpond(&t_IPPond);
                    m_newPortDia->setIPPool(&m_IPPool);
                    m_newPortDia->exec();

                }
            }
            else if (m_dragRectItem != NULL)
            {
                QRectF t_rect = m_dragRectItem->sceneBoundingRect().normalized();
                t_itemlist = m_scene->items(t_rect,Qt::IntersectsItemShape);//�������߿��⵽��ͼԪ
                qDebug("%d", t_itemlist.size());
                if(t_itemlist.size() <= 1)
                    m_moveitemFlag = false;
                m_scene->removeItem(m_dragRectItem);
            }
            for (int i = 0 ; i < t_itemlist.size(); ++i)
            {
                if (t_itemlist[i] == NULL)
                    continue;
                CSwitcher* t_switchitem = NULL;
                Clink* t_linkitem = NULL;

                t_switchitem = dynamic_cast<CSwitcher*>(t_itemlist[i]);
                t_linkitem = dynamic_cast<Clink*>(t_itemlist[i]);

                if (t_switchitem != NULL && t_switchitem->getSelectedFlag() == false)
                {
                    t_switchitem->setSelectedFlag(true);
                    CBoundingRectItem* t_brItem = new CBoundingRectItem(t_switchitem,m_scene);
                    t_brItem->m_Bzoom = &m_zoom;
                    t_brItem->setBoundingRect(t_switchitem->boundingRect());
                    t_brItem->drawBoundingItem();
                    m_itemGroup->addToGroup(t_switchitem);

                    m_moveitemFlag = true;
                }
                if(t_linkitem != NULL && t_linkitem->getSelectedFlag() == false)
                {
                    t_linkitem->setSelectedFlag(true);
                    m_itemGroup->addToGroup(t_linkitem);
                }

            }

            rect0sele = false;rect1sele = false; rect2sele = false; rect3sele = false;rect4sele = false; rect5sele = false; rect6sele = false; rect7sele = false;

            if (m_dragRectItem != NULL)
            {
                delete m_dragRectItem;
                m_dragRectItem = NULL;
            }

            for (int k = 0; k < m_vlink.size();++k)
            {
                if (m_vlink[k] !=NULL)
                {
                    m_vlink[k]->update();
                }
            }
        }
    }
}

void CTopologyWidget::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;
    if (event->modifiers() == 0x04000000)//ctrl is pressed
    {
        if(numSteps > 0)
       {
            QMatrix t_matrix;
            t_matrix.scale(1.25,1.25);
            setMatrix(t_matrix,true);
        }

       else
        {
             QMatrix t_matrix;
             t_matrix.scale(1/1.25,1/1.25);
             setMatrix(t_matrix,true);
        }
    }
    else
        QAbstractScrollArea::wheelEvent (event);
}

void CTopologyWidget::slotDrawLine()
{
    CSwitcher *_switcher = dynamic_cast<CSwitcher*>(m_pressItem->parentItem());
    CSwitcher *_linkSwitcher = dynamic_cast<CSwitcher*>(m_releaseItem->parentItem());
    _switcher->addLinkedNode(m_newPortDia->getPortInfo());
    _linkSwitcher->addLinkedNode(m_newPortDia->getLinkPortInfo());
    int s_switchID = _switcher->getSwitcherInfo().ID;
    int d_switchID = _linkSwitcher->getSwitcherInfo().ID;
    int EthernetCount = 0;
    int POSCount = 0;

    for(int i = 0; i < _switcher->getSwitcherInfo().LNodes.size(); i++)
    {
        if(_linkSwitcher->getSwitcherInfo().ID == _switcher->getSwitcherInfo().LNodes[i].linkPort_Device_ID)
        {
            if (_switcher->getSwitcherInfo().LNodes[i].linkType == POS)
            {
                POSCount++;
            }
            else if (_switcher->getSwitcherInfo().LNodes[i].linkType == Ethernet)
            {
                EthernetCount++;
            }
        }
    }
    Clink *link = new Clink( m_scene, m_pressItem, m_releaseItem);
    link->setLinkInfo(m_newPortDia->getLinkInfo());
    //link->getLinkInfo().pos_link_num = POSCount;
    //link->getLinkInfo().ethernet_link_num = EthernetCount;
    link->setID(s_switchID, d_switchID);
    QString linkType = "Ethernet";
    link->setToolTip(tr("����:") + linkType + "\n" +
                     tr("����:") + link->getLinkInfo().bandWidth);

    m_itemGroup->addToGroup(link);
    m_vlink.push_back(link);

    //updata all same type links, hide items
    for ( int i = 0; i < m_vlink.size(); ++i )
    {
        if (m_vlink[i] == link)
        {
            continue;
        }
        CLinkInfo& tmpLinkInfo = m_vlink[i]->getLinkInfo();
        if ((tmpLinkInfo.s_switchID == s_switchID && tmpLinkInfo.d_switchID == d_switchID) ||
            (tmpLinkInfo.s_switchID == d_switchID && tmpLinkInfo.d_switchID == s_switchID))
        {
            m_vlink[i]->hide();
        }
    }
}

//���˫���¼�
void CTopologyWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	//ѡ��һ��������ͼԪ����һ���豸ͼԪʱ���˫��
	QList<QGraphicsItem*> t_childItem = m_itemGroup->childItems();
	if (t_childItem.size() != 1)
	{
		return;
	}
	CSwitcher* t_switchItem = NULL;
    Clink* t_linkItem = NULL;

	t_switchItem = dynamic_cast<CSwitcher*>(t_childItem[0]);
    t_linkItem = dynamic_cast<Clink*>(t_childItem[0]);
	if (t_switchItem != NULL)  //���������ԶԻ�����ʾ
	{
		m_EditSwitchItem = t_switchItem;
		m_switchDia->SetSwitcherInfo(t_switchItem->getSwitcherInfo());
        m_switchDia->setIPpond(&t_IPPond);
        m_switchDia->setIPPool(&m_IPPool);
		m_switchDia->exec(); 
	}   
    else if (t_linkItem != NULL)//��ʾ��·�豸���ԶԻ���
    {
        return;//�����
    }
}

//�Ҽ��˵�
void CTopologyWidget::contextMenuEvent(QContextMenuEvent * event)
{
//	m_configuerAct->setEnabled(false);

	QPointF scenePnt = mapToScene(event->pos());
	QGraphicsItem* t_item = m_scene->itemAt(scenePnt);
	QList<QGraphicsItem*> t_childItems = m_itemGroup->childItems();

	CBoundingRectItem* t_brItem = NULL;
	t_brItem = dynamic_cast<CBoundingRectItem*>(t_item);

	//����հ� ���ߵ���Ѿ�ѡ�е�ͼԪ  ���������ʾ�˵�
	if (t_item == NULL || (t_brItem != NULL && t_brItem->parentItem() != NULL))
	{
		if (t_childItems.size() == 0)  //�޼���ͼԪ �Ҽ����а�ť��Ч
		{
            m_deleteAct->setEnabled(false);
            m_attributeAct->setEnabled(false);
            m_routeVisible->setEnabled(false);
            m_delayAct->setEnabled(false);
			m_rightMenu->exec(event->globalPos());
			return;
		}
        else if (t_childItems.size() == 1)  //����һ����ʱ��
		{
//            m_configuerAct->setEnabled(true);
            m_deleteAct->setEnabled(true);
            m_attributeAct->setEnabled(true);
            m_routeVisible->setEnabled(false);
            m_delayAct->setEnabled(false);
            m_rightMenu->exec(event->globalPos());
			return;
		}
        else if (t_childItems.size() == 2)  //������������ͼԪʱ  ��ɾ����ť��Ч
		{
			m_deleteAct->setEnabled(true);
            m_attributeAct->setEnabled(false);
            m_routeVisible->setEnabled(true);
            m_delayAct->setEnabled(true);
			m_rightMenu->exec(event->globalPos());
			return;
		}
        else
        {
            m_deleteAct->setEnabled(true);
            m_attributeAct->setEnabled(false);
            m_routeVisible->setEnabled(true);
            m_delayAct->setEnabled(false);
            m_rightMenu->exec(event->globalPos());
            return;
        }

        //�һ�δ��ѡ�е�ͼԪ����� �� ��ͼԪ��ѡ�� �ҳ�����Ӧ���Ҽ��˵�
        CSwitcher* te_switchItem = NULL;
        Clink* te_linkItem = NULL;
        te_switchItem = dynamic_cast<CSwitcher*>(t_item);
        te_linkItem = dynamic_cast<Clink*>(t_item);
        if (te_switchItem != NULL && te_switchItem->getSelectedFlag() == false)  //���н�����
        {
            //���group
            for (int i = 0; i < t_childItems.size(); ++i)
            {
                if (dynamic_cast<CSwitcher*>(t_childItems[i]) != NULL)
                {
                    dynamic_cast<CSwitcher*>(t_childItems[i])->setSelectedFlag(false);
                }
                else if (dynamic_cast<Clink*>(t_childItems[i]) != NULL)
                {
                    dynamic_cast<Clink*>(t_childItems[i])->setSelectedFlag(false);
                }
                deleteChildItems(t_childItems[i]);
                m_itemGroup->removeFromGroup(t_childItems[i]);
            }

            te_switchItem->setSelectedFlag(true);
            CBoundingRectItem* t_brItem = new CBoundingRectItem(te_switchItem,m_scene);
            t_brItem->setBoundingRect(te_switchItem->boundingRect());
            t_brItem->drawBoundingItem();
            m_itemGroup->addToGroup(te_switchItem);


            m_deleteAct->setEnabled(true);
            m_attributeAct->setEnabled(true);
            m_configuerAct->setEnabled(true);
            m_rightMenu->exec(event->globalPos());
            return;
        }
        else if (te_linkItem != NULL && te_linkItem->getSelectedFlag() == false)  //ѡ����·
        {
            for (int i = 0; i < t_childItems.size(); ++i)
            {
                if (dynamic_cast<CSwitcher*>(t_childItems[i]) != NULL)
                {
                    dynamic_cast<CSwitcher*>(t_childItems[i])->setSelectedFlag(false);
                }
                else if (dynamic_cast<Clink*>(t_childItems[i]) != NULL)
                {
                    dynamic_cast<Clink*>(t_childItems[i])->setSelectedFlag(false);
                }
                deleteChildItems(t_childItems[i]);
                m_itemGroup->removeFromGroup(t_childItems[i]);
            }
            te_linkItem->setSelectedFlag(true);
            //�����ѡ����
    //		CBoundingRectItem* t_brItem = new CBoundingRectItem(te_deviceItem,m_scene);
    //		t_brItem->setBoundingRect(te_deviceItem->boundingRect());
    //		t_brItem->setParentItem(te_deviceItem);
    //		t_brItem->drawBoundingItem();
            m_itemGroup->addToGroup(te_linkItem);
            m_deleteAct->setEnabled(true);
            m_attributeAct->setEnabled(true);
            //m_configuerAct->setEnabled(true);
            m_rightMenu0->exec(event->globalPos());
        }
    }
}

//�����¼�  ɾ��
void CTopologyWidget::keyPressEvent( QKeyEvent * event )
{
//	int _key = event->key();
//	if (_key == 0x01000007)  //delete��
//	{
//		slotDeleteAction();
//	}

    switch(event->key())
    {
    case Qt::Key_Delete:slotDeleteAction();
        break;
    default:break;

    }
}

/**********************************************************
���ܣ��������ɫ���ı߽�С�����У����ָ��ı�
��������
����ֵ����
**********************************************************/
void CTopologyWidget::cursorShapeChange(QPointF _scenePos,CBoundingRectItem* _brItem)
{
	if (_brItem == NULL)
	{
		return;
	}

	QList<QGraphicsItem *> _itemL;
	QList<QGraphicsItem*> itemL;
	_itemL = _brItem->childItems();
	itemL.clear();
	for(int i = 0; i < _itemL.size(); ++i)
	{
		QGraphicsRectItem* t_rectItem = dynamic_cast<QGraphicsRectItem*>(_itemL[i]);
		if (t_rectItem != NULL)
		{
			itemL.append(_itemL[i]);
		}
	}
	//�ı����ָ��
	for ( int j = 0; j < itemL.size(); j++ )
	{
		if (itemL[j]->contains(itemL[j]->mapFromScene(_scenePos)))
		{
			if (j == 0 || j == 3)//��������ϽǺ����½ǵ�С������
			{						
				setCursor(QCursor(Qt::SizeFDiagCursor)); 
			}					
			else if (j == 4 || j == 6)//������ϱߺ��±ߵ�С������
			{
				setCursor(QCursor(Qt::SizeVerCursor));
			}
			else if (j == 1 || j == 2)//��������ϽǺ����½ǵ�С������
			{
				setCursor(QCursor(Qt::SizeBDiagCursor));
			}	
			else if (j == 5 || j == 7)//�������ߺ��ұߵ�С������
			{
				setCursor(QCursor(Qt::SizeHorCursor));
			}					
		}
	}		
}
/********************************************************************
���ܣ������б߽�С���β��ƶ�ʱ�������ƶ������
��������
����ֵ����
*********************************************************************/
void CTopologyWidget::CreateMovableRect(QPointF _scenePos)
{
	QList<QGraphicsItem*> t_groupItenList = m_itemGroup->childItems();
	if (t_groupItenList.size() != 1)
	{
		return;
	}
	CSwitcher* t_switcherItem = NULL;
	t_switcherItem = dynamic_cast<CSwitcher*>(t_groupItenList[0]);
    if (t_switcherItem != NULL)   //ֻ��ѡ��һ��������ͼԪ
	{ 
		CBoundingRectItem* t_switchBRItem = NULL;
		for (int i = 0; i < t_switcherItem->childItems().size(); ++i)
		{
			t_switchBRItem = dynamic_cast<CBoundingRectItem*>(t_switcherItem->childItems()[i]);
			if (t_switchBRItem != NULL)
			{
				break;
			}
		}
		if (t_switchBRItem != NULL)
		{
			QList<QGraphicsItem *> itemL = t_switchBRItem->childItems();
			//�϶�С����ʱ������̬��
			for ( int j = 0; j < itemL.size(); j++ )
			{			
				QRectF temR;
				temR = t_switcherItem->sceneBoundingRect();		

				qreal tmpE = temR.bottomRight().x();
				qreal tmpW = temR.topLeft().x(); 
				qreal tmpN = temR.topLeft().y();
				qreal tmpS =  temR.bottomRight().y();

				if (rect0sele)//�϶����Ͻǵľ���
				{
					qreal width = tmpE - _scenePos.x();
					qreal height = tmpS - _scenePos.y();
					QRectF t_rect;
					if (width <= 45 || height <=55)
					{
						return;
					}

					if(width >= 300 || height >=450)
					{
						return;
					}
				
					t_rect = QRectF(0,0,width,height);
					t_switcherItem->setBoundingRect(t_rect);				
					t_switcherItem->setPos(m_itemGroup->mapFromScene(QPointF(_scenePos.x(),_scenePos.y())));
					t_switcherItem->update();


					t_switchBRItem->setBoundingRect(t_rect);
					t_switchBRItem->drawBoundingItem();
					break ;	
				}
				if (rect3sele)//�϶����½ǵľ���
				{
					qreal width = _scenePos.x() - tmpW;
					qreal height = _scenePos.y() - tmpN;
					QRectF t_rect;
					if (width <= 45 || height <=55)
					{
						return;
					}


					if(width >= 300 || height >=450)
					{
						return;
					}
					
					t_rect = QRectF(0,0,width,height);				
					t_switcherItem->setBoundingRect(t_rect);	
					t_switcherItem->setPos(m_itemGroup->mapFromScene(QPointF(tmpW,tmpN)));
					t_switcherItem->update();	

					t_switchBRItem->setBoundingRect(t_rect);
					t_switchBRItem->drawBoundingItem();
					break ;	
				}
				if (rect1sele)//�϶����Ͻǵľ���
				{
					qreal width =  _scenePos.x() - tmpW;
					qreal height =  tmpS - _scenePos.y();
					QRectF t_rect;
					if (width <= 45 || height <=55)
					{
						return;
					}


					if(width >= 300 || height >=450)
					{
						return;
					}

					t_rect = QRectF(0,0,width,height);
					t_switcherItem->setBoundingRect(t_rect);			
					t_switcherItem->setPos(m_itemGroup->mapFromScene(QPointF(tmpW,_scenePos.y())));
					t_switcherItem->update();	
				

					t_switchBRItem->setBoundingRect(t_rect);
					t_switchBRItem->drawBoundingItem();
					t_switchBRItem->setVisible(true);

					break ;						
				}
				if (rect2sele)//�϶����½ǵľ���
				{
					qreal width = tmpE - _scenePos.x();
					qreal height = _scenePos.y() - tmpN;
					QRectF t_rect;
					if (width <= 45 || height <=55)
					{
						return;
					}

					if(width >= 300 || height >=450)
					{
						return;
					}

                    t_rect = QRectF(0,0,width,height);
					t_switcherItem->setBoundingRect(t_rect);		
					t_switcherItem->setPos(m_itemGroup->mapFromScene(QPointF(_scenePos.x(),tmpN)));
					t_switcherItem->update();	


					t_switchBRItem->setBoundingRect(t_rect);
					t_switchBRItem->drawBoundingItem();
					t_switchBRItem->setVisible(true);
					break ;						
				}
				if (rect4sele)//�϶��ϱ�С����
				{
					qreal width = tmpE - tmpW;
					qreal height = tmpS - _scenePos.y();
					if (height <=55)
					{
						return;
					}


					if(height >=450)
					{
						return;
					}

					QRectF t_rect(0,0,width,height);

					t_switcherItem->setBoundingRect(t_rect);		
					t_switcherItem->setPos(m_itemGroup->mapFromScene(QPointF(tmpW,_scenePos.y())));
					t_switcherItem->update();	
	

					t_switchBRItem->setBoundingRect(t_rect);
					t_switchBRItem->drawBoundingItem();
					t_switchBRItem->setVisible(true);
					break ;								
				}
				if (rect6sele)//�϶��±�С����
				{
					qreal width = tmpE - tmpW;
					qreal height = _scenePos.y() - tmpN;
					if ( height <=55)
					{
						return;
					}


					if(height >=450)
					{
						return;
					}

					QRectF t_rect(0,0,width,height);

					t_switcherItem->setBoundingRect(t_rect);			
					t_switcherItem->setPos(m_itemGroup->mapFromScene(QPointF(tmpW,tmpN)));
					t_switcherItem->update();	

					t_switchBRItem->setBoundingRect(t_rect);
					t_switchBRItem->drawBoundingItem();
					t_switchBRItem->setVisible(true);
					break ;							
				}
				if (rect5sele)//�϶����С����
				{		
					qreal width =  tmpE - _scenePos.x();
					qreal height = tmpS - tmpN;
					if (width <= 45)
					{
						return;
					}

					if(width >= 300)
					{
						return;
					}

					QRectF t_rect(0,0,width,height);

					t_switcherItem->setBoundingRect(t_rect);		
					t_switcherItem->setPos(m_itemGroup->mapFromScene(QPointF(_scenePos.x(),tmpN)));
					t_switcherItem->update();	
	

					t_switchBRItem->setBoundingRect(t_rect);
					t_switchBRItem->drawBoundingItem();
					t_switchBRItem->setVisible(true);
					break ;							
				}
				if (rect7sele)//�϶��ұ�С����
				{
					qreal width = _scenePos.x() - tmpW;
					qreal height = tmpS - tmpN;
					if (width <=45)
					{
						return;
					}

					if(width >= 300)
					{
						return;
					}

					QRectF t_rect(0,0,width,height);

					t_switcherItem->setBoundingRect(t_rect);				
					t_switcherItem->setPos(m_itemGroup->mapFromScene(QPointF(tmpW,tmpN)));
					t_switcherItem->update();	

					t_switchBRItem->setBoundingRect(t_rect);
					t_switchBRItem->drawBoundingItem();
					break ;						
				}			
			}	
		}
	}
	return ;
}

/**********************************************************
���ܣ������Ҽ��˵�
��������
����ֵ����
**********************************************************/
void CTopologyWidget::creatRightMenu()
{
	m_rightMenu = new QMenu(this);

    m_copyAct = new QAction(tr("����"),this);
    m_pasteAct = new QAction(tr("ճ��"),this);
    connect(m_copyAct,SIGNAL(triggered() ), this, SLOT(slotCopyAction() ) );
    connect(m_pasteAct, SIGNAL(triggered() ), this, SLOT(slotPasteAction() ) );

    m_deleteAct = new QAction(tr("ɾ��"),this);
	m_attributeAct = new QAction(tr("�鿴����"),this);
	connect(m_deleteAct,SIGNAL(triggered()),this,SLOT(slotDeleteAction()));
	connect(m_attributeAct,SIGNAL(triggered()),this,SLOT(slotAttributeAction()));

//	m_configuerAct = new QAction(tr("���ò���"), this);
//	connect(m_configuerAct,SIGNAL(triggered()), this, SLOT(slotConfigureAction()));

    m_routeVisible = new QAction(tr("·�ɿ��ӻ�"), this);
    connect(m_routeVisible, SIGNAL(triggered()), this, SLOT(slotRouteVisible()));

    m_delayAct = new QAction(tr("�˵���ʱ��"), this);
    connect(m_delayAct, SIGNAL(triggered()), this, SLOT(slotDelayAction()));


	m_rightMenu->addSeparator();
    //m_rightMenu->addAction(m_deleteAct);
	m_rightMenu->addAction(m_attributeAct);
//	m_rightMenu->addAction(m_configuerAct);
   // m_rightMenu->addAction(m_routeVisible);
    //m_rightMenu->addAction(m_delayAct);
    m_deleteAct->setEnabled(false);
    m_attributeAct->setEnabled(false);
    m_routeVisible->setEnabled(false);
    m_delayAct->setEnabled(false);
}
/**********************************************************
���ܣ�����ͼԪ
��������
����ֵ����
**********************************************************/
void CTopologyWidget::slotCopyAction()
{
}

/**********************************************************
���ܣ�ճ��ͼԪ
��������
����ֵ����
**********************************************************/
void CTopologyWidget::slotPasteAction()
{
}

/**********************************************************
���ܣ�ɾ����ť
��������
����ֵ����
**********************************************************/
void CTopologyWidget::slotDeleteAction()
{

	QMessageBox::StandardButton rb = QMessageBox::question(this, tr("ɾ��ѡ����") , tr("��ȷ��Ҫɾ��ѡ������?") ,QMessageBox::Yes|QMessageBox::No , QMessageBox::Yes);

	if (rb == QMessageBox::No)
	{
		return;
	}

	QList<QGraphicsItem*> t_childItems = m_itemGroup->childItems();
    QList<QGraphicsItem*> t_itemlist = m_scene->items(QRectF(0,0,m_scene->width(),m_scene->height()));
    QList<CSwitcher*> t_switchers ;
    for(int i = 0; i < t_itemlist.size(); i++)
    {
        CSwitcher* t_item = dynamic_cast <CSwitcher *>(t_itemlist[i]);
        if(t_item != NULL)t_switchers.push_back(t_item);
    }

    for (int m = 0; m < t_childItems.size(); ++m)
	{
        CSwitcher* t_selectedSwitch = dynamic_cast<CSwitcher*>(t_childItems[m]);

        if (t_selectedSwitch != NULL)
        {
            //ɾ��Link�Ͷ˿�
            if(! t_selectedSwitch->getSwitcherInfo().LNodes.isEmpty())
            {
                for(int i = 0; i < t_selectedSwitch->getSwitcherInfo().LNodes.size(); i++)
                {
                    for(int j = 0; j < t_switchers.size(); j++)
                    {
                        if(t_switchers[j]->getSwitcherInfo().ID == t_selectedSwitch->getSwitcherInfo().LNodes[i].linkPort_Device_ID)
                        for(int k = 0; k < t_switchers[j]->getSwitcherInfo().LNodes.size(); k++)
                        {
                            if(t_switchers[j]->getSwitcherInfo().LNodes[k].linkPort_Device_ID == t_selectedSwitch->getSwitcherInfo().ID)
                            {
                                QString delIP = t_switchers[j]->getSwitcherInfo().LNodes[k].IP;
                                QString delIP2 = t_switchers[j]->getSwitcherInfo().LNodes[k].linkPort_IP;
                                t_IPPond.erase(delIP);
                                t_IPPond.erase(delIP2);
                                m_IPPool.erase(calculateNetworkID(delIP,t_switchers[j]->getSwitcherInfo().LNodes[k].subnetMask));
                                t_switchers[j]->getSwitcherInfo().LNodes.erase(t_switchers[j]->getSwitcherInfo().LNodes.begin() + k);
                            }
                        }
                    }
                }
            }

            for (int i = 0; i < m_vlink.size();++i)
            {
                if (m_vlink[i] != NULL && m_vlink[i]->getStartPort() != NULL && m_vlink[i]->getEndPort() != NULL)
                {
                    if(m_vlink[i]->getLinkInfo().s_switchID == t_selectedSwitch->getSwitcherInfo().ID ||
                       m_vlink[i]->getLinkInfo().d_switchID == t_selectedSwitch->getSwitcherInfo().ID)
                    {
                        m_scene->removeItem(m_vlink[i]);
                        m_vlink.erase(m_vlink.begin() + i);
                        i--;
                    }
                }
            }
            for (size_t k = 0; k < m_vSwtich.size();++k)
            {
                if (t_selectedSwitch->getSwitcherInfo().name == m_vSwtich[k].name)
                {
                    m_vSwtich.erase(m_vSwtich.begin() + k);
                    k--;
                }
            }
        }
        m_itemGroup->removeFromGroup(t_childItems[m]);
        delete t_childItems[m];
        t_childItems[m] = NULL;
	}
    m_moveitemFlag = false;
}

/**********************************************************
���ܣ�����ͼԪ�Ҽ������԰�ť
��������
����ֵ����
**********************************************************/
void CTopologyWidget::slotAttributeAction()
{
	//ѡ��һ��������ͼԪ����һ���豸ͼԪʱ���˫��
	QList<QGraphicsItem*> t_childItem = m_itemGroup->childItems();
	if (t_childItem.size() != 1)
	{
		return;
    }
    CSwitcher* t_switchItem = NULL;
    Clink* t_linkItem = NULL;

    t_switchItem = dynamic_cast<CSwitcher*>(t_childItem[0]);
    t_linkItem = dynamic_cast<Clink*>(t_childItem[0]);
    if (t_switchItem != NULL)  //���������ԶԻ�����ʾ
    {
        m_EditSwitchItem = t_switchItem;
        m_switchDia->SetSwitcherInfo(t_switchItem->getSwitcherInfo());
        m_switchDia->setIPpond(&t_IPPond);
        m_switchDia->setIPPool(&m_IPPool);
        m_switchDia->exec();
    }   
    else if (t_linkItem != NULL)//��ʾ��·�豸���ԶԻ���
    {
        return;//�����
    }
}

/**********************************************************
���ܣ�����ͼԪ�Ҽ������԰�ť
��������
����ֵ����
**********************************************************/
//void CTopologyWidget::slotConfigureAction()
//{
//	int selected_switchID;
//	//ѡ��һ��������ͼԪ����һ���豸ͼԪʱ���˫��
//	QList<QGraphicsItem*> t_childItem = m_itemGroup->childItems();
//	if (t_childItem.size() != 1)
//	{
//		return;
//	}
//	CSwitcher* t_switchItem = NULL;
//	t_switchItem = dynamic_cast<CSwitcher*>(t_childItem[0]);

//    if (t_switchItem != NULL)
//    {
//        selected_switchID = t_switchItem->getSwitcherInfo().ID;
//        emit SendSwitchID(selected_switchID);
//        return;
//    }
//}
/**********************************************************
���ܣ����ͼԪ�Ҽ���·�ɿ��ӻ���ť
��������
����ֵ����
**********************************************************/
void CTopologyWidget::slotRouteVisible()
{
    QList<QGraphicsItem*> t_childItem = m_itemGroup->childItems();
    if(t_childItem.size() >= 2)
    {
        QFile t_file("routeVisible.txt");
        if (!t_file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        for(int i = 0; i != t_childItem.size(); ++i)
        {
            CSwitcher* t_switch = dynamic_cast<CSwitcher *>(t_childItem[i]);
            if(t_switch != NULL)
            {
                QTextStream out(&t_file);
                CSwitcherInfo t_switchInfo = t_switch->getSwitcherInfo();
                    out<<t_switchInfo.name<<endl;
            }
        }
    }
}
/**********************************************************
���ܣ����ͼԪ�Ҽ��Ķ˵���ʱ�Ӱ�ť
��������
����ֵ����
**********************************************************/
void CTopologyWidget::slotDelayAction()
{
    QList<QGraphicsItem*> t_childItem = m_itemGroup->childItems();
    if(t_childItem.size() != 2)
        return;
    else
        {
            QFile t_file("delay.txt");
            if (!t_file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;
            for(int i = 0; i != t_childItem.size(); ++i)
            {
                CSwitcher* t_switch = dynamic_cast<CSwitcher *>(t_childItem[i]);
                if(t_switch != NULL)
                {
                    QTextStream out(&t_file);
                    CSwitcherInfo t_switchInfo = t_switch->getSwitcherInfo();
                        out<<t_switchInfo.name<<endl;
                }
            }
        }
}

/**********************************************************
���ܣ�ɾ����ͼԪ
��������
����ֵ����
**********************************************************/
void CTopologyWidget::deleteChildItems(QGraphicsItem* _item)
{
	if (_item == NULL)
	{
		return;
	}

	CSwitcher* t_switchItem = dynamic_cast<CSwitcher*>(_item);
	QList<QGraphicsItem *> t_childItems;

	if (t_switchItem != NULL)
	{
		t_childItems = t_switchItem->childItems();
	}

	for (int i = 0; i < t_childItems.size(); ++i)	
	{
		CBoundingRectItem* t_brItem = NULL;
		if (t_childItems[i] != NULL)
		{
			t_brItem = dynamic_cast<CBoundingRectItem*>(t_childItems[i]);
			if (t_brItem != NULL)
			{
                m_scene->removeItem(t_childItems[i]);
                delete t_childItems[i];
				t_childItems[i] = NULL;
			}			
		}
    }
}

//void CTopologyWidget::OutputTopology(QString path)
//{
//    QFile t_file(path);
//    if (!t_file.open(QIODevice::WriteOnly | QIODevice::Text))
//        return;

//    QTextStream out(&t_file);

//    QList<QGraphicsItem*> t_itemlist = m_scene->items();

//    int item_type = -1;
//    int count = 0;

//    for (int i = 0; i < t_itemlist.count(); i++)
//    {
//        CSwitcher * t_switch = NULL;

//        t_switch = dynamic_cast<CSwitcher*>(t_itemlist[i]);

//        if (t_switch != NULL)
//        {
//            count++;
//        }
//    }

//    out<<count<<endl;
//    for (int i = 0; i < t_itemlist.size() ; i++)
//    {
//        QPointF t_pos = t_itemlist[i]->pos();
//        QRectF t_rect = t_itemlist[i]->boundingRect();
//        CSwitcher * t_switch = NULL;

//        t_switch = dynamic_cast<CSwitcher*>(t_itemlist[i]);

//        if (t_switch != NULL)
//        {
//            CSwitcherInfo t_switchInfo = t_switch->getSwitcherInfo();
//            item_type = 0;

//            out<<item_type<<endl;
//            out<<(double)t_pos.x()<<endl
//               <<(double)t_pos.y()<<endl;
//            out<<(double)t_rect.topLeft().x()<<endl
//               <<(double)t_rect.topLeft().y()<<endl
//               <<(double)t_rect.width()<<endl
//               <<(double)t_rect.height()<<endl;
//            out<<t_switchInfo.switchType<<endl
//               <<t_switchInfo.name<<endl
//               <<t_switchInfo.ID<<endl
//               <<t_switchInfo.portNum<<endl
//               <<t_switchInfo.protocol<<endl
//               <<t_switchInfo.networkLocation<<endl
//               <<t_switchInfo.LNodes.size()<<endl;
//            qDebug()<<t_switchInfo.ID<<endl;
//            for (int j = 0; j < t_switchInfo.LNodes.size(); j ++)
//            {

//                SNodeStructInfo &t_node = t_switchInfo.LNodes[j];
//                out<<t_node.Port_ID<<"\n";
//                out<<t_node.Device_ID<<"\n";
//                out<<t_switchInfo.networkLocation<<"\n";
//                out<<t_node.name<<"\n";
//                out<<t_node.interfaceName<<"\n";
//                out<<t_node.area<<"\n";
//                out<<t_node.IP<<"\n";
//                out<<t_node.subnetMask<<"\n";
//                out<<t_node.describe<<"\n";
//                out<<t_node.costValue<<"\n";
//                out<<t_node.remark<<"\n";

//                out<<t_node.linkPort_ID<<"\n";
//                out<<t_node.linkPort_Device_ID<<"\n";
//                out<<t_node.linkPort_name<<"\n";
//                out<<t_node.linkPort_interfaceName<<"\n";
//                out<<t_node.linkPort_area<<"\n";
//                out<<t_node.linkPort_IP<<"\n";
//                out<<t_node.linkPort_subnetMask<<"\n";
//                out<<t_node.linkPort_describe<<"\n";
//                out<<t_node.linkPort_costValue<<"\n";
//                out<<t_node.linkPort_remark<<"\n";

//                out<<t_node.linkType<<"\n";
//                out<<t_node.bandWidth<<"\n";//��·����
//                out<<t_node.delay<<"\n";
//            }
//        }
//    }
//    //int a = m_vlink.size();
//    out<<m_vlink.size()<<"\n";
//    if(m_vlink.size() != 0)
//    {
//        for (int i = 0; i < m_vlink.size(); i++)
//        {
//            int s_nodeID = 0, e_nodeID = 0, item_type = 1;
//            s_nodeID = m_vlink[i]->getLinkInfo().s_switchID;
//            e_nodeID = m_vlink[i]->getLinkInfo().e_switchID;
//            out<<item_type<<"\n";
//            out<<s_nodeID<<"\n";
//            out<<e_nodeID<<"\n";
//            out<<m_vlink[i]->getLinkInfo().linkType<<"\n";
//            out<<m_vlink[i]->getLinkInfo().bandWidth<<"\n";
//            out<<m_vlink[i]->getLinkInfo().delay<<"\n";
//            out<<m_vlink[i]->getLinkInfo().pos_link_num<<"\n";
//            out<<m_vlink[i]->getLinkInfo().ethernet_link_num<<"\n";
//            qDebug()<< s_nodeID<< " "<< e_nodeID<< endl;
//        }
//    }

//    t_file.close();
//}

//void CTopologyWidget::OutputPosition(QString path)
//{
//    QFile t_file(path);
//    if (!t_file.open(QIODevice::WriteOnly | QIODevice::Text))
//        return;

//    QTextStream out(&t_file);

//    QList<QGraphicsItem*> t_itemlist = m_scene->items();
//    out<<m_scene->width()<<","<<m_scene->height()<<endl;
//    for (int i = t_itemlist.size()-1; i >= 0 ; i--)
//    {
//        QPointF t_pos = t_itemlist[i]->pos();
//        CSwitcher * t_switch = NULL;

//        t_switch = dynamic_cast<CSwitcher*>(t_itemlist[i]);

//        if (t_switch != NULL)
//        {
//            CSwitcherInfo t_switchInfo = t_switch->getSwitcherInfo();
//            out<<t_switchInfo.name<<" "<<(double)t_pos.x()<<" "<<(double)t_pos.y()<<endl;
//        }
//    }

//   t_file.close();
//}

QString CTopologyWidget::DecToHex(QString AreaID, QString portID)
{
	QString str;
	int tmpValue = QVariant(portID).toInt()-1;
	if (tmpValue > 9 &&tmpValue < 16)
	{
		char tmp = tmpValue%10 + 'a';
		str = "0x0" + AreaID + "000" + tmp;
	}
	else if (tmpValue > 15)
	{
		if (tmpValue%16 > 9)
		{
			str = "0x0" + AreaID + "00" + QVariant(tmpValue/16).toString() + char(tmpValue%16%10 + 'a');
		}
		else
        {
			str = "0x0" + AreaID + "00" + QVariant(tmpValue/16).toString() + QVariant(tmpValue%16).toString();
		}

	}
	else
	{
		str = "0x0" + AreaID + "000" +QVariant(tmpValue).toString();
	}
    return str;
}

void CTopologyWidget::setSwitchInfTable(const CSwitcherInfo &_switcherInfo)
{
    if(m_platform->ui_DeviceInfTable->item(0,0))
    {
        m_platform->ui_DeviceInfTable->clear();
    }
    //m_platform->ui_DeviceInfTable->resize(0,0);

    m_platform->ui_DeviceInfTable->setRowCount(12);
    m_platform->ui_DeviceInfTable->setColumnCount(2);
    //m_platform->ui_DeviceInfTable->verticalHeader()->setHidden(true);
    //m_platform->ui_DeviceInfTable->horizontalHeader()->setHidden(true);

    QTableWidgetItem *newItem00 = new QTableWidgetItem(tr("����: "));
    newItem00->setTextAlignment(Qt::AlignCenter| Qt::AlignVCenter);
    m_platform->ui_DeviceInfTable->setItem(0, 0, newItem00);
    QTableWidgetItem *newItem01 = new QTableWidgetItem(_switcherInfo.typeName);
    newItem01->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
    m_platform->ui_DeviceInfTable->setItem(0, 1, newItem01);

    QTableWidgetItem *newItem10 = new QTableWidgetItem(tr("����: "));
    newItem10->setTextAlignment(Qt::AlignCenter| Qt::AlignVCenter);
    m_platform->ui_DeviceInfTable->setItem(1, 0, newItem10);
    QTableWidgetItem *newItem11 = new QTableWidgetItem(_switcherInfo.name);
    newItem11->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
    m_platform->ui_DeviceInfTable->setItem(1, 1, newItem11);

    QTableWidgetItem *newItem20 = new QTableWidgetItem(tr("ID: "));
    newItem20->setTextAlignment(Qt::AlignCenter| Qt::AlignVCenter);
    m_platform->ui_DeviceInfTable->setItem(2, 0, newItem20);
    QTableWidgetItem *newItem21 = new QTableWidgetItem(QString("%1").arg(_switcherInfo.ID));
    newItem21->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
    m_platform->ui_DeviceInfTable->setItem(2, 1, newItem21);

    QTableWidgetItem *newItem30 = new QTableWidgetItem(tr("�˿���: "));
    newItem30->setTextAlignment(Qt::AlignCenter| Qt::AlignVCenter);
    m_platform->ui_DeviceInfTable->setItem(3, 0, newItem30);
    QTableWidgetItem *newItem31 = new QTableWidgetItem(QString("%1").arg(_switcherInfo.portNum));
    newItem31->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
    m_platform->ui_DeviceInfTable->setItem(3, 1, newItem31);


    m_platform->ui_DeviceInfTable->setSelectionMode(QAbstractItemView::SingleSelection);   //����ѡ���ģʽΪ��ѡ��
    m_platform->ui_DeviceInfTable->setSelectionBehavior(QAbstractItemView::SelectRows);    //����ѡ����Ϊʱÿ��ѡ��һ��
    //m_platform->ui_DeviceInfTable->setShowGrid(false);   //���ò���ʾ������
    //m_platform->ui_DeviceInfTable->resizeColumnToContents(0);
    //m_platform->ui_DeviceInfTable->resizeColumnToContents(1);
    m_platform->ui_DeviceInfTable->resizeColumnsToContents();
    m_platform->ui_DeviceInfTable->horizontalHeader()->setStretchLastSection(true);
    m_platform->ui_DeviceInfTable->verticalHeader()->setStretchLastSection(true);

    //m_platform->ui_DeviceInfTable->resize(m_platform->ui_DeviceInfTable->columnWidth(0)+ m_platform->ui_DeviceInfTable->columnWidth(1),\
                                         // m_platform->ui_DeviceInfTable->rowCount()*m_platform->ui_DeviceInfTable->rowHeight(0));
}

void CTopologyWidget::setHostInfTable(const CSwitcherInfo &_switcherInfo)
{
    if(m_platform->ui_DeviceInfTable->item(0,0))
    {
        m_platform->ui_DeviceInfTable->clear();
    }

    //m_platform->ui_DeviceInfTable->resize(0,0);

    m_platform->ui_DeviceInfTable->setRowCount(12);
    m_platform->ui_DeviceInfTable->setColumnCount(2);
//    m_platform->ui_DeviceInfTable->verticalHeader()->setHidden(true);
//    m_platform->ui_DeviceInfTable->horizontalHeader()->setHidden(true);


    m_platform->ui_DeviceInfTable->horizontalHeader()->setStretchLastSection(true);
    m_platform->ui_DeviceInfTable->verticalHeader()->setStretchLastSection(true);

    QTableWidgetItem *newItem00 = new QTableWidgetItem(tr("����: "));
    newItem00->setTextAlignment(Qt::AlignCenter| Qt::AlignVCenter);
    m_platform->ui_DeviceInfTable->setItem(0, 0, newItem00);
    QTableWidgetItem *newItem01 = new QTableWidgetItem(_switcherInfo.typeName);
    newItem01->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
    m_platform->ui_DeviceInfTable->setItem(0, 1, newItem01);

    QTableWidgetItem *newItem10 = new QTableWidgetItem(tr("����: "));
    newItem10->setTextAlignment(Qt::AlignCenter| Qt::AlignVCenter);
    m_platform->ui_DeviceInfTable->setItem(1, 0, newItem10);
    QTableWidgetItem *newItem11 = new QTableWidgetItem(_switcherInfo.name);
    newItem11->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
    m_platform->ui_DeviceInfTable->setItem(1, 1, newItem11);

    QTableWidgetItem *newItem20 = new QTableWidgetItem(tr("MAC: "));
    newItem20->setTextAlignment(Qt::AlignCenter| Qt::AlignVCenter);
    m_platform->ui_DeviceInfTable->setItem(2, 0, newItem20);
    QTableWidgetItem *newItem21 = new QTableWidgetItem(QString("%1").arg(_switcherInfo.macAddr));
    newItem21->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
    m_platform->ui_DeviceInfTable->setItem(2, 1, newItem21);

    QTableWidgetItem *newItem30 = new QTableWidgetItem(tr("ID: "));
    newItem30->setTextAlignment(Qt::AlignCenter| Qt::AlignVCenter);
    m_platform->ui_DeviceInfTable->setItem(3, 0, newItem30);
    QTableWidgetItem *newItem31 = new QTableWidgetItem(QString("%1").arg(_switcherInfo.ID));
    newItem31->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
    m_platform->ui_DeviceInfTable->setItem(3, 1, newItem31);

    QTableWidgetItem *newItem40 = new QTableWidgetItem(tr("���ӽ�����DPID: "));
    newItem40->setTextAlignment(Qt::AlignCenter| Qt::AlignVCenter);
    m_platform->ui_DeviceInfTable->setItem(4, 0, newItem40);
    QTableWidgetItem *newItem41 = new QTableWidgetItem(QString("%1").arg(_switcherInfo.attachSDPID));
    newItem41->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
    m_platform->ui_DeviceInfTable->setItem(4, 1, newItem41);

    QTableWidgetItem *newItem50 = new QTableWidgetItem(tr("���ӵĽ�����ID: "));
    newItem50->setTextAlignment(Qt::AlignCenter| Qt::AlignVCenter);
    m_platform->ui_DeviceInfTable->setItem(5, 0, newItem50);
    QTableWidgetItem *newItem51 = new QTableWidgetItem(QString("%1").arg(_switcherInfo.attachSId));
    newItem51->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
    m_platform->ui_DeviceInfTable->setItem(5, 1, newItem51);

    QTableWidgetItem *newItem60 = new QTableWidgetItem(tr("�������˿ں�: "));
    newItem60->setTextAlignment(Qt::AlignRight| Qt::AlignVCenter);
    m_platform->ui_DeviceInfTable->setItem(6, 0, newItem60);
    QTableWidgetItem *newItem61 = new QTableWidgetItem(QString("%1").arg(_switcherInfo.attachSPort));
    newItem61->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
    m_platform->ui_DeviceInfTable->setItem(6, 1, newItem61);


    m_platform->ui_DeviceInfTable->setSelectionMode(QAbstractItemView::SingleSelection);   //����ѡ���ģʽΪ��ѡ��
    m_platform->ui_DeviceInfTable->setSelectionBehavior(QAbstractItemView::SelectRows);    //����ѡ����Ϊʱÿ��ѡ��һ��
    //m_platform->ui_DeviceInfTable->setShowGrid(false);   //���ò���ʾ������

    //m_platform->ui_DeviceInfTable->resizeColumnToContents(0);
    //m_platform->ui_DeviceInfTable->resizeColumnToContents(1);
    m_platform->ui_DeviceInfTable->resizeColumnsToContents();
    //m_platform->ui_DeviceInfTable->resizeRowsToContents();
    qDebug()<<"width0: "<<m_platform->ui_DeviceInfTable->columnWidth(0);
    qDebug()<<"width1: "<<m_platform->ui_DeviceInfTable->columnWidth(1);
    m_platform->ui_DeviceInfTable->resize(m_platform->ui_DeviceInfTable->columnWidth(0)+m_platform->ui_DeviceInfTable->columnWidth(1)+10,\
                                          m_platform->ui_DeviceInfTable->rowCount()*m_platform->ui_DeviceInfTable->rowHeight(0));

}

bool CTopologyWidget::getSwitchHostDevice()
{
    QString t_switchName;//������curl��Ϣ
    QString t_hostDevices;//host curl��Ϣ
    QVector<QString> t_switchNames;//���������ּ���
    m_curl->getSwitchesInf(t_switchName);
    if(t_switchName.isEmpty())
        return false;
    m_curl->getHostDevices(t_hostDevices);
    if(t_hostDevices.isEmpty())
        return false;

    QJson::Parser parser;
    bool ok;

    //��������
    QVariantList t_sDevicesList = parser.parse(t_switchName.toUtf8(), &ok).toList();
    if (!ok) {
        qFatal("An error occurred during parsing");
        exit (1);
    }
    for(int i=0; i<t_sDevicesList.size(); ++i)
    {
       QVariantMap t_deviceMap= t_sDevicesList.at(i).toMap();
       QString t_sName =  t_deviceMap["switchDPID"].toString();
       qDebug()<<"Sname: "<<t_sName;
       t_switchNames.append(t_sName);
    }
    //ͼԪid���
    m_id = 0;
    for(int i=0; i<t_switchNames.size();++i)
    {
        struct SwitchNode *t_node1 = new SwitchNode;
        t_node1->type = SWITCH;
        t_node1->DPID = t_switchNames.at(i);
        t_node1->SID = t_switchNames.at(i).right(2).toInt();
        t_node1->ID = m_id++;
        t_node1->hardwareDesc = t_switchNames.at(i);
        t_node1->portNum = 3;
        t_node1->level = 2;
        t_node1->portInfList.clear();

        NodeInf t_nInf1;
        t_nInf1.type = SWITCH;
        t_nInf1.pNode = t_node1;
        m_switchIDmap.insert(t_node1->ID,t_node1);
        m_switchSIDmap.insert(t_node1->SID, t_node1);
        m_switchNameMap.insert(t_node1->DPID,t_node1);
        m_nodelist.append(t_nInf1);
    }

    //����host
    QVariantList t_hDevicesList = parser.parse(t_hostDevices.toUtf8(), &ok).toList();
    if (!ok) {
        qFatal("An error occurred during parsing");
        exit (1);
    }

    //qDebug() << result.size();
    //qDebug() << result;
    for(int i=0; i<t_hDevicesList.size(); ++i)
    {
        qDebug() << "$$$$$$***********************$$$$$";
        QVariantMap t_deviceMap = t_hDevicesList.at(i).toMap();
       // qDebug() << t_deviceMap.size();
        //qDebug() << t_deviceMap;
        QVariantList t_attachSList = t_deviceMap["attachmentPoint"].toList();
        //qDebug() << t_attachSList.size();
        //qDebug() << t_attachSList;

        QString t_attachSDPID;
        int t_sid;
        int t_attachSPortNum;
        for(int j=0; j<t_attachSList.size(); ++j)
        {
            QVariantMap t_attachMap = t_attachSList.at(j).toMap();
            //qDebug() << t_attachMap.size();
            //qDebug() << t_attachMap;
            //���ӵĽ�����DPID
            t_attachSDPID = t_attachMap["switchDPID"].toString();
            //���ӵĽ������˿�
            t_attachSPortNum = t_attachMap["port"].toInt();
            qDebug() << "DPID: "<<t_attachSDPID;
            qDebug() << "port: "<<t_attachSPortNum;

            if(t_attachSDPID.isEmpty())
            {
                t_sid = -1;
                t_attachSPortNum = -1;
            }
            else{
                t_sid = t_attachSDPID.right(2).toInt();
            }

        }
        //host��mac��ַ
        QString t_hostMac = t_deviceMap["mac"].toList().at(0).toString();
        qDebug() << "host mac: " << t_hostMac;

        HostNode *t_h1 = new HostNode;
        t_h1->type = HOST;
        t_h1->macAddr = t_hostMac;
        t_h1->ID = m_id++;
        t_h1->switchDPID = t_attachSDPID;
        t_h1->attachSId = t_sid;
        t_h1->attachSPort = t_attachSPortNum;
        t_h1->userName = t_hostMac;
        t_h1->userGroup = "group";
        t_h1->level = 0;

        if(!t_h1->switchDPID.isEmpty())//��ֹΪ��
        {
            NodeInf t_nInf;
            t_nInf.type = HOST;
            t_nInf.pNode = t_h1;
            m_nodelist.append(t_nInf);

            LinkInf t_link;
            t_link.src_nodeId=m_switchNameMap[t_h1->switchDPID]->ID;
            t_link.src_port = t_h1->attachSPort;
            t_link.des_nodeId=t_h1->ID;
            t_link.des_port = 0;
            m_linkList.append(t_link);
        }
    }

    return true;
}

bool CTopologyWidget::getTopoLinks()
{
    QString t_topoStr;//����curl��Ϣ
    m_curl->getTopo(t_topoStr);
    if(t_topoStr.isEmpty())
        return false;

    QJson::Parser parser;
    bool ok;
    QVariantList t_topoList = parser.parse(t_topoStr.toUtf8(), &ok).toList();
    if (!ok) {
        qFatal("An error occurred during parsing");
        exit (1);
    }

//    qDebug() << t_topoList.size();
//    qDebug() << t_topoList;

    for(int i=0; i<t_topoList.size(); ++i)
    {
        QVariantMap t_linkMap = t_topoList.at(i).toMap();
        //qDebug() << t_linkMap.size();
        //qDebug() << t_linkMap;

        QString t_dstSwitch = t_linkMap["dst-switch"].toString();
        int t_dstPort = t_linkMap["dst-port"].toInt();
        QString t_srcSwitch = t_linkMap["src-switch"].toString();
        int t_srcPort = t_linkMap["src-port"].toInt();
        qDebug() <<"links:"<< t_srcSwitch << ":"<<t_srcPort<<"<->"<<t_dstSwitch<<":"<<t_dstPort;

        LinkInf t_link;
        t_link.src_nodeId=m_switchNameMap[t_srcSwitch]->ID;
        qDebug() <<"src Id"<< t_link.src_nodeId;
        t_link.src_port = t_srcPort;
        t_link.des_nodeId= m_switchNameMap[t_dstSwitch]->ID;
        qDebug() <<"dst Id"<< t_link.des_nodeId;
        t_link.des_port = t_dstPort;
        m_linkList.append(t_link);
    }

    return true;
}

void CTopologyWidget::RefreshTopology()
{
    if(!m_nodelist.isEmpty())
    {
        for (int i = 0; i < m_nodelist.size(); i++)
        {
            if (m_nodelist[i].type == SWITCH)
            {
                SwitchNode *t_switch = (SwitchNode *)(m_nodelist[i].pNode);
                delete t_switch;
             }

            else if (m_nodelist[i].type == HOST)
            {
                HostNode *t_host = (HostNode *)(m_nodelist[i].pNode);
                delete t_host;
            }
        }
        m_nodelist.clear();
    }
    if(!m_linkList.isEmpty())
    {
        m_linkList.clear();
    }

    if(!getSwitchHostDevice())
        return;
    if(!getTopoLinks())
        return;

    m_platform->historyListWidget->append(NetworkSimulationPlatform::getSysTime()+tr(": refreshing topology..."));
    clearScene();
    this->centerOn(0,0);

    QPointF t_pos;
    QRectF t_rect;
    double oriX = 600;
    double oriY = 350;

    int ms=0,mh=0;

    for (int i = 0; i < m_nodelist.size(); i++)
    {
        double x,y,width,height;
        x=y=0.0;
        CSwitcherInfo t_switchInfo;

        if (m_nodelist[i].type == SWITCH)
        {
            SwitchNode *t_switch = (SwitchNode *)(m_nodelist[i].pNode);

            t_pos.setX(oriX + 100*ms);
            t_pos.setY(oriY + 100*(t_switch->level));
            width = 60;
            height = 60;
            t_rect = QRectF(x,y,width,height);

            t_switchInfo.type = t_switch->type;
            t_switchInfo.typeName = "Switch";
            t_switchInfo.name = t_switch->DPID;
            t_switchInfo.ID = t_switch->ID;
            t_switchInfo.portNum = t_switch->portNum;
            ms++;
            //t_switchInfo.networkLocation = lineList[lineNum];
        }

        else if (m_nodelist[i].type == HOST)
        {
            HostNode *t_host = (HostNode *)(m_nodelist[i].pNode);

            t_pos.setX(oriX + 100*mh);
            t_pos.setY(oriY + 100*(t_host->level));
            width = 50;
            height = 50;
            t_rect = QRectF(x,y,width,height);

            t_switchInfo.type = t_host->type;
            t_switchInfo.typeName = "Host";
            t_switchInfo.name = t_host->macAddr;
            t_switchInfo.ID = t_host->ID;
            t_switchInfo.portNum = 1;
            t_switchInfo.attachSId = t_host->attachSId;
            t_switchInfo.attachSPort = t_host->attachSPort;
            t_switchInfo.attachSDPID = t_host->switchDPID;
            t_switchInfo.macAddr = t_host->macAddr;
            mh++;
            //t_switchInfo.networkLocation = lineList[lineNum];
        }
        else{
            return;
        }
        CSwitcher* t_switch = new CSwitcher(NULL,m_scene);
        t_switch->setPos(t_pos);
        t_switch->setToolTip(tr("����: ") + t_switchInfo.name + "\n"+
                             tr("�ͺ�:") + t_switchInfo.typeName);
        t_switch->setBoundingRect(t_rect);

        CNetworkPort *t_port = new CNetworkPort(t_switch, m_scene);
        QRectF m_boundingRect = t_switch->boundingRect();
        t_port->setPos(m_boundingRect.width()/2, m_boundingRect.height()/2+5);
        t_port->setID(t_switchInfo.ID);
        t_switch->setSwitcherInfo(t_switchInfo);
        m_vSwtich.push_back(t_switchInfo);

        t_vPortInOut.push_back(t_port);
    }

    for(int i=0; i < m_linkList.size();++i)
    {
        int s_swichID = m_linkList[i].src_nodeId;

        int d_swichID = m_linkList[i].des_nodeId;
        int ss_ID = 0, dd_ID = 0;
        for(size_t i = 0; i < t_vPortInOut.size(); i++)
        {
            if(t_vPortInOut[i]->getID() == s_swichID)
                ss_ID = i;
            else if(t_vPortInOut[i]->getID() == d_swichID)
                dd_ID = i;
        }
        Clink* t_link = new Clink(m_scene,t_vPortInOut[ss_ID], t_vPortInOut[dd_ID]);
        CLinkInfo t_linkInfo;
        t_linkInfo.s_switchID = s_swichID;
        t_linkInfo.d_switchID = d_swichID;
        t_linkInfo.bandWidth = "10M";
        //t_linkInfo.delay = _delay;

        t_link->setLinkInfo(t_linkInfo);
        QString linkType = "Ethernet";
        t_link->setToolTip(tr("����:") + linkType + "\n" +
                         tr("����:") + t_linkInfo.bandWidth);

        m_scene->addItem(t_link);
        m_itemGroup->addToGroup(t_link);

        //updata all same type links, hide items
        for ( int i = 0; i < m_vlink.size(); ++i )
        {
            CLinkInfo& tmpLinkInfo = m_vlink[i]->getLinkInfo();
            if ((tmpLinkInfo.s_switchID == s_swichID && tmpLinkInfo.d_switchID == d_swichID) ||
                (tmpLinkInfo.s_switchID == d_swichID && tmpLinkInfo.d_switchID == s_swichID))
            {
                m_vlink[i]->hide();
            }
        }

        m_vlink.push_back(t_link);
    }
    m_platform->historyListWidget->append(NetworkSimulationPlatform::getSysTime()+tr(": refresh finished!"));
}

void CTopologyWidget::zoomIn()
{
    QMatrix t_matrix;
    t_matrix.scale(1.25,1.25);
    setMatrix(t_matrix, true);
    m_zoom++;
}

void CTopologyWidget::zoomOut()
{
        this->centerOn(750,750);
        QMatrix t_matrix;
        t_matrix.scale(1.0/1.25,1.0/1.25);
        setMatrix(t_matrix,true);
        m_zoom--;


        //this->move(150, 150);
}

QString CTopologyWidget::calculateNetworkID(QString _IP, QString _mask)
{
    QList<QString> ipList = _IP.split('.');
    QList<QString> maskList = _mask.split('.');
    if(ipList.size() != 4 || maskList.size() != 4)
    {
        QMessageBox::critical(this, tr("����"),tr("IP��ַ�������������!"),QMessageBox::Ok);
        return NULL;
    }
    QString networkID1 = QString::number(ipList[0].toInt() & maskList[0].toInt());
    QString networkID2 = QString::number(ipList[1].toInt() & maskList[1].toInt());
    QString networkID3 = QString::number(ipList[2].toInt() & maskList[2].toInt());
    QString networkID4 = QString::number(ipList[3].toInt() & maskList[3].toInt());
    return networkID1 + "." + networkID2 + "." + networkID3 + "." + networkID4;
}

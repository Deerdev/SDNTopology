#include "CNetworkEditWidget.h"
#include <algorithm>

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

    //对话框
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
    drawLegend();
}

CTopologyWidget::~CTopologyWidget()
{

}

void CTopologyWidget::setLabelWidget(CUnitboxWidget* _unitW)
{
    //放图片标签的窗口
	m_unitWidget = _unitW;	
}

//清空画布并初始化成员变量
void CTopologyWidget::clearScene(void)
{
    //清空框选组图元
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

//画图例
void CTopologyWidget::drawLegend()
{
    QFont t_font("Times New Roman");
    t_font.setPointSize(10);
    m_scene->addText("POS", t_font)->setPos(1250, 1410);
    m_scene->addText("Ethernet", t_font)->setPos(1240, 1440);
    m_scene->addText(tr("一条"), t_font)->setPos(1400, 1350);
    m_scene->addText(tr("二条"), t_font)->setPos(1400, 1380);
    m_scene->addText(tr("三条"), t_font)->setPos(1400, 1410);
    m_scene->addText(tr("四条"), t_font)->setPos(1400, 1440);
    m_scene->addText(tr("五条"), t_font)->setPos(1400, 1470);
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

					//生成id
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
                    //画设备前属性对话框
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
                QMessageBox::information(m_newSwitcherDia,tr("警告"),tr("相同名称的路由器已经存在"));
				return;
			}
		}

        for (size_t i = 0; i < m_vSwtich.size(); ++i)
		{
			if (m_newSwitcherDia->getSwitcherInfo().ID == m_vSwtich[i].ID)
			{
                QMessageBox::information(m_newSwitcherDia,tr("警告"),tr("相同域名的路由器已经存在"));
				return;
			}
		}

        CSwitcher* t_switch = new CSwitcher(NULL, m_scene);
        CNetworkPort *t_port = new CNetworkPort(t_switch, m_scene);
        QRectF m_boundingRect = t_switch->boundingRect();

        t_switch->setPos(m_dropPos);
        t_switch->setToolTip(tr("名称: ") + t_switch->getSwitcherInfo().name + "\n"+
                             tr("型号:") + t_switch->getSwitcherInfo().switchType + "\n" +
                             tr("位置: ") + t_switch->getSwitcherInfo().networkLocation);

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
                QMessageBox::information(m_switchDia,tr("警告"),tr("相同名称的交换机已经存在"));
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

//鼠标选中事件，编辑虚线边框
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
        if(t_backItem != NULL || t_item == NULL) //点击背景或空白  取消选中的图元  删除子图元
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
            switch(t_item->type())
            {
            case QGraphicsItem::UserType + 1://swicher
                t_switchItem = dynamic_cast<CSwitcher*>(t_item);

                if (t_switchItem->getSelectedFlag() == false)  //没被选中
                {
                    if (event->modifiers() != 0x04000000)  //没有按下ctrl键  清除组中的图元
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
            // 拖动放大与缩小  只选中一个图元的时候才可放大缩小
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
                            if (itemL[j]->contains(itemL[j]->mapFromScene(scenePos)))   //点中小矩形的时候 才为真
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
功能：鼠标移动事件
参数:   无
返回值：无
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
		if (t_smallRect != NULL && t_smallRect->parentItem() != NULL) //表示矩形框小方格被选中
		{
			cursorShapeChange(nowScenePos,dynamic_cast<CBoundingRectItem*>(t_smallRect->parentItem()));//光标在小方框处时改变形状
		}
		else
		{
			setCursor(QCursor(Qt::ArrowCursor));
		}
	}


	if (event->buttons() & Qt::LeftButton)
	{
		//拖动选中的图元
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
        //创建临时虚线
        else if(m_tempadge != NULL)
        {
            QPointF m_portPos = m_pressItem->parentItem()->mapToScene(m_pressItem->mapToParent(QPointF(0,0)));
            m_tempadge->setLine(m_portPos.x(), m_portPos.y(), nowScenePos.x(), nowScenePos.y());
        }
		//创建框选虚线框
		else if (m_dragRectItem != NULL)
		{
            if(nowScenePos.x() > m_pressScenePoint_formove.x() && nowScenePos.y() > m_pressScenePoint_formove.y())
                m_dragRectItem->setRect(0,0,nowScenePos.x() - m_pressScenePoint_formove.x(),nowScenePos.y() - m_pressScenePoint_formove.y());
		}
		else if (rect0sele==true||rect1sele==true||rect2sele==true||rect3sele==true||rect4sele==true||rect5sele==true||rect6sele==true||rect7sele==true)//点重小矩形且拖动的时候
		{			
			CreateMovableRect(nowScenePos);//鼠标点中色表块的边界小矩形并移动时创建可移动的虚框	
            for (int k = 0; k < m_vlink.size();++k)
			{
                if (m_vlink[k] !=NULL)
                {
                    CNetworkPort* t_startPort = m_vlink[k]->getStartPort();
                    CNetworkPort* t_endPort = m_vlink[k]->getEndPort();
                    if (t_startPort != NULL && t_startPort->pos().x() > 20) //右边
                    {
                        CSwitcher* temp_switchItem = dynamic_cast<CSwitcher*>(t_startPort->parentItem());
                        if (temp_switchItem != NULL)
                        {
                            QPointF t_iniPos = t_startPort->pos();
                            t_startPort->setPos(temp_switchItem->boundingRect().bottomRight().x()-9,t_iniPos.y());
                        }
                    }
                    if (t_endPort != NULL && t_endPort->pos().x() > 20) //右边
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

//鼠标释放事件
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
                        QMessageBox::information(this, tr("提示"), tr("两个设备间链路数不超过5条！"));
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
                t_itemlist = m_scene->items(t_rect,Qt::IntersectsItemShape);//返回虚线框检测到的图元
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
    int e_switchID = _linkSwitcher->getSwitcherInfo().ID;
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
    link->getLinkInfo().pos_link_num = POSCount;
    link->getLinkInfo().ethernet_link_num = EthernetCount;
    link->setID(s_switchID, e_switchID);
    QString linkType = (link->getLinkInfo().linkType == 0)?"POS":"Ethernet";
    link->setToolTip(tr("类型:") + linkType + "\n" +
                     tr("速率:") + link->getLinkInfo().bandWidth + "\n" +
                     tr("时延:") + link->getLinkInfo().delay);

    m_itemGroup->addToGroup(link);
    m_vlink.push_back(link);

    //updata all same type links, hide items
    for ( int i = 0; i < m_vlink.size(); ++i )
    {
        if (m_vlink[i] == link || m_vlink[i]->getLinkInfo().linkType != link->getLinkInfo().linkType)
        {
            continue;
        }
        CLinkInfo& tmpLinkInfo = m_vlink[i]->getLinkInfo();
        if ((tmpLinkInfo.s_switchID == s_switchID && tmpLinkInfo.e_switchID == e_switchID) ||
            (tmpLinkInfo.s_switchID == e_switchID && tmpLinkInfo.e_switchID == s_switchID))
        {
            m_vlink[i]->hide();
        }
    }
}

//鼠标双击事件
void CTopologyWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	//选中一个交换机图元或者一个设备图元时候的双击
	QList<QGraphicsItem*> t_childItem = m_itemGroup->childItems();
	if (t_childItem.size() != 1)
	{
		return;
	}
	CSwitcher* t_switchItem = NULL;
    Clink* t_linkItem = NULL;

	t_switchItem = dynamic_cast<CSwitcher*>(t_childItem[0]);
    t_linkItem = dynamic_cast<Clink*>(t_childItem[0]);
	if (t_switchItem != NULL)  //交换机属性对话框显示
	{
		m_EditSwitchItem = t_switchItem;
		m_switchDia->SetSwitcherInfo(t_switchItem->getSwitcherInfo());
        m_switchDia->setIPpond(&t_IPPond);
        m_switchDia->setIPPool(&m_IPPool);
		m_switchDia->exec(); 
	}   
    else if (t_linkItem != NULL)//显示链路设备属性对话框
    {
        return;//待添加
    }
}

//右键菜单
void CTopologyWidget::contextMenuEvent(QContextMenuEvent * event)
{
//	m_configuerAct->setEnabled(false);

	QPointF scenePnt = mapToScene(event->pos());
	QGraphicsItem* t_item = m_scene->itemAt(scenePnt);
	QList<QGraphicsItem*> t_childItems = m_itemGroup->childItems();

	CBoundingRectItem* t_brItem = NULL;
	t_brItem = dynamic_cast<CBoundingRectItem*>(t_item);

	//点击空白 或者点击已经选中的图元  根据情况显示菜单
	if (t_item == NULL || (t_brItem != NULL && t_brItem->parentItem() != NULL))
	{
		if (t_childItems.size() == 0)  //无激活图元 右键所有按钮无效
		{
            m_deleteAct->setEnabled(false);
            m_attributeAct->setEnabled(false);
            m_routeVisible->setEnabled(false);
            m_delayAct->setEnabled(false);
			m_rightMenu->exec(event->globalPos());
			return;
		}
        else if (t_childItems.size() == 1)  //仅仅一个的时候
		{
//            m_configuerAct->setEnabled(true);
            m_deleteAct->setEnabled(true);
            m_attributeAct->setEnabled(true);
            m_routeVisible->setEnabled(false);
            m_delayAct->setEnabled(false);
            m_rightMenu->exec(event->globalPos());
			return;
		}
        else if (t_childItems.size() == 2)  //多余两个激活图元时  仅删除按钮有效
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

        //右击未被选中的图元的情况 ： 此图元被选中 且出现相应的右键菜单
        CSwitcher* te_switchItem = NULL;
        Clink* te_linkItem = NULL;
        te_switchItem = dynamic_cast<CSwitcher*>(t_item);
        te_linkItem = dynamic_cast<Clink*>(t_item);
        if (te_switchItem != NULL && te_switchItem->getSelectedFlag() == false)  //点中交换机
        {
            //清空group
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
        else if (te_linkItem != NULL && te_linkItem->getSelectedFlag() == false)  //选中链路
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
            //待添加选定框
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

//键盘事件  删除
void CTopologyWidget::keyPressEvent( QKeyEvent * event )
{
//	int _key = event->key();
//	if (_key == 0x01000007)  //delete键
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
功能：鼠标移至色表块的边界小矩形中，鼠标指针改变
参数：无
返回值：无
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
	//改变鼠标指针
	for ( int j = 0; j < itemL.size(); j++ )
	{
		if (itemL[j]->contains(itemL[j]->mapFromScene(_scenePos)))
		{
			if (j == 0 || j == 3)//鼠标在左上角和右下角的小矩形中
			{						
				setCursor(QCursor(Qt::SizeFDiagCursor)); 
			}					
			else if (j == 4 || j == 6)//鼠标在上边和下边的小矩形中
			{
				setCursor(QCursor(Qt::SizeVerCursor));
			}
			else if (j == 1 || j == 2)//鼠标在右上角和左下角的小矩形中
			{
				setCursor(QCursor(Qt::SizeBDiagCursor));
			}	
			else if (j == 5 || j == 7)//鼠标在左边和右边的小矩形中
			{
				setCursor(QCursor(Qt::SizeHorCursor));
			}					
		}
	}		
}
/********************************************************************
功能：鼠标点中边界小矩形并移动时创建可移动的虚框
参数：无
返回值：无
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
    if (t_switcherItem != NULL)   //只有选中一个交换机图元
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
			//拖动小矩形时创建动态框
			for ( int j = 0; j < itemL.size(); j++ )
			{			
				QRectF temR;
				temR = t_switcherItem->sceneBoundingRect();		

				qreal tmpE = temR.bottomRight().x();
				qreal tmpW = temR.topLeft().x(); 
				qreal tmpN = temR.topLeft().y();
				qreal tmpS =  temR.bottomRight().y();

				if (rect0sele)//拖动左上角的矩形
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
				if (rect3sele)//拖动右下角的矩形
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
				if (rect1sele)//拖动右上角的矩形
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
				if (rect2sele)//拖动左下角的矩形
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
				if (rect4sele)//拖动上边小矩形
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
				if (rect6sele)//拖动下边小矩形
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
				if (rect5sele)//拖动左边小矩形
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
				if (rect7sele)//拖动右边小矩形
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
功能：创建右键菜单
参数：无
返回值：无
**********************************************************/
void CTopologyWidget::creatRightMenu()
{
	m_rightMenu = new QMenu(this);

    m_copyAct = new QAction(tr("复制"),this);
    m_pasteAct = new QAction(tr("粘贴"),this);
    connect(m_copyAct,SIGNAL(triggered() ), this, SLOT(slotCopyAction() ) );
    connect(m_pasteAct, SIGNAL(triggered() ), this, SLOT(slotPasteAction() ) );

    m_deleteAct = new QAction(tr("删除"),this);
	m_attributeAct = new QAction(tr("查看属性"),this);
	connect(m_deleteAct,SIGNAL(triggered()),this,SLOT(slotDeleteAction()));
	connect(m_attributeAct,SIGNAL(triggered()),this,SLOT(slotAttributeAction()));

//	m_configuerAct = new QAction(tr("配置参数"), this);
//	connect(m_configuerAct,SIGNAL(triggered()), this, SLOT(slotConfigureAction()));

    m_routeVisible = new QAction(tr("路由可视化"), this);
    connect(m_routeVisible, SIGNAL(triggered()), this, SLOT(slotRouteVisible()));

    m_delayAct = new QAction(tr("端到端时延"), this);
    connect(m_delayAct, SIGNAL(triggered()), this, SLOT(slotDelayAction()));


	m_rightMenu->addSeparator();
	m_rightMenu->addAction(m_deleteAct);
	m_rightMenu->addAction(m_attributeAct);
//	m_rightMenu->addAction(m_configuerAct);
    m_rightMenu->addAction(m_routeVisible);
    m_rightMenu->addAction(m_delayAct);
    m_deleteAct->setEnabled(false);
    m_attributeAct->setEnabled(false);
    m_routeVisible->setEnabled(false);
    m_delayAct->setEnabled(false);
}
/**********************************************************
功能：复制图元
参数：无
返回值：无
**********************************************************/
void CTopologyWidget::slotCopyAction()
{
}

/**********************************************************
功能：粘贴图元
参数：无
返回值：无
**********************************************************/
void CTopologyWidget::slotPasteAction()
{
}

/**********************************************************
功能：删除按钮
参数：无
返回值：无
**********************************************************/
void CTopologyWidget::slotDeleteAction()
{

	QMessageBox::StandardButton rb = QMessageBox::question(this, tr("删除选中项") , tr("您确定要删除选中项吗?") ,QMessageBox::Yes|QMessageBox::No , QMessageBox::Yes);

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
            //删除Link和端口
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
                       m_vlink[i]->getLinkInfo().e_switchID == t_selectedSwitch->getSwitcherInfo().ID)
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
功能：单个图元右键的属性按钮
参数：无
返回值：无
**********************************************************/
void CTopologyWidget::slotAttributeAction()
{
	//选中一个交换机图元或者一个设备图元时候的双击
	QList<QGraphicsItem*> t_childItem = m_itemGroup->childItems();
	if (t_childItem.size() != 1)
	{
		return;
    }
    CSwitcher* t_switchItem = NULL;
    Clink* t_linkItem = NULL;

    t_switchItem = dynamic_cast<CSwitcher*>(t_childItem[0]);
    t_linkItem = dynamic_cast<Clink*>(t_childItem[0]);
    if (t_switchItem != NULL)  //交换机属性对话框显示
    {
        m_EditSwitchItem = t_switchItem;
        m_switchDia->SetSwitcherInfo(t_switchItem->getSwitcherInfo());
        m_switchDia->setIPpond(&t_IPPond);
        m_switchDia->setIPPool(&m_IPPool);
        m_switchDia->exec();
    }   
    else if (t_linkItem != NULL)//显示链路设备属性对话框
    {
        return;//待添加
    }
}

/**********************************************************
功能：单个图元右键的属性按钮
参数：无
返回值：无
**********************************************************/
//void CTopologyWidget::slotConfigureAction()
//{
//	int selected_switchID;
//	//选中一个交换机图元或者一个设备图元时候的双击
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
功能：多个图元右键的路由可视化按钮
参数：无
返回值：无
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
功能：多个图元右键的端到端时延按钮
参数：无
返回值：无
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
功能：删除子图元
参数：无
返回值：无
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

void CTopologyWidget::OutputTopology(QString path)
{
    QFile t_file(path);
    if (!t_file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&t_file);

    QList<QGraphicsItem*> t_itemlist = m_scene->items();

    int item_type = -1;
    int count = 0;

    for (int i = 0; i < t_itemlist.count(); i++)
    {
        CSwitcher * t_switch = NULL;

        t_switch = dynamic_cast<CSwitcher*>(t_itemlist[i]);

        if (t_switch != NULL)
        {
            count++;
        }
    }

    out<<count<<endl;
    for (int i = 0; i < t_itemlist.size() ; i++)
    {
        QPointF t_pos = t_itemlist[i]->pos();
        QRectF t_rect = t_itemlist[i]->boundingRect();
        CSwitcher * t_switch = NULL;

        t_switch = dynamic_cast<CSwitcher*>(t_itemlist[i]);

        if (t_switch != NULL)
        {
            CSwitcherInfo t_switchInfo = t_switch->getSwitcherInfo();
            item_type = 0;

            out<<item_type<<endl;
            out<<(double)t_pos.x()<<endl
               <<(double)t_pos.y()<<endl;
            out<<(double)t_rect.topLeft().x()<<endl
               <<(double)t_rect.topLeft().y()<<endl
               <<(double)t_rect.width()<<endl
               <<(double)t_rect.height()<<endl;
            out<<t_switchInfo.switchType<<endl
               <<t_switchInfo.name<<endl
               <<t_switchInfo.ID<<endl
               <<t_switchInfo.portNum<<endl
               <<t_switchInfo.protocol<<endl
               <<t_switchInfo.networkLocation<<endl
               <<t_switchInfo.LNodes.size()<<endl;
            qDebug()<<t_switchInfo.ID<<endl;
            for (int j = 0; j < t_switchInfo.LNodes.size(); j ++)
            {

                SNodeStructInfo &t_node = t_switchInfo.LNodes[j];
                out<<t_node.Port_ID<<"\n";
                out<<t_node.Device_ID<<"\n";
                out<<t_switchInfo.networkLocation<<"\n";
                out<<t_node.name<<"\n";
                out<<t_node.interfaceName<<"\n";
                out<<t_node.area<<"\n";
                out<<t_node.IP<<"\n";
                out<<t_node.subnetMask<<"\n";
                out<<t_node.describe<<"\n";
                out<<t_node.costValue<<"\n";
                out<<t_node.remark<<"\n";

                out<<t_node.linkPort_ID<<"\n";
                out<<t_node.linkPort_Device_ID<<"\n";
                out<<t_node.linkPort_name<<"\n";
                out<<t_node.linkPort_interfaceName<<"\n";
                out<<t_node.linkPort_area<<"\n";
                out<<t_node.linkPort_IP<<"\n";
                out<<t_node.linkPort_subnetMask<<"\n";
                out<<t_node.linkPort_describe<<"\n";
                out<<t_node.linkPort_costValue<<"\n";
                out<<t_node.linkPort_remark<<"\n";

                out<<t_node.linkType<<"\n";
                out<<t_node.bandWidth<<"\n";//链路带宽
                out<<t_node.delay<<"\n";
            }
        }
    }
    //int a = m_vlink.size();
    out<<m_vlink.size()<<"\n";
    if(m_vlink.size() != 0)
    {
        for (int i = 0; i < m_vlink.size(); i++)
        {
            int s_nodeID = 0, e_nodeID = 0, item_type = 1;
            s_nodeID = m_vlink[i]->getLinkInfo().s_switchID;
            e_nodeID = m_vlink[i]->getLinkInfo().e_switchID;
            out<<item_type<<"\n";
            out<<s_nodeID<<"\n";
            out<<e_nodeID<<"\n";
            out<<m_vlink[i]->getLinkInfo().linkType<<"\n";
            out<<m_vlink[i]->getLinkInfo().bandWidth<<"\n";
            out<<m_vlink[i]->getLinkInfo().delay<<"\n";
            out<<m_vlink[i]->getLinkInfo().pos_link_num<<"\n";
            out<<m_vlink[i]->getLinkInfo().ethernet_link_num<<"\n";
            qDebug()<< s_nodeID<< " "<< e_nodeID<< endl;
        }
    }

    t_file.close();
}

void CTopologyWidget::OutputPosition(QString path)
{
    QFile t_file(path);
    if (!t_file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&t_file);

    QList<QGraphicsItem*> t_itemlist = m_scene->items();
    out<<m_scene->width()<<","<<m_scene->height()<<endl;
    for (int i = t_itemlist.size()-1; i >= 0 ; i--)
    {
        QPointF t_pos = t_itemlist[i]->pos();
        CSwitcher * t_switch = NULL;

        t_switch = dynamic_cast<CSwitcher*>(t_itemlist[i]);

        if (t_switch != NULL)
        {
            CSwitcherInfo t_switchInfo = t_switch->getSwitcherInfo();
            out<<t_switchInfo.name<<" "<<(double)t_pos.x()<<" "<<(double)t_pos.y()<<endl;
        }
    }

   t_file.close();
}

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

void CTopologyWidget::InputTopology(QString path)
{
    this->centerOn(0,0);

    QFile t_file(path);

    if (!t_file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    else
        clearScene();
    QTextStream out(&t_file);
    char item_type = -1;
    int count = 0;
    count = QVariant(out.readLine()).toInt();

    QPointF t_pos;
    QRectF t_rect;

    QList<QString>  lineList;
    while(!out.atEnd())
    {
        QString eachline = out.readLine();
        lineList.append(eachline);
    }
    int lineNum = 0;
    for (int i = 0; i < count; i++)
    {

        double x,y,z,w;
        if (lineNum >= lineList.size())
        {
            break;
        }

        item_type = QVariant(lineList[lineNum]).toInt();
        lineNum++;

        t_pos.setX(QVariant(lineList[lineNum]).toDouble());
        lineNum++;
        t_pos.setY(QVariant(lineList[lineNum]).toDouble());
        lineNum++;

        x = QVariant(lineList[lineNum]).toDouble();
        lineNum++;
        y = QVariant(lineList[lineNum]).toDouble();
        lineNum++;
        z = QVariant(lineList[lineNum]).toDouble();
        lineNum++;
        w = QVariant(lineList[lineNum]).toDouble();
        lineNum++;
        t_rect = QRectF(x,y,z,w);


        //交换机
        if (item_type == 0)
        {
            CSwitcherInfo t_switchInfo;
            int vnodeSize = 0;
            t_switchInfo.switchType = lineList[lineNum];
            lineNum++;
            t_switchInfo.name = lineList[lineNum];
            lineNum++;
            t_switchInfo.ID = QVariant(lineList[lineNum]).toInt();
            lineNum++;
            t_switchInfo.portNum = QVariant(lineList[lineNum]).toInt();
            lineNum++;
            t_switchInfo.protocol = lineList[lineNum];
            lineNum++;
            t_switchInfo.networkLocation = lineList[lineNum];
            lineNum++;
            vnodeSize =  QVariant(lineList[lineNum]).toInt();
            lineNum++;
            //t_switchInfo.LNodes.resize(vnodeSize);
            for (int j = 0; j < vnodeSize; j ++)
            {
                SNodeStructInfo t_node;
                t_node.Port_ID = QVariant(lineList[lineNum]).toInt();
                lineNum++;
                t_node.Device_ID = QVariant(lineList[lineNum]).toInt();
                lineNum++;
                t_node.networkLocation = lineList[lineNum];
                lineNum++;
                t_node.name = lineList[lineNum];                
                lineNum++;
                t_node.interfaceName = lineList[lineNum];
                lineNum++;
                t_node.area = lineList[lineNum];
                lineNum++;
                t_node.IP = lineList[lineNum];
                lineNum++;
                t_node.subnetMask = lineList[lineNum];
                lineNum++;

                if(!(t_IPPond.count(t_node.IP)))
                    t_IPPond.insert(t_node.IP);
                QString networkID = calculateNetworkID(t_node.IP,t_node.subnetMask);

                if (t_node.IP == networkID )
                {
                    QMessageBox::information(this, tr("错误"), tr("%1 IP地址不能为所在网号！").arg(t_node.name),QMessageBox::Ok);
                    return;
                }
                if(!m_IPPool.count(networkID))
                    m_IPPool[networkID] = 1;
                else
                {
                    //full
                    if(m_IPPool[networkID] == 2)
                    {
                        QMessageBox::information(this,tr("错误"),tr("%1 IP地址所在网络已被使用!").arg(t_node.name),QMessageBox::Ok);
                        return;
                    }
                    else
                        m_IPPool[networkID] = 2;
                }

                t_node.describe = lineList[lineNum];
                lineNum++;
                t_node.costValue = lineList[lineNum];
                lineNum++;
                t_node.remark = lineList[lineNum];
                lineNum++;

                t_node.linkPort_ID =QVariant(lineList[lineNum]).toInt();
                lineNum++;
                t_node.linkPort_Device_ID =QVariant(lineList[lineNum]).toInt();
                lineNum++;
                t_node.linkPort_name = lineList[lineNum];
                lineNum++;                
                t_node.linkPort_interfaceName = lineList[lineNum];
                lineNum++;
                t_node.linkPort_area = lineList[lineNum];
                lineNum++;
                t_node.linkPort_IP = lineList[lineNum];
                lineNum++;
                t_node.linkPort_subnetMask = lineList[lineNum];
                lineNum++;

                if(!(t_IPPond.count(t_node.linkPort_IP)))
                    t_IPPond.insert(t_node.linkPort_IP);

                QString desNetworkID = calculateNetworkID(t_node.linkPort_IP, t_node.linkPort_subnetMask);
                if(desNetworkID != networkID)
                {
                    QMessageBox::information(this,tr("错误"),tr("%1 IP地址与对端不在同一网络!").arg(t_node.linkPort_name),QMessageBox::Ok);
                }
                if (t_node.linkPort_IP == networkID )
                {
                    QMessageBox::information(this, tr("错误"), tr("%1 IP地址不能为所在网号！").arg(t_node.linkPort_name),QMessageBox::Ok);
                }

                t_node.linkPort_describe = lineList[lineNum];
                lineNum++;
                t_node.linkPort_costValue = lineList[lineNum];
                lineNum++;
                t_node.linkPort_remark = lineList[lineNum];
                lineNum++;
                t_node.linkType = QVariant(lineList[lineNum]).toInt();
                lineNum++;
                t_node.bandWidth = QVariant(lineList[lineNum]).toInt();
                lineNum++;
                t_node.delay = QVariant(lineList[lineNum]).toFloat();
                lineNum++;
                t_switchInfo.LNodes.append(t_node);
            }
            CSwitcher* t_switch = new CSwitcher(NULL,m_scene);
            t_switch->setPos(t_pos);
            t_switch->setToolTip(tr("名称: ") + t_switchInfo.name + "\n"+
                                 tr("型号:") + t_switchInfo.switchType + "\n" +
                                 tr("位置: ") + t_switchInfo.networkLocation);
            t_switch->setBoundingRect(t_rect);

            CNetworkPort *t_port = new CNetworkPort(t_switch, m_scene);
            QRectF m_boundingRect = t_switch->boundingRect();
            t_port->setPos(m_boundingRect.width()/2, m_boundingRect.height()/2+5);
            t_port->setID(t_switchInfo.ID);
            t_switch->setSwitcherInfo(t_switchInfo);
            m_vSwtich.push_back(t_switchInfo);
            //t_vSwitchInOut.push_back(t_switch);
            t_vPortInOut.push_back(t_port);
        }

    }
    //解析link
    int t_linksize = QVariant(lineList[lineNum]).toInt();
    lineNum++;
    if(t_linksize != 0)
    {
        for (int i = 0; i < t_linksize; i++)
        {
            int item_type =0;
            item_type = QVariant(lineList[lineNum]).toInt();
            lineNum++;
            if(item_type == 1)
            {
                int s_swichID = QVariant(lineList[lineNum]).toInt();
                lineNum++;
                int e_swichID = QVariant(lineList[lineNum]).toInt();
                lineNum++;
                int _linkType = QVariant(lineList[lineNum]).toInt();
                lineNum++;
                QString _bandWidth = lineList[lineNum];
                lineNum++;
                QString _delay = lineList[lineNum];
                lineNum++;
                int pos_linkNum = QVariant(lineList[lineNum]).toInt();
                lineNum++;
                int ethernet_linkNum = QVariant(lineList[lineNum]).toInt();
                lineNum++;
                int ss_ID = 0, ee_ID = 0;
                for(size_t i = 0; i < t_vPortInOut.size(); i++)
                {
                    if(t_vPortInOut[i]->getID() == s_swichID)
                        ss_ID = i;
                    else if(t_vPortInOut[i]->getID() == e_swichID)
                        ee_ID = i;
                }
                Clink* t_link = new Clink(m_scene,t_vPortInOut[ss_ID], t_vPortInOut[ee_ID]);
                CLinkInfo t_linkInfo;
                t_linkInfo.s_switchID = s_swichID;
                t_linkInfo.e_switchID = e_swichID;
                t_linkInfo.linkType = _linkType;
                t_linkInfo.bandWidth = _bandWidth;
                t_linkInfo.delay = _delay;
                t_linkInfo.pos_link_num = pos_linkNum;
                t_linkInfo.ethernet_link_num = ethernet_linkNum;
                t_link->setLinkInfo(t_linkInfo);
                QString linkType = (_linkType == 0)?"POS":"Ethernet";
                t_link->setToolTip(tr("类型:") + linkType + "\n" +
                                 tr("速率:") + _bandWidth + "\n" +
                                 tr("时延:") + _delay + "\n");

                m_scene->addItem(t_link);
                m_itemGroup->addToGroup(t_link);

                //updata all same type links, hide items
                for ( int i = 0; i < m_vlink.size(); ++i )
                {
                    if (m_vlink[i]->getLinkInfo().linkType != _linkType)
                    {
                        continue;
                    }
                    CLinkInfo& tmpLinkInfo = m_vlink[i]->getLinkInfo();
                    if ((tmpLinkInfo.s_switchID == s_swichID && tmpLinkInfo.e_switchID == e_swichID) ||
                        (tmpLinkInfo.s_switchID == e_swichID && tmpLinkInfo.e_switchID == s_swichID))
                    {
                        m_vlink[i]->hide();
                    }
                }

                m_vlink.push_back(t_link);                

            }
        }
    }
}


void CTopologyWidget::RefreshTopology()
{
    this->centerOn(0,0);


    QPointF t_pos;
    QRectF t_rect;

    QList<QString>  lineList;
    while(!out.atEnd())
    {
        QString eachline = out.readLine();
        lineList.append(eachline);
    }
    int lineNum = 0;
    for (int i = 0; i < count; i++)
    {
        double x,y,z,w;
        if (lineNum >= lineList.size())
        {
            break;
        }

        item_type = QVariant(lineList[lineNum]).toInt();

        t_pos.setX(QVariant(lineList[lineNum]).toDouble());

        t_pos.setY(QVariant(lineList[lineNum]).toDouble());


        x = QVariant(lineList[lineNum]).toDouble();

        y = QVariant(lineList[lineNum]).toDouble();

        z = QVariant(lineList[lineNum]).toDouble();

        w = QVariant(lineList[lineNum]).toDouble();

        t_rect = QRectF(x,y,z,w);


        //交换机
        if (item_type == 0)
        {
            CSwitcherInfo t_switchInfo;
            int vnodeSize = 0;
            t_switchInfo.switchType = lineList[lineNum];

            t_switchInfo.name = lineList[lineNum];

            t_switchInfo.ID = QVariant(lineList[lineNum]).toInt();

            t_switchInfo.portNum = QVariant(lineList[lineNum]).toInt();

            t_switchInfo.protocol = lineList[lineNum];

            t_switchInfo.networkLocation = lineList[lineNum];

            vnodeSize =  QVariant(lineList[lineNum]).toInt();

            //t_switchInfo.LNodes.resize(vnodeSize);
            for (int j = 0; j < vnodeSize; j ++)
            {
                SNodeStructInfo t_node;
                t_node.Port_ID = QVariant(lineList[lineNum]).toInt();

                t_node.Device_ID = QVariant(lineList[lineNum]).toInt();

                t_node.networkLocation = lineList[lineNum];

                t_node.name = lineList[lineNum];

                t_node.interfaceName = lineList[lineNum];

                t_node.area = lineList[lineNum];

                t_node.IP = lineList[lineNum];

                t_node.subnetMask = lineList[lineNum];


                if(!(t_IPPond.count(t_node.IP)))
                    t_IPPond.insert(t_node.IP);
                QString networkID = calculateNetworkID(t_node.IP,t_node.subnetMask);

                if (t_node.IP == networkID )
                {
                    QMessageBox::information(this, tr("错误"), tr("%1 IP地址不能为所在网号！").arg(t_node.name),QMessageBox::Ok);
                    return;
                }
                if(!m_IPPool.count(networkID))
                    m_IPPool[networkID] = 1;
                else
                {
                    //full
                    if(m_IPPool[networkID] == 2)
                    {
                        QMessageBox::information(this,tr("错误"),tr("%1 IP地址所在网络已被使用!").arg(t_node.name),QMessageBox::Ok);
                        return;
                    }
                    else
                        m_IPPool[networkID] = 2;
                }

                t_node.describe = lineList[lineNum];

                t_node.costValue = lineList[lineNum];

                t_node.remark = lineList[lineNum];


                t_node.linkPort_ID =QVariant(lineList[lineNum]).toInt();

                t_node.linkPort_Device_ID =QVariant(lineList[lineNum]).toInt();

                t_node.linkPort_name = lineList[lineNum];

                t_node.linkPort_interfaceName = lineList[lineNum];

                t_node.linkPort_area = lineList[lineNum];

                t_node.linkPort_IP = lineList[lineNum];

                t_node.linkPort_subnetMask = lineList[lineNum];


                if(!(t_IPPond.count(t_node.linkPort_IP)))
                    t_IPPond.insert(t_node.linkPort_IP);

                QString desNetworkID = calculateNetworkID(t_node.linkPort_IP, t_node.linkPort_subnetMask);
                if(desNetworkID != networkID)
                {
                    QMessageBox::information(this,tr("错误"),tr("%1 IP地址与对端不在同一网络!").arg(t_node.linkPort_name),QMessageBox::Ok);
                }
                if (t_node.linkPort_IP == networkID )
                {
                    QMessageBox::information(this, tr("错误"), tr("%1 IP地址不能为所在网号！").arg(t_node.linkPort_name),QMessageBox::Ok);
                }

                t_node.linkPort_describe = lineList[lineNum];

                t_node.linkPort_costValue = lineList[lineNum];

                t_node.linkPort_remark = lineList[lineNum];

                t_node.linkType = QVariant(lineList[lineNum]).toInt();

                t_node.bandWidth = QVariant(lineList[lineNum]).toInt();

                t_node.delay = QVariant(lineList[lineNum]).toFloat();

                t_switchInfo.LNodes.append(t_node);
            }
            CSwitcher* t_switch = new CSwitcher(NULL,m_scene);
            t_switch->setPos(t_pos);
            t_switch->setToolTip(tr("名称: ") + t_switchInfo.name + "\n"+
                                 tr("型号:") + t_switchInfo.switchType + "\n" +
                                 tr("位置: ") + t_switchInfo.networkLocation);
            t_switch->setBoundingRect(t_rect);

            CNetworkPort *t_port = new CNetworkPort(t_switch, m_scene);
            QRectF m_boundingRect = t_switch->boundingRect();
            t_port->setPos(m_boundingRect.width()/2, m_boundingRect.height()/2+5);
            t_port->setID(t_switchInfo.ID);
            t_switch->setSwitcherInfo(t_switchInfo);
            m_vSwtich.push_back(t_switchInfo);
            //t_vSwitchInOut.push_back(t_switch);
            t_vPortInOut.push_back(t_port);
        }

    }
    //解析link
    int t_linksize = QVariant(lineList[lineNum]).toInt();

    if(t_linksize != 0)
    {
        for (int i = 0; i < t_linksize; i++)
        {
            int item_type =0;
            item_type = QVariant(lineList[lineNum]).toInt();

            if(item_type == 1)
            {
                int s_swichID = QVariant(lineList[lineNum]).toInt();

                int e_swichID = QVariant(lineList[lineNum]).toInt();

                int _linkType = QVariant(lineList[lineNum]).toInt();

                QString _bandWidth = lineList[lineNum];

                QString _delay = lineList[lineNum];

                int pos_linkNum = QVariant(lineList[lineNum]).toInt();

                int ethernet_linkNum = QVariant(lineList[lineNum]).toInt();

                int ss_ID = 0, ee_ID = 0;
                for(size_t i = 0; i < t_vPortInOut.size(); i++)
                {
                    if(t_vPortInOut[i]->getID() == s_swichID)
                        ss_ID = i;
                    else if(t_vPortInOut[i]->getID() == e_swichID)
                        ee_ID = i;
                }
                Clink* t_link = new Clink(m_scene,t_vPortInOut[ss_ID], t_vPortInOut[ee_ID]);
                CLinkInfo t_linkInfo;
                t_linkInfo.s_switchID = s_swichID;
                t_linkInfo.e_switchID = e_swichID;
                t_linkInfo.linkType = _linkType;
                t_linkInfo.bandWidth = _bandWidth;
                t_linkInfo.delay = _delay;
                t_linkInfo.pos_link_num = pos_linkNum;
                t_linkInfo.ethernet_link_num = ethernet_linkNum;
                t_link->setLinkInfo(t_linkInfo);
                QString linkType = (_linkType == 0)?"POS":"Ethernet";
                t_link->setToolTip(tr("类型:") + linkType + "\n" +
                                 tr("速率:") + _bandWidth + "\n" +
                                 tr("时延:") + _delay + "\n");

                m_scene->addItem(t_link);
                m_itemGroup->addToGroup(t_link);

                //updata all same type links, hide items
                for ( int i = 0; i < m_vlink.size(); ++i )
                {
                    if (m_vlink[i]->getLinkInfo().linkType != _linkType)
                    {
                        continue;
                    }
                    CLinkInfo& tmpLinkInfo = m_vlink[i]->getLinkInfo();
                    if ((tmpLinkInfo.s_switchID == s_swichID && tmpLinkInfo.e_switchID == e_swichID) ||
                        (tmpLinkInfo.s_switchID == e_swichID && tmpLinkInfo.e_switchID == s_swichID))
                    {
                        m_vlink[i]->hide();
                    }
                }

                m_vlink.push_back(t_link);

            }
        }
    }
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

void CTopologyWidget::ConstructTopology(const QString &_path)
{
    this->centerOn(0,0);
    DragNetwork dragNetwork(string(_path.toLocal8Bit().data()));
    dragNetwork.ConstructStruct();
    if (!dragNetwork.GetSuccessFlag())
    {
        return;
    }
    vector<SNodeStruct> nodeStructVec = dragNetwork.GetNodeStruct();
    vector<SEdgeStruct> edgeStructVec = dragNetwork.GetEdgeStruct();
    //vector<vector<int> > adjacentMatrix = dragNetwork.GetAdjacentMatrix();
    vector<vector<SRouterInterfaceVec> > interfaceVec = dragNetwork.GetRouterInterfaceVec();

    //绘制拓扑
    clearScene();   

    int nodeNum = dragNetwork.GetNodeNum();
    int width = m_scene->width();
    int height = m_scene->height();
    //qDebug("size:(%d,%d)",width,height);

    //find minX & minY
    double minX = 0.0;
    double minY = 0.0;
    double maxX = 0.0;
    double maxY = 0.0;
    for ( int index = 0; index < nodeNum; ++index )
    {
        if ( nodeStructVec[index].point.x < minX )
        {
            minX = nodeStructVec[index].point.x;
        }
        if ( nodeStructVec[index].point.x > maxX )
        {
            maxX = nodeStructVec[index].point.x;
        }
        if ( nodeStructVec[index].point.y < minY )
        {
            minY = nodeStructVec[index].point.y;
        }
        if ( nodeStructVec[index].point.y > maxY )
        {
            maxY = nodeStructVec[index].point.y;
        }
    }

    //transform
    //(x-minX)/(x0-Badle) = (maxX-minX)/(width-2*Badle)
    //=>
    //x0 = (x-minX)(width-2*Badle)/(maxX-minX)+Badle
    const int Badle = 100;
    for ( int index = 0; index < nodeNum; ++index )
    {
        nodeStructVec[index].point.x = (nodeStructVec[index].point.x-minX)*(width-2*Badle)/(maxX-minX) + Badle;
        nodeStructVec[index].point.y = (nodeStructVec[index].point.y-minY)*(height-2*Badle)/(maxY-minY) + Badle;
    }

    QPointF t_pos;
    QRectF t_rect;

    for (int i = 0; i < nodeNum; i++)
    {
        t_pos.setX(nodeStructVec[i].point.x);
        t_pos.setY(nodeStructVec[i].point.y);
        //qDebug("%d:%s(%f,%f)",i,nodeStructVec[i].nName.c_str(), nodeStructVec[i].point.x,nodeStructVec[i].point.y);

        t_rect = QRectF(0,0,60,60);

        CSwitcherInfo t_switchInfo;
        t_switchInfo.switchType = "Cisco 7609";
        t_switchInfo.name = QString::fromLocal8Bit(nodeStructVec[i].nName.c_str());
        t_switchInfo.ID = nodeStructVec[i].nID;
        t_switchInfo.portNum = 32;
        t_switchInfo.protocol = "Default";
        t_switchInfo.networkLocation = tr("骨干层");

        for (size_t j = 0; j < interfaceVec[i].size(); j ++)
        {
            SNodeStructInfo t_node;
            t_node.Port_ID = interfaceVec[i][j].interfaceIndex;
            t_node.Device_ID = i;
            t_node.networkLocation = tr("骨干层");
            t_node.name = QString::fromLocal8Bit(interfaceVec[i][j].routerName.c_str());
            t_node.interfaceName = QString::fromLocal8Bit(interfaceVec[i][j].interfaceName.c_str());
            t_node.area = "0";
            t_node.IP = QString::fromLocal8Bit(interfaceVec[i][j].interfaceIP.c_str());
            t_node.subnetMask = QString::fromLocal8Bit(interfaceVec[i][j].Mask.c_str());

            if(!(t_IPPond.count(t_node.IP)))
                t_IPPond.insert(t_node.IP);
            QString networkID = calculateNetworkID(t_node.IP,t_node.subnetMask);

            if (t_node.IP == networkID )
            {
                QMessageBox::information(this, tr("错误"), tr("%1 IP地址不能为所在网号！").arg(t_node.name),QMessageBox::Ok);
            }
            if(!m_IPPool.count(networkID))
                m_IPPool[networkID] = 1;
            else
            {
                //full
                if(m_IPPool[networkID] == 2)
                    QMessageBox::information(this,tr("错误"),tr("%1 IP地址所在网络已被使用!").arg(t_node.name),QMessageBox::Ok);
                else
                    m_IPPool[networkID] = 2;
            }

            t_node.describe = "";
            t_node.costValue = "0";
            t_node.remark = "";

            t_node.linkPort_ID = interfaceVec[i][j].desInterfaceIndex;
            t_node.linkPort_Device_ID = interfaceVec[i][j].eEndNode;
            t_node.linkPort_name = QString::fromLocal8Bit(interfaceVec[i][j].desRouterName.c_str());
            t_node.linkPort_interfaceName = QString::fromLocal8Bit(interfaceVec[i][j].desInterfaceName.c_str());
            t_node.linkPort_area = "0";
            t_node.linkPort_IP = QString::fromLocal8Bit(interfaceVec[i][j].desInterfaceIP.c_str());
            t_node.linkPort_subnetMask = QString::fromLocal8Bit(interfaceVec[i][j].Mask.c_str());

            if(!(t_IPPond.count(t_node.linkPort_IP)))
                t_IPPond.insert(t_node.linkPort_IP);

            QString desNetworkID = calculateNetworkID(t_node.linkPort_IP, t_node.linkPort_subnetMask);
            if(desNetworkID != networkID)
            {
                QMessageBox::information(this,tr("错误"),tr("%1 IP地址与对端不在同一网络!").arg(t_node.linkPort_name),QMessageBox::Ok);
            }
            if (t_node.linkPort_IP == networkID )
            {
                QMessageBox::information(this, tr("错误"), tr("%1 IP地址不能为所在网号！").arg(t_node.linkPort_name),QMessageBox::Ok);
            }

            t_node.linkPort_describe = "";
            t_node.linkPort_costValue = "0";
            t_node.linkPort_remark = "";

            t_node.linkType = POS;
            t_node.bandWidth = 622;
            t_node.delay = 2.0f;

            t_switchInfo.LNodes.append(t_node);
        }
        CSwitcher* t_switch = new CSwitcher(NULL,m_scene);
        t_switch->setPos(t_pos);
        t_switch->setBoundingRect(t_rect);
        CNetworkPort *t_port = new CNetworkPort(t_switch, m_scene);
        QRectF m_boundingRect = t_switch->boundingRect();
        t_port->setPos(m_boundingRect.width()/2, m_boundingRect.height()/2+5);
        t_port->setID(t_switchInfo.ID);
        t_switch->setSwitcherInfo(t_switchInfo);
        m_vSwtich.push_back(t_switchInfo);
        //t_vSwitchInOut.push_back(t_switch);
        t_vPortInOut.push_back(t_port);
    }
    //解析link
    int t_linksize = dragNetwork.GetEdgetNum();
    if(t_linksize != 0)
    {
        for (int i = 0; i < t_linksize; i++)
        {
            int s_portID = edgeStructVec[i].eStartNode;
            int e_portID = edgeStructVec[i].eEndNode;
            int _linkNum = edgeStructVec[i].eEdgeIndex;

            int ss_ID = 0, ee_ID = 0;
            for(size_t i = 0; i < t_vPortInOut.size(); i++)
            {
                if(t_vPortInOut[i]->getID() == s_portID)
                    ss_ID = i;
                else if(t_vPortInOut[i]->getID() == e_portID)
                    ee_ID = i;
            }
            Clink* t_link = new Clink(m_scene,t_vPortInOut[ss_ID], t_vPortInOut[ee_ID]);
            t_link->getLinkInfo().s_switchID = s_portID;
            t_link->getLinkInfo().e_switchID = e_portID;
            t_link->getLinkInfo().linkType = POS;//POS
            t_link->getLinkInfo().bandWidth = "622";//622MB/s
            t_link->getLinkInfo().delay = "0.5";//0.5ms
            t_link->getLinkInfo().pos_link_num = _linkNum;
            t_link->getLinkInfo().ethernet_link_num = 0;
            m_scene->addItem(t_link);
            m_itemGroup->addToGroup(t_link);
            m_vlink.push_back(t_link);
        }
    }
}

void CTopologyWidget::configTask()
{
    CTaskConfigDialog setTask;
    setTask.setSwichterInfo(m_scene->items());
    setTask.exec();
}

void CTopologyWidget::eventSchedul()
{
    CEventSchedulDialog setEventSchedul;
    setEventSchedul.setSwichterInfo(&m_vSwtich);
    setEventSchedul.exec();
}

QString CTopologyWidget::calculateNetworkID(QString _IP, QString _mask)
{
    QList<QString> ipList = _IP.split('.');
    QList<QString> maskList = _mask.split('.');
    if(ipList.size() != 4 || maskList.size() != 4)
    {
        QMessageBox::critical(this, tr("错误"),tr("IP地址或子网掩码错误!"),QMessageBox::Ok);
        return NULL;
    }
    QString networkID1 = QString::number(ipList[0].toInt() & maskList[0].toInt());
    QString networkID2 = QString::number(ipList[1].toInt() & maskList[1].toInt());
    QString networkID3 = QString::number(ipList[2].toInt() & maskList[2].toInt());
    QString networkID4 = QString::number(ipList[3].toInt() & maskList[3].toInt());
    return networkID1 + "." + networkID2 + "." + networkID3 + "." + networkID4;
}

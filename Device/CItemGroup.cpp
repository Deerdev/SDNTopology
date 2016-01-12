#include "CItemGroup.h"

CItemGroup::CItemGroup(QGraphicsItem *parent, QGraphicsScene *scene):QGraphicsItemGroup(parent,scene)
{
	m_scene = scene;

}

CItemGroup::~CItemGroup()
{

}

QPainterPath CItemGroup::shape() const
{
	QPainterPath t_path;

	for (int i = 0; i < childItems().size(); ++i)
	{
		t_path.addPath(childItems()[i]->shape());
	}
	return t_path;
}
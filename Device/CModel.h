#ifndef _MODEL_H
#define _MODEL_H
/************************************************************************
模块描述:  交换机设备
************************************************************************/

#include "CBaseObject.h"

class CModel: public CBaseObject
{
private:
	 
public:	
	CModel(QGraphicsItem *parent, QGraphicsScene *scene):CBaseObject(parent,scene){};
	~CModel(){};
	 
};

#endif // _MODEL_H
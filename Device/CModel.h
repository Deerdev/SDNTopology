#ifndef _MODEL_H
#define _MODEL_H
/************************************************************************
ģ������:  �������豸
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
/****************************
节点布局算法模块
****************************/
#ifndef PLACEMENTALGORITHM_H
#define PLACEMENTALGORITHM_H

#include "StructDefine.h"
#include <cstdlib>

class CPlacement
{
public:
	CPlacement();
	~CPlacement();

	//力引导布局
	void ForceDirected2D( vector<SNodeStruct>& _vNodes, vector<SEdgeStruct> & _vEdges );

	//设置温度参数
	void SetTemperature(double temp) { m_temperature = temp; }

private:
    int m_fixedNodes;
    double m_para_k;		//力引导算法中的参数k，引力斥力函数参数
    double m_temperature;	//温度参数
    unsigned short m_iterNum;		//迭代次数
    SPlacementRange m_range;		//布局范围
    vector<SDisplacement> m_vDisplacement;//节点移动坐标集合
	
	//节点位置初始化为随机坐标
	void PosInitialize(vector<SNodeStruct> &vNodes);

};

#endif

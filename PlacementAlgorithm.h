/****************************
�ڵ㲼���㷨ģ��
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

	//����������
	void ForceDirected2D( vector<SNodeStruct>& _vNodes, vector<SEdgeStruct> & _vEdges );

	//�����¶Ȳ���
	void SetTemperature(double temp) { m_temperature = temp; }

private:
    int m_fixedNodes;
    double m_para_k;		//�������㷨�еĲ���k������������������
    double m_temperature;	//�¶Ȳ���
    unsigned short m_iterNum;		//��������
    SPlacementRange m_range;		//���ַ�Χ
    vector<SDisplacement> m_vDisplacement;//�ڵ��ƶ����꼯��
	
	//�ڵ�λ�ó�ʼ��Ϊ�������
	void PosInitialize(vector<SNodeStruct> &vNodes);

};

#endif

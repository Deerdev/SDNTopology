#include <math.h>
#include <time.h>
//#include <QProgressDialog>
//#include <QApplication>
#include "PlacementAlgorithm.h"

CPlacement::CPlacement()
{
	m_temperature = 0.1;
	m_iterNum = 100;
	m_fixedNodes = 5;
}

CPlacement::~CPlacement()
{

}

void CPlacement::ForceDirected2D( vector<SNodeStruct>& _vNodes, vector<SEdgeStruct> & _vEdges )
{
	//进度条
	//QProgressDialog *vProgress = new QProgressDialog( QObject::tr("正在进行力导引布局···"), QObject::tr("..."), 0, m_iterNum-1);
	//vProgress->setWindowModality( Qt::ApplicationModal );
	//vProgress->setValue(0);
	//qApp->processEvents();
	//vProgress->show();

/*
	int nodeCount = 0;
	int edgeCount = 0;
	fscanf(pFile, "N:%d E:%d", &nodeCount, &edgeCount);

	//read nodes
	vNodes.resize(nodeCount);
	for ( int i = 0; i < nodeCount; ++i )
	{
		vNodes[i].nID = i+1;
	}	

	//read edges
	vEdges.resize(edgeCount);
	std::vector<SEdgeStruct>::iterator iterEdge = vEdges.begin();
	for ( ; iterEdge != vEdges.end(); ++iterEdge )
	{
		int start, end;
		fscanf(pFile, "%d %d", &start, &end);
		iterEdge->eStartNode = start-1;
		iterEdge->eEndNode = end-1;
	}

*/

	//对社团代表节点进行布局
	m_vDisplacement.clear();
	m_vDisplacement.resize(_vNodes.size());
	PosInitialize(_vNodes);

	size_t nodeSize = _vNodes.size();
	size_t edgeSize = _vEdges.size();

	m_para_k = sqrt(1.0/nodeSize);

	double d_temp = m_temperature/(m_iterNum+1);
	double temp = m_temperature;
	for ( unsigned short i = 0; i < m_iterNum; ++i )
	{
		//vProgress->setValue(i);
		//qApp->processEvents();

		//initialize displacement
		for ( size_t j = 0; j < nodeSize; ++j )
		{
			m_vDisplacement[j].d_x = 0.0;
			m_vDisplacement[j].d_y = 0.0;
		}

		//node 斥力
		for ( size_t j = 0; j < nodeSize; ++j )
		{
			SPoint3 &point_1 = _vNodes[j].point;
			for ( size_t k = 0; k < nodeSize; ++k )
			{
				if ( k == j )
				{
					continue;
				}
				SPoint3 &point_2 = _vNodes[k].point;

				double dis = sqrt((point_1.x-point_2.x)*(point_1.x-point_2.x)
					+(point_1.y-point_2.y)*(point_1.y-point_2.y));
				if ( dis < 0.01 )
				{
					dis = 0.01;
				}
				m_vDisplacement[j].d_x += (point_1.x-point_2.x)*m_para_k*m_para_k/(dis*dis);
				m_vDisplacement[j].d_y += (point_1.y-point_2.y)*m_para_k*m_para_k/(dis*dis);
			}
		}

		//edge 引力
		for ( size_t j = 0; j < edgeSize; ++j )
		{
			const SEdgeStruct &t_edge = _vEdges[j];
			SPoint3 &point_1 = _vNodes[t_edge.eStartNode].point;
			SPoint3 &point_2 = _vNodes[t_edge.eEndNode].point;

			double dis = sqrt((point_1.x-point_2.x)*(point_1.x-point_2.x)
				+(point_1.y-point_2.y)*(point_1.y-point_2.y));
			if ( dis < 0.01 )
			{
				dis = 0.01;
			}
			m_vDisplacement[t_edge.eStartNode].d_x -= (point_1.x-point_2.x)*dis/m_para_k;
			m_vDisplacement[t_edge.eStartNode].d_y -= (point_1.y-point_2.y)*dis/m_para_k;
			m_vDisplacement[t_edge.eEndNode].d_x += (point_1.x-point_2.x)*dis/m_para_k;
			m_vDisplacement[t_edge.eEndNode].d_y += (point_1.y-point_2.y)*dis/m_para_k;
		}

		//move
		for ( size_t j = 0; j < nodeSize; ++j )
		{
			SDisplacement &disp = m_vDisplacement[j];
			double length = sqrt(disp.d_x*disp.d_x+disp.d_y*disp.d_y);
			if ( length < 0.01 )
			{
				length = 0.1;
			}
			_vNodes[j].point.x += disp.d_x*temp/length;
			_vNodes[j].point.y += disp.d_y*temp/length;
		}
		temp -= d_temp;
	}
	//vProgress->setValue(m_iterNum-1);
	//qApp->processEvents();
	//delete vProgress;
	//vProgress = NULL;
}

void CPlacement::PosInitialize(vector<SNodeStruct> &vNodes)
{
	//0-1000
	vector<SNodeStruct>::iterator iter = vNodes.begin();

	//随即种子
	//srand((unsigned)time(NULL));
	for ( ; iter != vNodes.end(); ++iter )
	{
		int x_int = rand()%1000;
		int y_int = rand()%1000;

		iter->point.x = static_cast<float>(x_int)/1000;
		iter->point.y = static_cast<float>(y_int)/1000;
	}

}
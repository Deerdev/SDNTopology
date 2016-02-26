#ifndef STRUCTDEFINE_H
#define STRUCTDEFINE_H

#include <string>
#include <vector>

using namespace std;

const float PI = 3.1415926f;
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define max(a, b)  (((a) > (b)) ? (a) : (b))

struct IPPair 
{
	string IP;
	string Mask;
	string NetworkIP;
};

struct RouterInfo
{
	string routerName;
	string interfaceName;
	string interfaceIP;
    int interfaceIndex;

    string desRouterName;
    string desInterfaceName;
    string desInterfaceIP;

    string Mask;
};

//定义视图显示范围
struct SViewRange
{
	int w;
	int h;
	int d;			//视图显示范围的宽，高，深度
};

//定义三维空间坐标点结构
struct SPoint3
{
	double x,y,z;			//点的三维坐标
    SPoint3(){ x = y = z = 0; }	//默认构造函数
	SPoint3(float _x, float _y, float _z){
		x=_x,y=_y,z=_z;}          //构造函数
};

//定义节点结构
struct SNodeStruct
{
	string nName;			//节点名字
	size_t nID;				//节点id
	SPoint3	 point;			//节点的三维空间坐标
};

//定义边结构
struct SEdgeStruct
{
	size_t eStartNode;		//边起始端点索引
	size_t eEndNode;		//边终止端点索引
    int eEdgeIndex;
};

struct SRouterInterfaceVec
{
    size_t eEndNode;		//边终止端点索引

    int interfaceIndex;
    string routerName;
    string interfaceName;
    string interfaceIP;

    int desInterfaceIndex;
    string desRouterName;
    string desInterfaceName;
    string desInterfaceIP;

    string Mask;
};

//定义社团结构
struct SCommunityStruct
{
	vector<SNodeStruct> cNodes;			//社团中的所有节点
	vector<SEdgeStruct> cEdges;			//社团中的所有边
};

//布局范围（长、宽、高）
struct SPlacementRange
{
	double length;
	double width;
	double height;
	SPlacementRange() { length = 1.0; width = 1.0; height = 1.0; }
};

//节点移动坐标
struct SDisplacement
{
	double d_x;
	double d_y;
	double d_z;
	//SDisplacement() { d_x = 0.0; d_y = 0.0; d_z = 0.0; }
};

#endif

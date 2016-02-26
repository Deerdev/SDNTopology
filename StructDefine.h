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

//������ͼ��ʾ��Χ
struct SViewRange
{
	int w;
	int h;
	int d;			//��ͼ��ʾ��Χ�Ŀ��ߣ����
};

//������ά�ռ������ṹ
struct SPoint3
{
	double x,y,z;			//�����ά����
    SPoint3(){ x = y = z = 0; }	//Ĭ�Ϲ��캯��
	SPoint3(float _x, float _y, float _z){
		x=_x,y=_y,z=_z;}          //���캯��
};

//����ڵ�ṹ
struct SNodeStruct
{
	string nName;			//�ڵ�����
	size_t nID;				//�ڵ�id
	SPoint3	 point;			//�ڵ����ά�ռ�����
};

//����߽ṹ
struct SEdgeStruct
{
	size_t eStartNode;		//����ʼ�˵�����
	size_t eEndNode;		//����ֹ�˵�����
    int eEdgeIndex;
};

struct SRouterInterfaceVec
{
    size_t eEndNode;		//����ֹ�˵�����

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

//�������Žṹ
struct SCommunityStruct
{
	vector<SNodeStruct> cNodes;			//�����е����нڵ�
	vector<SEdgeStruct> cEdges;			//�����е����б�
};

//���ַ�Χ���������ߣ�
struct SPlacementRange
{
	double length;
	double width;
	double height;
	SPlacementRange() { length = 1.0; width = 1.0; height = 1.0; }
};

//�ڵ��ƶ�����
struct SDisplacement
{
	double d_x;
	double d_y;
	double d_z;
	//SDisplacement() { d_x = 0.0; d_y = 0.0; d_z = 0.0; }
};

#endif

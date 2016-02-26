#ifndef DRAGNETWORK_H
#define DRAGNETWORK_H 

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>
#include <map>
#include <algorithm>
#include <set>
#include "StructDefine.h"
#include "PlacementAlgorithm.h"
using namespace std;

class DragNetwork
{
public:
    DragNetwork(const string& _filePath);
	~DragNetwork();

	vector<SNodeStruct> GetNodeStruct();
	vector<SEdgeStruct> GetEdgeStruct();
	int GetNodeNum();
	int GetEdgetNum();
    vector<vector<int> > GetAdjacentMatrix();
    vector<string> GetNodeNames();
    vector<vector<SRouterInterfaceVec> > GetRouterInterfaceVec();
    bool GetSuccessFlag();

    void ConstructStruct();

private:
	string intTostr(const int & _int); 
	string CalculateNetworkID(const string _IP, const string _Mask);
	void Drag();
	void OutputFile(const string& _fileName);	

private:
	vector<IPPair> m_IPPairVec;
	map<string,string> m_IP_Mask;
	map<string,int> m_IPPairTable;
	map<string,string> m_IP_Router;
	map<string,string> m_IP_Interface;
	map<string,int> m_Router_Index;
    map<string,int> m_IP_InterfaceIndex;
	vector<RouterInfo> m_routerInfoVec;
	vector<SNodeStruct> m_nodeStruct;
	vector<SEdgeStruct> m_edgeStruct;
	vector<vector<int> > m_adjMatrix;
    vector<vector<SRouterInterfaceVec> > m_routerInterfaceVec;
	vector<string> m_nodeNameVec;
    int m_nodeNum;
	int m_edgeNum;
    bool m_success;
    string m_filePath;
};
#endif //DRAGNETWORK_H

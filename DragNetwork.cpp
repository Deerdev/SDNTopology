#include "DragNetwork.h"

DragNetwork::DragNetwork(const string &_filePath)
{
    m_success = true;
    m_filePath = _filePath;
	Drag();	
}

string DragNetwork::intTostr(const int & _int)  
{  
	stringstream stream;  
	stream<<_int;  
	return stream.str();  
}  

string DragNetwork::CalculateNetworkID(const string _IP, const string _Mask)
{
	//calculate network id
	//split ip
	string tmpIP = _IP;
	int IPAIndex = tmpIP.find_first_of(".");
	int IPA = atoi(tmpIP.substr(0, IPAIndex).c_str());
	tmpIP.erase(0,IPAIndex+1);		

	int IPBIndex = tmpIP.find_first_of(".");
	int IPB = atoi(tmpIP.substr(0, IPBIndex).c_str());
	tmpIP.erase(0,IPBIndex+1);	

	int IPCIndex = tmpIP.find_first_of(".");
	int IPC = atoi(tmpIP.substr(0, IPCIndex).c_str());
	tmpIP.erase(0,IPCIndex+1);	

	int IPD = atoi(tmpIP.c_str());

	//split mask
	string tmpMask = _Mask;
	int MaskAIndex = tmpMask.find_first_of(".");
	int MaskA = atoi(tmpMask.substr(0, MaskAIndex).c_str());
	tmpMask.erase(0,MaskAIndex+1);		

	int MaskBIndex = tmpMask.find_first_of(".");
	int MaskB = atoi(tmpMask.substr(0, MaskBIndex).c_str());
	tmpMask.erase(0,MaskBIndex+1);	

	int MaskCIndex = tmpMask.find_first_of(".");
	int MaskC = atoi(tmpMask.substr(0, MaskCIndex).c_str());
	tmpMask.erase(0,MaskCIndex+1);	

	int MaskD = atoi(tmpMask.c_str());

	//calculate
	int networkA = IPA & MaskA;
	int networkB = IPB & MaskB;
	int networkC = IPC & MaskC;
	int networkD = IPD & MaskD;

	string networkID = intTostr(networkA) + "." + intTostr(networkB) + "." + intTostr(networkC) + "." + intTostr(networkD);
	return networkID;
}

void DragNetwork::Drag()
{
    ifstream ipFile(m_filePath.c_str());
	if (ipFile.fail())
	{
        cout<<"ERROR!Can not open "<<m_filePath<<"!"<<endl;
        m_success = false;
        return;
	}

    while (!ipFile.eof())
    {
        RouterInfo routerInfo;
        IPPair tmpIPPair;
        ipFile>>routerInfo.routerName>>routerInfo.interfaceIndex>>routerInfo.interfaceName>>routerInfo.interfaceIP>>routerInfo.Mask;
        if ( routerInfo.routerName.empty() || routerInfo.interfaceName.empty() || routerInfo.interfaceIP.empty() || routerInfo.Mask.empty())
            continue;
        tmpIPPair.IP = routerInfo.interfaceIP;
        tmpIPPair.Mask = routerInfo.Mask;
        m_IP_Mask[tmpIPPair.IP] = tmpIPPair.Mask;
        tmpIPPair.NetworkIP = CalculateNetworkID(tmpIPPair.IP, tmpIPPair.Mask);
        m_IPPairVec.push_back(tmpIPPair);

        m_IP_Router[routerInfo.interfaceIP] = routerInfo.routerName;
        m_IP_Interface[routerInfo.interfaceIP] = routerInfo.interfaceName;
        m_IP_InterfaceIndex[routerInfo.interfaceIP] = routerInfo.interfaceIndex;
        m_routerInfoVec.push_back(routerInfo);
    }
    ipFile.close();

	vector<bool> flag(m_IPPairVec.size(), false);

	//construct IP pair
	int ipNum = m_IPPairVec.size();
	for ( int i = 0; i < ipNum; ++i )
	{
		if (flag[i])
			continue;
		for ( int j = i; j < ipNum; ++j )
		{
			if (m_IPPairVec[i].NetworkIP == m_IPPairVec[j].NetworkIP)
			{
				m_IPPairTable[m_IPPairVec[i].IP] = j;
				m_IPPairTable[m_IPPairVec[j].IP] = i;
				flag[j] = true;
			}
		}
	}
}

void DragNetwork::OutputFile( const string& _fileName )
{
	ofstream outFile(_fileName.c_str());
	for ( size_t i = 0; i <m_routerInfoVec.size(); ++i )
	{
		m_routerInfoVec[i].desInterfaceIP = m_IPPairVec[m_IPPairTable[m_routerInfoVec[i].interfaceIP]].IP;
		m_routerInfoVec[i].desRouterName = m_IP_Router[m_routerInfoVec[i].desInterfaceIP];
		m_routerInfoVec[i].desInterfaceName = m_IP_Interface[m_routerInfoVec[i].desInterfaceIP];

		outFile<<"骨干网"<<"\t"
			<<m_routerInfoVec[i].routerName<<"\t"
			<<"0"<<"\t"
			<<m_routerInfoVec[i].interfaceIP<<"\t"
			<<m_IP_Mask[m_routerInfoVec[i].interfaceIP]<<"\t"
			<<"\t"
			<<"\t"
			<<m_routerInfoVec[i].desRouterName<<"\t"
			<<m_routerInfoVec[i].desInterfaceName<<"\t"
			<<"0"<<"\t"
			<<m_routerInfoVec[i].desInterfaceIP<<"\t"
			<<m_IP_Mask[m_routerInfoVec[i].desInterfaceIP]<<"\t"
			<<"\t"
			<<"\t"
			<<"\t"<<endl;
	}

	outFile.close();
}

vector<SNodeStruct> DragNetwork::GetNodeStruct()
{
	return m_nodeStruct;
}

vector<SEdgeStruct> DragNetwork::GetEdgeStruct()
{
	return m_edgeStruct;
}

void DragNetwork::ConstructStruct()
{
	if ( m_IPPairTable.size() % 2 != 0 )
	{
		cout<<"IP adress number may be wrong!Please check!"<<endl;
		return;
	}
	m_edgeNum = m_IPPairTable.size() / 2;

	//construct node struct
	size_t routerInfoNum = m_routerInfoVec.size();
	
	int routerIndex = 0;
	for ( size_t i = 0; i < routerInfoNum; ++i )
	{
		if (find(m_nodeNameVec.begin(), m_nodeNameVec.end(), m_routerInfoVec[i].routerName) != m_nodeNameVec.end())
		{
			continue;
		}
		
		m_nodeNameVec.push_back(m_routerInfoVec[i].routerName);
		m_Router_Index[m_routerInfoVec[i].routerName] = routerIndex;	
		SNodeStruct tmpNodeStruct;
		tmpNodeStruct.nID = routerIndex;
		tmpNodeStruct.nName = m_routerInfoVec[i]. routerName;
		m_nodeStruct.push_back(tmpNodeStruct);
		routerIndex++;
	}

	m_nodeNum = routerIndex;

    m_adjMatrix = vector<vector<int> > (m_nodeNum, vector<int>(m_nodeNum, 0));

	//construct edge struct
	vector<bool> flag(m_IPPairVec.size(), false);

	for ( size_t edgeIndex = 0; edgeIndex < m_IPPairVec.size(); ++edgeIndex )
	{
		if (flag[edgeIndex])
			continue;
		
		string srcIP = m_IPPairVec[edgeIndex].IP;
		string desIP = m_IPPairVec[m_IPPairTable[srcIP]].IP;
		SEdgeStruct tmpEdgeStruct;
		tmpEdgeStruct.eStartNode = m_Router_Index[m_IP_Router[srcIP]];
		tmpEdgeStruct.eEndNode = m_Router_Index[m_IP_Router[desIP]];
        tmpEdgeStruct.eEdgeIndex = 1;
        m_adjMatrix[tmpEdgeStruct.eStartNode][tmpEdgeStruct.eEndNode]++;
		m_adjMatrix[tmpEdgeStruct.eEndNode][tmpEdgeStruct.eStartNode]++;
		flag[m_IPPairTable[srcIP]] = true;
		m_edgeStruct.push_back(tmpEdgeStruct);
	}

    //Construct Edge Index
    vector<bool> edgeflag(m_edgeStruct.size(),false);
    for ( size_t edgeIndex = 0; edgeIndex < m_edgeStruct.size(); ++edgeIndex )
    {
        if (edgeflag[edgeIndex])
        {
            continue;
        }
        size_t startNode = m_edgeStruct[edgeIndex].eStartNode;
        size_t endNode = m_edgeStruct[edgeIndex].eEndNode;
        int num = 1;
        for (size_t matchNodeIndex = edgeIndex + 1; matchNodeIndex < m_edgeStruct.size(); ++matchNodeIndex )
        {
            if ((m_edgeStruct[matchNodeIndex].eStartNode == startNode &&
                 m_edgeStruct[matchNodeIndex].eEndNode == endNode) ||
                (m_edgeStruct[matchNodeIndex].eStartNode == endNode &&
                 m_edgeStruct[matchNodeIndex].eEndNode == startNode))
            {
                edgeflag[matchNodeIndex] = true;
                m_edgeStruct[matchNodeIndex].eEdgeIndex = ++num;
            }
        }
    }

    //Construct router interface set
    m_routerInterfaceVec.resize(m_nodeNum);
    for ( size_t index = 0; index < m_routerInfoVec.size(); ++index )
    {
        int routerIndex = m_Router_Index[m_routerInfoVec[index].routerName];
        SRouterInterfaceVec interfaceVec;
        interfaceVec.routerName = m_routerInfoVec[index].routerName;
        interfaceVec.interfaceIndex = m_routerInfoVec[index].interfaceIndex;
        interfaceVec.interfaceIP = m_routerInfoVec[index].interfaceIP;
        interfaceVec.interfaceName = m_routerInfoVec[index].interfaceName;

        interfaceVec.desInterfaceIP = m_IPPairVec[m_IPPairTable[interfaceVec.interfaceIP]].IP;
        interfaceVec.desInterfaceName = m_IP_Interface[interfaceVec.desInterfaceIP];
        interfaceVec.desInterfaceIndex = m_IP_InterfaceIndex[interfaceVec.desInterfaceIP];
        interfaceVec.Mask = m_IP_Mask[interfaceVec.interfaceIP];

        interfaceVec.desRouterName = m_IP_Router[interfaceVec.desInterfaceIP];
        interfaceVec.eEndNode = m_Router_Index[interfaceVec.desRouterName];

        m_routerInterfaceVec[routerIndex].push_back(interfaceVec);
    }

    if ((int)m_edgeStruct.size() != m_edgeNum)
	{
		cout<<"Drag edge error!"<<endl;
		return;
	}

	CPlacement layout;
	layout.ForceDirected2D(m_nodeStruct, m_edgeStruct);
}

DragNetwork::~DragNetwork()
{

}

int DragNetwork::GetNodeNum()
{
	return m_nodeNum;
}

int DragNetwork::GetEdgetNum()
{
	return m_edgeNum;
}

vector<vector<int> > DragNetwork::GetAdjacentMatrix()
{
	return m_adjMatrix;
}

vector<string> DragNetwork::GetNodeNames()
{
	return m_nodeNameVec;
}

vector<vector<SRouterInterfaceVec> > DragNetwork::GetRouterInterfaceVec()
{
    return m_routerInterfaceVec;
}

bool DragNetwork::GetSuccessFlag()
{
    return m_success;
}

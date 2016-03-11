#ifndef STRUCTFILE_H
#define STRUCTFILE_H

#include <vector>
#include <QList>

#include "DataDefine.h"

using namespace std;

typedef vector<QString> VecQStr;
typedef vector<VecQStr> VecVecQStr;

struct TaskStruct
{
    QString srcDevice;
    QString srcIP;
};

enum LinkType
{
    POS = 0, Ethernet
};

struct SNodeStructInfo
{
    QString		nodetype;			//节点类型
    QString networkLocation;

    int Device_ID;//当前设备id
    int Port_ID;//当前端口id
    QString name;//当前设备名字
    QString interfaceName;//当前端口名字
    QString area;
    QString IP;
    QString subnetMask;
    QString describe;
    QString costValue;
    QString remark;

    int linkPort_Device_ID;
    int linkPort_ID;
    QString linkPort_name;
    QString linkPort_interfaceName;
    QString linkPort_area;
    QString linkPort_IP;
    QString linkPort_subnetMask;
    QString linkPort_describe;
    QString linkPort_costValue;
    QString linkPort_remark;

    int linkType;
    int bandWidth;//链路带宽
    QString delay;
};

//交换机属性
struct CSwitcherInfo
{
    QString name;//设备名称
    NodeType type;
    QString typeName;
    int ID;//域ID

    //交换机信息
    int portNum;//端口数
    //QString protocol; //协议名
    QString networkLocation;//网络位置
    QVector<SNodeStructInfo> LNodes;  //已经连接的端口属性

    //主机信息
    QString macAddr;
    //
    QString attachSDPID;
    //
    int attachSId;
    //
    int attachSPort;
    //
    QString userName;
    //
    QString userGroup;
};

//链路属性
struct CLinkInfo
{
    int s_switchID;
    int d_switchID;
    //int linkType;//链路类型
    QString bandWidth;//链路带宽
    //QString delay;
    //int pos_link_num;//链路数
    //int ethernet_link_num;
};

#endif

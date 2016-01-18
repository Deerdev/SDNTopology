#ifndef DATADEFINE_H
#define DATADEFINE_H

#include <iostream>
#include <QString>
#include <QVector>

enum NodeType{SWITCH,HOST};

struct FlowInf
{
    QString m_in_port;
    QString m_cookie;
    QString m_priority;
    QString m_active;
    QString m_name;
    QString m_actions;
    QString m_switch;
};
struct SwitchPortInf{

};

struct SwitchNode
{
    QString DPID;
    int SID;
    QString hardwareDesc;
    int portNum;
    QVector<SwitchPortInf *> portInfList;
};

struct HostNode
{
    //
    QString macAddr;
    //
    int HID;
    //
    int attachSId;
    //
    int attachSPort;
    //
    QString userName;
    //
    QString userGroup;
};

struct NodeInf{
    NodeType type;
    void *pNode;
};

struct LinkInf{
    int src_nodeId;
    int des_nodeId;
    int src_port;
    int des_port;
};

struct NodeList{
    QVector<NodeInf> nodeList;
    QVector<LinkInf> linkList;
    int switchNum;
    int hostNum;
    int linkNum;
};

#endif // DATADEFINE_H

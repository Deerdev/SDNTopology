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
    QString		nodetype;			//�ڵ�����
    QString networkLocation;

    int Device_ID;//��ǰ�豸id
    int Port_ID;//��ǰ�˿�id
    QString name;//��ǰ�豸����
    QString interfaceName;//��ǰ�˿�����
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
    int bandWidth;//��·����
    QString delay;
};

//����������
struct CSwitcherInfo
{
    QString name;//�豸����
    NodeType type;
    QString typeName;
    int ID;//��ID

    //��������Ϣ
    int portNum;//�˿���
    //QString protocol; //Э����
    QString networkLocation;//����λ��
    QVector<SNodeStructInfo> LNodes;  //�Ѿ����ӵĶ˿�����

    //������Ϣ
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

//��·����
struct CLinkInfo
{
    int s_switchID;
    int d_switchID;
    //int linkType;//��·����
    QString bandWidth;//��·����
    //QString delay;
    //int pos_link_num;//��·��
    //int ethernet_link_num;
};

#endif

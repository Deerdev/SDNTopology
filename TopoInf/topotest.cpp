#include "topotest.h"



topotest::topotest()
{
    int m_id = 1;
    struct SwitchNode *t_node1 = new SwitchNode;
    t_node1->type = SWITCH;
    t_node1->DPID = "00:00:00:00:00:00:00:01";
    t_node1->SID = 1;
    t_node1->ID = m_id++;
    t_node1->hardwareDesc = "s1";
    t_node1->portNum = 3;
    t_node1->level = 1;
    t_node1->portInfList.clear();

    NodeInf t_nInf1;
    t_nInf1.type = SWITCH;
    t_nInf1.pNode = t_node1;
    m_nodelist.append(t_nInf1);

    //--------------------------------
    struct SwitchNode *t_node2 = new SwitchNode;
    t_node2->type = SWITCH;
    t_node2->DPID = "00:00:00:00:00:00:00:02";
    t_node2->SID = 2;
    t_node2->ID = m_id++;
    t_node2->hardwareDesc = "s2";
    t_node2->portNum = 2;
    t_node2->level = 2;
    t_node2->portInfList.clear();

    NodeInf t_nInf2;
    t_nInf2.type = SWITCH;
    t_nInf2.pNode = t_node2;
    m_nodelist.append(t_nInf2);

    //--------------------------------
    struct SwitchNode *t_node3 = new SwitchNode;
    t_node3->type = SWITCH;
    t_node3->DPID = "00:00:00:00:00:00:00:03";
    t_node3->SID = 3;
    t_node3->ID = m_id++;
    t_node3->hardwareDesc = "s3";
    t_node3->portNum = 2;
    t_node3->level = 2;
    t_node3->portInfList.clear();

    NodeInf t_nInf3;
    t_nInf3.type = SWITCH;
    t_nInf3.pNode = t_node3;
    m_nodelist.append(t_nInf3);

    //--------------------------------
    HostNode *t_h1 = new HostNode;
    t_h1->type = HOST;
    t_h1->macAddr = "00:00:00:00:00:01";
    t_h1->ID = m_id++;
    t_h1->attachSId = 1;
    t_h1->attachSPort=1;
    t_h1->userName="h1";
    t_h1->userGroup = "g1";
    t_h1->level = 0;

    NodeInf t_nInf4;
    t_nInf4.type = HOST;
    t_nInf4.pNode = t_h1;
    m_nodelist.append(t_nInf4);

    //--------------------------------
    HostNode *t_h2 = new HostNode;
    t_h2->type = HOST;
    t_h2->macAddr = "00:00:00:00:00:02";
    t_h2->ID = m_id++;
    t_h2->attachSId = 2;
    t_h2->attachSPort=2;
    t_h2->userName="h2";
    t_h2->userGroup = "g2";
    t_h2->level = 3;

    NodeInf t_nInf5;
    t_nInf5.type = HOST;
    t_nInf5.pNode = t_h2;
    m_nodelist.append(t_nInf5);

    //--------------------------------
    HostNode *t_h3 = new HostNode;
    t_h3->type = HOST;
    t_h3->macAddr = "00:00:00:00:00:03";
    t_h3->ID = m_id++;
    t_h3->attachSId = 3;
    t_h3->attachSPort=3;
    t_h3->userName="h3";
    t_h3->userGroup = "g3";
    t_h3->level = 3;

    NodeInf t_nInf6;
    t_nInf6.type = HOST;
    t_nInf6.pNode = t_h3;
    m_nodelist.append(t_nInf6);

    m_switchmap.insert(t_node1->ID,t_node1);
    m_switchmap.insert(t_node2->ID,t_node2);
    m_switchmap.insert(t_node3->ID,t_node3);

    m_hostmap.insert(t_h1->ID, t_h1);
    m_hostmap.insert(t_h2->ID, t_h2);
    m_hostmap.insert(t_h3->ID, t_h3);

    foreach(int dpid, m_switchmap.keys())
    {
    }

    LinkInf t_link;
    t_link.src_nodeId=4;
    t_link.src_port = 0;
    t_link.des_nodeId=1;
    t_link.des_port = 1;
    m_linkList.append(t_link);

    LinkInf t_link1;
    t_link1.src_nodeId=5;
    t_link1.src_port = 0;
    t_link1.des_nodeId=2;
    t_link1.des_port = 2;
    m_linkList.append(t_link1);

    LinkInf t_link2;
    t_link2.src_nodeId=6;
    t_link2.src_port = 0;
    t_link2.des_nodeId=3;
    t_link2.des_port = 2;
    m_linkList.append(t_link2);

    LinkInf t_link3;
    t_link3.src_nodeId=1;
    t_link3.src_port = 2;
    t_link3.des_nodeId=2;
    t_link3.des_port = 1;
    m_linkList.append(t_link3);

    LinkInf t_link4;
    t_link4.src_nodeId=1;
    t_link4.src_port = 3;
    t_link4.des_nodeId=3;
    t_link4.des_port = 1;
    m_linkList.append(t_link4);



}

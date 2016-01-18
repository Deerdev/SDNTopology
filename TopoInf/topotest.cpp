#include "topotest.h"

topotest::topotest()
{
    struct SwitchNode *t_node1 = new SwitchNode;
    t_node1->DPID = "00:00:00:00:00:00:00:01";
    t_node1->SID = 1;
    t_node1->hardwareDesc = "s1";
    t_node1->portNum = 3;
    t_node1->portInfList.clear();

    struct SwitchNode *t_node2 = new SwitchNode;
    t_node2->DPID = "00:00:00:00:00:00:00:02";
    t_node2->SID = 2;
    t_node2->hardwareDesc = "s2";
    t_node2->portNum = 2;
    t_node2->portInfList.clear();

    struct SwitchNode *t_node3 = new SwitchNode;
    t_node3->DPID = "00:00:00:00:00:00:00:03";
    t_node3->SID = 3;
    t_node3->hardwareDesc = "s3";
    t_node3->portNum = 2;
    t_node3->portInfList.clear();

    HostNode *t_h1 = new HostNode;
    t_h1->macAddr = "00:00:00:00:00:01";
    t_h1->HID = 1;
    t_h1->attachSId = 1;
    t_h1->attachSPort=1;
    t_h1->userName="h1";
    t_h1->userGroup = "g1";

    HostNode *t_h2 = new HostNode;
    t_h2->macAddr = "00:00:00:00:00:02";
    t_h2->HID = 2;
    t_h2->attachSId = 2;
    t_h2->attachSPort=2;
    t_h2->userName="h2";
    t_h2->userGroup = "g2";

    HostNode *t_h3 = new HostNode;
    t_h3->macAddr = "00:00:00:00:00:03";
    t_h3->HID = 3;
    t_h3->attachSId = 3;
    t_h3->attachSPort=3;
    t_h3->userName="h3";
    t_h3->userGroup = "g3";

    m_switchmap.insert(t_node1->SID,t_node1);
    m_switchmap.insert(t_node2->SID,t_node2);
    m_switchmap.insert(t_node3->SID,t_node3);

    m_hostmap.insert(t_h1->HID, t_h1);
    m_hostmap.insert(t_h2->HID, t_h2);
    m_hostmap.insert(t_h3->HID, t_h3);

}

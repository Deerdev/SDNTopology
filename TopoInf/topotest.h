#ifndef TOPOTEST_H
#define TOPOTEST_H

#include "DataDefine.h"
#include <QMap>
#include <QVector>

class topotest
{
public:
    topotest();

    QMap<int, SwitchNode*> m_switchmap;
    QMap<int, HostNode*> m_hostmap;
    QMap<int , int> m_id2Dpid;
    int m_id;
    QVector<NodeInf> m_nodelist;
    QVector<LinkInf> m_linkList;
};

#endif // TOPOTEST_H

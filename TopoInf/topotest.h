#ifndef TOPOTEST_H
#define TOPOTEST_H

#include "DataDefine.h"
#include <QMap>

class topotest
{
public:
    topotest();
    struct NodeList m_nodelist;
    QMap<int, SwitchNode*> m_switchmap;
    QMap<int, HostNode*> m_hostmap;
};

#endif // TOPOTEST_H

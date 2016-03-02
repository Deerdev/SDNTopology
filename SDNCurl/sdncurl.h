#ifndef SDNCURL_H
#define SDNCURL_H

#include "curl.h"
#include <iostream>
#include <string>
#include <QObject>
#include <QList>

#define MAX_BUFSIZE 1024
class QString;

class SDNCurl
{
//    Q_OBJECT
public:
    SDNCurl();
    bool getFlows(QString &_flowsStr, QString _DPID = "all");
    bool addFlows(QList<QString> &_sjsonVec);
    bool deleteFlow(QString);
    bool getTopo(QString &_TopoStr);
    bool getSwitchesInf(QString &_switchInf);
    bool getHostDevices(QString &_hostInf);
    static size_t write_data( void *buffer, size_t size, size_t nmemb, void *userp );

private:
    static char wr_buf[MAX_BUFSIZE];
    static int wr_index;
};



#endif // SDNCURL_H

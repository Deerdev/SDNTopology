#ifndef SDNSERVER_H
#define SDNSERVER_H

#include <QTcpServer>
#include "DataDefine.h"

class SDNServer: public QTcpServer
{
    Q_OBJECT

public:
    SDNServer(QObject *parent=NULL);
    ~ParaServer();

private:
    void incomingConnection(int _socketId);

signals:
    void senddata2Socket(SockData);
    void recvDataFromSocket(SockData);

private slots:
    void emitSendDataSlot(SockData);
    void emitScokDataSlot(SockData);

private:
    int m_socketId;
};

#endif // SDNSERVER_H

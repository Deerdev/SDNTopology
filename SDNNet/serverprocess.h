#ifndef SERVERPROCESS_H
#define SERVERPROCESS_H

#include <QTcpSocket>
#include "DataDefine.h"

class ServerProcess: public QObject
{
    Q_OBJECT
public:
    ServerProcess(int socketDescriptor, QObject *parent=0);
    ~ServerProcess();

    QTcpSocket *m_socket;

signals:
    void finished();

    void recvedData(SockData);

private slots:
    void readData();
    void sendData(SockData _data);
    void connect2client();
    void connectionClosedbyClient();

private:
    int socketDescriptor;
    quint16 nextBlockSize;
};

#endif // SERVERPROCESS_H

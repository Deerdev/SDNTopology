#include "sdnserver.h"
#include <QThread>
#include"serverprocess.h"


SDNServer::SDNServer(QObject *parent): QTcpServer(parent)
{
}


void SDNServer::incomingConnection(int _socketId)
{
    qDebug()<<"incoming-server, current thread Id: "<<QThread::currentThreadId ()<<endl;
    QThread *sockThread = new QThread();
    ServerProcess *cpu = new ServerProcess(_socketId);
    cpu->moveToThread (sockThread);


    connect(cpu, SIGNAL(finished()), sockThread, SLOT(quit()));
    connect(cpu, SIGNAL(finished()), cpu, SLOT(deleteLater()));
    connect(sockThread,SIGNAL(started()),cpu,SLOT(connect2client()));
    connect(sockThread, SIGNAL(finished()), sockThread, SLOT(deleteLater()));

    qRegisterMetaType<SockData>("SockData");
    qRegisterMetaType<SockData>("SockData&");
    connect(this,SIGNAL(senddata2Socket(SockData)),cpu,SLOT(sendData(SockData)));
    connect(cpu,SIGNAL(recvedData(SockData)),this,SLOT(emitScokDataSlot(SockData)));

    sockThread->start();
}

void SDNServer::emitSendDataSlot(SockData _data)
{
    qDebug()<<"senddata-server, current thread Id: "<<QThread::currentThreadId ()<<endl;
    emit senddata2Socket(_data);
}

void SDNServer::emitScokDataSlot(SockData _data)
{
    emit recvDataFromSocket(_data);
}

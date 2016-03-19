#include "serverprocess.h"
#include <QThread>

ServerProcess::ServerProcess(int socketDescriptor, QObject *parent)
    : QObject(parent),socketDescriptor(socketDescriptor)
{
    m_socket = NULL;
    nextBlockSize = 0;
}


ServerProcess::~ServerProcess()
{
    if (m_socket)
    {
        delete m_socket;
        m_socket = NULL;
    }
    emit finished();
}

void ServerProcess::readData()
{
    qDebug()<<"r-sprocess, current thread Id: "<<QThread::currentThreadId ()<<endl;
    //QByteArray buff=m_socket->readAll();
    qDebug()<<"read-sprocess: ";
    //qDebug()<<buff<<endl;

//    quint8 tag = CGraphicItemData::TAG;
//    QDataStream in_data(m_socket);
//    in_data.setVersion(QDataStream::Qt_4_3);

//    if (nextBlockSize == 0)
//    {
//        if (m_socket->bytesAvailable() < sizeof(quint16))
//            return;
//        in_data >> nextBlockSize;
//    }
//    if (m_socket->bytesAvailable() < nextBlockSize)
//        return;
//    SockData t_data;
//    in_data >> t_data.msgType;
//    CGraphicItemData *pada = new CGraphicItemData;
//    in_data >> tag;
//    in_data >> (*pada);
//    t_data.pData = pada;
//    nextBlockSize = 0;
//    emit recvedData(t_data);
}

void ServerProcess::sendData(SockData _data)
{
    qDebug()<<"w-sprocess, current thread Id: "<<QThread::currentThreadId()<<endl;
    qDebug()<<"write!"<<endl;

//    QByteArray block;
//    QDataStream out(&block, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_4_3);
//    out<< quint16(0);
//    out << _data.msgType;

//    out << quint8(CGraphicItemData::TAG) << (*_data.pData);
//    out.device()->seek(0);
//    int t_size = quint16(block.size() - sizeof(quint16));
//    out << quint16(block.size() - sizeof(quint16));
//    m_socket->write(block);
}

void ServerProcess::connect2client()
{
    nextBlockSize = 0;

    m_socket=new QTcpSocket;
    connect(m_socket,SIGNAL(readyRead()),this,SLOT(readData()));
    connect(m_socket,SIGNAL(disconnected()),this,SLOT(connectionClosedbyClient()));
    qDebug()<<"setfd-sprocess, current thread Id: "<<QThread::currentThreadId ()<<endl;

    //将Server传来的socketDescriptor与刚创建的tcpSocket关联
    if (!m_socket->setSocketDescriptor(socketDescriptor)) {
        //emit error(m_socket->error());
        qDebug()<<"setfd-sprocess, erro";
        return;
    }
    qDebug()<<socketDescriptor;
}

void ServerProcess::connectionClosedbyClient()
{
    qDebug()<<"client exit!";
    m_socket->close();
}

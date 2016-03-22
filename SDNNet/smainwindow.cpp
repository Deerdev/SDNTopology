#include "smainwindow.h"
#include "ui_smainwindow.h"
#include <QFileDialog>
#include <QObject>
#include <QMessageBox>
#include <QTextCodec>

SMainWindow::SMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SMainWindow)
{
    ui->setupUi(this);

    /********** Send and receive message though UCP protocol **************/
    UdpSender = new QUdpSocket(this);
    UdpReader = new QUdpSocket(this);

    // 将UdpReader绑定到广播地址的6666端口，接收发到这个端口的信息
    UdpReader->bind(6666, QUdpSocket::ShareAddress);
    connect(UdpReader, SIGNAL(readyRead()), this, SLOT(readMessage()));


    /********** Send and receive files though TCP protocol **************/
    loadSize = 4*1024;  //将整个大的文件分成很多小的部分进行发送，每部分为4字节
    totalBytes = 0;
    bytesWritten = 0;
    bytesToWrite = 0;
    bytesReceived = 0;
    fileNameSize = 0;

    tcpServer = new QTcpServer(this);
    tcpSocket = new QTcpSocket(this);

    //当发现新连接时发出newConnection()信号
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));

    ui->sendFileButton->setEnabled(false); //开始使”发送文件“按钮不可用
    ui->sendMesButton->setEnabled(true); //开始使”发送信息“按钮可用

    // 服务器的缺省值
    hostIP = getHostIP();
    if (hostIP == 0)
        ui->hostLineEdit->setText("127.0.0.1");
    else
        ui->hostLineEdit->setText(hostIP);
    ui->portLineEdit->setText("5555");

    helpDialog = new CCDialog;
    helpDialog->hide();

    histroyMessageDialog = new hisMesDialog;
    histroyMessageDialog->hide();

    historyMessage = new QFile("historyMessage.txt");
    if(!historyMessage->open(QFile::Append))
    {
        qDebug() << "writing history Message file error!";
        return;
    }
}

SMainWindow::~SMainWindow()
{
    delete ui;
}


// 监听
void SMainWindow::listening()
{
    //初始化已发送字节为0
    bytesWritten = 0;
    bytesReceived = 0;
    blockSize = 0; //初始化其为0

    ui->listenButton->setEnabled(false);

    if(!tcpServer->listen(QHostAddress::Any, ui->portLineEdit->text().toInt()))
    {
        qDebug() << tcpServer->errorString();
        close();
        return;
    }

    ui->currentStatusLabel->setText("Status: listening...");
}

// 连接后的对话框提示
void SMainWindow::acceptConnection()
{
    //QMessageBox box_success;
    //box_success.information(NULL, "提示", "会话建立成功", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    tcpSocket = tcpServer->nextPendingConnection();

    // 当有数据发送成功时，我们更新进度条
    connect(tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(updateClientProgress(qint64)));

    // 当有数据接收成功时，我们更新进度条
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(updateServerProgress()));

    // 绑定错误处理
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));

    tcpServer->close();

    ui->currentStatusLabel->setText("Status: Waiting for file transfer...");
}

// 发送信息
void SMainWindow::sendMessage()
{
    localMessage = ui->inputTextEdit->toPlainText();

    QByteArray datagram = localMessage.toUtf8();

    //进行UDP数据报（datagrams）的发送
    UdpSender->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, 5566);

    // 显示消息
    showMessage(true);
}

// 接收信息
void SMainWindow::readMessage()
{
    //拥有等待的数据报
    while(UdpReader->hasPendingDatagrams())
    {
        QByteArray datagram; //拥于存放接收的数据报

        //让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据
        datagram.resize(UdpReader->pendingDatagramSize());

        //接收数据报，将其存放到datagram中
        UdpReader->readDatagram(datagram.data(), datagram.size());

        //将数据报内容显示出来
        serverMessage = datagram;
    }
    // 显示消息
    showMessage(false);
}

// 显示消息
void SMainWindow::showMessage(bool isSend)
{
    QDateTime time = QDateTime::currentDateTime();  //获取系统现在的时间
    QString str = time.toString("hh:mm:ss ddd");     //设置显示格式
    QString str4file = time.toString("yyyy-MM-dd hh:mm:ss");     //设置显示格式
    blockSize = 0;

    QFont font;
    font.setPixelSize(18);
    ui->textBrowser->setFont(font);

    QTextStream stream(historyMessage);

    if (isSend)
    {
        // 用不同的颜色显示信息所属和当前时间
        ui->textBrowser->setTextColor(QColor(0, 0, 0));
        QString entraMess = "I say: " + str;
        ui->textBrowser->append(entraMess);
        stream<<"I say: "<<str4file<<"\n"; // 写入 历史信息 文件时需要保存日期

        ui->textBrowser->setTextColor(QColor(0, 0, 255));
        ui->textBrowser->append(localMessage);
        ui->inputTextEdit->clear();
        ui->currentStatusLabel->setText("Status: send message successfully...");
        stream<<localMessage<<"\n";
    }
    else
    {
        // 用不同的颜色显示信息所属和当前时间
        ui->textBrowser->setTextColor(QColor(0, 0, 0));
        QString entraMess = "He/she: " + str;
        ui->textBrowser->append(entraMess);
        stream<<"He/she: "<<str4file<<"\n";

        ui->textBrowser->setTextColor(QColor(255, 0, 0));
        ui->textBrowser->append(serverMessage);
        ui->currentStatusLabel->setText("Status: new message coming...");
        stream<<localMessage<<"\n";
    }
}

// 打开文件
void SMainWindow::openFile()
{
    fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
    {
        ui->sendFileButton->setEnabled(true);
        QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
        ui->currentStatusLabel->setText(tr("Status: open file %1 successfully!").arg(currentFileName));
    }
}

// 实现文件大小等信息的发送
void SMainWindow::startTransferFile()
{
    localFile = new QFile(fileName);
    if(!localFile->open(QFile::ReadOnly))
    {
        qDebug() << "open file error!";
        return;
    }
    totalBytes = localFile->size(); //文件总大小

    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_0);

    QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);

    //依次写入总大小信息空间，文件名大小信息空间，文件名
    sendOut << qint64(0) << qint64(0) << currentFileName;

    //这里的总大小是文件名大小等信息和实际文件大小的总和
    totalBytes += outBlock.size();

    //返回outBolock的开始，用实际的大小信息代替两个qint64(0)空间
    sendOut.device()->seek(0);
    sendOut<<totalBytes<<qint64((outBlock.size() - sizeof(qint64)*2));

    //发送完头数据后剩余数据的大小
    bytesToWrite = totalBytes - tcpSocket->write(outBlock);

    ui->currentStatusLabel->setText("Status: start transfering...");
    outBlock.resize(0);
}

// 更新进度条，实现文件的传送
void SMainWindow::updateClientProgress(qint64 numBytes)
{
    //已经发送数据的大小
    bytesWritten += (int)numBytes;
    if(bytesToWrite > 0) //如果已经发送了数据
    {
        //每次发送loadSize大小的数据，这里设置为4KB，如果剩余的数据不足4KB，就发送剩余数据的大小
        outBlock = localFile->read(qMin(bytesToWrite, loadSize));

        //发送完一次数据后还剩余数据的大小
        bytesToWrite -= (int)tcpSocket->write(outBlock);

        //清空发送缓冲区
        outBlock.resize(0);
    }
    else
    {
        //如果没有发送任何数据，则关闭文件
        localFile->close();
    }

    //更新进度条
    ui->serverProgressBar->setMaximum(totalBytes);
    ui->serverProgressBar->setValue(bytesWritten);

    if(bytesWritten == totalBytes) //发送完毕
    {
        QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
        ui->currentStatusLabel->setText(tr("Status: transfer file %1 ...").arg(currentFileName));
        localFile->close();
        bytesWritten = 0; // clear fot next send
        bytesToWrite = 0;
        totalBytes = 0;
        //tcpSocket->close();
    }
}

// 更新进度条，实现文件的接收
void SMainWindow::updateServerProgress()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    //如果接收到的数据小于等于16个字节，那么是刚开始接收数据，我们保存为头文件信息
    if(bytesReceived <= sizeof(qint64)*2)
    {
        //接收数据总大小信息和文件名大小信息
        if((tcpSocket->bytesAvailable() >= sizeof(qint64)*2) && (fileNameSize == 0))
        {
            in >> totalBytes >> fileNameSize;
            bytesReceived += sizeof(qint64) * 2;
        }

        //接收文件名，并建立文件
        if((tcpSocket->bytesAvailable() >= fileNameSize) && (fileNameSize != 0))
        {
            in >> fileName;
            ui->currentStatusLabel->setText(tr("Accept file %1 ...").arg(fileName));
            bytesReceived += fileNameSize;

            localFile = new QFile(fileName);
            if(!localFile->open(QFile::WriteOnly))
            {
                qDebug() << "writing file error!";
                return;
            }
        }
        else
            return;

    }

    //如果接收的数据小于总数据，那么写入文件
    if(bytesReceived < totalBytes)
    {
        bytesReceived += tcpSocket->bytesAvailable();
        inBlock = tcpSocket->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }

    //更新进度条
    ui->serverProgressBar->setMaximum(totalBytes);
    ui->serverProgressBar->setValue(bytesReceived);

    //接收数据完成时
    if(bytesReceived == totalBytes)
    {
        //tcpSocket->close();
        localFile->close();
        ui->listenButton->setEnabled(true);
        ui->currentStatusLabel->setText(tr("Status: receive file %1 successfully!").arg(fileName));
        bytesReceived = 0; // clear for next receive
        totalBytes = 0;
        fileNameSize = 0;
    }
}

// 显示错误
void SMainWindow::displayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpSocket->errorString();
    tcpSocket->close();
    ui->serverProgressBar->reset();
    ui->currentStatusLabel->setText(tr("Status: connect error! open client and retry"));

    ui->listenButton->setEnabled(true); //开始使”监听“按钮可用
    ui->sendFileButton->setEnabled(false);
}

// 开始监听
void SMainWindow::on_listenButton_clicked()
{
    listening();
}

// 断开连接
void SMainWindow::on_disconectButton_clicked()
{
    tcpSocket->disconnectFromHost();

    ui->listenButton->setEnabled(true); //开始使”监听“按钮可用
    ui->sendFileButton->setEnabled(false); //开始使”发送文件“按钮不可用
}

// 发送信息
void SMainWindow::on_sendMesButton_clicked()
{
    sendMessage();
}

// 打开文件
void SMainWindow::on_openButton_clicked()
{
    openFile();
}

// 发送文件
void SMainWindow::on_sendFileButton_clicked()
{
    startTransferFile();
}

// 帮助说明
void SMainWindow::on_helpButton_clicked()
{
    helpDialog->show();
}

// 消息记录
void SMainWindow::on_mesHistoryButton_clicked()
{
    histroyMessageDialog->showHisMesDialog();
    histroyMessageDialog->show();
}

// 关闭程序
void SMainWindow::on_quitButton_clicked()
{
    this->close();
}

// 文字多了会增加滚动条并显示最后的信息
void SMainWindow::on_textBrowser_textChanged()
{
    ui->textBrowser->moveCursor(QTextCursor::End);
}

QString SMainWindow::getHostIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
        //我们使用IPv4地址
       if(address.protocol() == QAbstractSocket::IPv4Protocol)
            return address.toString();
    }
    return 0;
}

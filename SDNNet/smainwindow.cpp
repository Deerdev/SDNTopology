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

    // ��UdpReader�󶨵��㲥��ַ��6666�˿ڣ����շ�������˿ڵ���Ϣ
    UdpReader->bind(6666, QUdpSocket::ShareAddress);
    connect(UdpReader, SIGNAL(readyRead()), this, SLOT(readMessage()));


    /********** Send and receive files though TCP protocol **************/
    loadSize = 4*1024;  //����������ļ��ֳɺܶ�С�Ĳ��ֽ��з��ͣ�ÿ����Ϊ4�ֽ�
    totalBytes = 0;
    bytesWritten = 0;
    bytesToWrite = 0;
    bytesReceived = 0;
    fileNameSize = 0;

    tcpServer = new QTcpServer(this);
    tcpSocket = new QTcpSocket(this);

    //������������ʱ����newConnection()�ź�
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));

    ui->sendFileButton->setEnabled(false); //��ʼʹ�������ļ�����ť������
    ui->sendMesButton->setEnabled(true); //��ʼʹ��������Ϣ����ť����

    // ��������ȱʡֵ
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


// ����
void SMainWindow::listening()
{
    //��ʼ���ѷ����ֽ�Ϊ0
    bytesWritten = 0;
    bytesReceived = 0;
    blockSize = 0; //��ʼ����Ϊ0

    ui->listenButton->setEnabled(false);

    if(!tcpServer->listen(QHostAddress::Any, ui->portLineEdit->text().toInt()))
    {
        qDebug() << tcpServer->errorString();
        close();
        return;
    }

    ui->currentStatusLabel->setText("Status: listening...");
}

// ���Ӻ�ĶԻ�����ʾ
void SMainWindow::acceptConnection()
{
    //QMessageBox box_success;
    //box_success.information(NULL, "��ʾ", "�Ự�����ɹ�", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    tcpSocket = tcpServer->nextPendingConnection();

    // �������ݷ��ͳɹ�ʱ�����Ǹ��½�����
    connect(tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(updateClientProgress(qint64)));

    // �������ݽ��ճɹ�ʱ�����Ǹ��½�����
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(updateServerProgress()));

    // �󶨴�����
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));

    tcpServer->close();

    ui->currentStatusLabel->setText("Status: Waiting for file transfer...");
}

// ������Ϣ
void SMainWindow::sendMessage()
{
    localMessage = ui->inputTextEdit->toPlainText();

    QByteArray datagram = localMessage.toUtf8();

    //����UDP���ݱ���datagrams���ķ���
    UdpSender->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, 5566);

    // ��ʾ��Ϣ
    showMessage(true);
}

// ������Ϣ
void SMainWindow::readMessage()
{
    //ӵ�еȴ������ݱ�
    while(UdpReader->hasPendingDatagrams())
    {
        QByteArray datagram; //ӵ�ڴ�Ž��յ����ݱ�

        //��datagram�Ĵ�СΪ�ȴ���������ݱ��Ĵ�С���������ܽ��յ�����������
        datagram.resize(UdpReader->pendingDatagramSize());

        //�������ݱ��������ŵ�datagram��
        UdpReader->readDatagram(datagram.data(), datagram.size());

        //�����ݱ�������ʾ����
        serverMessage = datagram;
    }
    // ��ʾ��Ϣ
    showMessage(false);
}

// ��ʾ��Ϣ
void SMainWindow::showMessage(bool isSend)
{
    QDateTime time = QDateTime::currentDateTime();  //��ȡϵͳ���ڵ�ʱ��
    QString str = time.toString("hh:mm:ss ddd");     //������ʾ��ʽ
    QString str4file = time.toString("yyyy-MM-dd hh:mm:ss");     //������ʾ��ʽ
    blockSize = 0;

    QFont font;
    font.setPixelSize(18);
    ui->textBrowser->setFont(font);

    QTextStream stream(historyMessage);

    if (isSend)
    {
        // �ò�ͬ����ɫ��ʾ��Ϣ�����͵�ǰʱ��
        ui->textBrowser->setTextColor(QColor(0, 0, 0));
        QString entraMess = "I say: " + str;
        ui->textBrowser->append(entraMess);
        stream<<"I say: "<<str4file<<"\n"; // д�� ��ʷ��Ϣ �ļ�ʱ��Ҫ��������

        ui->textBrowser->setTextColor(QColor(0, 0, 255));
        ui->textBrowser->append(localMessage);
        ui->inputTextEdit->clear();
        ui->currentStatusLabel->setText("Status: send message successfully...");
        stream<<localMessage<<"\n";
    }
    else
    {
        // �ò�ͬ����ɫ��ʾ��Ϣ�����͵�ǰʱ��
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

// ���ļ�
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

// ʵ���ļ���С����Ϣ�ķ���
void SMainWindow::startTransferFile()
{
    localFile = new QFile(fileName);
    if(!localFile->open(QFile::ReadOnly))
    {
        qDebug() << "open file error!";
        return;
    }
    totalBytes = localFile->size(); //�ļ��ܴ�С

    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_0);

    QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);

    //����д���ܴ�С��Ϣ�ռ䣬�ļ�����С��Ϣ�ռ䣬�ļ���
    sendOut << qint64(0) << qint64(0) << currentFileName;

    //������ܴ�С���ļ�����С����Ϣ��ʵ���ļ���С���ܺ�
    totalBytes += outBlock.size();

    //����outBolock�Ŀ�ʼ����ʵ�ʵĴ�С��Ϣ��������qint64(0)�ռ�
    sendOut.device()->seek(0);
    sendOut<<totalBytes<<qint64((outBlock.size() - sizeof(qint64)*2));

    //������ͷ���ݺ�ʣ�����ݵĴ�С
    bytesToWrite = totalBytes - tcpSocket->write(outBlock);

    ui->currentStatusLabel->setText("Status: start transfering...");
    outBlock.resize(0);
}

// ���½�������ʵ���ļ��Ĵ���
void SMainWindow::updateClientProgress(qint64 numBytes)
{
    //�Ѿ��������ݵĴ�С
    bytesWritten += (int)numBytes;
    if(bytesToWrite > 0) //����Ѿ�����������
    {
        //ÿ�η���loadSize��С�����ݣ���������Ϊ4KB�����ʣ������ݲ���4KB���ͷ���ʣ�����ݵĴ�С
        outBlock = localFile->read(qMin(bytesToWrite, loadSize));

        //������һ�����ݺ�ʣ�����ݵĴ�С
        bytesToWrite -= (int)tcpSocket->write(outBlock);

        //��շ��ͻ�����
        outBlock.resize(0);
    }
    else
    {
        //���û�з����κ����ݣ���ر��ļ�
        localFile->close();
    }

    //���½�����
    ui->serverProgressBar->setMaximum(totalBytes);
    ui->serverProgressBar->setValue(bytesWritten);

    if(bytesWritten == totalBytes) //�������
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

// ���½�������ʵ���ļ��Ľ���
void SMainWindow::updateServerProgress()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    //������յ�������С�ڵ���16���ֽڣ���ô�Ǹտ�ʼ�������ݣ����Ǳ���Ϊͷ�ļ���Ϣ
    if(bytesReceived <= sizeof(qint64)*2)
    {
        //���������ܴ�С��Ϣ���ļ�����С��Ϣ
        if((tcpSocket->bytesAvailable() >= sizeof(qint64)*2) && (fileNameSize == 0))
        {
            in >> totalBytes >> fileNameSize;
            bytesReceived += sizeof(qint64) * 2;
        }

        //�����ļ������������ļ�
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

    //������յ�����С�������ݣ���ôд���ļ�
    if(bytesReceived < totalBytes)
    {
        bytesReceived += tcpSocket->bytesAvailable();
        inBlock = tcpSocket->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }

    //���½�����
    ui->serverProgressBar->setMaximum(totalBytes);
    ui->serverProgressBar->setValue(bytesReceived);

    //�����������ʱ
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

// ��ʾ����
void SMainWindow::displayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpSocket->errorString();
    tcpSocket->close();
    ui->serverProgressBar->reset();
    ui->currentStatusLabel->setText(tr("Status: connect error! open client and retry"));

    ui->listenButton->setEnabled(true); //��ʼʹ����������ť����
    ui->sendFileButton->setEnabled(false);
}

// ��ʼ����
void SMainWindow::on_listenButton_clicked()
{
    listening();
}

// �Ͽ�����
void SMainWindow::on_disconectButton_clicked()
{
    tcpSocket->disconnectFromHost();

    ui->listenButton->setEnabled(true); //��ʼʹ����������ť����
    ui->sendFileButton->setEnabled(false); //��ʼʹ�������ļ�����ť������
}

// ������Ϣ
void SMainWindow::on_sendMesButton_clicked()
{
    sendMessage();
}

// ���ļ�
void SMainWindow::on_openButton_clicked()
{
    openFile();
}

// �����ļ�
void SMainWindow::on_sendFileButton_clicked()
{
    startTransferFile();
}

// ����˵��
void SMainWindow::on_helpButton_clicked()
{
    helpDialog->show();
}

// ��Ϣ��¼
void SMainWindow::on_mesHistoryButton_clicked()
{
    histroyMessageDialog->showHisMesDialog();
    histroyMessageDialog->show();
}

// �رճ���
void SMainWindow::on_quitButton_clicked()
{
    this->close();
}

// ���ֶ��˻����ӹ���������ʾ������Ϣ
void SMainWindow::on_textBrowser_textChanged()
{
    ui->textBrowser->moveCursor(QTextCursor::End);
}

QString SMainWindow::getHostIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
        //����ʹ��IPv4��ַ
       if(address.protocol() == QAbstractSocket::IPv4Protocol)
            return address.toString();
    }
    return 0;
}

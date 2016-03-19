#ifndef SMAINWINDOW_H
#define SMAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include "ccdialog.h"
#include "hismesdialog.h"

namespace Ui {
class SMainWindow;
}

class SMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SMainWindow(QWidget *parent = 0);
    ~SMainWindow();
private slots:
    /********** Send and receive message though UCP protocol **************/
    void sendMessage(); // 发送消息
    void readMessage(); // 接受信息
    void showMessage(bool isSend); // 显示消息

    /********** Send and receive files though TCP protocol **************/
    void acceptConnection();  // 接收客户端请求
    void listening(); // 监听
    void displayError(QAbstractSocket::SocketError); //显示错误

    /************* Send file **********************/
    void openFile();  //打开文件
    void startTransferFile();  //发送文件大小等信息
    void updateClientProgress(qint64); //发送数据，更新进度条

    /************* Receive file *******************/
    void updateServerProgress(); //接收数据，更新进度条

    /************************* Button Event ****************************/
    void on_openButton_clicked();
    void on_listenButton_clicked();
    void on_disconectButton_clicked();
    void on_sendMesButton_clicked();
    void on_sendFileButton_clicked();
    void on_helpButton_clicked();
    void on_mesHistoryButton_clicked();
    void on_quitButton_clicked();
    void on_textBrowser_textChanged();

private:
    Ui::SMainWindow *ui;
    /********** Send and receive message though UCP protocol **************/
    QUdpSocket *UdpSender;
    QUdpSocket *UdpReader;
    QString localMessage; // 存放本地要发送的信息
    QString serverMessage;  //存放从服务器接收到的信息

    /********** Send and receive files though TCP protocol **************/
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;

    /************* Send file **********************/
    quint16 blockSize;  //存放接收到的信息大小
    QFile *localFile;  //要发送的文件
    qint64 totalBytes;  //数据总大小
    qint64 bytesWritten;  //已经发送数据大小
    qint64 bytesToWrite;   //剩余数据大小
    qint64 loadSize;   //每次发送数据的大小
    QString fileName;  //保存文件路径
    QByteArray outBlock;  //数据缓冲区，即存放每次要发送的数据

    /************* Receive file *******************/
    //qint64 totalBytes;  //存放总大小信息
    qint64 bytesReceived;  //已收到数据的大小
    qint64 fileNameSize;  //文件名的大小信息
    //QString fileName;   //存放文件名
    //QFile *localFile;   //本地文件
    QByteArray inBlock;   //数据缓冲区

    CCDialog *helpDialog;
    hisMesDialog *histroyMessageDialog;
    QFile *historyMessage;

    QString hostIP;
    QString getHostIP();
};

#endif // SMAINWINDOW_H

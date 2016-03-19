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
    void sendMessage(); // ������Ϣ
    void readMessage(); // ������Ϣ
    void showMessage(bool isSend); // ��ʾ��Ϣ

    /********** Send and receive files though TCP protocol **************/
    void acceptConnection();  // ���տͻ�������
    void listening(); // ����
    void displayError(QAbstractSocket::SocketError); //��ʾ����

    /************* Send file **********************/
    void openFile();  //���ļ�
    void startTransferFile();  //�����ļ���С����Ϣ
    void updateClientProgress(qint64); //�������ݣ����½�����

    /************* Receive file *******************/
    void updateServerProgress(); //�������ݣ����½�����

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
    QString localMessage; // ��ű���Ҫ���͵���Ϣ
    QString serverMessage;  //��Ŵӷ��������յ�����Ϣ

    /********** Send and receive files though TCP protocol **************/
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;

    /************* Send file **********************/
    quint16 blockSize;  //��Ž��յ�����Ϣ��С
    QFile *localFile;  //Ҫ���͵��ļ�
    qint64 totalBytes;  //�����ܴ�С
    qint64 bytesWritten;  //�Ѿ��������ݴ�С
    qint64 bytesToWrite;   //ʣ�����ݴ�С
    qint64 loadSize;   //ÿ�η������ݵĴ�С
    QString fileName;  //�����ļ�·��
    QByteArray outBlock;  //���ݻ������������ÿ��Ҫ���͵�����

    /************* Receive file *******************/
    //qint64 totalBytes;  //����ܴ�С��Ϣ
    qint64 bytesReceived;  //���յ����ݵĴ�С
    qint64 fileNameSize;  //�ļ����Ĵ�С��Ϣ
    //QString fileName;   //����ļ���
    //QFile *localFile;   //�����ļ�
    QByteArray inBlock;   //���ݻ�����

    CCDialog *helpDialog;
    hisMesDialog *histroyMessageDialog;
    QFile *historyMessage;

    QString hostIP;
    QString getHostIP();
};

#endif // SMAINWINDOW_H

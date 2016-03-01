#include <QtGui/QApplication>
#include <QTextCodec>
#include <QTranslator>
#include "NetworkSimulationPlatform.h"

int main(int argc, char *argv[])
{
    //QTranslator oTranslator;
    //oTranslator.load("qt_zh_CN.qm"); //ע��˴��ַ����ԡ���/����ͷ����ӵ��ַ����ǸղŸ��Ƶ�qm�ļ�������
	QApplication a(argc, argv);
    //a.installTranslator(&oTranslator);

    QTextCodec *codec2 = QTextCodec::codecForName("gbk");
    QTextCodec *codec = QTextCodec::codecForName("utf8");
    QTextCodec::setCodecForLocale(codec2);
    QTextCodec::setCodecForCStrings(codec2);
    QTextCodec::setCodecForTr(codec2); //����ǽ��tr("����")���ص�

    //QTextCodec::setCodecForTr(QTextCodec::codecForName("gbk"));//���ÿ�����ʾ����

    NetworkSimulationPlatform w;
    //w.showMaximized();
    w.show();
	return a.exec();
}

#include <QtGui/QApplication>
#include <QTextCodec>
#include <QTranslator>
#include "NetworkSimulationPlatform.h"

int main(int argc, char *argv[])
{
	QTranslator oTranslator;
    oTranslator.load("qt_zh_CN.qm"); //ע��˴��ַ����ԡ���/����ͷ����ӵ��ַ����ǸղŸ��Ƶ�qm�ļ�������
	QApplication a(argc, argv);
	a.installTranslator(&oTranslator);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gbk"));//���ÿ�����ʾ����

    NetworkSimulationPlatform w;
	w.showMaximized();
	return a.exec();
}

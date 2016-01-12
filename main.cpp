#include <QtGui/QApplication>
#include <QTextCodec>
#include <QTranslator>
#include "NetworkSimulationPlatform.h"

int main(int argc, char *argv[])
{
	QTranslator oTranslator;
    oTranslator.load("qt_zh_CN.qm"); //注意此处字符串以“：/”开头，后接的字符串是刚才复制的qm文件的名字
	QApplication a(argc, argv);
	a.installTranslator(&oTranslator);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gbk"));//设置可以显示中文

    NetworkSimulationPlatform w;
	w.showMaximized();
	return a.exec();
}

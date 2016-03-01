#include <QtGui/QApplication>
#include <QTextCodec>
#include <QTranslator>
#include "NetworkSimulationPlatform.h"

int main(int argc, char *argv[])
{
    //QTranslator oTranslator;
    //oTranslator.load("qt_zh_CN.qm"); //注意此处字符串以“：/”开头，后接的字符串是刚才复制的qm文件的名字
	QApplication a(argc, argv);
    //a.installTranslator(&oTranslator);

    QTextCodec *codec2 = QTextCodec::codecForName("gbk");
    QTextCodec *codec = QTextCodec::codecForName("utf8");
    QTextCodec::setCodecForLocale(codec2);
    QTextCodec::setCodecForCStrings(codec2);
    QTextCodec::setCodecForTr(codec2); //这个是解决tr("中文")的重点

    //QTextCodec::setCodecForTr(QTextCodec::codecForName("gbk"));//设置可以显示中文

    NetworkSimulationPlatform w;
    //w.showMaximized();
    w.show();
	return a.exec();
}

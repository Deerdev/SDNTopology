#include "hismesdialog.h"
#include "ui_hismesdialog.h"
#include <QFile>
#include <QTextStream>

hisMesDialog::hisMesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::hisMesDialog)
{
    ui->setupUi(this);
}

hisMesDialog::~hisMesDialog()
{
    delete ui;
}

void hisMesDialog::showHisMesDialog()
{
    QFile file("historyMessage.txt");
    if(!file.open(QFile::ReadOnly))
    {
        return;
    }

    QTextStream fileStream(&file);
    QString line;

    while ( !fileStream.atEnd() )
    {
        line = fileStream.readLine(); // 不包括“\n”的一行文本
        ui->textBrowser->append(line);
    }
    file.close();
}

void hisMesDialog::on_pushButton_clicked()
{
    hisMesDialog::close();
}

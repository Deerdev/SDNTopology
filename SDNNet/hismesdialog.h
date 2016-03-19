#ifndef HISMESDIALOG_H
#define HISMESDIALOG_H

#include <QDialog>

namespace Ui {
class hisMesDialog;
}

class hisMesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit hisMesDialog(QWidget *parent = 0);
    ~hisMesDialog();

    void showHisMesDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::hisMesDialog *ui;
};

#endif // HISMESDIALOG_H

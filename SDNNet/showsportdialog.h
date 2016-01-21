#ifndef SHOWSPORTDIALOG_H
#define SHOWSPORTDIALOG_H

#include <QDialog>

namespace Ui {
class ShowSPortDialog;
}

class ShowSPortDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShowSPortDialog(QWidget *parent = 0);
    ~ShowSPortDialog();

private:
    Ui::ShowSPortDialog *ui;
};

#endif // SHOWSPORTDIALOG_H

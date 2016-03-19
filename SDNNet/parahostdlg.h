#ifndef PARAHOSTDLG_H
#define PARAHOSTDLG_H

#include <QDialog>

namespace Ui {
class ParaHostDlg;
}

class ParaHostDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ParaHostDlg(QWidget *parent = 0);
    ~ParaHostDlg();

private:
    Ui::ParaHostDlg *ui;
};

#endif // PARAHOSTDLG_H

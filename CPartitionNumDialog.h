#ifndef CPARTITIONNUMDIALOG_H
#define CPARTITIONNUMDIALOG_H

#include <QDialog>

namespace Ui {
class CPartitionNumDialog;
}

class CPartitionNumDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CPartitionNumDialog(QWidget *parent = 0);
    ~CPartitionNumDialog();

    int getPartitonNum();
    bool getCancel();

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    bool m_cancel;
    Ui::CPartitionNumDialog *ui;
};

#endif // CPARTITIONNUMDIALOG_H

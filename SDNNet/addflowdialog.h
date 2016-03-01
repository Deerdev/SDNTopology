#ifndef ADDFLOWDIALOG_H
#define ADDFLOWDIALOG_H

#include <QDialog>

class SDNCurl;
class NetworkSimulationPlatform;

namespace Ui {
class AddFlowDialog;
}

class AddFlowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddFlowDialog(NetworkSimulationPlatform *_platform,QWidget *parent = 0);
    ~AddFlowDialog();

private slots:
    void on_submitButton_clicked();

    void on_addAgainButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::AddFlowDialog *ui;
    NetworkSimulationPlatform *m_platform;
    SDNCurl *m_curl;
    QList<QString> m_jsonVec;
};

#endif // ADDFLOWDIALOG_H

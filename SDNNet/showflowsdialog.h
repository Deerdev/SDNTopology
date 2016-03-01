#ifndef SHOWFLOWSDIALOG_H
#define SHOWFLOWSDIALOG_H

#include <QDialog>
#include "SDNCurl/sdncurl.h"

class NetworkSimulationPlatform;

namespace Ui {
class ShowFlowsDialog;
}

class ShowFlowsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShowFlowsDialog(NetworkSimulationPlatform *_platform,QWidget *parent = 0);
    ~ShowFlowsDialog();

private slots:

    void on_addFlowButton_clicked();

    void on_getFlowButton_clicked();

    void on_delFlowButton_clicked();

    void on_clearFlowButton_clicked();

private:
    void getFlowsFromController();
    void getDeviceName();
    Ui::ShowFlowsDialog *ui;
    NetworkSimulationPlatform *m_platform;
    SDNCurl *m_curl;
    int m_switchNum;
    QVector<QString> m_switchNames;
};

#endif // SHOWFLOWSDIALOG_H

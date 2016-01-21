#ifndef ADDFLOWDIALOG_H
#define ADDFLOWDIALOG_H

#include <QDialog>

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

private:
    Ui::AddFlowDialog *ui;
    NetworkSimulationPlatform *m_platform;
};

#endif // ADDFLOWDIALOG_H

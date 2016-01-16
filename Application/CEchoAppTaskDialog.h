#ifndef CECHOAPPTASKDIALOG_H
#define CECHOAPPTASKDIALOG_H

#include <QDialog>
#include <QGraphicsItem>
#include <vector>
#include <QMessageBox>
#include "../CNewSwitcher.h"
#include "../Device/CSwitcher.h"

namespace Ui {
class CEchoAppTaskDialog;
}

class CEchoAppTaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CEchoAppTaskDialog(QWidget *parent = 0);
    ~CEchoAppTaskDialog();

    QString getDesDevice();
    QString getDesIP();
    QString getTask();
    vector<TaskStruct> getTasks();
    QString getMaxPackets();
    QString getInterval();
    QString getPayload();
    QString getClientStartTime();
    QString getClientEndTime();
    QString getSeverStartTime();
    QString getSeverEndTime();

    bool getClose();
    int getSelectState();

    void init(QList<QGraphicsItem*>& _switcherInfo);
    void sendTaskFlag(int taskFlag);

private:
    bool validParameter();
    void initSelectTableWidget();

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();
    void on_addTaskButton_clicked();

    void on_selectManyCheckBox_stateChanged(int _state);
    void on_selectAllCheckBox_stateChanged(int _state);

    void on_srcDeviceComboBox_activated(int _index);
    void on_desDeviceComboBox_activated(int _index);
    void on_desIPComboBox_activated(int _index);


private:
    enum SelectTableWidget{SrcDevice,SrcIP,DesDevice,DesIP};
    Ui::CEchoAppTaskDialog *ui;

    QString m_task;

    bool m_close;
    bool m_selectMany;
    bool m_selectAll;

    QList<QGraphicsItem*> m_switcherInfo;
    VecVecQStr m_IPVec;
    VecQStr m_nodeNameVec;
    vector<TaskStruct> m_tasks;
    int m_preDesNode;
    int m_EchoOrDelayFlag;
};

#endif // CECHOAPPTASKDIALOG_H

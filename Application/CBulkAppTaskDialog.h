#ifndef CBULKAPPTASKDIALOG_H
#define CBULKAPPTASKDIALOG_H

#include <QDialog>
#include <QGraphicsItem>
#include <vector>
#include <QMessageBox>
#include "../CNewSwitcher.h"
#include "../Device/CSwitcher.h"

typedef vector<QString> VecQStr;
typedef vector<VecQStr> VecVecQStr;

namespace Ui {
class CBulkAppTaskDialog;
}

class CBulkAppTaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CBulkAppTaskDialog(QWidget *parent = 0);
    ~CBulkAppTaskDialog();

    vector<TaskStruct> getTasks();
    QString getPayload();
    QString getClientStartTime();
    QString getClientEndTime();
    QString getSeverStartTime();
    QString getSeverEndTime();
    QString getTask();
    QString getDesDevice();
    QString getDesIP();
    QString getPacketSize();
    bool getClose();
    int getSelectState();

    void init(QList<QGraphicsItem*>& _switcherInfo);

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
    void initSelectTableWidget();
    bool validParameter();

private:
    enum SelectTableWidget{SrcDevice,SrcIP,DesDevice,DesIP};
    Ui::CBulkAppTaskDialog *ui;

    bool m_close;
    bool m_selectMany;
    bool m_selectAll;
    QString m_task;
    int m_preDesNode;

    QList<QGraphicsItem*> m_switcherInfo;
    VecVecQStr m_IPVec;
    VecQStr m_nodeNameVec;
    vector<TaskStruct> m_tasks;
};

#endif // CBULKAPPTASKDIALOG_H

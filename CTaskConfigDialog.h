#ifndef CTASKCONFIGDIALOG_H
#define CTASKCONFIGDIALOG_H

#include <QDialog>
#include "Application/CEchoAppTaskDialog.h"
#include "Application/COnOffAppTaskDialog.h"
#include "Application/CBulkAppTaskDialog.h"

namespace Ui {
class CTaskConfigDialog;
}

class CTaskConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CTaskConfigDialog(QWidget *parent = 0);
    ~CTaskConfigDialog();
    void setSwichterInfo(QList<QGraphicsItem*> _switcherInfo);

private:
    void init(void);

private slots:
    void on_saveButton_clicked();
    void on_cancelButton_clicked();
    void addTask(int _index);
    void addEchoTask(int index);
    void addOnOffTask();
    void addBulkTask();

private:
    enum {EchoApplication, OnOffApplication, BulkSendApplication, DelayApplication};
    Ui::CTaskConfigDialog *ui;

    QList<QGraphicsItem*> m_switcherInfo;
    QString m_NS3Path;

    vector<vector<QString> > m_task;
    vector<vector<TaskStruct> > m_taskStructVec;
};

#endif // CTASKCONFIGDIALOG_H

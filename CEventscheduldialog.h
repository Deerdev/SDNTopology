#ifndef CEVENTSCHEDULDIALOG_H
#define CEVENTSCHEDULDIALOG_H

#include <QDialog>
#include <QtCore>
#include <QFileDialog>
#include <StructFile.h>

namespace Ui {
class CEventSchedulDialog;
}

class CEventSchedulDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CEventSchedulDialog(QWidget *parent = 0);
    ~CEventSchedulDialog();
    void setSwichterInfo(vector<CSwitcherInfo> *v_switcherInfo);
private:
    void showEventParameter(void);
private:
    Ui::CEventSchedulDialog *ui;
    vector<CSwitcherInfo>* m_VswitcherInfo;
    CSwitcherInfo* m_selectSwitcher;

private slots:
    void showPorts();
    void on_eventParameter_currentIndexChanged(int index);
    void on_okButton_clicked();
    void on_cancelButton_clicked();
};

#endif // CEVENTSCHEDULDIALOG_H

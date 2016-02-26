#ifndef CPORTDIALOG_H
#define CPORTDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "StructFile.h"
#include "ui_CPortDialog.h"
#include <set>

namespace Ui {
class CPortDialog;
}

class CPortDialog : public QDialog
{
    Q_OBJECT
    
public:
    CPortDialog(QWidget *parent = 0);
    ~CPortDialog();

    void setPortName(QString _address, QString _portName, QString _linkPortName, int s_parentID, int e_parentID);
    SNodeStructInfo& getPortInfo(){return m_PortInfo;}
    SNodeStructInfo& getLinkPortInfo(){return m_LinkPortInfo;}
    CLinkInfo& getLinkInfo(){return m_linkInfo;}
    void setPortIDandType(int _portID, int _linkPortID);
    void setVports(QVector<SNodeStructInfo> _ports){v_ports = _ports;}
    void setLinkVports(QVector<SNodeStructInfo> _link_ports){link_v_ports = _link_ports;}
    void setIPpond(set<QString> *_ipPond){m_ipPond = _ipPond;}
    void setIPPool(map<QString,int> *_ipPool){m_ipPool = _ipPool;}
    void clear(void);
private:
    Ui::CPortDialog *ui;
    SNodeStructInfo m_PortInfo;
    SNodeStructInfo m_LinkPortInfo;
    CLinkInfo       m_linkInfo;
    QVector<SNodeStructInfo> v_ports;
    QVector<SNodeStructInfo> link_v_ports;
    set<QString> *m_ipPond;
    map<QString, int>* m_ipPool;
private:
    QString calculateNetworkID(QString _IP, QString _mask);
    QString calculateNetworkBroadcast(QString _IP, QString _mask);

private slots:
    void on_CancelButton_clicked();
    void on_OkButton_clicked();
    void changeLink(int _index);
};

#endif // CPORTDIALOG_H

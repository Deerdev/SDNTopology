#ifndef CSWITHERCONFIGURE_H
#define CSWITHERCONFIGURE_H

#include<QDialog>
#include <QMessageBox>
#include <set>
#include <stack>
#include "ui_CSwitherConfigure.h"
#include "../StructFile.h"


class CSwitherConfigure : public QDialog
{
	Q_OBJECT

public:
    CSwitherConfigure(QWidget *parent = 0);
	~CSwitherConfigure();
    void setIPpond(set<QString> *_ipPond){m_ipPond = _ipPond;}
    void setIPPool(map<QString,int> *_ipPool){m_ipPool = _ipPool;}
	void SetSwitcherInfo(CSwitcherInfo &_switcherInfo);
    //void SetPortConnection(vector<pair<int,int> > &_vPortConnection){m_PortConnection = _vPortConnection;}//���ý������ӿ�
	CSwitcherInfo GetSwitcherInfo(){return m_switcherInfo;}//���ؽ���������
    vector<SNodeStructInfo> GetSwitchDeletePortsInfo(){return m_deletePortsInfo;}//���ؽ�����ɾ���˿�

private:
    QString calculateNetworkID(QString _IP, QString _mask);
    QString calculateNetworkBroadcast(QString _IP, QString _mask);
private:
	Ui::CSwitherConfigureClass ui;
	CSwitcherInfo m_switcherInfo;
    vector<SNodeStructInfo> m_deletePortsInfo;
    set<QString> *m_ipPond;
    map<QString, int>* m_ipPool;
    vector<QString> m_vDeleteInterfaceName;//���ɾ�������ӵĶ˿���
    //vector<pair<int,int> >  m_PortConnection;//ǰ��

private slots:
    void on_cancelButton_clicked();
    void on_okButton_clicked();
    void on_deleteButton_clicked();
};

#endif // CSWITHERCONFIGURE_H

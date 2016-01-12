#pragma once

#include <QDialog>
#include "ui_CNewSwitcher.h"
#include "StructFile.h"
class CNewSwitcher : public QDialog
{
	Q_OBJECT

public:
	CNewSwitcher(QWidget *parent = 0);
	~CNewSwitcher();

	CSwitcherInfo& getSwitcherInfo(){return m_switherInfo;}
	void setSwitchIDandType(int _switchID);
    void clear(void);

private:
	Ui::CNewSwitcherClass ui;
	CSwitcherInfo m_switherInfo;

private slots:
	void cancelProcess(void);
	void okProcess(void);	
};

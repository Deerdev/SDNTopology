#pragma once
#include <QLabel>

class CLabel : public QLabel
{
public:
	CLabel(QWidget *parent=0):QLabel(parent){m_name = "abc";}
	~CLabel(){}

	void setName(QString _name){m_name = _name;}

	QString getName(){return m_name;}

private:
	QString m_name;
};



#ifndef CWIZARD_H
#define CWIZARD_H

#include <QDialog>
#include "ui_wizard.h"
#include <QStringList>
#include <QTextStream>

class QComboBox;
class QDir;
class QLabel;
class QPushButton;
class QTableWidget;
class CWizard : public QDialog
{
	Q_OBJECT

public:
    CWizard(QWidget *parent = 0);
    ~CWizard();
    QString GetRoute();
    QString getFilename();
    QString GetFilePath();
    void setFileName(QString _fileName);
    void setFilePath(QString _filePath);

private:
    Ui::wizardClass ui;
    QString m_absolutePath;
    QString m_fileName;
    QString m_filePath;

private slots:
    void on_cancelButton_clicked();
    void on_okButton_clicked();
    void on_scanButton_clicked();



//private:
//void showFiles(const QDir &directory, const QStringList &files);

};

#endif // WIZARD_H

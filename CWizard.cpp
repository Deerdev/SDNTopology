#include "CWizard.h"
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

CWizard::CWizard(QWidget *parent)
	: QDialog(parent)
{
    ui.setupUi(this);
    ui.lineEdit->setText("CoreNetwork");
    setWindowTitle(tr("新建项目窗口"));
    ui.comboBox->setEditable(true);
    //ui.comboBox->setEditText(QDir::currentPath());
    QFile infile("Config");
    if(!infile.exists())
    {
        qDebug("Can not find Config file！");
    }
    if(!infile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug("Can not open Config file!");
    }
    QTextStream fileStream(&infile);
    m_filePath = fileStream.readAll();
    m_filePath.replace("\n","");
    infile.close();

    ui.comboBox->setEditText(m_filePath + "/data");
    ui.okButton->setFocus(); //设置默认焦点
    ui.okButton->setDefault(true);

    m_absolutePath = m_filePath + "/data/CoreNetwork";
    m_fileName = "CoreNetwork";
}
CWizard::~CWizard()
{

}

//确定
void CWizard::on_okButton_clicked()
{
	if (ui.lineEdit->text() == "")
	{
		QMessageBox::warning(this, tr("提示"), tr("请输入项目名！"));
		return;
	}
    m_fileName = ui.lineEdit->text();
	if (ui.comboBox->currentText() == "")
	{
		QMessageBox::warning(this, tr("提示"), tr("请输入路径！"));
		return;
	}

    m_absolutePath = ui.comboBox->currentText() + "/" + ui.lineEdit->text() ;
     emit accept();
     //close();
}
 //取消
void CWizard::on_cancelButton_clicked()
{
	 close();
}

//浏览
void CWizard::on_scanButton_clicked()
{
	QString directory = QFileDialog::getExistingDirectory(this, tr("Open File"),
		"/home");
	/*(this,
		tr("Find Files"), QDir::currentPath());*/

	
	if (!directory.isEmpty()) 
	{
		ui.comboBox->setEditText(directory);
	}
}

void CWizard::setFileName(QString _fileName)
{
    ui.lineEdit->setText(_fileName);
}

void CWizard::setFilePath(QString _filePath)
{
    ui.comboBox->setEditText(_filePath);
}

QString CWizard::GetRoute()
{
    return m_absolutePath;
}

QString CWizard::getFilename()
{
    return m_fileName;
}

QString CWizard::GetFilePath()
{
    return m_filePath;
}

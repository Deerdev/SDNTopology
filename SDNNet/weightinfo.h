#ifndef WEIGHTINFO_H
#define WEIGHTINFO_H

#include <QDialog>

namespace Ui {
class WeightInfo;
}

class WeightInfo : public QDialog
{
    Q_OBJECT

public:
    explicit WeightInfo(QWidget *parent = 0);
    ~WeightInfo();

private:
    Ui::WeightInfo *ui;
};

#endif // WEIGHTINFO_H

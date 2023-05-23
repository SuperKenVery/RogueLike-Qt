#include "diewindow.h"
#include "ui_diewindow.h"
#include <QtCore/qstring.h>

DieWindow::DieWindow(uint seonds, uint coins,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DieWindow)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    connect(ui->okButton,&QPushButton::clicked,this,[=](){
        this->close();
    });

    ui->scoreLabel->setText(QString("You survived for %1 seconds and earned %2 coins. ").arg(seonds).arg(coins));
}

DieWindow::~DieWindow()
{
    delete ui;
}

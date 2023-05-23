#include "resumefailedwindow.h"
#include "ui_resumefailedwindow.h"

ResumeFailedWindow::ResumeFailedWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResumeFailedWindow)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    connect(ui->returnButton,&QPushButton::clicked,this,[=](){
        this->close();
    });
}

ResumeFailedWindow::~ResumeFailedWindow()
{
    delete ui;
}

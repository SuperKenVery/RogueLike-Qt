#include "EnhanceOption.h"
#include <QLabel>
#include <QWidget>
#include <QString>
#include <QEvent>

EnhanceOption::EnhanceOption(enhance_action action,QString msg,  EnhancePanel *parent):
QLabel(parent),
panel(parent),
action(action){
    this->setText(msg);
    // Set font size to 36 px
    // Color white

    this->setStyleSheet("font-size:30px;color:white");
}

void EnhanceOption::enterEvent(QEnterEvent *event){
    this->setStyleSheet("font-size:30px;color:blue;");
}

void EnhanceOption::leaveEvent(QEvent *event){
    this->setStyleSheet("font-size:30px;color:white");
}

void EnhanceOption::mousePressEvent(QMouseEvent *event){
    this->action();
    this->panel->enhanced();
}


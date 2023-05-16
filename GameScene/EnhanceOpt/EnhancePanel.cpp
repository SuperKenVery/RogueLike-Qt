#include "EnhancePanel.h"
#include "EnhanceOption.h"
#include <QtWidgets/qgraphicsview.h>

EnhancePanel::EnhancePanel(vector<pair<QString,enhance_action>> enhancements,enhance_callback cb,QWidget *parent):
QWidget(parent),
cb(cb){
    auto width=this->parentWidget()->width();
    auto x=0,w=width/3;
    for(auto enhancement:enhancements){
        auto [msg, action]=enhancement;
        EnhanceOption *option=new EnhanceOption(action,msg,this);
        option->show();
        this->options.push_back(option);
        option->setGeometry(w*x++,0,w,50);
    }

    // Move to bottom
    this->move(0,this->parentWidget()->height()-this->height()-15);
}

void EnhancePanel::enhanced(){
    this->deleteLater();
}

EnhancePanel::~EnhancePanel(){
    for(auto *i: this->options){
        delete i;
    }
    this->cb();
}

#include "EnhancePanel.h"
#include "EnhanceOption.h"
#include <QtWidgets/qgraphicsview.h>

EnhancePanel::EnhancePanel(vector<pair<QString,enhance_action>> enhancements,QWidget *parent):
QWidget(parent){
    auto width=this->parentWidget()->width();
    width=800;
    auto x=0,w=width/3;
    for(auto enhancement:enhancements){
        auto [msg, action]=enhancement;
        EnhanceOption *option=new EnhanceOption(action,msg,this);
        option->show();
        this->options.push_back(option);
        option->setGeometry(w*x++,0,w,50);
    }

    // Move to bottom
    printf("Parent height: %d\n",this->parentWidget()->height());
    // this->move(0,((QGraphicsView*)this->parent())->height()-100);
    // Hardcode 600 for now
    // TODO: dynamically get height
    this->move(0,600-this->height()-15);
}

void EnhancePanel::enhanced(){
    this->deleteLater();
}

EnhancePanel::~EnhancePanel(){
    for(auto *i: this->options){
        delete i;
    }
}

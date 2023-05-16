#include "HarmAnimation.h"
#include <QtGui/qimage.h>
#include <QColorTransform>
#include <QtWidgets/qlabel.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <string>

HarmAnimation::HarmAnimation(Base *obj,uint damage):
obj(obj){
    this->label=new QLabel(to_string(damage).c_str(),obj->scene()->views()[0]);
    this->label->setStyleSheet("color:red;opacity:0.7;font-size:10px;");
    // About 200x200: It doesn't scale the text, just provides the space
    this->label->setGeometry(0,obj->boundingRect().width()/2,200,200);
    this->label->show();
}

bool HarmAnimation::tick(uint step){
    if(this->frame==this->total_frames){
        this->label->deleteLater();
        return true;
    }
    this->frame+=step;
    this->label->move(this->label->x(),this->label->y()-step);
    double opacity=0.7*(this->total_frames-this->frame)/this->total_frames;
    string style="color:red;opacity:"+to_string(opacity)+";font-size:10px;";
    this->label->setStyleSheet(style.c_str());
    return false;
}


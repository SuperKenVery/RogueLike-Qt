#include "HarmAnimation.h"
#include <QtGui/qimage.h>
#include <QColorTransform>
#include <QtGui/qpalette.h>
#include <QtWidgets/qlabel.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <string>

/* We're hooking to the paint method rather than using QLabel
 * QLabel seems to be using premultiplied alpa and we cannot turn it off,
 * and that makes alpha completely disappear for red colors
*/

HarmAnimation::HarmAnimation(Base *obj,uint damage):
obj(obj){
    this->text=QString("-%1").arg(damage);
}

bool HarmAnimation::tick(uint step){
    if(this->frame==this->total_frames){
        return true;
    }
    this->frame+=step;

    this->opacity=1.0-(double(this->frame)/this->total_frames);
    this->hOffset+=((double)this->frame/this->total_frames)*30;
    return false;
}

void HarmAnimation::paintHook(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->drawText(0,-this->obj->boundingRect().height()/2-this->hOffset,this->text);
}


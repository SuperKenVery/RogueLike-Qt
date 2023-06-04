#include "HarmAnimation.h"
#include "Animation.h"
#include <QtCore/qrect.h>
#include <QtGui/qfont.h>
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

HarmAnimation::HarmAnimation(Base *obj,uint damage,bool debug):
obj(obj),
debug(debug),
Animation(obj){
    this->text=QString("-%1").arg(damage);
}

QRectF HarmAnimation::boundingRect() const{
    return QRectF(-this->obj->boundingRect().width()/2,-this->obj->boundingRect().height()/2-this->height,this->obj->boundingRect().width(),this->obj->boundingRect().height()+this->height);
}

void HarmAnimation::advance(int step){
    if(this->frame>=this->total_frames){
        delete this;
        return;
    }
    this->frame+=step;

    double frame=this->frame;

    this->opacity=1.0-(frame/this->total_frames);
    this->hOffset=(frame/this->total_frames)*this->height;
    this->update();
}

void HarmAnimation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setPen(QColor(255,0,0,255*this->opacity));
    painter->drawText(0,-this->obj->boundingRect().height()/2-this->hOffset,this->text);
}


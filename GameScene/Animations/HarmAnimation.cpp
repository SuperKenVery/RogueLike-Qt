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
    this->label=new QLabel(to_string(damage).c_str(),obj->scene()->views()[0]);
    this->label->setStyleSheet("color:red;font-size:30px;");
    auto p=this->label->palette();
    auto c=p.color(QPalette::Text);
    c.setAlpha(128);
    p.setColor(QPalette::Text,c);
    this->label->setPalette(p);

    // From https://stackoverflow.com/questions/45617752/calculate-the-size-of-rendered-qlabel
    this->label->adjustSize();
    this->label->resize(this->label->sizeHint());
    auto size=this->label->size();

    // Should have mapped to view, but that's not possible (no such function) and the scene
    // actually is at the same position as the view
    auto sceneRect=obj->sceneBoundingRect();
    sceneRect.setRect(0, 0, 800, 600);
    auto scenePos=obj->mapToScene(
        -double(size.width())/2,
        -size.height()-obj->boundingRect().height()/2
    );
    auto pos=scenePos+QPointF(sceneRect.width()/2,sceneRect.height()/2);

    this->label->setGeometry(pos.x(),pos.y(),size.width(),size.height());
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
    auto p=this->label->palette();
    auto c=p.color(QPalette::Text);
    c.setAlpha(128);
    p.setColor(QPalette::Text,c);
    this->label->setPalette(p);

    return false;
}


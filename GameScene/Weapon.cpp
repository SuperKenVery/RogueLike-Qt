#include "Weapon.h"
#include "GameScene.h"
#include "Base.h"
#include <QtCore/qpoint.h>
#include <QtCore/qrect.h>
#include <QtGui/qcolor.h>
#include <QtGui/qpainterpath.h>
#include <QtWidgets/qgraphicsitem.h>

/*
Construct a weapon

Example Config:
{
    "attack":10,
    "distance":50
}
*/
Weapon::Weapon(json config,attackable_list *attackables,QGraphicsItem* parent):
QGraphicsItem(parent),
attack(config["attack"]),
range(config["range"]),
attackables(attackables){

}

QRectF Weapon::boundingRect() const{
    int r=this->range;
    return QRectF(
        -r,
        -r,
        2*r,
        2*r
    );
}

void Weapon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    int x=this->frame;
    if(x>=this->total_frames) return;

    double max_height=0.25*this->total_frames*this->total_frames;
    double ratio=-x*(x-this->total_frames)/max_height;
    assert(ratio<=1);
    int r=ratio*this->range;

    double opacity=ratio*1.0*0.1;

    painter->setPen(QColor(0,0,255));
    painter->setBrush(QColor(32,128,0,opacity*255));
    painter->drawEllipse(
        QPoint(0,0),
        r,
        r
    );
    this->frame++;
}

void Weapon::advance(int step){
    // DBGPRINT("Advance count=%d step=%d\n",count,step);
    this->count+=step;
    if(this->count>100){
        auto attackables_copy=*this->attackables;
        for(auto o: attackables_copy){
            auto x1=o->pos().x(),y1=o->pos().y(),x2=this->parentItem()->pos().x(),y2=this->parentItem()->pos().y();
            auto dist=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
            if(dist<=this->range){
                auto real_damage=o->harm(this->attack);
                this->hp+=real_damage;
            }
        }
        this->count-=100;
        this->frame=0;
    }

    if(this->frame<this->total_frames){
        this->update();
    }

}

json Weapon::dumpState(){
    return json(
        {
            {"hp",this->hp},
            {"attack",this->attack},
            {"range",this->range}
        }
    );
}

void Weapon::resumeState(json storage){
    this->hp=storage["hp"];
    this->attack=storage["attack"];
    this->range=storage["range"];
}


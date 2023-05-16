#include "Weapon.h"
#include "GameScene.h"
#include "Base.h"
#include <QtCore/qrect.h>
#include <QtGui/qpainterpath.h>
#include <QtWidgets/qgraphicsitem.h>

#define DBGPRINT(...) if(this->debug)printf(__VA_ARGS__)

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
    return this->parentItem()->boundingRect();
}

void Weapon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    // Do nothing
}

void Weapon::advance(int step){
    static unsigned int count=0;
    count+=step;
    if(count>100){
        DBGPRINT("Player weapon going to attack...\n");
        for(auto o: *this->attackables){
            auto x1=o->pos().x(),y1=o->pos().y(),x2=this->pos().x(),y2=this->pos().y();
            auto dist=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
            if(dist<=this->range){
                o->harm(this->attack);
                this->hp+=this->attack;
                DBGPRINT("Player weapon attacked! HP += %d to %d\n",this->attack,this->hp);
            }
        }
        count-=100;
    }

}


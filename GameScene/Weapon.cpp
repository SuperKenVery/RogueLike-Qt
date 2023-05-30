#include "Weapon.h"
#include "GameScene.h"
#include "Base.h"
#include <QtCore/qrect.h>
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
    return this->parentItem()->boundingRect();
}

void Weapon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    // Do nothing
}

void Weapon::advance(int step){
    // DBGPRINT("Advance count=%d step=%d\n",count,step);
    this->count+=step;
    if(this->count>100){
        DBGPRINT("Player weapon going to attack... size of attackables is %lu\n",this->attackables->size())
        printf("Going to attack... size of attackables is %lu\n",this->attackables->size());
        auto attackables_copy=*this->attackables;
        for(auto o: attackables_copy){
            auto x1=o->pos().x(),y1=o->pos().y(),x2=this->parentItem()->pos().x(),y2=this->parentItem()->pos().y();
            auto dist=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
            if(dist<=this->range){
                printf("Attacking obj at %p, dbg=%d\n",o,this->debug);
                auto real_damage=o->harm(this->attack);
                this->hp+=real_damage;
                DBGPRINT("Player weapon attacked!\n")
            }
        }
        printf("Done attacking\n");
        this->count-=100;
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


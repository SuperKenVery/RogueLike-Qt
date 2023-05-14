#include "Weapon.h"
#include "GameScene.h"
#include "Base.h"
#include <QtCore/qrect.h>
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
    static unsigned int count=0;
    count+=step;
    if(count>100){
        for(auto o: *this->attackables){
            if(o->collidesWithItem(this)){
                printf("Fucking an item!\n");
                o->harm(this->attack);
            }
        }
        count-=100;
    }

}

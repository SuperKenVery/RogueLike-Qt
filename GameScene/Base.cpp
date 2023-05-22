#include "Base.h"
#include "Animations/HarmAnimation.h"
#include "GameScene.h"
#include "Map.h"
#include <QtCore/qrect.h>

Base::Base(uint life,uint size,QImage image,QGraphicsItem* parent):
QGraphicsItem(parent),
life(life),
size(size){
    if(image.isNull()) throw "Failed to load image";
    else this->image=image;
}

uint Base::harm(uint damage){
    auto real_damage=0;
    if(damage>this->life){
        real_damage=this->life;
        this->life=0;
        this->die();
    }else{
        this->life-=damage;
        real_damage=damage;
    }
    this->animations.push_back(new HarmAnimation(
        this,real_damage
    ));
    return real_damage;
}

QRectF Base::boundingRect() const{
    return QRectF(-float(this->size)/2,-float(this->size)/2,this->size,this->size);
}

void Base::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    for(auto a:this->animations) a->paintHook(painter, option, widget);
    painter->drawImage(this->boundingRect(),this->image);
}

void Base::advance(int step){
    if(step==0) return;

    auto animations_copy=this->animations;
    for(auto a:animations_copy){
        auto finished=a->tick(step);
        if(finished)
            for(auto t=this->animations.begin();t!=this->animations.end();t++)
                if(*t==a){
                    printf("An animation has finished\n");
                    this->animations.erase(t);
                    break;
                }
    }
}

bool Base::valid(){
    // Bound check
    if(this->scene()->sceneRect().contains(mapRectToParent(this->boundingRect()))==false)
        return false;

    // Collision detection
    auto scene=static_cast<GameScene*>(this->scene());
    auto walls=scene->walls;
    for(auto wall:walls)
        if(wall->collidesWithItem(this))
            return false;

    return true;
}

#include "Base.h"
#include "Animations/HarmAnimation.h"
#include "GameScene.h"
#include "Map.h"
#include <QtCore/qrect.h>
#include <QtGui/qcolor.h>

Base::Base(uint life,uint size,QImage image,QGraphicsItem* parent,bool debug):
QGraphicsItem(parent),
life(life),
total_life(life),
size(size),
debug(debug){
    if(image.isNull()) throw "Failed to load image";
    else this->image=image;
}

uint Base::harm(uint damage){
    auto real_damage=0;
    if(damage>this->life){
        real_damage=this->life;
        this->life=0;
        DBGPRINT("Player die!\n")
        this->die();
        return real_damage;
    }else{
        this->life-=damage;
        real_damage=damage;

        DBGPRINT("Player life -%d to %d\n",real_damage,this->life)
    }

    this->animations.push_back(new HarmAnimation(
        this,real_damage,this->debug
    ));

    return real_damage;
}

QRectF Base::boundingRect() const{
    return QRectF(-float(this->size)/2,-float(this->size)/2,this->size,this->size);
}

void Base::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->drawImage(this->boundingRect(),this->image);

    // Life bar
    const auto height=5;
    painter->fillRect(
        QRect(-(this->size/2),this->size/2-height,this->size,height),
        QColor(100,100,100)
    );
    double life_ratio=((double)this->life)/this->total_life;
    painter->fillRect(
        QRect(-(this->size/2),this->size/2-height,this->size*life_ratio,height),
        QColor(0,255,0)
    );
}

void Base::advance(int step){
    if(step==0) return;

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

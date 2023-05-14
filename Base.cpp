#include "Base.h"
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

void Base::harm(uint damage){
    if(damage>this->life){
        this->life=0;
        this->die();
    }else{
        this->life-=damage;
    }
}

QRectF Base::boundingRect() const{
    return QRectF(-float(this->size)/2,-float(this->size)/2,this->size,this->size);
}

void Base::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->drawImage(this->boundingRect(),this->image);
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

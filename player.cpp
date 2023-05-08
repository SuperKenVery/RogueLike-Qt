#include "player.h"
#include "GameScene.h"
#include <QPainter>
#include<QKeyEvent>
#include <QtCore/qrect.h>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QtGui/qpainterpath.h>
#include <QtWidgets/qgraphicsitem.h>
#include <QtWidgets/qwidget.h>
#include <cstdio>
#include <QGraphicsWidget>

Player::Player(QImage image):
    image(image)
{
    this->setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable);

}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QRect pos(-this->image.width()/2,-this->image.height()/2,this->image.width()/2,this->image.height()/2);
    painter->drawImage(pos,this->image);
}

void Player::keyPressEvent(QKeyEvent *event){
    printf("Key pressed\n");
    switch(event->key()){
    case Qt::Key_A:
        this->direction.setX(-1);
        break;
    case Qt::Key_D:
        this->direction.setX(1);
        break;
    case Qt::Key_S:
        this->direction.setY(1);
        break;
    case Qt::Key_W:
        this->direction.setY(-1);
        break;
    default:
        QGraphicsItem::keyPressEvent(event);
    }
}

void Player::keyReleaseEvent(QKeyEvent *event){
    switch(event->key()){
    case Qt::Key_A:
        this->direction.setX(0);
        break;
    case Qt::Key_D:
        this->direction.setX(0);
        break;
    case Qt::Key_S:
        this->direction.setY(0);
        break;
    case Qt::Key_W:
        this->direction.setY(0);
        break;
    }
}

void Player::advance(int step){
    // Basic check
    if(this->direction.length()==0) return;
    if(step==0) return;

    for(int i=0;i<this->speed;i++){
        // Advance
        auto oldPos=this->pos();
        setPos(mapToParent(this->direction.x()/this->direction.length(),this->direction.y()/this->direction.length()));

        if(this->valid()==false){
            setPos(oldPos);
            break;
        }
    }

}

bool Player::valid(){
    // Bound check
    if(this->scene()->sceneRect().contains(this->boundingRect())==false){
        auto s=this->scene()->sceneRect();
        printf("Out of bound. Scene at (%f,%f,%f,%f), pos at (%f,%f,%f,%f)\n",s.x(),s.y(),s.width(),s.height(),this->boundingRect().x(),this->boundingRect().y(),this->boundingRect().width(),this->boundingRect().height());
        return false;
    }

    // Collision check
    auto scene=static_cast<GameScene*>(this->scene());
    auto walls=scene->walls;

    for(auto wall:walls)
        if(wall->collidesWithItem(this))
            return false;

    return true;
}

QRectF Player::boundingRect() const{
    return QRectF(-float(this->image.width())/2,
                    -float(this->image.height())/2,
                    float(this->image.width())/2,
                    float(this->image.height())/2);
}

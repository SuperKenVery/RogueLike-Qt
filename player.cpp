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

/*
Construct a player

Config Example:
{
    "image":"images/player.png",
    "life":120,
    "speed":5,
    "size":50,
    "weapon":{
        "attack":5,
        "distance":250
    }
}
*/
Player::Player(json config,GameScene *scene):Base(config["life"],config["size"],QImage(QString::fromStdString(config["image"]))){
    this->speed=config["speed"];

    this->weapon=new Weapon(
        config["weapon"],
        this
    );

    this->setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable);

}

void Player::setAttackables(vector<Base *> attackables){
    this->weapon->setAttackables(attackables);
}

void Player::keyPressEvent(QKeyEvent *event){
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
        setPos(mapToParent(this->direction.toPointF()/this->direction.length()));

        if(this->valid()==false){
            setPos(oldPos);
            break;
        }
    }

}

void Player::die(){
    // TODO: die
}


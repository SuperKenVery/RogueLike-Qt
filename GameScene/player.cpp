#include "Player.h"
#include "Base.h"
#include "GameScene.h"
#include "EnhanceOpt/EnhancePanel.h"
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
#include <random>

/*
Construct a player

Config Example:
{
    "image":"images/player.png",
    "life":120,
    "speed":5,
    "size":50,
    "upgrade_hp": [100, 200, 300, 400, 500],
    "weapon":{
        "attack":5,
        "range":250
    }
}
*/


Player::Player(json config,attackable_list *attackables,GameScene *scene):
Base(config["life"],config["size"],QImage(QString::fromStdString(config["image"]))){
    this->speed=config["speed"];
    this->enhance_hp=vector<uint>(config["enhance_hp"]);
    this->next_enhance_hp=this->enhance_hp.begin();

    this->weapon=new Weapon(
        config["weapon"],
        attackables,
        this
    );
    this->weapon->debug=true;

    this->setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable);

}

Player::~Player(){
    delete this->weapon;
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
    Base::advance(step);

    // Basic check
    if(step==0) return;

    // Move
    if(this->direction.length()!=0)
        for(int i=0;i<this->speed;i++){
            // Advance
            auto oldPos=this->pos();
            setPos(mapToParent(this->direction.toPointF()/this->direction.length()));

            if(this->valid()==false){
                setPos(oldPos);
                break;
            }
        }

    // Enhance
    if(this->weapon->hp>=*this->next_enhance_hp){
        printf("HP is %d, enhancing...\n",this->weapon->hp);
        this->weapon->hp-=*this->next_enhance_hp;
        (*this->next_enhance_hp)++;
        if(this->next_enhance_hp==this->enhance_hp.end()){
            this->next_enhance_hp--;
        }
        this->enhance();
    }else{
        // printf("HP is %d, not enough to enhance\n",this->weapon->hp);
    }
}

void Player::enhance(){
    static uint enhance_count=0;
    static vector<pair<QString,enhance_action>> enhance_options={
        {
            "Speed 1.5x",
            [&](){
                this->speed*=1.5;
            }
        },
        {
            "Weapon Range 2x",
            [&](){
                this->weapon->range*=2;
            }
        },
        {
            "Weapon Attack 2x",
            [&](){
                this->weapon->attack*=2;
            }
        },
        {
            "Speed 2x",
            [&](){
                this->speed*=2;
            }
        },
        {
            "Weapon Attack 3x",
            [&](){
                this->weapon->attack*=3;
            }
        }
    };

    static function<void()> do_enhance=[=](){
        shuffle(enhance_options.begin(),enhance_options.end(),default_random_engine(time(NULL)));

        auto panel=new EnhancePanel(
            {enhance_options[0],enhance_options[1],enhance_options[2]},
            [&](){
                enhance_count--;
                if(enhance_count>0) do_enhance();
            },
            this->scene()->views()[0]
        );
        panel->show();
    };

    enhance_count++;
    if(enhance_count==1)
        do_enhance();
}

void Player::die(){
    // TODO: die
}


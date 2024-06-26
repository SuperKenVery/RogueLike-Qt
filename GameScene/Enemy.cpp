#include "Enemy.h"
#include <QtCore/qpoint.h>
#include <QtCore/qrect.h>
#include <QtGui/qvectornd.h>
#include <cstdio>
#include <cstdlib>
#include <unordered_map>
#include <random>
#include "Drops.h"
#include "GameScene.h"
#include "Weapon.h"
#include "Player.h"



/*
Construct an Enemy

Example Config:
{
    "image":"images/enemy.png",
    "life":30,
    "move_strategy":"towards_player",
    "speed":5,
    "size":50,
    "weapon":{
        "attack":10,
        "distance":50
    }
}
*/
Enemy::Enemy(json config,attackable_list *attackables,GameScene *scene):
Base(config["life"],config["size"],QImage(QString::fromStdString(config["image"]))),
config(config){
    this->speed=config["speed"];
    this->moveStrategyName=config["move_strategy"];
    this->strategy=move_strategies[config["move_strategy"]];
    if(this->strategy==nullptr) throw "Invalid move strategy";

    this->weapon=new Weapon(
        config["weapon"],
        attackables,
        this
    );
    auto edge=rand()%4;
    /*
        0-top edge
        1-right edge
        2-bottom edge
        3-left edge
    */
    auto xw=scene->width()-this->size, xh=scene->height()-this->size;
    switch(edge){
    case 0:
        this->setPos(
            rand()%int(xw)-xw/2,
            -xh/2
        );
        break;
    case 1:
        this->setPos(
            xw/2,
            rand()%int(xh)-xh/2
        );
        break;
    case 2:
        this->setPos(
            rand()%int(xw)-xw/2,
            xh/2
        );
        break;
    case 3:
        this->setPos(
            -xw/2,
            rand()%int(xh)-xh/2
        );
        break;
    default:
        throw "Invalid edge";
    }
}


void Enemy::advance(int step){
    Base::advance(step);

    for(auto i=0;i<step;i++){
        auto old_pos=this->pos();
        this->strategy(this);

        if(this->valid()==false){
            setPos(old_pos);
            break;
        }
    }

    auto player=static_cast<GameScene*>(this->scene())->player;

}

void Enemy::die(){
    // Create a Dropping
    int dropOrNot=rand()%3;
    if(dropOrNot==0){
        auto dropIndex=rand()%this->config["drops"].size();
        auto drop=new Drop(
            this->config["drops"][dropIndex],
            this->pos()
        );
        auto scene=static_cast<GameScene*>(this->scene());
        scene->addItem(drop);
        scene->drops.push_back({dropIndex,drop});
    }

    // Cleanup
    // static_cast<GameScene*>(this->scene())->deleteEnemy(this);
    auto scene=static_cast<GameScene*>(this->scene());
    scene->removeItem(this);
    for(auto it=scene->enemies.begin();it!=scene->enemies.end();it++){
        auto [index, obj]=*it;
        if(obj==this){
            scene->enemies.erase(it);
            break;
        }
    }
    delete this;
}

unordered_map<string, move_strategy> Enemy::move_strategies={
    {"stupid",[](Enemy* enemy){
        // Move towards a fixed direction, and change when met obstacle
        auto getNewDirection=[](){
            auto direction=QVector2D(float(rand())/RAND_MAX-0.5,float(rand())/RAND_MAX-0.5);

            // Should rarely happen
            if(direction.length()==0)
                direction=QVector2D(1,0);

            return direction;
        };

        if(enemy->direction.length()==0){
            // Uninitialized
            srand(time(0));
            enemy->direction=getNewDirection();
        }
        auto try_left=4;
        while(try_left--){
            auto oldPos=enemy->pos();
            enemy->setPos(enemy->mapToParent(enemy->direction.toPointF()/enemy->direction.length()));
            if(!enemy->valid()){
                // Change direction
                enemy->setPos(oldPos);
                enemy->direction=getNewDirection();
            }else{
                break;
            }
        }
    }},
    {"towards_player",[](Enemy* enemy){
        // Always move towards the player

        auto scene=static_cast<GameScene*>(enemy->scene());
        auto player=scene->player;
        auto pos=player->mapToItem(enemy,QPointF(0,0));

        enemy->direction=QVector2D(pos);
        enemy->setPos(enemy->mapToParent(enemy->direction.toPointF()/enemy->direction.length()));
    }}
};

json Enemy::dumpState(){
    return json(
        {
            {"weapon",this->weapon->dumpState()},
            {
                "pos",
                {
                    this->pos().x(),
                    this->pos().y()
                }
            },
            {"speed",this->speed},
            {
                "direction",
                {
                    {"x",this->direction.x()},
                    {"y",this->direction.y()}
                }
            },
            {"life",this->life},
            {"move_strategy",this->moveStrategyName},
        }
    );
}

void Enemy::resumeState(json storage){
    this->weapon->resumeState(storage["weapon"]);
    this->setPos(storage["pos"][0],storage["pos"][1]);
    this->speed=storage["speed"];
    this->direction=QVector2D(storage["direction"]["x"],storage["direction"]["y"]);
    this->life=storage["life"];
    this->moveStrategyName=storage["move_strategy"];
    this->strategy=move_strategies[storage["move_strategy"]];
}

Enemy::~Enemy(){
    delete this->weapon;
}


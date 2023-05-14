#include "Enemy.h"
#include <QtCore/qpoint.h>
#include <QtCore/qrect.h>
#include <QtGui/qvectornd.h>
#include <cstdlib>
#include <unordered_map>
#include <random>
#include "GameScene.h"
#include "player.h"

unordered_map<string, move_strategy> Enemy::move_strategies={
    {"stupid",[](Enemy* enemy){
        // Move towards a fixed direction, and change when met obstacle
        auto getNewDirection=[](){
            auto direction=QVector2D(float(rand())/RAND_MAX,float(rand())/RAND_MAX);

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
Enemy::Enemy(json config,GameScene *scene):
Base(config["life"],config["size"],QImage(QString::fromStdString(config["image"]))){
    this->speed=config["speed"];
    this->strategy=move_strategies[config["move_strategy"]];
    this->weapon=new Weapon(
        config["weapon"],
        this
    );

    if(this->strategy==nullptr) throw "Invalid move strategy";
}

void Enemy::setAttackables(vector<Base *> attackables){
    this->weapon->setAttackables(attackables);
}


void Enemy::advance(int step){
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
    // TODO: die
}



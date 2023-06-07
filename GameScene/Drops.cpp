#include "Drops.h"
#include <QtCore/qrect.h>
#include <QtCore/qstring.h>
#include <QtWidgets/qgraphicsitem.h>
#include <functional>
#include <map>
#include <string>
#include "Base.h"
#include "GameScene.h"
#include "Player.h"

/* Construct a dropping item
 *
 * @param config The configuration of the item
 * Example config:
 * {
 *     "image":"images/drops/vegetable.png",
 *     "action":"life +10",
 *     "size":50
 * }
 * @param pos The position of the item
 * @param pickAction The action to be executed when the item is picked
 * @param parent The parent item
*/
Drop::Drop(json config, QPointF pos,QGraphicsItem* parent):
QGraphicsItem(parent),
image(QString::fromStdString(config["image"])),
size(config["size"]){
    if(this->image.isNull()) throw "Failed to load image";
    this->setPos(pos);

    std::string actionName=config["action"];
    this->actionName=actionName;
    this->action=this->actions[actionName];
}

QRectF Drop::boundingRect() const{
    int s=this->size;
    return QRectF(
        -s,
        -s,
        2*s,
        2*s
    );
}

void Drop::advance(int step){
    auto scene=static_cast<GameScene*>(this->scene());
    auto players=scene->players;

    // There should only be one player though
    bool touched=false;
    for(auto [index,player]: players){
        if(player->collidesWithItem(this)){
            this->action((Player*)player);
            touched=true;
        }
    }

    if(touched){
        auto scene=static_cast<GameScene*>(this->scene());
        scene->removeItem(this);
        for(auto it=scene->drops.begin();it!=scene->drops.end();it++){
            auto [index,drop]=*it;
            if(drop==this){
                scene->drops.erase(it);
                break;
            }
        }

        scene->removeItem(this);
        delete this;
    }
}

void Drop::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->drawImage(this->boundingRect(),this->image);
}

json Drop::dumpState(){
    return json({
        {
            "pos",
            {
                {"x",this->x()},
                {"y",this->y()}
            }
        },
        {
            "action",this->actionName
        }
    });
}

void Drop::resumeState(json storage){
    this->setPos(storage["pos"]["x"],storage["pos"]["y"]);
    this->actionName=storage["action"];
    this->action=this->actions[this->actionName];
}

std::map<std::string,std::function<void(Player*)> > Drop::actions={
    {
        "attack +10",
        [](Player* player){
            player->weapon->attack+=10;
        }
    },
    {
        "life +10",
        [](Player* player){
            player->life+=10;
            if(player->life>player->total_life) player->life=player->total_life;
        }
    },
    {
        "speed +10",
        [](Player* player){
            player->speed+=10;
        }
    }
};


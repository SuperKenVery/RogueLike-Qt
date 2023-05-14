#include "Map.h"
#include "GameScene.h"
#include <QtCore/qpoint.h>
#include <QtCore/qrect.h>
#include <QtGui/qimage.h>
#include <QtWidgets/qgraphicsitem.h>
#include <QtWidgets/qgraphicsscene.h>
#include <vector>

/*
Construct a Map


Example Config:
[
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,1,0,0,0,0],
    [0,0,0,0,0,1,0,0,0,0],
    [0,0,0,1,1,1,0,0,0,0],
    [0,0,0,1,0,0,0,0,0,0],
    [0,0,0,1,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0]
]
Where 0 is ground, 1 is wall.
*/
Map::Map(json config, GameScene *scene):
blocks(config.size(),vector<MapBlock*>(config[0].size())),
matrix(config.size(),vector<block_type>(config[0].size())){
    this->setPos(0,0);

    auto scenerect=(static_cast<QGraphicsScene*>(scene))->sceneRect();
    auto block_width=scenerect.width()/matrix[0].size();
    auto block_height=scenerect.height()/matrix.size();

    for(auto y=0;y<config.size();y++){
        auto line=config[y];
        for(auto x=0;x<line.size();x++){
            auto type=line[x];
            if(type!=ground&&type!=wall){
                printf("Invalid block type at line %d, column %d\n",y+1,x+1);
                exit(1);
            }
            matrix[y][x]=type;
            this->blocks[y][x]=new MapBlock(
                this,
                QRect(scenerect.left()+x*block_width,scenerect.top()+y*block_height,block_width,block_height),
                type);
            if(type==block_type::wall)
                this->walls.push_back(this->blocks[y][x]);
        }
    }
    this->images[block_type::ground]=QImage("images/ground.jpeg");
    this->images[block_type::wall]=QImage("images/wall.jpeg");
}

QRectF Map::boundingRect() const{
    return this->scene()->sceneRect();
}

void Map::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    // Do nothing
}


MapBlock::MapBlock(QGraphicsItem *parent,QRect position,block_type type): QGraphicsItem(parent){
    this->position=position;
    this->type=type;
}

QRectF MapBlock::boundingRect() const {
    return this->position;
}

void MapBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    auto parent=static_cast<Map*>(this->parentItem());
    // TODO: cache the image lookup result

    // The item's position is actually (0,0). In this way we draw into the correct position.
    painter->drawImage(this->position,parent->images[this->type]);
}

QPointF Map::getFreePos(){
    for(auto line: this->blocks)
        for(auto i: line)
            if(i->type==ground)
                return i->position.center();

}

#include "Map.h"
#include <QtCore/qrect.h>
#include <QtGui/qimage.h>
#include <vector>

Map::Map(vector<vector<block_type>> matrix){
    this->blocks=vector<vector<MapBlock*>>(matrix.size(), vector<MapBlock*>(matrix[0].size()));

    auto scenerect=this->scene()->sceneRect();
    auto block_width=scenerect.width()/matrix[0].size();
    auto block_height=scenerect.height()/matrix.size();

    for(auto y=0;y<matrix.size();y++){
        auto line=matrix[y];
        for(auto x=0;x<line.size();x++){
            auto type=line[x];

            auto block=new MapBlock(this,QRect(x*block_width,y*block_height,block_width,block_height),type);
            this->blocks[y][x]=block;
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
    auto parent=dynamic_cast<Map*>(this->parentItem());
    painter->drawImage(this->position,parent->images[this->type]);
}
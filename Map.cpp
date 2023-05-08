#include "Map.h"
#include <QtCore/qrect.h>
#include <QtGui/qimage.h>
#include <QtWidgets/qgraphicsitem.h>
#include <QtWidgets/qgraphicsscene.h>
#include <vector>

Map::Map(vector<vector<block_type>> matrix, QGraphicsScene *scene){
    this->setPos(0,0);
    this->blocks=vector<vector<MapBlock*>>(matrix.size(), vector<MapBlock*>(matrix[0].size()));

    auto scenerect=scene->sceneRect();
    auto block_width=scenerect.width()/matrix[0].size();
    auto block_height=scenerect.height()/matrix.size();

    for(auto y=0;y<matrix.size();y++){
        auto line=matrix[y];
        for(auto x=0;x<line.size();x++){
            auto type=line[x];

            auto block=new MapBlock(this,QRect(scenerect.left()+x*block_width,scenerect.top()+y*block_height,block_width,block_height),type);
            this->blocks[y][x]=block;

            if(type==block_type::wall)
                this->walls.push_back(block);
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
    painter->drawImage(this->position,parent->images[this->type]);
}


#include "GameScene.h"
#include "Map.h"
#include <QImage>
#include <QtCore/qlogging.h>
#include <QtGui/qevent.h>
#include <QtGui/qimage.h>
#include <QtWidgets/qgraphicsscene.h>
#include <QtWidgets/qwidget.h>
#include <cstddef>
#include <fstream>

using namespace std;


GameScene::GameScene(QWidget *parent):
QGraphicsScene(parent){
    // Load configuration file
    ifstream config_file("config.json");
    config_file >> this->config;

    // Set size
    this->setSceneRect(-400,-300,800,600);

    // Create map
    auto map_config=config["map"];
    vector<vector<block_type>> matrix(map_config.size(),vector<block_type>(map_config[0].size()));
    for(auto y=0;y<map_config.size();y++){
        auto line=map_config[y];
        for(auto x=0;x<line.size();x++){
            auto type=line[x];
            if(type!=ground&&type!=wall){
                printf("Invalid block type at line %d, column %d\n",y+1,x+1);
                exit(1);
            }
            matrix[y][x]=type;
        }
    }
    this->map=new Map(matrix,this);
    this->walls=this->map->walls;
    this->addItem(this->map);

    // Create a player
    this->player=new Player(QImage("images/player.png"));
    this->player->setPos(0,0);
    this->player->focusItem();
    // this->setFocusItem(this->player);
    this->addItem(this->player);

}


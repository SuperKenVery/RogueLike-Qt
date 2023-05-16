#include "GameScene.h"
#include "Enemy.h"
#include "Player.h"
#include "EnhanceOpt/EnhancePanel.h"
#include "Map.h"
#include <QImage>
#include <QtCore/qlogging.h>
#include <QtCore/qrect.h>
#include <QtCore/qtimer.h>
#include <QtGui/qevent.h>
#include <QtGui/qimage.h>
#include <QtWidgets/qgraphicsscene.h>
#include <QtWidgets/qwidget.h>
#include <cstddef>
#include <fstream>
#include <vector>

using namespace std;


GameScene::GameScene(QWidget *parent):
QGraphicsScene(parent),
createEnemyTimer(this){
    // Load configuration file
    ifstream config_file("config.json");
    config_file >> this->config;
    auto &config=this->config;

    // Set size
    this->setSceneRect(-400,-300,800,600);

    // Create map
    auto map_config=config["map"];
    this->map=new Map(map_config,this);
    this->walls=this->map->walls;
    this->addItem(this->map);

    // Create a player
    this->player=new Player(config["players"][1],&this->enemies,this);
    this->player->setPos(this->map->getFreePos());
    this->player->focusItem();
    this->addItem(this->player);
    this->players=vector<Base*>({this->player});

    // Create enemy timer
    connect(&this->createEnemyTimer,&QTimer::timeout,this,&GameScene::newEnemy);
    this->createEnemyTimer.start(5000);
    /* Create an enemy now */
    this->newEnemy();


}

void GameScene::debug_panel(){
    // Only for test, don't actually do the enhancement but print them
    auto panel=new EnhancePanel({
        {
            "Speed 1.5x",
            [&](){
                printf("Speed 1.5x\n");
            }
        },
        {
            "Weapon Range 2x",
            [&](){
                printf("Weapon Range 2x\n");
            }
        },
        {
            "Weapon Attack 2x",
            [&](){
                printf("Weapon Attack 2x\n");
            }
        },
        {
            "Speed 2x",
            [&](){
                printf("Speed 2x\n");
            }
        },
        {
            "Weapon Attack 3x",
            [&](){
                printf("Weapon Attack 3x\n");
            }
        }
    },this->views()[0]);
    panel->show();
}

void GameScene::newEnemy(){
    auto numEnemyTypes=this->config["enemies"].size()-1;
    auto e=new Enemy(this->config["enemies"][rand()%numEnemyTypes+1],&this->players,this);
    this->addItem(e);
    this->enemies.push_back(e);
}

GameScene::~GameScene(){
    for(auto i: this->enemies)
        delete i;
    delete this->player;
    delete this->map;
}



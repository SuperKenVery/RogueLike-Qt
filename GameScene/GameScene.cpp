#include "GameScene.h"
#include "Enemy.h"
#include "Player.h"
#include "Map.h"
#include <QImage>
#include <QtCore/qlogging.h>
#include <QtCore/qrect.h>
#include <QtCore/qtimer.h>
#include <QtGui/qevent.h>
#include <QtGui/qimage.h>
#include <QtGui/qpalette.h>
#include <QtWidgets/qgraphicsscene.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qwidget.h>
#include <cstddef>
#include <fstream>
#include <vector>

using namespace std;


GameScene::GameScene(QWidget *parent):
QGraphicsScene(parent),
enemyCreationTimer(this){
    // Record time to calculate live time
    this->startTime=time(nullptr);

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

    // Enemy creation timer
    connect(&this->enemyCreationTimer,&QTimer::timeout,this,&GameScene::newEnemy);
    this->enemyCreationTimer.start(5000);

    // Create an enemy now
    this->newEnemy();

    // Start ticking
    this->timer=new QTimer;
    connect(timer, &QTimer::timeout, this, &QGraphicsScene::advance);
    timer->start(1000 / 60);
}

// Resume from a saved game
GameScene::GameScene(json storage, QWidget *parent):QGraphicsScene(parent),
enemyCreationTimer(this){
    // Record time to calculate live time
    uint alreadyPassed=storage["liveTime"];
    this->startTime=time(nullptr)-alreadyPassed;

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

    // !!! TODO: Record player/enemy index for restoring!!!

    // Create a player
    // this->player=new Player(config["players"][1],&this->enemies,this);
    // this->player->setPos(this->map->getFreePos());
    // this->player->focusItem();
    // this->addItem(this->player);
    // this->players=vector<Base*>({this->player});

    // Restore Enemies
    for(auto enemyStorage: storage["enemies"]){
        auto e=new Enemy(enemyStorage,&this->players,this);
        this->addItem(e);
        this->enemies.push_back(e);
    }

    // Enemy creation timer
    connect(&this->enemyCreationTimer,&QTimer::timeout,this,&GameScene::newEnemy);
    this->enemyCreationTimer.start(5000);

    // Create an enemy now
    this->newEnemy();

    // Start ticking
    this->timer=new QTimer;
    connect(timer, &QTimer::timeout, this, &QGraphicsScene::advance);
    timer->start(1000 / 60);
}

void GameScene::debug_panel(){
    this->player->setPos(0,0);
    this->player->harm(10);
}

void GameScene::newEnemy(){
    auto numEnemyTypes=this->config["enemies"].size()-1;
    auto e=new Enemy(this->config["enemies"][rand()%numEnemyTypes+1],&this->players,this);
    this->addItem(e);
    this->enemies.push_back(e);
}

json GameScene::dumpState(){
    json::array_t enemy_states;
    for(auto enemy: this->enemies){
        enemy_states.push_back(enemy->dumpState());
    }
    return json(
        {
            {"liveTime",time(nullptr)-this->startTime},
            {"player",this->player->dumpState()},
            {"enemies",enemy_states}
        }
    );
}

GameScene::~GameScene(){
    auto liveSeconds=time(nullptr)-this->startTime;
    auto coinAdd=liveSeconds/6; // 10 coins a minute

    // Save state
    fstream storageFileStream("storage.json");
    json storage;
    storageFileStream>>storage;

    uint currentCoins=storage["coins"];
    storage["coins"]=currentCoins+coinAdd;
    storage["state"]=this->dumpState();

    storageFileStream << storage.dump();

    for(auto i: this->enemies)
        delete i;
    delete this->player;
    delete this->map;
}



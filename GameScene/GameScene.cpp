#include "GameScene.h"
#include "Enemy.h"
#include "Player.h"
#include "Map.h"
#include <QImage>
#include <QtCore/qeventloop.h>
#include <QtCore/qlogging.h>
#include <QtCore/qrect.h>
#include <QtCore/qtimer.h>
#include <QtCore/qtmetamacros.h>
#include <QtGui/qevent.h>
#include <QtGui/qimage.h>
#include <QtGui/qpalette.h>
#include <QtWidgets/qgraphicsscene.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qwidget.h>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include "ResumeFailedWindow/resumefailedwindow.h"
#include "DieWindow/diewindow.h"

using namespace std;

GameScene::GameScene(QWidget *parent):
QGraphicsScene(parent),
enemyCreationTimer(this){
    // Record time to calculate live time
    this->startTime=time(nullptr);

    // Load configuration from storage's configOverride
    ifstream storage_file("storage.json");
    json storage;
    storage_file >> storage;
    this->config=storage["configOverride"];
    auto &config=this->config;

    // Set size
    this->setSceneRect(-400,-300,800,600);

    // Create map
    auto map_config=config["map"];
    this->map=new Map(map_config,this);
    this->walls=this->map->walls;
    this->addItem(this->map);

    // Create a player
    this->playerIndex=1;
    this->player=new Player(config["players"][this->playerIndex],&this->enemies,this);
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


GameScene::GameScene(json storage, QWidget *parent):QGraphicsScene(parent),
enemyCreationTimer(this){
    // Record time to calculate live time
    uint alreadyPassed=storage["state"]["liveTime"];
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

    // Resume player
    this->playerIndex=storage["state"]["playerIndex"];
    this->player=new Player(config["players"][this->playerIndex],&this->enemies,this);
    this->player->resumeState(storage["state"]["player"]);
    this->player->focusItem();
    this->addItem(this->player);
    this->players=vector<Base*>({this->player});

    // Restore Enemies
    auto enemyIndexes=storage["state"]["enemyIndexes"];
    for(auto i=0;i<storage["state"]["enemies"].size();i++){
        uint index=enemyIndexes[i];
        auto eneConf=this->config["enemies"][index];
        auto enemy=new Enemy(eneConf,&this->players,this);
        enemy->resumeState(storage["state"]["enemies"][i]);
        this->addItem(enemy);
        this->enemies.push_back(enemy);
        this->enemyIndexes.push_back(index);
    }

    // Enemy creation timer
    connect(&this->enemyCreationTimer,&QTimer::timeout,this,&GameScene::newEnemy);
    this->enemyCreationTimer.start(5000);

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
    auto enemyIndex=rand()%numEnemyTypes+1;
    auto e=new Enemy(this->config["enemies"][enemyIndex],&this->players,this);
    this->addItem(e);
    this->enemies.push_back(e);
    this->enemyIndexes.push_back(enemyIndex);
}

void GameScene::deleteEnemy(Enemy *e){
    auto eIt=find(this->enemies.begin(),this->enemies.end(),e);
    if(eIt!=this->enemies.end()){
        printf("Delete enemy at %p\n",*eIt);
        delete *eIt;
        this->enemies.erase(eIt);
        printf("Size after delete: %lu\n",this->enemies.size());
    }else{
        throw "Cannot delete enemy";
    }
}

void GameScene::die(){
    this->timer->stop();
    this->enemyCreationTimer.stop();
    auto liveSeconds=time(nullptr)-this->startTime;
    auto coinAdd=liveSeconds/6; // 10 coins a minute

    // Add coins
    uint currentCoins=0;

    json storage;
    ifstream storageFileStream("storage.json");
    storageFileStream>>storage;

    uint oriCoins=storage["coins"];
    storage["coins"]=oriCoins+coinAdd;
    storage["state"]=R"(
        {
            "exist":false
        }
    )"_json;

    ofstream storageFileWriteStream("storage.json");
    storageFileWriteStream << storage.dump();

    this->shouldSaveState=false;

    // Show die dialog
    auto win=new DieWindow(liveSeconds,coinAdd);
    win->show();
    waitUntilClose(win);
    this->views()[0]->window()->close();
}

json GameScene::dumpState(){
    json::array_t enemy_states;
    for(auto enemy: this->enemies){
        enemy_states.push_back(enemy->dumpState());
    }
    return json(
        {
            {"exist",true},
            {"liveTime",time(nullptr)-this->startTime},
            {"player",this->player->dumpState()},
            {"playerIndex",this->playerIndex},
            {"enemies",enemy_states},
            {"enemyIndexes",this->enemyIndexes}
        }
    );
}

void GameScene::waitUntilClose(QWidget *w){
    QEventLoop loop;
    connect(w,&QWidget::destroyed,&loop,&QEventLoop::quit);
    loop.exec();
}

GameScene::~GameScene(){
    if(this->shouldSaveState){
        auto liveSeconds=time(nullptr)-this->startTime;
        auto coinAdd=liveSeconds/6; // 10 coins a minute

        // Save state
        json storage;
        ifstream storageFileStream("storage.json");
        storageFileStream>>storage;

        uint oriCoins=storage["coins"];
        storage["coins"]=oriCoins+coinAdd;
        storage["state"]=this->dumpState();

        ofstream storageFileWriteStream("storage.json");
        storageFileWriteStream << storage.dump();
    }

    for(auto i: this->enemies)
        delete i;
    delete this->player;
    delete this->map;
    this->timer->stop();
    delete this->timer;
}




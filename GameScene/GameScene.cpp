#include "GameScene.h"
#include "Enemy.h"
#include "Player.h"
#include "Map.h"
#include "Drops.h"
#include <QImage>
#include <QtCore/qeventloop.h>
#include <QtCore/qlogging.h>
#include <QtCore/qpoint.h>
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
#include "DieWindow/diewindow.h"

using namespace std;

GameScene::GameScene(QWidget *parent,uint playerIndex):
QGraphicsScene(parent),
playerIndex(playerIndex),
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
    this->player=new Player(config["players"][this->playerIndex],&this->enemies,this);
    this->player->setPos(this->map->getFreePos());
    this->player->focusItem();
    this->addItem(this->player);
    this->players=vector<pair<int,Base*>>({{this->playerIndex,this->player}});

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

    // Load configuration
    this->config=storage["configOverride"];
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
    this->players=vector<pair<int,Base*> >({{this->playerIndex,this->player}});

    // Restore Enemies
    auto enemyIndexes=storage["state"]["enemyIndexes"];
    for(auto i=0;i<storage["state"]["enemies"].size();i++){
        uint index=enemyIndexes[i];
        auto eneConf=this->config["enemies"][index];
        eneConf["drops"]=this->config["drops"];
        auto enemy=new Enemy(eneConf,&this->players,this);
        enemy->resumeState(storage["state"]["enemies"][i]);
        this->addItem(enemy);
        this->enemies.push_back({index,enemy});
    }

    // Resume drops
    auto drops=storage["state"]["drops"];
    auto dropIndexes=storage["state"]["dropIndexes"];
    for(auto i=0;i<drops.size();i++){
        auto dropState=drops[i];
        int dropIndex=dropIndexes[i];
        auto dropConfig=this->config["drops"][dropIndex];

        auto drop=new Drop(dropConfig,QPointF(0,0)); // Pos will be resumes fromo storage
        drop->resumeState(dropState);
        this->addItem(drop);
        this->drops.push_back({dropIndex,drop});
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

    auto enemyConfig=this->config["enemies"][enemyIndex];
    enemyConfig["drops"]=this->config["drops"];
    auto e=new Enemy(enemyConfig,&this->players,this);
    this->addItem(e);
    this->enemies.push_back({enemyIndex,e});
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
    vector<int> enemyIndexes;
    for(auto [index,enemy]: this->enemies){
        enemy_states.push_back(enemy->dumpState());
        enemyIndexes.push_back(index);
    }

    vector<json> dropStates;
    vector<int> dropIndexes;
    for(auto [index,drop]:this->drops){
        dropStates.push_back(drop->dumpState());
        dropIndexes.push_back(index);
    }

    return json(
        {
            {"exist",true},
            {"liveTime",time(nullptr)-this->startTime},
            {"player",this->player->dumpState()},
            {"playerIndex",this->playerIndex},
            {"enemies",enemy_states},
            {"enemyIndexes",enemyIndexes},
            {"drops",dropStates},
            {"dropIndexes",dropIndexes}
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

    for(auto [index,enemy]: this->enemies)
        delete enemy;

    for(auto [index,drop]:this->drops)
        delete drop;

    delete this->player;
    delete this->map;
    this->timer->stop();
    delete this->timer;
}




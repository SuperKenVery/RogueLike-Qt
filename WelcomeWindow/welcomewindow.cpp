#include "welcomewindow.h"
#include "ui_welcomewindow.h"
#include <QtCore/qnamespace.h>
#include <QtGui/qwindowdefs.h>
#include <QGraphicsView>
#include <QWindow>
#include <QWidget>
#include <QtWidgets/qmainwindow.h>
#include "../GameScene/GameScene.h"
#include "../Store/store.h"
#include "../GameScene/ResumeFailedWindow/resumefailedwindow.h"

WelcomeWindow::WelcomeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WelcomeWindow)
{
    // Setup storage file
    if(!filesystem::exists("storage.json")||filesystem::is_empty("storage.json")){
        auto storageFileStream=ofstream("storage.json");
        auto configFileStream=ifstream("config.json");
        json storage,config;
        configFileStream >> config;
        storage["configOverride"]=config;
        storage["coins"]=0;
        storage["state"]=R"(
            {"exist": false}
        )"_json;
        storageFileStream << storage.dump();
        storageFileStream.close();
    }

    ui->setupUi(this);

    // Setup player chooser
    auto storageFileStream=ifstream("storage.json");
    json storage;
    storageFileStream >> storage;
    auto &config=storage["configOverride"];
    auto &players=config["players"];
    for(auto playerIndex=1;playerIndex<players.size();playerIndex++){
        printf("Adding %s\n",players[playerIndex]["name"].get<string>().c_str());
        ui->playerChooser->addItem(QString::fromStdString(players[playerIndex]["name"]));
    }

    // Connect button events
    connect(ui->startGameButton,&QPushButton::clicked,this,&WelcomeWindow::startGame);
    connect(ui->storeButton,&QPushButton::clicked,this,&WelcomeWindow::store);
    connect(ui->continueGameButton,&QPushButton::clicked,this,&WelcomeWindow::continueGame);
}

void WelcomeWindow::store(){
    auto store=new Store(this);
    store->show();
}

void makeFit(QMainWindow *win,QGraphicsView *graphicsView){
    win->setCentralWidget(graphicsView);
    auto bound=graphicsView->scene()->itemsBoundingRect();
    win->setFixedSize(bound.width(),bound.height());
}

void WelcomeWindow::startGame(){
    // When an Qt objects is destroyed it destroys its children: https://stackoverflow.com/a/52288523
    // So let them `leak` here.

    auto win=new QMainWindow(this);
    win->setAttribute(Qt::WA_DeleteOnClose);

    auto graphicsView=new QGraphicsView(win);
    auto playerIndex=this->ui->playerChooser->currentIndex()+1;
    auto scene=new GameScene(graphicsView,playerIndex);
    graphicsView->setScene(scene);

    makeFit(win, graphicsView);
    win->show();
}

void WelcomeWindow::continueGame(){


    ifstream storageFileStream("storage.json");
    json storage;
    storageFileStream >> storage;
    bool stateExist=storage["state"]["exist"];
    if(!stateExist){
        auto win=new ResumeFailedWindow();
        win->show();
        return;
    }
    auto win=new QMainWindow(this);
    win->setAttribute(Qt::WA_DeleteOnClose);

    auto graphicsView=new QGraphicsView(win);

    auto scene=new GameScene(storage,graphicsView);
    graphicsView->setScene(scene);

    makeFit(win, graphicsView);
    win->show();
}

WelcomeWindow::~WelcomeWindow()
{
    delete ui;
}

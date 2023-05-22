#include "welcomewindow.h"
#include "ui_welcomewindow.h"
#include <QtGui/qwindowdefs.h>
#include <QGraphicsView>
#include <QWindow>
#include <QWidget>
#include <QtWidgets/qmainwindow.h>
#include "../GameScene/GameScene.h"
#include "../Store/store.h"

WelcomeWindow::WelcomeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WelcomeWindow)
{
    ui->setupUi(this);
    connect(ui->startGameButton,&QPushButton::clicked,this,&WelcomeWindow::startGame);
    connect(ui->storeButton,&QPushButton::clicked,this,&WelcomeWindow::store);
    connect(ui->continueGameButton,&QPushButton::clicked,this,&WelcomeWindow::continueGame);
}

void WelcomeWindow::store(){
    auto store=new Store(this);
    store->show();
}

void WelcomeWindow::startGame(){
    // When an Qt objects is destroyed it destroys its children: https://stackoverflow.com/a/52288523
    // So let them `leak` here.

    auto win=new QMainWindow(this);

    auto graphicsView=new QGraphicsView(win);
    auto scene=new GameScene(graphicsView);
    graphicsView->setScene(scene);
    win->setCentralWidget(graphicsView);

    auto bound=scene->itemsBoundingRect();
    win->setFixedSize(bound.width(),bound.height());
    win->show();
}

void WelcomeWindow::continueGame(){

}

WelcomeWindow::~WelcomeWindow()
{
    delete ui;
}

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtCore/qobjectdefs.h>
#include <QtCore/qtimer.h>
#include <QtCore/qtmetamacros.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qgraphicsview.h>
#include <QLayout>
#include "GameScene/GameScene.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->graphicsView=new QGraphicsView(this);
    this->scene=new GameScene(this->graphicsView);
    this->graphicsView->setScene(this->scene);
    auto bound=this->scene->itemsBoundingRect();
    printf("itemsboundingrect is %f %f %f %f\n",bound.x(),bound.y(),bound.width(),bound.height());

    this->setCentralWidget(this->graphicsView);
    this->setFixedSize(bound.width(),bound.height());

    this->timer=new QTimer;
    connect(timer, &QTimer::timeout, this->scene, &QGraphicsScene::advance);
    timer->start(1000 / 60);

    // auto t=new QTimer;
    // connect(timer, &QTimer::timeout, this, &MainWindow::debug);
    // timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::debug(){
    printf("View height: %d\n",this->graphicsView->height());
}



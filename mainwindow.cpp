#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtCore/qobjectdefs.h>
#include <QtCore/qtimer.h>
#include <QtCore/qtmetamacros.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qgraphicsview.h>
#include <QLayout>
#include "GameScene.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->graphicsView=new QGraphicsView(this);
    this->scene=new GameScene(this->graphicsView);
    this->graphicsView->setScene(this->scene);
    this->setCentralWidget(this->graphicsView);

    this->timer=new QTimer;
    connect(timer, &QTimer::timeout, this->scene, &QGraphicsScene::advance);
    timer->start(1000 / 60);
}

MainWindow::~MainWindow()
{
    delete ui;
}



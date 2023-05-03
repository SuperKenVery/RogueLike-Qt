#include "GameScene.h"
#include "player.h"
#include <QImage>
#include <QtCore/qlogging.h>
#include <QtGui/qevent.h>
#include <QtGui/qimage.h>
#include <QtWidgets/qgraphicsscene.h>
#include <QtWidgets/qwidget.h>
#include <cstddef>

GameScene::GameScene(QWidget *parent):
QGraphicsScene(parent){
    // Set size
    this->setSceneRect(-400,-300,800,600);

    // Create map


    // Create a player
    auto player=new Player(QImage("images/player.png"));
    player->setPos(0,0);
    this->setFocusItem(player);
    this->addItem(player);

}

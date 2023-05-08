#ifndef GameScene_h
#define GameScene_h

#include "Map.h"
#include "player.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtCore/qtmetamacros.h>
#include <QtWidgets/qgraphicsscene.h>
#include <QtWidgets/qwidget.h>
#include <nlohmann/json.hpp>
using json=nlohmann::json;

class GameScene: public QGraphicsScene{
    Q_OBJECT
public:
    GameScene(QWidget *parent=nullptr);
    vector<MapBlock*> walls;
private:
    json config;
    Map *map;
    Player *player;
};

#endif
#ifndef GameScene_h
#define GameScene_h

#include <QtCore/qrect.h>
#include <vector>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtCore/qtmetamacros.h>
#include <QtWidgets/qgraphicsscene.h>
#include <QtWidgets/qwidget.h>
#include <QTimer>
#include <nlohmann/json.hpp>
using json=nlohmann::json;
using namespace std;

class Player;
class Map;
class MapBlock;
class Enemy;
class Base;

class GameScene: public QGraphicsScene{
    Q_OBJECT
public:
    GameScene(QWidget *parent=nullptr);
    ~GameScene();
    vector<MapBlock*> walls;
    json config;
    Map *map;
    Player *player;
    vector<Base*> enemies,players;
    QTimer createEnemyTimer;
    void debug_panel();
public slots:
    void newEnemy();
private:
};

#endif
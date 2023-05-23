#ifndef GameScene_h
#define GameScene_h

#include <QtCore/qrect.h>
#include <QtCore/qtypes.h>
#include <sys/_types/_time_t.h>
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
    /* Resume from a saved game
     * @param storage: the storage of the saved game
     * @param parent: the parent widget
     *
     * Should be called **instead** of the above contructor
    */
    GameScene(json storage, QWidget *parent=nullptr);
    ~GameScene();
    json dumpState();
    vector<MapBlock*> walls;
    json config;
    Map *map;
    Player *player;
    vector<Base*> enemies,players;
    QTimer enemyCreationTimer;
    void die();
    void debug_panel();
public slots:
    void newEnemy();
    void deleteEnemy(Enemy *e);
private:
    void waitUntilClose(QWidget *w);
    QTimer *timer;
    time_t startTime;
    uint playerIndex;
    vector<ulong> enemyIndexes;
    bool shouldSaveState=true;
};

#endif
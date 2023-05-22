#ifndef Enemy_h
#define Enemy_h

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QtCore/qpoint.h>
#include <QtGui/qvectornd.h>
#include <functional>
#include <unordered_map>
#include <string>
#include <nlohmann/json.hpp>
#include "Base.h"
#include "GameScene.h"
#include "Weapon.h"
using json=nlohmann::json;
using namespace std;


class Enemy;
typedef std::function<void(Enemy*)> move_strategy;
typedef vector<Base*> attackable_list;

class Enemy: public Base{
public:
    Enemy(json config,attackable_list *attackables,GameScene *scene);
    static unordered_map<string, move_strategy> move_strategies;
    void die() override;
    json dumpState();
protected:
    void advance(int step) override;
private:
    Weapon *weapon;
    uint speed;
    move_strategy strategy;
    QVector2D direction;
    string moveStrategyName;
};

#endif

#ifndef Weapon_h
#define Weapon_h

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QtWidgets/qgraphicsitem.h>
#include <QtWidgets/qgraphicsscene.h>
#include <nlohmann/json.hpp>
#include "Base.h"
using json=nlohmann::json;
using namespace std;

class Weapon: public QGraphicsItem{
public:
    Weapon(json config,QGraphicsItem* parent);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void advance(int step) override;
    void setAttackables(vector<Base*> attackables);
private:
    uint attack,range;
    vector<Base*> attackables;
};

#endif

#ifndef Weapon_h
#define Weapon_h

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QtGui/qpainterpath.h>
#include <QtWidgets/qgraphicsitem.h>
#include <QtWidgets/qgraphicsscene.h>
#include <nlohmann/json.hpp>

using json=nlohmann::json;
using namespace std;

class Base;
typedef vector<pair<int,Base*> > attackable_list;

class Weapon: public QGraphicsItem{
public:
    Weapon(json config,attackable_list *attackables,QGraphicsItem* parent);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void advance(int step) override;
    json dumpState();
    void resumeState(json storage);
    uint hp=0;
    uint attack,range;
    bool debug=false;
private:
    attackable_list *attackables;
    uint count=0;
    constexpr static uint total_frames=50;
    uint frame=total_frames;
};

#endif

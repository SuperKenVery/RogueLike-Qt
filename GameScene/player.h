#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsItem>
#include<QImage>
#include<QVector2D>
#include <QtCore/qtmetamacros.h>
#include <QtCore/qtypes.h>
#include <QtWidgets/qwidget.h>
#include <nlohmann/json.hpp>
#include <vector>
#include "Base.h"
#include "GameScene.h"
#include "Weapon.h"
using json=nlohmann::json;

typedef vector<pair<int,Base*> > attackable_list;

class Player : public Base
{
public:
    Player(json config,attackable_list *attackables,GameScene *scene);
    ~Player();
    QImage image;
    void die() override;
    json dumpState() override;
    void resumeState(json storage) override;
    Weapon *weapon;
    uint speed;

protected:
    void advance(int step) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void enhance();
    QVector2D direction=QVector2D(0,0);
    vector<uint> enhance_hp;
    vector<uint>::iterator next_enhance_hp;
    uint enhanceCount=0;
};

#endif // PLAYER_H

#ifndef Enemy_h
#define Enemy_h

#include <QGraphicsItem>
#include <functional>
#include <unordered_map>
#include <string>
#include <nlohmann/json.hpp>
using json=nlohmann::json;
using namespace std;


class Enemy;
typedef std::function<void(Enemy*)> move_strategy;

class Enemy: public QGraphicsItem{
public:
    Enemy();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    static unordered_map<string, move_strategy> move_strategies;
};

#endif

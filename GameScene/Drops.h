// 掉落物品
#ifndef Drop_h
#define Drop_h

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QPointF>
#include <functional>
#include <QImage>
#include <nlohmann/json.hpp>
#include "Base.h"
using json=nlohmann::json;


class Drop: public QGraphicsItem{
public:
    /* Construct a dropped item
     *
     * @param pickAction The action to be executed when the item is picked
     * @param parent The parent item
    */
    Drop(json config,QPointF pos,QGraphicsItem* parent=nullptr);
    QRectF boundingRect() const override;
    void advance(int step) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    json dumpState();
    void resumeState(json storage);

    QImage image;
    uint size;

    std::string actionName;
    std::function<void(Player*)> action;

    static std::map<std::string,std::function<void(Player*)> > actions;
};

#endif

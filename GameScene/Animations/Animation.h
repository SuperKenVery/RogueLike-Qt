#ifndef Animation_h
#define Animation_h

#include <QPainter>
#include <QStyleOption>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QtCore/qrect.h>

class Animation: public QGraphicsItem {
public:
    Animation(QGraphicsItem* parent=nullptr):QGraphicsItem(parent){}
    virtual void advance(int step) override=0;
    virtual QRectF boundingRect() const override=0;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override=0;

    virtual ~Animation()=default;
};

#endif
